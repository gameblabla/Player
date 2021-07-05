/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <cstdio>

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "graphics.h"
#include "output.h"

#ifdef GEKKO
#  include <unistd.h>
#  include <gccore.h>
#  include <sys/iosupport.h>
#elif defined(__SWITCH__)
#  include <unistd.h>
#elif defined(__ANDROID__)
#  include <android/log.h>
#elif defined(EMSCRIPTEN)
#  include <emscripten.h>
#endif

#include "filefinder.h"
#include "input.h"
#include "options.h"
#include "output.h"
#include "player.h"
#include "bitmap.h"
#include "main_data.h"
#include "message_overlay.h"
#include "utils.h"
#include "font.h"
#include "baseui.h"

using namespace std::chrono_literals;

namespace {
	constexpr const char* const log_prefix[4] = {
		"Error: ",
		"Warning: ",
		"Info: ",
		"Debug: "
	};
	LogLevel log_level = LogLevel::Debug;

	static const char* GetLogPrefix(LogLevel lvl) {
		return log_prefix[static_cast<int>(lvl)];
	}

	Filesystem_Stream::OutputStream LOG_FILE;
	bool output_recurse = false;
	bool init = false;

	std::ostream& output_time() {
		if (!init) {
			LOG_FILE = FileFinder::Save().OpenOutputStream(OUTPUT_FILENAME, std::ios_base::out | std::ios_base::app);
			init = true;
		}
		std::time_t t = std::time(NULL);
		char timestr[100];
		strftime(timestr, 100, "[%Y-%m-%d %H:%M:%S] ", std::localtime(&t));
		return LOG_FILE << timestr;
	}

	bool ignore_pause = false;

	std::vector<std::string> log_buffer;
	// pair of repeat count + message
	struct {
		int repeat = 0;
		std::string msg;
		LogLevel lvl = {};
	} last_message;

#ifdef GEKKO
	/* USBGecko Debugging on Wii */
	bool usbgecko = false;
	mutex_t usbgecko_mutex = 0;

	static ssize_t __usbgecko_write(struct _reent * /* r */, void* /* fd */, const char *ptr, size_t len) {
		uint32_t level;

		if (!ptr || !len || !usbgecko)
			return 0;

		LWP_MutexLock(usbgecko_mutex);
		level = IRQ_Disable();
		usb_sendbuffer(1, ptr, len);
		IRQ_Restore(level);
		LWP_MutexUnlock(usbgecko_mutex);

		return len;
	}

	const devoptab_t dotab_geckoout = {
		"stdout", 0, NULL, NULL, __usbgecko_write, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL
	};
#endif

}

LogLevel Output::GetLogLevel() {
	return log_level;
}

void Output::SetLogLevel(LogLevel ll) {
	log_level = ll;
}

void Output::IgnorePause(bool const val) {
	ignore_pause = val;
}

static void WriteLog(LogLevel lvl, std::string const& msg, Color const& c = Color()) {
#ifndef NO_LOG
	const char* prefix = GetLogPrefix(lvl);
	// Skip logging to file in the browser
#ifndef EMSCRIPTEN
	bool add_to_buffer = true;

	// Prevent recursion when the Save filesystem writes to the logfile on startup before it is ready
	if (!output_recurse) {
		output_recurse = true;
		if (FileFinder::Save()) {
			add_to_buffer = false;

			// Only write to file when save path is initialized
			// (happens after parsing the command line)
			if (!log_buffer.empty()) {
				std::vector<std::string> local_log_buffer = std::move(log_buffer);
				for (std::string& log : local_log_buffer) {
					output_time() << log << '\n';
				}
				local_log_buffer.clear();
			}

			// Every new message is written once to the file.
			// When it is repeated increment a counter until a different message appears,
			// then write the buffered message with the counter.
			if (msg == last_message.msg) {
				last_message.repeat++;
			} else {
				if (last_message.repeat > 0) {
					output_time() << GetLogPrefix(last_message.lvl) << last_message.msg << " [" << last_message.repeat + 1 << "x]" << std::endl;
					output_time() << prefix << msg << '\n';
				} else {
					output_time() << prefix << msg << '\n';
				}
				last_message.repeat = 0;
				last_message.msg = msg;
				last_message.lvl = lvl;
			}
		}
		output_recurse = false;
	}

	if (add_to_buffer) {
		// buffer log messages until file system is ready
		log_buffer.push_back(prefix + msg);
	}
#endif

#ifdef __ANDROID__
	__android_log_print(lvl == LogLevel::Error ? ANDROID_LOG_ERROR : ANDROID_LOG_INFO, "EasyRPG Player", "%s", msg.c_str());
#else
	std::cerr << prefix << msg << '\n';
#endif

	if (lvl != LogLevel::Debug && lvl != LogLevel::Error) {
		Graphics::GetMessageOverlay().AddMessage(msg, c);
	}
#endif
}

