all: bin shell noyau ecran

bin: 
	@if [ ! -d "bin" ]; then mkdir bin; fi

shell: 
	gcc -o bin/shell src/shell.c

noyau: 
	gcc -o bin/noyau src/noyau.c

ecran: 
	gcc -o bin/ecran src/ecran.c