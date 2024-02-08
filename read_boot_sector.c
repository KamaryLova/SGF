#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Structure representing the BIOS Parameter Block (BPB) of the boot sector
typedef struct {
    uint8_t  jmpBoot[3];          // Jump instruction
    char     oemName[8];          // OEM name
    uint16_t bytesPerSector;      // Bytes per sector
    uint8_t  sectorsPerCluster;   // Sectors per cluster
    uint16_t reservedSectors;     // Reserved sectors count
    uint8_t  numFATs;             // Number of FATs
    uint16_t rootEntries;         // Number of root directory entries (0 for FAT32)
    uint16_t totalSectors16;      // Total sectors (only used if totalSectors32 is 0)
    uint8_t  media;               // Media descriptor
    uint16_t sectorsPerFAT16;     // Sectors per FAT (only used if FAT32 isn't present)
    uint16_t sectorsPerTrack;     // Sectors per track
    uint16_t numHeads;            // Number of heads
    uint32_t hiddenSectors;       // Hidden sectors count
    uint32_t totalSectors32;      // Total sectors (used for FAT32)
    uint32_t sectorsPerFAT32;     // Sectors per FAT (only used for FAT32)
    uint16_t flags;               // Flags
    uint16_t version;             // File system version
    uint32_t rootCluster;         // Root directory cluster (only used for FAT32)
    uint16_t fsInfoSector;        // FS Info sector (only used for FAT32)
    uint16_t backupBootSector;    // Backup boot sector (only used for FAT32)
    uint8_t  reserved[12];        // Reserved
    uint8_t  driveNumber;         // Drive number
    uint8_t  reserved1;           // Reserved
    uint8_t  bootSignature;       // Boot signature
    uint32_t volumeID;            // Volume ID
    char     volumeLabel[11];     // Volume label
    char     fsType[8];           // File system type
    char     bootCode[448];       // Boot code
    uint16_t bootSectorSig;       // Boot sector signature (0xAA55)
} __attribute__((packed)) BootSector;

// Structure representing a directory entry in the root directory
typedef struct {
    char name[11];          // 8.3 format name
    uint8_t attributes;     // File attributes
    uint32_t firstCluster;  // First cluster of the file
    uint32_t fileSize;      // File size in bytes
} __attribute__((packed)) DirectoryEntry;

BootSector bpb;
DirectoryEntry dir_entry;

// Function to create a directory in the root directory of the FAT32 file system
int fat32_mkdir(FILE *fp, const char *dirname) {
    // Seek to the beginning of the root directory
    fseek(fp, 0x2600, SEEK_SET); // Adjust this offset based on your specific FAT32 layout

    // Create a directory entry structure
    DirectoryEntry entry;
    memset(&entry, 0, sizeof(DirectoryEntry));

    // Set directory name
    strncpy(entry.name, dirname, 11);

    // Set attribute to directory
    entry.attributes = 0x10;

    // Find an empty slot in the root directory
    int found = 0;
    while (fread(&entry, sizeof(DirectoryEntry), 1, fp) == 1) {
        if (entry.name[0] == 0x00 || entry.name[0] == 0xE5) { // Empty slot or deleted file
            found = 1;
            break;
        }
    }

    // If an empty slot is found, write the new directory entry
    if (found) {
        fseek(fp, -sizeof(DirectoryEntry), SEEK_CUR);
        fwrite(&entry, sizeof(DirectoryEntry), 1, fp);
        fflush(fp);
        printf("Directory '%s' created successfully.\n", dirname);
        return 0;
    } else {
        printf("No space available in root directory to create directory '%s'.\n", dirname);
        return 1;
    }
}

// Function to create a file in the specified directory of the FAT32 file system
int fat32_touch(FILE *fp, const char *filename, uint32_t directoryCluster) {
    // Calculate the starting offset of the directory
    uint32_t offset = (directoryCluster - 2) * 512 + 0x10000; // Adjust this offset based on your specific FAT32 layout

    printf("%zu", offset);

    // Seek to the beginning of the directory
    fseek(fp, offset, SEEK_SET);



    // Create a directory entry structure
    DirectoryEntry entry;
    memset(&entry, 0, sizeof(DirectoryEntry));

    // Set file name
    strncpy(entry.name, filename, 11);

    // Set attribute to archive
    entry.attributes = 0x20;

    // Find an empty slot in the directory
    int found = 0;
    while (fread(&entry, sizeof(DirectoryEntry), 1, fp) == 1) {
        if (entry.name[0] == 0x00 || entry.name[0] == 0xE5) { // Empty slot or deleted file
            found = 1;
            break;
        }
    }

    // If an empty slot is found, write the new file entry
    if (found) {
        fseek(fp, -sizeof(DirectoryEntry), SEEK_CUR);
        fwrite(&entry, sizeof(DirectoryEntry), 1, fp);
        fflush(fp);
        printf("File '%s' created successfully.\n", filename);
        return 0;
    } else {
        printf("No space available in directory to create file '%s'.\n", filename);
        return 1;
    }
}