static void HandleErrorOutput(const std::string& err) {
#ifndef NO_LOG
	// Drawing directly on the screen because message_overlay is not visible
	// when faded out
	BitmapRef surface = DisplayUi->GetDisplaySurface();
	surface->FillRect(surface->GetRect(), Color(255, 0, 0, 128));

	std::string error = "Error:\n";
	error += err;

	error += "\n\nEasyRPG Player will close now.\nPress [ENTER] key to exit...";

	Text::Draw(*surface, 11, 11, *Font::Default(), Color(0, 0, 0, 255), error);
	Text::Draw(*surface, 10, 10, *Font::Default(), Color(255, 255, 255, 255), error);
	DisplayUi->UpdateDisplay();

	if (ignore_pause) { return; }

	Input::ResetKeys();
	while (!Input::IsAnyPressed()) {
#if !defined(USE_LIBRETRO)
		Game_Clock::SleepFor(1ms);
#endif
		DisplayUi->ProcessEvents();

		if (Player::exit_flag) break;

		Input::Update();
	}
#endif
}

void Output::Quit() {
#ifndef NO_LOG
	if (LOG_FILE) {
		LOG_FILE.clear();
	}

	int log_size = 1024 * 100;

	char* buf = new char[log_size];

	auto in = FileFinder::Save().OpenInputStream(OUTPUT_FILENAME, std::ios_base::in);
	if (in) {
		in.seekg(0, std::ios_base::end);
		if (in.tellg() > log_size) {
			in.seekg(-log_size, std::ios_base::end);
			// skip current incomplete line
			in.getline(buf, 1024 * 100);
			in.read(buf, 1024 * 100);
			size_t read = in.gcount();

			auto out = FileFinder::Save().OpenOutputStream(OUTPUT_FILENAME, std::ios_base::out);
			if (out) {
				out.write(buf, read);
			}
		}
	}

	delete[] buf;
#endif
}

bool Output::TakeScreenshot() {
#ifndef NO_LOG
	int index = 0;
	std::string p;
	do {
		p = "screenshot_" + std::to_string(index++) + ".png";
	} while(FileFinder::Save().Exists(p));
	return TakeScreenshot(p);
#else
	return false;
#endif
}

bool Output::TakeScreenshot(StringView file) {
#ifndef NO_LOG
	auto ret = FileFinder::Save().OpenOutputStream(file, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);

	if (ret) {
		Output::Debug("Saving Screenshot {}", file);
		return Output::TakeScreenshot(ret);
	}
#endif
	return false;
}

bool Output::TakeScreenshot(Filesystem_Stream::OutputStream& os) {
	return DisplayUi->GetDisplaySurface()->WritePNG(os);
}

void Output::ToggleLog() {
#ifndef NO_LOG
	static bool show_log = true;
	Graphics::GetMessageOverlay().SetShowAll(show_log);
	show_log = !show_log;
#else
	Graphics::GetMessageOverlay().SetShowAll(false);
#endif
}

void Output::ErrorStr(std::string const& err) {
	#ifndef NO_LOG
	WriteLog(LogLevel::Error, err);
	static bool recursive_call = false;
	if (!recursive_call && DisplayUi) {
		recursive_call = true;
		HandleErrorOutput(err);
		DisplayUi.reset();
	} else {
		// Fallback to Console if the display is not ready yet
		std::cout << err << std::endl;
		std::cout << std::endl;
		std::cout << "EasyRPG Player will close now.";
#if defined (GEKKO) || defined(__SWITCH__)
		// stdin is non-blocking
		sleep(5);
#elif defined (EMSCRIPTEN)
		// Don't show JavaScript Window.prompt from stdin call
		std::cout << " Process finished.";
#else
		std::cout << " Press any key..." << std::endl;
		std::cin.get();
#endif
	}

	exit(EXIT_FAILURE);
	#endif
}

void Output::WarningStr(std::string const& warn) {
	#ifndef NO_LOG
	if (log_level < LogLevel::Warning) {
		return;
	}
	WriteLog(LogLevel::Warning, warn, Color(255, 255, 0, 255));
	#endif
}

void Output::InfoStr(std::string const& msg) {
	#ifndef NO_LOG
	if (log_level < LogLevel::Info) {
		return;
	}
	WriteLog(LogLevel::Info, msg, Color(255, 255, 255, 255));
	#endif
}

void Output::DebugStr(std::string const& msg) {
	#ifndef NO_LOG
	if (log_level < LogLevel::Debug) {
		return;
	}
	WriteLog(LogLevel::Debug, msg, Color(128, 128, 128, 255));
	#endif
}

#ifdef GEKKO
extern const devoptab_t dotab_stdnull;

void Output::WiiSetConsole() {
	LWP_MutexInit(&usbgecko_mutex, false);
	usbgecko = usb_isgeckoalive(1);

	if (usbgecko) {
		devoptab_list[STD_OUT] = &dotab_geckoout;
		devoptab_list[STD_ERR] = &dotab_geckoout;
	} else {
		devoptab_list[STD_OUT] = &dotab_stdnull;
		devoptab_list[STD_ERR] = &dotab_stdnull;
	}
}
#endif
