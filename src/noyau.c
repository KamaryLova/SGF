#include "noyau.h"
#include "shell.h"

CommandAction commands[] = {
    {"exit", actionExit, 0, "Quitte le shell"},
    {"hello", actionHello, 1, "Dit bonjour"},
    {"help", actionHelp, 0, "Affiche l'aide"},
    {"example", actionExample, 2, "Exemple d'action avec des arguments"}};

const int NUM_COMMANDS = sizeof(commands) / sizeof(CommandAction);

void handleCommand(char *command, int pipefd_child_to_parent[], char *args[])
{
    for (int i = 0; i < NUM_COMMANDS; ++i)
    {
        if (strcmp(command, commands[i].command) == 0)
        {
            if (commands[i].num_args > 0)
            {
                commands[i].action(pipefd_child_to_parent, args);
            }
            else
            {
                commands[i].action(pipefd_child_to_parent, NULL);
            }
            return;
        }
    }
    // Command not found
    if (write(pipefd_child_to_parent[1], "Command not found", strlen("Command not found") + 1) == -1)
    {
        perror("write");
        _exit(EXIT_FAILURE);
    }
}

void runNoyau(int pipefd_parent_to_child[], int pipefd_child_to_parent[])
{
    char buffer[BUFFER_SIZE];
    close(pipefd_parent_to_child[1]); // Close unused write end
    close(pipefd_child_to_parent[0]); // Close unused read end

    ssize_t bytes_read;
    while ((bytes_read = read(pipefd_parent_to_child[0], buffer, BUFFER_SIZE)) > 0)
    {
        // Check for arguments
        char *args[MAX_ARGUMENTS];
        char *token = strtok(buffer, " ");
        int i = 0;
        if (token == NULL)
        {
            args[i] = NULL;
        }
        else
        {
            // Skip the first token (the command)
            token = strtok(NULL, " ");

            while (token != NULL && i < MAX_ARGUMENTS - 1)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL; // Null-terminate the array
        }
        handleCommand(buffer, pipefd_child_to_parent, args);
    }

    if (bytes_read == -1)
    {
        perror("read");
        _exit(EXIT_FAILURE);
    }
    printf("Noyau: Fin du processus enfant\n");
    close(pipefd_parent_to_child[0]);
    close(pipefd_child_to_parent[1]);
    _exit(EXIT_SUCCESS);
}

void writeResponse(int pipefd_child_to_parent[], char *response)
{
    if (write(pipefd_child_to_parent[1], response, strlen(response) + 1) == -1)
    {
        perror("write");
        _exit(EXIT_FAILURE);
    }
}

#pragma region CommandActions
// Toutes les actions pour les commandes disponibles sont définies ici

// Exemple d'action
void actionExample(int pipefd_child_to_parent[], char *args[])
{
    if (args[0] != NULL)
    {
        write(pipefd_child_to_parent[1], args[0], strlen(args[0]) + 1);
    }
    if (args[1] != NULL)
    {
        printf("Argument 2: %s\n", args[1]);
    }
    // Add more argument handling as needed...
}

void actionExit(int pipefd_child_to_parent[], char *args[])
{
    writeResponse(pipefd_child_to_parent, "Exiting...");
}

void actionHello(int pipefd_child_to_parent[], char *args[])
{
    char response[BUFFER_SIZE]; // Assuming BUFFER_SIZE is defined somewhere
    if (args[0] != NULL)
    {
        sprintf(response, "Hello %s", args[0]);
    }
    else
    {
        strcpy(response, "Hello, World!");
    }
    writeResponse(pipefd_child_to_parent, response);
}

void actionHelp(int pipefd_child_to_parent[], char *args[])
{
    char buffer[BUFFER_SIZE];
    strcpy(buffer, "Commandes disponibles :\n");
    for (int i = 0; i < NUM_COMMANDS; ++i)
    {
        strcat(buffer, "  ");
        strcat(buffer, commands[i].command);
        strcat(buffer, " - ");
        strcat(buffer, commands[i].description);

        if (i < NUM_COMMANDS - 1) // Ajouter une nouvelle ligne pour toutes les commandes sauf la dernière
            strcat(buffer, "\n");
    }
    writeResponse(pipefd_child_to_parent, buffer);
}

#pragma endregion