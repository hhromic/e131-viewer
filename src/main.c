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
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <e131.h>
#include <arpa/inet.h>
#include "prototypes.h"

int main(int argc, char **argv) {
  int opt;
  int e131_fd;
  uint16_t universe = 0x0001;
  e131_packet_t e131_packet;
  e131_error_t e131_error;
  uint8_t last_seq_number = 0x00;

  // program options
  while ((opt = getopt(argc, argv, "hu:")) != -1) {
    switch (opt) {
      case 'h':
        show_usage(argv[0]);
        exit(EXIT_SUCCESS);
      case 'u':
        sscanf(optarg, "%" SCNu16, &universe);
        if (universe < 0x0001 || universe > 0xf9ff) {
          fprintf(stderr, "error: universe must be between 1-63999\n");
          exit(EXIT_FAILURE);
        }
        break;
      default:
        fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  // create a socket for E1.31
  if ((e131_fd = e131_socket()) < 0)
    err(EXIT_FAILURE, "e131_socket");

  // bind the socket to the default E1.31 port and join multicast group
  if (e131_bind(e131_fd, E131_DEFAULT_PORT) < 0)
    err(EXIT_FAILURE, "e131_bind");
  if (e131_multicast_join(e131_fd, universe) < 0)
    err(EXIT_FAILURE, "e131_multicast_join");
  fprintf(stderr, "E1.31 multicast server listening on port %d\n", E131_DEFAULT_PORT);

  // loop to receive E1.31 packets
  fprintf(stderr, "waiting for E1.31 (sACN) data, use CTRL+C to stop\n");
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

    // write RGB data to the console
    write_console(e131_packet.dmp.prop_val + 1, \
      ntohs(e131_packet.dmp.prop_val_cnt) - 1);
    last_seq_number = e131_packet.frame.seq_number;
  }

  // finished
  close(e131_fd);
  exit(EXIT_SUCCESS);
}
