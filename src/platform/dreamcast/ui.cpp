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
#include <cstring>

#include <kos.h>

#include <dc/pvr.h>
#include <dc/maple.h>
#include <dc/fmath.h>
#include <dc/maple/controller.h>

#include "system.h"

#include "ui.h"
#include "color.h"
#include "graphics.h"
#include "keys.h"
#include "output.h"
#include "player.h"
#include "bitmap.h"

#ifdef SUPPORT_AUDIO
#include "audio.h"

AudioInterface& DreamcastUi::GetAudio() {
	return *audio_;
}
#endif

mouse_state_t *mstate;
maple_device_t *cont, *kbd,  *mouse;
cont_state_t *state;
pvr_ptr_t front_tex;

pvr_poly_cxt_t cxt;
pvr_poly_hdr_t hdr;
pvr_vertex_t vert;
uint8_t* pix_dc;
DreamcastUi::DreamcastUi(long width, long height, const Game_Config& cfg) : BaseUi(cfg)
{
#ifdef RGBA_CODEPATH
	current_display_mode.height = 320;
	current_display_mode.width = 240;
	
	vid_set_mode(DM_320x240, PM_RGB888P);

	// Create the surface we draw on
	/*DynamicFormat format = DynamicFormat(
		32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000,
		PF::NoAlpha);*/
		
	DynamicFormat format = DynamicFormat(
		24,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0x00000000,
		PF::NoAlpha);

	Bitmap::SetFormat(Bitmap::ChooseFormat(format));

	pix_dc = (uint8_t*)aligned_alloc(32, (320 * 240)*3);

	main_surface = Bitmap::Create(pix_dc, 320, 240, 320*3, format);
#else
	current_display_mode.height = 320;
	current_display_mode.width = 240;

    /* init kos  */
    vid_set_mode(DM_640x480, PM_RGB888P);
	pvr_init_defaults();
    pvr_dma_init();
    
    front_tex = pvr_mem_malloc((512*256)*2);
    
    pix_dc = (uint8_t*)aligned_alloc(32, (512 * 240)*2);

	// Create the surface we draw on
	DynamicFormat format = format_A1R5G5B5_n().format();
	Bitmap::SetFormat(Bitmap::ChooseFormat(format));

	main_surface = Bitmap::Create(pix_dc, 320, 240, 1024, format);
#endif

#ifdef SUPPORT_AUDIO
	if (!Player::no_audio_flag) {
		audio_ = std::make_unique<DreamcastAudio>(cfg.audio);
		return;
	}
#endif
}

DreamcastUi::~DreamcastUi() {
	//Quit
}

void DreamcastUi::BeginDisplayModeChange() {
	last_display_mode = current_display_mode;
	current_display_mode.effective = false;
	mode_changing = true;
}

void DreamcastUi::EndDisplayModeChange() {

}

bool DreamcastUi::RefreshDisplayMode() {
	//current_display_mode.bpp = sdl_surface->format->BitsPerPixel;
	//current_display_mode.bpp = 2;
	return true;
}

void DreamcastUi::ToggleFullscreen() {
}

void DreamcastUi::ToggleZoom() {

}

void DreamcastUi::ProcessEvents() {
#if defined(USE_JOYSTICK) && defined(SUPPORT_JOYSTICK)
	if (!cont)
	{
		for(int i=0;i<4;i++)
		{
			cont = maple_enum_type(i, MAPLE_FUNC_CONTROLLER);
			if (cont) break;
		}
	}
	
	if(cont) state = (cont_state_t *)maple_dev_status(cont);
		
	keys[Input::Keys::JOY_DPAD_LEFT] = ((state->buttons & CONT_DPAD_LEFT) || state->joyx < -16) ? 1 : 0; 
	keys[Input::Keys::JOY_DPAD_RIGHT] = ((state->buttons & CONT_DPAD_RIGHT) || state->joyx > 16) ? 1 : 0; 
	keys[Input::Keys::JOY_DPAD_UP] = ((state->buttons & CONT_DPAD_UP) || state->joyy < -16) ? 1 : 0; 
	keys[Input::Keys::JOY_DPAD_DOWN] = ((state->buttons & CONT_DPAD_DOWN) || state->joyy > 16) ? 1 : 0; 
	
	keys[Input::Keys::JOY_A] = (state->buttons & CONT_A) ? 1 : 0; 
	keys[Input::Keys::JOY_B] = (state->buttons & CONT_B) ? 1 : 0; 
	keys[Input::Keys::JOY_X] = (state->buttons & CONT_X) ? 1 : 0; 
	keys[Input::Keys::JOY_Y] = (state->buttons & CONT_Y) ? 1 : 0; 
	
	keys[Input::Keys::JOY_SHOULDER_LEFT] = (state->ltrig > 16) ? 1 : 0; 
	keys[Input::Keys::JOY_SHOULDER_RIGHT] = (state->rtrig > 16) ? 1 : 0; 
	
	keys[Input::Keys::JOY_START] = (state->buttons & CONT_START) ? 1 : 0; 
#else
	#error "Missing joystick support"
#endif
}

