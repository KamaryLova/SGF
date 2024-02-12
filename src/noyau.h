#ifndef NOYAU_H
#define NOYAU_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Nombre maximum d'arguments pour une commande
#define MAX_ARGUMENTS 3

/// @brief Structure pour les commandes disponibles
/// @param command La commande
/// @param action La fonction à appeler pour cette commande
/// @param num_args Le nombre d'arguments pour cette commande
/// @param description La description de la commande
typedef struct
{
    const char *command;
    void (*action)(int[], char *[]);
    int num_args;
    const char *description;
} CommandAction;

/// @brief Fonction qui permet d'executer une commande dans le noyau
/// @param command La commande à executer
/// @param args Les arguments de la commande
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
void handleCommand(char *command, int pipefd_child_to_parent[], char *args[]);

/// @brief Démarrer le noyau
/// @param pipefd_parent_to_child Le pipe pour communiquer du parent vers l'enfant
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
void runNoyau(int pipefd_parent_to_child[], int pipefd_child_to_parent[]);

/// @brief Fonction pour écrire une réponse dans le pipe
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
/// @param response La réponse à écrire
void writeResponse(int pipefd_child_to_parent[], char *response);

/// @brief Commande exemple
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
void actionExample(int pipefd_child_to_parent[], char *args[]);

/// @brief Commande pour quitter le noyau
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
void actionExit(int pipefd_child_to_parent[], char *args[]);

/// @brief Commande pour dire bonjour
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
void actionHello(int pipefd_child_to_parent[], char *args[]);

/// @brief Commande pour afficher l'aide
/// @param pipefd_child_to_parent Le pipe pour communiquer de l'enfant vers le parent
void actionHelp(int pipefd_child_to_parent[], char *args[]);

#endif // NOYAU_H