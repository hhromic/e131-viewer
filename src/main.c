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

void write_console(const uint8_t *data, const size_t size) {
  if (size % 3 != 0) return;
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

int main() {
  int e131_fd;
  e131_packet_t e131_packet;
  e131_error_t e131_error;
  uint8_t last_seq_number = 0x00;

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
    if ((e131_error = e131_pkt_validate(&e131_packet)) != E131_ERR_NONE) {
      fprintf(stderr, "e131_pkt_validate: %s\n", e131_strerror(e131_error));
      continue;
    }
    if (e131_pkt_discard(&e131_packet, last_seq_number)) {
      fprintf(stderr, "warning: out of order E1.31 packet received\n");
      last_seq_number = e131_packet.frame.seq_number;
      continue;
    }
    write_console(e131_packet.dmp.prop_val + 1, \
      ntohs(e131_packet.dmp.prop_val_cnt) - 1);
    last_seq_number = e131_packet.frame.seq_number;
  }

  // finished
  close(e131_fd);
  exit(EXIT_SUCCESS);
}
