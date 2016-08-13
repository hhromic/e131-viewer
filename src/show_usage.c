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

// display program options and usage
void show_usage(char *prog_name) {
  fprintf(stderr, "Usage: %s [-h] [-u universe]\n", prog_name);
  fprintf(stderr, "View E1.31 (sACN) network data using xterm256 ANSI colors.\n");
  fprintf(stderr, "\nOptions:\n");
  fprintf(stderr, "  -h             show this help screen\n");
  fprintf(stderr, "  -u universe    DMX universe number for Multicast (default: 1)\n");
  fprintf(stderr, "\n");
}