// Function to list files and directories in a directory of the FAT32 file system
void fat32_ls(FILE *fp, const char *dirname) {
    // Calculate the starting offset of the directory
    uint32_t offset = (bpb.reservedSectors + bpb.numFATs * bpb.sectorsPerFAT32) * bpb.bytesPerSector +
                      bpb.rootCluster * 512; // Assuming 512 bytes per cluster

    // Seek to the beginning of the directory
    fseek(fp, offset, SEEK_SET);

    // Loop through directory entries
    DirectoryEntry entry;
    while (fread(&entry, sizeof(DirectoryEntry), 1, fp) == 1) {
        if (entry.name[0] == 0x00) // End of directory
            break;
        if (entry.name[0] != 0xE5 && entry.attributes != 0x0F) { // Not deleted or long file name
            // Print directory entry details
            printf("%s\n", entry.name);
        }
        // Seek to the next directory entry
        fseek(fp, sizeof(DirectoryEntry) - 1, SEEK_CUR);
    }
}

// Function to remove a file or directory from the FAT32 file system
int fat32_rm(FILE *fp, const char *name, int isDir) {
    // Calculate the starting offset of the directory
    uint32_t offset = (bpb.reservedSectors + bpb.numFATs * bpb.sectorsPerFAT32) * bpb.bytesPerSector +
                      bpb.rootCluster * 512; // Assuming 512 bytes per cluster

    // Seek to the beginning of the directory
    fseek(fp, offset, SEEK_SET);

    // Loop through directory entries
    DirectoryEntry entry;
    while (fread(&entry, sizeof(DirectoryEntry), 1, fp) == 1) {
        if (entry.name[0] == 0x00) // End of directory
            break;
        if (entry.name[0] != 0xE5 && entry.attributes != 0x0F) { // Not deleted or long file name
            if (strncmp(entry.name, name, 11) == 0) {
                if ((isDir && (entry.attributes & 0x10)) || (!isDir && !(entry.attributes & 0x10))) {
                    // Mark the directory entry as deleted
                    fseek(fp, -sizeof(DirectoryEntry), SEEK_CUR);
                    memset(&entry, 0xE5, sizeof(DirectoryEntry));
                    fwrite(&entry, sizeof(DirectoryEntry), 1, fp);
                    fflush(fp);
                    printf("'%s' removed successfully.\n", name);
                    return 0;
                } else {
                    printf("'%s' is not a %s.\n", name, isDir ? "directory" : "file");
                    return 1;
                }
            }
        }
        // Seek to the next directory entry
        fseek(fp, sizeof(DirectoryEntry) - 1, SEEK_CUR);
    }

    printf("'%s' not found.\n", name);
    return 1;
}

int main() {
    FILE *fp;

    // Open the disk image file in binary mode
    fp = fopen("disk_image.img", "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Seek to the beginning of the disk image (boot sector)
    fseek(fp, 0, SEEK_SET);

    // Read the boot sector into the BPB structure
    fread(&bpb, sizeof(BootSector), 1, fp);

    // Print some information from the boot sector
    printf("Bytes per sector: %u\n", bpb.bytesPerSector);
    printf("Sectors per cluster: %u\n", bpb.sectorsPerCluster);
    printf("Number of FATs: %u\n", bpb.numFATs);
    printf("Total sectors: %u\n", bpb.totalSectors32);
    printf("Sectors per FAT: %u\n", bpb.sectorsPerFAT32);
    printf("Volume label: %s\n", bpb.volumeLabel);

    // Create a file named "example.txt" in the root directory (cluster 2)
    fat32_touch(fp, "example.txt", 2);

    // Create a directory named "test" in the root directory
    fat32_mkdir(fp, "test");

    fat32_ls(fp, "test");

    // Remove a directory named "test" from the root directory
    // Note: Pass 1 as the second argument to remove a directory
    fat32_rm(fp, "test", 1);

    // Remove a file named "example.txt" from the root directory
    // Note: Pass 0 as the second argument to remove a file
    fat32_rm(fp, "example.txt", 0);

    // Close the disk image file
    fclose(fp);

    return 0;
}

