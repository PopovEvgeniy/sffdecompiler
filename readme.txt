                              SFF DECOMPILER

                 Mugen graphics extractor by Popov Evgeniy Alekseyevich

            Some code taken from Sff extract by Osuna Richert Christophe

                             Version 1.9.7.1

System requirements

68 kilobytes of free space on storage media.

Description and usage

This program lets you extract graphics from games that are built on MUGEN game engine.
This program takes a target file name as a command-line argument. 

Exit codes

0 - Operation successfully completed.
1 - Can't open input file.
2 - Can't create output file.
3 - Can't jump to target offset.
4 - Can't allocate memory.
5 - Invalid format.

License

This program is distributed under GNU GENERAL PUBLIC LICENSE.

Source code

The  code was compiled under Open Watcom.
But you can compile it under any modern C compiler.
Source code can be compiled for a wide range of operating systems, but you need to create a makefile or build script by yourself.
I provide only two things: the binary file for Windows and makefile for Linux.

Install and uninstall under Linux

Follow these steps:

1. Extract the content of the source code archive.
2. Open a terminal and go to the source code directory.
3.
Run as root for install: make install
Run as root for uninstall: make uninstall

Contact

You can send me at letter to tuzik87@inbox.ru.

Version history

1.0 — Initial version.
1.0.1 — 1.5.9 — Old stable builds.
1.6 — 1.6.8 — Small changes.
1.6.9 — Source code improved. Linux support has been added.
1.6.9.0.1 – Documentation updated.
1.7 — 1.7.6 — Small changes.
1.7.6.1 — Makefile updated.
1.7.7 — 1.8 — Small changes.
1.8.1 — Small bug fixed.
1.8.2 — 1.8.8 — Small changes.
1.8.8.1 — 1.8.8.2 — Makefile updated.
1.8.9 — 1.9.6 — Small changes.
1.9.7 — Visual C++ support improved.
1.9.7.1 – Documentation updated.