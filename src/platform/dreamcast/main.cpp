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

#include <cstdlib>
#include <string>
#include <vector>
#include "player.h"
#include "utils.h"
#include "output.h"

#ifdef _WIN32
#  include <windows.h>
#  include <shellapi.h>
#elif defined(__ANDROID__)
#  include <android/log.h>
#elif defined(__WIIU__)
#  include <coreinit/debug.h>
#endif


#include <kos.h>
#include <kos.h>
#include <dc/sd.h>
#include <kos/blockdev.h>
#include <fat/fs_fat.h>
#include <ext2/fs_ext2.h>
KOS_INIT_FLAGS(INIT_DEFAULT);

kos_blockdev_t sd_dev;
uint8 partition_type;

#if defined(__ANDROID__) || defined(__WIIU__)
static void LogCallback(LogLevel lvl, std::string const& msg, LogCallbackUserData /* userdata */) {
#  if defined(__ANDROID__)
#    ifdef NDEBUG
	// docs say debugging logs should be disabled for release builds
	if (lvl == LogLevel::Debug || lvl == LogLevel::Info) return;
#    endif

	int prio = (lvl == LogLevel::Error) ? ANDROID_LOG_ERROR :
		(lvl == LogLevel::Warning) ? ANDROID_LOG_WARN :
		(lvl == LogLevel::Debug) ? ANDROID_LOG_DEBUG :
		ANDROID_LOG_INFO;

	__android_log_write(prio, GAME_TITLE, msg.c_str());
#  elif defined(__WIIU__)
	std::string m = std::string("[" GAME_TITLE "] ") +
		Output::LogLevelToString(lvl) + ": " + msg;

	OSReport("%s\n", m.c_str());
#  endif
}
#endif

/**
 * If the main function ever needs to change, be sure to update the `main()`
 * functions of the other platforms as well.
 */
extern "C" int main(int argc, char* argv[]) {
	std::vector<std::string> args;

#if defined(_WIN32)
	// Use widestring args
	int argc_w;
	LPWSTR *argv_w = CommandLineToArgvW(GetCommandLineW(), &argc_w);
	if (argc_w > 0) {
		args.reserve(argc_w - 1);
	}
	for (int i = 0; i < argc_w; ++i) {
		args.push_back(Utils::FromWideString(argv_w[i]));
	}
	LocalFree(argv_w);
#else
	args.assign(argv, argv + argc);
#endif
/*
#ifdef DREAMCAST
	args.push_back("--project-path");
	args.push_back("/cd/");
#endif
*/
	cont_btn_callback(0, CONT_START | CONT_A | CONT_B | CONT_X | CONT_Y, (void (*)(unsigned char, long  unsigned int))arch_exit);
	auto savefs = FileFinder::Root().Create(FileFinder::MakeCanonical("/vmu/a1/", 0));
	FileFinder::SetSaveFilesystem(savefs);
	/*if(sd_init()) 
	{
		printf("No SD card detected. Make sure to have SD card !\n");
		auto savefs = FileFinder::Root().Create(FileFinder::MakeCanonical("/ram/", 0));
		FileFinder::SetSaveFilesystem(savefs);
	}
	else
	{
		sd_blockdev_for_partition(0, &sd_dev, &partition_type);
		if (partition_type == 0x04 || partition_type == 0x06 || partition_type == 0x0B || partition_type == 0x0C) 
		{
			fs_fat_init();
			fs_fat_mount("/sd", &sd_dev, FS_FAT_MOUNT_READWRITE);
			//args.push_back("--save-path");
			//args.push_back("/sd");
			auto savefs = FileFinder::Root().Create(FileFinder::MakeCanonical("/sd/", 0));
			FileFinder::SetSaveFilesystem(savefs);
			printf("SD card init !\n");
		}
		else
		{
			printf("Only FAT32 SD is supported\n");
			auto savefs = FileFinder::Root().Create(FileFinder::MakeCanonical("/ram/", 0));
			FileFinder::SetSaveFilesystem(savefs);
			//args.push_back("--save-path");
			//args.push_back("/ram");
		}
	}*/

	Player::Init(std::move(args));
	Player::Run();

	// Close
	return Player::exit_code;
}
