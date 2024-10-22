                              SFF DECOMPILER

                 Mugen graphics extractor by Popov Evgeniy Alekseyevich

            Some code taken from Sff extract by Osuna Richert Christophe

                             Version 1.9.8.1

System requirements

68 kilobytes of free space on storage media.

Description and usage

This program lets you extract graphics from games that are built on the MUGEN game engine.
This program takes a target file name as the command-line argument. 

Exit codes

0: Operation was successfully completed.
1: Can't open the input file.
2: Can't create the output file.
3: Can't jump to the target offset.
4: Can't allocate memory.
5: Invalid format.

License

This program is distributed under GNU GENERAL PUBLIC LICENSE.

Source code

The  code was compiled under Open Watcom.
But you can compile it under any modern C compiler.
The source code can be compiled for a wide range of operating systems, but you need to create a makefile or a build script by yourself.
I provide only two things: the binary file for Windows and the makefile for Linux.

Install and uninstall under Linux

Follow these steps:

1. Extract the content of the source code archive.
2. Open a terminal and go to the source code directory.
3.
Run as root for install: make install
Run as root for uninstall: make uninstall

Contact

You can send me a letter at tuzik87@inbox.ru.

Version history

1.0: Initial version.
1.0.1-1.5.9: The old stable builds.
1.6-1.6.8: The small changes.
1.6.9: The source code is improved. Linux support has been added.
1.6.9.0.1: Documentation updated.
1.7-1.7.6: The small changes.
1.7.6.1: The makefile updated.
1.7.7-1.8: The small changes.
1.8.1: A small bug is fixed.
1.8.2-1.8.8: The small changes.
1.8.8.1-1.8.8.2: The makefile updated.
1.8.9-1.9.6: The small changes.
1.9.7: Visual C++ support improved.
1.9.7.1: Documentation updated.
1.9.8: The small changes.
1.9.8.1: Documentation updated.