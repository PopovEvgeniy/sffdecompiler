install:
	@echo Installation in progress. Please wait
	@gcc sffdecompiler.c -g0 -O0 -o sffdecompiler
	@chmod ugo+rx sffdecompiler
	@cp -f -p sffdecompiler /bin/
	@echo Installation was successfuly complete
uninstall:
	@rm -f /bin/sffdecompiler
	@echo Uninstallation was successfuly complete