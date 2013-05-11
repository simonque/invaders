/*
 images.h
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

#ifndef _IMAGES_H_
#define _IMAGES_H_

#include <stdint.h>

struct SDL_Surface;

#ifdef __AVR__
#define MAX_NUM_IMAGES         16
#else
#define MAX_NUM_IMAGES         32
#endif

#define MAX_STRING_LENGTH     256

namespace Graphics {

      struct ImageListEntry {
#ifdef __AVR__
          union {
              const void*         ptr;
              uint32_t           addr;
          };
          uint16_t      size;
          ImageListEntry() : addr(0), size(0) {
          }
          ImageListEntry(const void* ptr, uint16_t size) {
              this->ptr = ptr;
              this->size = size;
          }
#endif
      };

      class Images {
      private:
#ifdef __AVR__
          struct ImageListEntry images[MAX_NUM_IMAGES];
#else
          SDL_Surface* images[MAX_NUM_IMAGES];
          char filenames[MAX_STRING_LENGTH][MAX_NUM_IMAGES];
#endif

          int num_images;
      public:
          Images();
          ~Images();

          // Loads images specified by an array of strings |filenames|,
          // with the end of the array indicated by a null pointer.  Returns
          // true on success.
          bool load_images(const char** filenames);

          // Load image list specified by an array of <addr, size> pairs. The
          // array is terminated by addr=0 and size=0.
          bool load_images(const ImageListEntry* image_list);

          // Returns the total amount of VRAM used by images.
          uint16_t get_total_image_size() const;

          // Image lookup by filename.
          SDL_Surface *get_image(const char* filename);

          // Image lookup by index and type.

          // Returns offset in VRAM.
          uint16_t get_image_offset(int type);
          // Returns SDL surface containing image.
          SDL_Surface *get_image(int type, int index);

          // Get index of image with file name.  Returns -1 if not found.
          int get_image_index(const char* filename);
      };

}  // namespace Graphics

#endif  // _IMAGES_H_
