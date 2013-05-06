/*
 screen.cpp
 Classic Invaders

 Copyright (c) 2013, Todd Steinackle, Simon Que
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted
 provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions
 and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the documentation and/or other
 materials provided with the distribution.

 * Neither the name of The No Quarter Arcade (http://www.noquarterarcade.com/)  nor the names of
 its contributors may be used to endorse or promote products derived from this software without
 specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "screen.h"

#include <stdio.h>

#ifdef __AVR__
    #include "cc_core.h"
#endif

#include "game_entity.h"
#include "images.h"

namespace Graphics {
    Screen::Screen() : num_blits(0),
                       image_lib(NULL) {}

    bool Screen::init() {
#ifdef __AVR__
        CC_Init();
        printf("ChronoCube initialization complete.\n");
#else
        int flags = SDL_SWSURFACE;
        screen = SDL_SetVideoMode(screen_w, screen_h, 16, flags);
        if (screen == NULL) {
            fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
            return false;
        }

        clip.x = 0; clip.y = 50; clip.w = screen_w; clip.h = 515;
        SDL_SetClipRect(screen, &clip);
#endif  // defined(__AVR__)
        return true;
    }

    void Screen::set_image_lib(Images* images) {
        image_lib = images;

#ifndef __AVR__
        wave_background = images->get_image("wave_background.png");
        background = images->get_image("background.png");
        ui_header = images->get_image("ui_header.png");
        ui_points = images->get_image("ui_points.png");
#endif
    }

    void Screen::schedule_blit(const GameEntities::GameEntity* object) {
        uint8_t type = object->get_type();
        uint8_t image_index = object->get_current_image();
        int x = object->get_x();
        int y = object->get_y();

        if (num_blits >= max_updates) {
            fprintf(stderr, "Exceeded max number of blits (%d).\n", max_updates);
            return;
        }

#ifndef __AVR__
        blit* update = &blits[num_blits++];
        update->type = type;
        update->image_index = image_index;
        update->x = x;
        update->y = y;
#endif
    }

    void Screen::flush_blits() {
#ifndef __AVR__
        for (int i = 0; i < num_blits && image_lib; ++i) {
            SDL_Surface* image =
                image_lib->get_image(blits[i].type, blits[i].image_index);
            if (image) {
                SDL_Rect dst;
                dst.x = blits[i].x;
                dst.y = blits[i].y;
                SDL_BlitSurface(image, NULL, screen, &dst);
            }
        }
        num_blits = 0;
        SDL_UpdateRect(screen, clip.x, clip.y, clip.w, clip.h);
#endif
    }

    void Screen::update() {
#ifndef __AVR__
        SDL_BlitSurface(wave_background, NULL, screen, NULL);
#endif
        flush_blits();
    }
}