static inline void bit64_sq_cpy(void *dest, void *src, int n)
{
    uint32 *d, *s;
    uint32 r0, r1, r2, r3, r4, r5, r6, r7;
    
    // Set up destination pointer with SQ address space and alignment
    d = (uint32 *)(0xe0000000 | (((uint32)dest) & 0x03ffffe0));
    // Set up source pointer
    s = (uint32 *)(src);

    // Configure memory-mapped registers for SQ access
    *((volatile unsigned int*)0xFF000038) = ((((uint32)dest) >> 26) << 2) & 0x1c;
    *((volatile unsigned int*)0xFF00003C) = ((((uint32)dest) >> 26) << 2) & 0x1c;

    // Convert n to number of 64-byte blocks
    n >>= 6;

    // Main copy loop
    while (n--) 
    {
        // Expose loads to the compiler
        r0 = *s++; r1 = *s++;
        r2 = *s++; r3 = *s++;
        r4 = *s++; r5 = *s++;
        r6 = *s++; r7 = *s++;

        __asm__ volatile (
            // Store Queue 0 (sq0) operations
            "mov.l %2,@%0  ; mov.l %3,@(4,%0) \n\t"
            "mov.l %4,@(8,%0) ; mov.l %5,@(12,%0) \n\t"
            "mov.l %6,@(16,%0) ; mov.l %7,@(20,%0) \n\t"
            "mov.l %8,@(24,%0) ; mov.l %9,@(28,%0) \n\t"
            "pref @%0 \n\t"
            "ocbi @%1 \n\t"
            "add #32,%0 \n\t"

            // Load next batch
            : "+r" (d)
            : "r" (dest), "r" (r0), "r" (r1), "r" (r2), "r" (r3), 
              "r" (r4), "r" (r5), "r" (r6), "r" (r7)
            : "memory"
        );

        // Expose loads to the compiler for the next batch
        r0 = *s++; r1 = *s++;
        r2 = *s++; r3 = *s++;
        r4 = *s++; r5 = *s++;
        r6 = *s++; r7 = *s++;

        __asm__ volatile (
            // Store Queue 1 (sq1) operations
            "mov.l %2,@%0  ; mov.l %3,@(4,%0) \n\t"
            "mov.l %4,@(8,%0) ; mov.l %5,@(12,%0) \n\t"
            "mov.l %6,@(16,%0) ; mov.l %7,@(20,%0) \n\t"
            "mov.l %8,@(24,%0) ; mov.l %9,@(28,%0) \n\t"
            "pref @%0 \n\t"
            "ocbi @%1 \n\t"
            "add #32,%0 \n\t"

            : "+r" (d)
            : "r" (dest), "r" (r0), "r" (r1), "r" (r2), "r" (r3), 
              "r" (r4), "r" (r5), "r" (r6), "r" (r7)
            : "memory"
        );
    }

    // Clear SQ registers after operation
    *((uint32 *)(0xe0000000)) = 0;
    *((uint32 *)(0xe0000020)) = 0;
}


void DreamcastUi::UpdateDisplay() {
#ifdef RGBA_CODEPATH
	bit64_sq_cpy(vram_l, main_surface->pixels(), 320*240*4);
	vid_waitvbl();
#else
	bit64_sq_cpy(front_tex, pix_dc, 512*240*2);
	
    pvr_wait_ready();
    pvr_scene_begin();

    pvr_list_begin(PVR_LIST_OP_POLY);

	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_ARGB1555|PVR_TXRFMT_NONTWIDDLED, 512, 256, front_tex, PVR_FILTER_NEAREST);

    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));

    vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);    
    vert.oargb = 0;
    vert.flags = PVR_CMD_VERTEX;
    
    vert.x = 0;
    vert.y = 0;
    vert.z = 1;
    vert.u = 0.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));
    
    vert.x = 640+240+90+34+13+4+2+1;
    vert.y = 0;
    vert.z = 1;
    vert.u = 1.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));
    
    vert.x = 0;
    vert.y = 512;
    vert.z = 1;
    vert.u = 0.0;
    vert.v = 1.0;
    pvr_prim(&vert, sizeof(vert));
    
    vert.x = 640+240+90+34+13+4+2+1;
    vert.y = 512;
    vert.z = 1;
    vert.u = 1.0;
    vert.v = 1.0;
    vert.flags = PVR_CMD_VERTEX_EOL;
    pvr_prim(&vert, sizeof(vert));
	

    pvr_list_finish();

    pvr_scene_finish();
#endif
}

void DreamcastUi::SetTitle(const std::string &title) {
}

bool DreamcastUi::ShowCursor(bool flag) {
	return 0;
}

void DreamcastUi::vGetConfig(Game_ConfigVideo& cfg) const {
#ifdef __wii__
	cfg.renderer.Lock("SDL1 (Software, Wii)");
#else
	cfg.renderer.Lock("SDL1 (Software)");
#endif

	cfg.fullscreen.SetOptionVisible(toggle_fs_available);
	cfg.pause_when_focus_lost.SetOptionVisible(toggle_fs_available);

#ifdef SUPPORT_ZOOM
	cfg.window_zoom.SetOptionVisible(true);
	cfg.window_zoom.Set(current_display_mode.zoom);
#endif
}
