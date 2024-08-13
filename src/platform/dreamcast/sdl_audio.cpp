 
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

#include "system.h"

#ifdef SUPPORT_AUDIO

#include "fake_assert.h"
#include <cstdint>
#include <chrono>
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_version.h>
#include "audio.h"
#include "output.h"

using namespace std::chrono_literals;

void sdl_audio_callback(void* userdata, uint8_t* stream, int length) {
	// no mutex locking required, SDL does this before calling
	static_cast<GenericAudio*>(userdata)->Decode(stream, length);
}

DreamcastAudio::DreamcastAudio(const Game_ConfigAudio& cfg) :
	GenericAudio(cfg)
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);

#ifdef LOW_MEMORY_DEVICES
	const int frequency = 11025;
#else
	const int frequency = 44100;
#endif

	SDL_AudioSpec want = {};
	SDL_AudioSpec have = {};
	want.freq = frequency;
	want.format = AUDIO_S16SYS;
	want.channels = 2;
#ifdef LOW_MEMORY_DEVICES
	want.samples = 512;
#else
	want.samples = 2048;
#endif
	want.callback = sdl_audio_callback;
	want.userdata = this;

	bool init_success = SDL_OpenAudio(&want, &have) >= 0;

	SetFormat(11025, AudioDecoder::Format::S16, 2);

	SDL_PauseAudio(0);
}

DreamcastAudio::~DreamcastAudio() {
	SDL_CloseAudio();
}

void DreamcastAudio::LockMutex() const {
	SDL_LockAudio();
}

void DreamcastAudio::UnlockMutex() const {
	SDL_UnlockAudio();
}

#endif
