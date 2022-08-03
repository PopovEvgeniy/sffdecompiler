install:
	@echo Installation in progress. Please wait
	@gcc sffdecompiler.c -g0 -O0 -o /bin/sffdecompiler
	@chmod ugo+rx /bin/sffdecompiler
	@echo Installation successfully complete
uninstall:
	@rm -f /bin/sffdecompiler
	@echo Uninstallation successfully complete