// shell.h - Header file for shell.c

#ifndef SHELL_H
#define SHELL_H

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define FIFO_FILE "tmp/ecran_fifo"

/// @brief Traitement de la commande, envoyée par le parent (shell) au noyau
/// @param command La commande à traiter
/// @param pipefd_parent_to_child Le pipe pour communiquer du parent vers l'enfant
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
void processCommand(char *command, int pipefd_parent_to_child[], int pipefd_child_to_parent[]);

/// @brief Envoie une chaîne de caractères à l'écran du shell par tube nommé
/// @param message La chaîne de caractères à envoyer
/// @param fd Le descripteur de fichier du tube nommé
void sendToScreen(char *message, int fd);

/// @brief Ouvre le tube nommé pour l'écran
int openFifoScreen();

/// @brief Ferme le tube nommé pour l'écran
/// @param fd Le descripteur de fichier du tube nommé
void closeFifoScreen(int fd);

#endif // SHELL_H