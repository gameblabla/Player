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

#ifndef EP_SDL_UI_H
#define EP_SDL_UI_H

// Headers
#include "baseui.h"
#include "color.h"
#include "rect.h"
#include "system.h"
#include "axis.h"


struct AudioInterface;

/**
 * DreamcastUi class.
 */
class DreamcastUi final : public BaseUi {
public:
	/**
	 * Constructor.
	 *
	 * @param width window client width.
	 * @param height window client height.
	 * @param cfg config options
	 */
	DreamcastUi(long width, long height, const Game_Config& cfg);

	/**
	 * Destructor.
	 */
	~DreamcastUi() override;

	/**
	 * Inherited from BaseUi.
	 */
	/** @{ */

	void ToggleFullscreen() override;
	void ToggleZoom() override;
	void UpdateDisplay() override;
	void SetTitle(const std::string &title) override;
	bool ShowCursor(bool flag) override;
	void ProcessEvents() override;
	void vGetConfig(Game_ConfigVideo& cfg) const override;

#ifdef SUPPORT_AUDIO
	AudioInterface& GetAudio() override;
#endif

	/** @} */

private:
	/**
	 * Refreshes the display mode after it was changed.
	 *
	 * @return whether the change was successful.
	 */
	bool RefreshDisplayMode();

	void BeginDisplayModeChange();
	void EndDisplayModeChange();

	/** @} */

	/**
	 * Resets keys states.
	 */
	void ResetKeys();

	bool zoom_available = false;
	bool toggle_fs_available = false;

	bool RequestVideoMode(int width, int height, bool fullscreen);

	/** Last display mode. */
	DisplayMode last_display_mode;

	/** Mode is being changing flag */
	bool mode_changing = false;

	/** Bitmap handle to sdl_surface */
	BitmapRef sdl_surface_bmp;

#ifdef SUPPORT_AUDIO
	std::unique_ptr<AudioInterface> audio_;
#endif
};

#endif
