# E1.31 (sACN) Xterm256 Console Viewer

## Compiling

To compile the viewer, simply use the `make` utility:

    $ make clean
    $ make

You must have the `libe131` ([get it here][libe131]) development library installed on your system. Once compiled, you can install the output binary file wherever you want.

[libe131]: https://github.com/hhromic/libe131/releases/latest

## Running

To run the program, you can supply an optional DMX universe number for joining a multicast group.

    Usage: e131-viewer [-h] [-u universe]
    View E1.31 (sACN) network data using xterm256 ANSI colors.

    Options:
      -h             show this help screen
      -u universe    DMX universe number for Multicast (default: 1)

Example invocation for viewing E1.31 RGB data using DMX universe `2`:

    $ ./e131-viewer -u 2

## License

This software is under the **Apache License 2.0**.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
