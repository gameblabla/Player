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
#include <cstring>
#include <csetjmp>
#include <vector>
#include <fstream>

#include "output.h"
#include "image_png.h"

#include "spng.h"

static const char* const ERROR_MSG = "Error reading PNG data.";

static bool ReadPNGData(const unsigned char* png_data, size_t len, bool transparent, ImageOut& output) {
    spng_ctx *ctx = spng_ctx_new(0);
    unsigned char* pixel_buffer = nullptr;
    size_t image_size = 0;

    if (!ctx) {
        Output::Warning(ERROR_MSG);
        return false;
    }

    // Set an input buffer
    if (spng_set_png_buffer(ctx, png_data, len) != 0) {
        goto error;
    }

    // Retrieve the image header information
    struct spng_ihdr ihdr;
    if (spng_get_ihdr(ctx, &ihdr) != 0) {
        goto error;
    }

    // Decode image to buffer
    image_size = ihdr.width * ihdr.height * 4; // RGBA8
    pixel_buffer = (unsigned char*)malloc(image_size);
    if (!pixel_buffer) {
        goto error;
    }

    if (spng_decode_image(ctx, pixel_buffer, image_size, SPNG_FMT_RGBA8, SPNG_DECODE_TRNS) != 0) {
		free(pixel_buffer);
        goto error;
    }

    output.bpp = 32;

    // Check for indexed color
    if (ihdr.color_type == SPNG_COLOR_TYPE_INDEXED) {
        // Get the palette
        struct spng_plte plte = {0};
        if (spng_get_plte(ctx, &plte) == 0 && plte.n_entries > 0) {
            if (transparent) {
                uint32_t index_color = *(uint32_t*)(&plte.entries[0]);
                for (size_t i = 0; i < image_size; i+=4) {
                    uint32_t* pixel = (uint32_t*)&pixel_buffer[i];
                    if (*pixel == index_color) {
                        *pixel &= 0x00FFFFFF; // Set alpha channel to 0
                    }
                }
            }
            output.bpp = 8;
        }
    }

    // Set output image properties
    output.pixels = pixel_buffer;
    output.width = ihdr.width;
    output.height = ihdr.height;

    spng_ctx_free(ctx);
    return true;

error:
    Output::Warning(ERROR_MSG);
	spng_ctx_free(ctx);
    return false;
}

bool ImagePNG::Read(const void* buffer, unsigned len, bool transparent, ImageOut& output)  {
    const unsigned char* png_data = static_cast<const unsigned char*>(buffer);
    return ReadPNGData(png_data, len, transparent, output);
}

bool ImagePNG::Read(Filesystem_Stream::InputStream& stream, bool transparent, ImageOut& output) {
    std::vector<unsigned char> png_data((std::istreambuf_iterator<char>(stream)),
                                         std::istreambuf_iterator<char>());
    return ReadPNGData(png_data.data(), png_data.size(), transparent, output);
}

bool ImagePNG::Write(std::ostream& os, uint32_t width, uint32_t height, uint32_t* data) {
	return false;
}
