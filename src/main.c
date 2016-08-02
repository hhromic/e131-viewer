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
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <e131.h>
#include <arpa/inet.h>

void output(const uint8_t *data, const size_t size) {
  if (size % 3 != 0) return;
  for (size_t pos = 0; pos < size; pos+=3) {
    uint8_t r = data[pos] * 5 / 0xFF;
    uint8_t g = data[pos + 1] * 5 / 0xFF;
    uint8_t b = data[pos + 2] * 5 / 0xFF;
    fprintf(stderr, "\x1b[48;5;%dm ", 16 + (r * 36) + (g * 6) + b);
  }
  fprintf(stderr, "\x1b[0m\n");
}

int main() {
  int e131_fd;
  e131_packet_t e131_packet;
  uint8_t curr_sequence = 0x00;

  // create a socket for E1.31
  if ((e131_fd = e131_socket()) < 0)
    err(EXIT_FAILURE, "e131_socket");

  // bind the socket to the default E1.31 port
  if (e131_bind(e131_fd, E131_DEFAULT_PORT) < 0)
    err(EXIT_FAILURE, "e131_bind");

  // loop to receive E1.31 packets
  fprintf(stderr, "waiting for E1.31 packets ...\n");
  for (;;) {
    if (e131_recv(e131_fd, &e131_packet) < 0)
      err(EXIT_FAILURE, "e131_recv");
    if (e131_pkt_validate(&e131_packet) != E131_ERR_NONE)
      continue;
    if (e131_packet.frame.sequence_number != curr_sequence++) {
      curr_sequence = e131_packet.frame.sequence_number + 1;
      continue;
    }
    output(e131_packet.dmp.property_values + 1, \
      ntohs(e131_packet.dmp.property_value_count) - 1);
  }

  // finished
  close(e131_fd);
  exit(EXIT_SUCCESS);
}
