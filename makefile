install:
	@echo Installation in progress. Please wait
	@$(CC) sffdecompiler.c -g0 -O0 -o /bin/sffdecompiler
	@chmod ugo+rx /bin/sffdecompiler
	@echo Installation was successfully completed
uninstall:
	@rm /bin/sffdecompiler
	@echo Uninstallation was successfully completed