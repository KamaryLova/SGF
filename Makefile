CC=gcc

SRC_DIR=src
BUILD_DIR=bin

.PHONY: all shell noyau ecran clean always tools_fat

all: shell noyau ecran

disk_image: $(BUILD_DIR)/main_disk.img

shell: 
	$(CC) -o bin/shell src/shell.c

noyau: 
	$(CC) -o bin/noyau src/noyau.c

ecran: 
	$(CC) -o bin/ecran src/ecran.c

always:
		mkdir -p $(BUILD_DIR)

clean:
		rm -rf $(BUILD_DIR)/*