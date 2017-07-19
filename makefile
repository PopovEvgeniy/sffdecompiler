install:
	@echo Compiling the program source code. Please wait
	@gcc sffdecompiler.c -g0 -O0 -o sffdecompiler
	@echo Compilation successfuly complete
	@chmod ugo+rx sffdecompiler
	@echo Enter you user password for do last installation step
	@sudo cp -f -p sffdecompiler /bin/
	@echo Installation complete
uninstall:
	@echo Enter you user password for uninstall this software
	@sudo rm -f /bin/sffdecompiler
	@echo Uninstallation complete