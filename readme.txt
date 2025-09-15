                              SFF DECOMPILER

                 Mugen graphics extractor by Popov Evgeniy Alekseyevich

            Some code taken from Sff extract by Osuna Richert Christophe

                             Version 2.0.4.1

The program description.

This program lets you extract graphics from games that are built on the MUGEN game engine.
This program takes a target file name as the command-line argument. 

Exit codes.

0: The operation was successfully completed.
1: Can't open the input file.
2: Can't create the output file.
3: Can't jump to the target offset.
4: Can't allocate memory.
5: The invalid format.

License.

This program is distributed under the GNU GENERAL PUBLIC LICENSE.

The source code

The code was compiled under Open Watcom.
But you can compile it under any modern C compiler.
The source code can be compiled for a wide range of operating systems, but you need to create a makefile or a build script by yourself.
I provide only two things: the binary file for Windows and the makefile for Linux.

Install and uninstall under Linux.

Follow these steps:

1. Extract the content of the source code archive.
2. Open a terminal and go to the source code directory.
3.
Run as root to install: make install
Run as root to uninstall: make uninstall

Version history.

1.0: The initial version.
1.0.1-1.5.9: The old builds.
1.6-1.6.8: The small changes.
1.6.9: The source code has improved. Linux support has been added.
1.6.9.0.1: The documentation has been updated.
1.7-1.7.6: The small changes.
1.7.6.1: The makefile has been updated.
1.7.7-1.8: The small changes.
1.8.1: A small bug has been fixed.
1.8.2-1.8.8: The small changes.
1.8.8.1-1.8.8.2: The makefile has been updated.
1.8.9-1.9.6: The small changes.
1.9.7: Visual C++ support has improved.
1.9.7.1: The documentation has been updated.
1.9.8: The small changes.
1.9.8.1-1.9.8.3: The documentation has been updated.
1.9.9: The source code was recompiled under Tiny C Compiler.
2.0: The extraction process has improved.
2.0.0.1: The makefile has been updated.
2.0.1-2.0.3: The small changes.
2.0.3.1: The documentation has been updated.
2.0.4: The small changes.
2.0.4.1: The documentation has been updated.