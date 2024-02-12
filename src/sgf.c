/*
 * Systèmes de Gestion de Fichiers (SGF)
 * Ce permet de simuler un système de gestion de fichiers simple
 * avec des blocs de données et des métadonnées.
 * L'allocation des blocs est chaînée.
 * Le fichier binaire simule le disque.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 64
#define NUM_BLOCKS 100
#define MAX_FILENAME_SIZE 32
#define ID_SIZE 16
#define FILENAME "filesystem.bin"

typedef struct Metadata
{
    char filename[MAX_FILENAME_SIZE];
    int firstBlock;
} Metadata;

typedef struct Inode
{
    int fileSize;
    char filename[MAX_FILENAME_SIZE];
    int nodeNum;

} Inode;

/// @brief Structure pour un bloc de données
/// @param metadata Métadonnées du bloc
/// @param data Données du bloc
/// @param nextBlock Index du bloc suivant dans le fichier binaire, -1 si c'est le dernier bloc
typedef struct Block
{
    Metadata metadata;
    char data[BLOCK_SIZE - sizeof(int)];
    int nextBlock;
} Block;

/// @brief Simule la création d'un fichier vide dans le système de fichiers
/// @param filename Le nom du fichier
void mycreate(const char *filename)
{
    FILE *file = fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fseek(file, 0, SEEK_END);                 // Se déplacer à la fin du fichier
    int fileSize = ftell(file);               // Taille actuelle du fichier
    int numBlocks = fileSize / sizeof(Block); // Nombre de blocs actuels dans le fichier

    // Créer un nouveau bloc pour les métadonnées du fichier
    Block newBlock;
    newBlock.nextBlock = -1; // Pas de bloc suivant pour l'instant
    Metadata metadata;
    strcpy(metadata.filename, filename);
    metadata.firstBlock = numBlocks + 1; // Le premier bloc de données sera le prochain bloc
    newBlock.metadata = metadata;

    // Écrire les métadonnées dans le dernier bloc du fichier
    fseek(file, fileSize, SEEK_SET);
    fwrite(&newBlock, sizeof(Block), 1, file);

    fclose(file);
}

/// @brief Simule l'écriture de données dans un fichier du système de fichiers
/// @param filename Le nom du fichier
/// @param data Les données à écrire
void mywrite(const char *filename, const char *data)
{
    FILE *file = fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        printf("Le fichier n'existe pas.\n");
        return;
    }

    // Rechercher les métadonnées du fichier
    Block firstBlock;
    fseek(file, 0, SEEK_SET);
    fread(&firstBlock, sizeof(Block), 1, file);
    while (1)
    {
        if (strcmp(firstBlock.metadata.filename, filename) == 0)
            break;
        if (firstBlock.nextBlock == -1)
        {
            printf("Le fichier n'existe pas.\n");
            return;
        }
        fseek(file, firstBlock.nextBlock * sizeof(Block), SEEK_SET);
        fread(&firstBlock, sizeof(Block), 1, file);
    }

    // Ajouter les données dans le fichier
    int dataSize = strlen(data);
    int numBlocks = dataSize / (BLOCK_SIZE - sizeof(int)) + 1;
    int fileSize = ftell(file);
    int lastBlockIndex = fileSize / sizeof(Block) - 1;
    int firstBlockIndex = firstBlock.metadata.firstBlock;
    int currentBlockIndex = firstBlockIndex;

    // Ajouter les données dans le fichier bloc par bloc
    for (int i = 0; i < numBlocks; ++i)
    {
        Block block;
        block.nextBlock = -1;

        if (i == 0)
        {
            // Si c'est le premier bloc, lire le bloc existant à l'index courant
            fseek(file, currentBlockIndex * sizeof(Block), SEEK_SET);
            fread(&block, sizeof(Block), 1, file);
            fseek(file, currentBlockIndex * sizeof(Block), SEEK_SET);
        }
        else
        {
            // Sinon, déplacer le curseur à la fin du fichier et mettre à jour le bloc suivant
            fseek(file, 0, SEEK_END);
            block.nextBlock = ftell(file) / sizeof(Block);
        }

        if (i == 0)
        {
            // Si c'est le premier bloc, copier les données directement
            strcpy(block.data, data);
        }
        else
        {
            // Sinon, copier une partie des données dans le bloc
            int start = i * (BLOCK_SIZE - sizeof(int));
            int end = (i + 1) * (BLOCK_SIZE - sizeof(int));
            if (end > dataSize)
                end = dataSize;
            strncpy(block.data, data + start, end - start);
        }

        // Écrire le bloc dans le fichier
        fwrite(&block, sizeof(Block), 1, file);
    }

    fclose(file);
}

void myread(const char *filename)
{
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL)
    {
        printf("Le fichier n'existe pas.\n");
        return;
    }

    // Rechercher les métadonnées du fichier
    Block firstBlock;
    fseek(file, 0, SEEK_SET);
    fread(&firstBlock, sizeof(Block), 1, file);
    while (1)
    {
        if (strcmp(firstBlock.metadata.filename, filename) == 0)
            break;
        if (firstBlock.nextBlock == -1)
        {
            printf("Le fichier n'existe pas.\n");
            return;
        }
        fseek(file, firstBlock.nextBlock * sizeof(Block), SEEK_SET);
        fread(&firstBlock, sizeof(Block), 1, file);
    }

    // Lire les données du fichier
    int currentBlockIndex = firstBlock.metadata.firstBlock;
    while (currentBlockIndex != -1)
    {
        Block block;
        fseek(file, currentBlockIndex * sizeof(Block), SEEK_SET);
        fread(&block, sizeof(Block), 1, file);
        printf("%s", block.data);
        currentBlockIndex = block.nextBlock;
    }

    fclose(file);
}

int main()
{
    // Créer le fichier
    FILE *file = fopen(FILENAME, "wb");
    fclose(file);

    const char *filename = "test.txt";
    // Créer un fichier
    mycreate(filename);

    // Écrire des données dans le fichier
    mywrite(filename, "Ceci est une première ligne.12345678901234567890\n");

    // Lire les données du fichier
    myread(filename);

    // Ajouter plus de données dans le fichier
    mywrite(filename, "Ceci est une deuxième ligne.\n");

    // Lire à nouveau les données du fichier
    myread(filename);

    fopen(FILENAME, "rb+");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    // Afficher la taille du fichier
    fseek(file, 0, SEEK_END);
    printf("Taille du fichier : %ld octets\n", ftell(file));
    printf("Taille du bloc : %ld octets\n", sizeof(Block));
    printf("Nombre de blocs : %ld\n", ftell(file) / sizeof(Block));
    printf("Nombre de blocs disponibles : %ld\n", NUM_BLOCKS - ftell(file) / sizeof(Block));
    fclose(file);
    return 0;
}