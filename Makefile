all: bin bin/tmp shell ecran

bin: 
	@if [ ! -d "bin" ]; then mkdir bin; fi

bin/tmp: 
	@if [ ! -d "bin/tmp" ]; then mkdir bin/tmp; fi

shell: 
	gcc -o bin/shell src/shell.c src/noyau.c

noyau: 
	$(CC) -o bin/noyau src/noyau.c

ecran: 
	gcc -o bin/ecran src/ecran.c