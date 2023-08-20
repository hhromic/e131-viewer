/**
 * E1.31 (sACN) viewer
 * Hugo Hromic - http://github.com/hhromic
 *
 * Copyright 2016 Hugo Hromic
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdint.h>

// write RGB color data to the console using xterm256 ANSI colors
void write_console(const uint8_t *data, size_t size) {
  // If size isn't a multiple of 3, just truncate the extra
  size = (size / 3) * 3;
  uint8_t r, g, b, output;
  for (size_t pos = 0; pos < size; pos+=3) {
    r = data[pos]; g = data[pos + 1]; b = data[pos + 2];
    if (r == g && g == b && r == b) { // use the grayscale palette
      output = r * 25 / 0xFF;
      if (output > 0 && output < 25) output += 231;
      if (output == 0) output = 16;
      if (output == 25) output = 231;
    } else { // use the 6x6x6 color cube
      output = 16 + ((r * 5 / 0xFF) * 36) + ((g * 5 / 0xFF) * 6) + (b * 5 / 0xFF);
    }
    fprintf(stderr, "\x1b[48;5;%dm ", output);
  }
  fprintf(stderr, "\x1b[0m\n");
}
