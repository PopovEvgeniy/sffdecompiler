                              SFF DECOMPILER

                 Mugen graphics extractor by Popov Evgeniy Alekseyevich

            Some code taken from Sff extract by Osuna Richert Christophe

                             Version 1.9.6

System requirement

68 kilobytes free space on storage media.

Description and usage

This program let you extract a graphics from games that built on MUGEN game engine.
This program takes a target file name as command line argument. 

Exit codes

0 - Operation successfully complete.
1 - Can't open input file.
2 - Can't create output file.
3 - Can't jump to target offset.
4 - Can't allocate memory.
5 - Invalid format.

License

This program distributed under GNU GENERAL PUBLIC LICENSE.

Source code

The program source code compiled under Open Watcom.
But you can do it under any modern C compiler.
Source code can be compiled for wide range operating systems, but you need to create makefile or build script by yourself.
I provide only two things: compiled the binary file for Windows and makefile for Linux.

Install and uninstall under Linux

Follow these steps:

1.Extract content of source code archive.
2.Open terminal and go to source code directory.
3.
Run as root for install: make install
Run as root for uninstall: make uninstall

Contact

You can send me a letter to tuzik87@inbox.ru.

Version history

1.0 - Initial version.
1.0.1 - 1.5.9 - Old stable builds.
1.6 - 1.6.8 - Small changes.
1.6.9 - Source code improved. Linux support added.
1.6.9.0.1 – Documentation updated.
1.7 - 1.7.6 - Small changes.
1.7.6.1 - Makefile updated.
1.7.7 - 1.8 - Small changes.
1.8.1 - Small bug fixed.
1.8.2 - 1.8.8 - Small changes.
1.8.8.1 - 1.8.8.2 - Makefile updated.
1.8.9 - 1.9.6 - Small changes.