
#include "shell.h"

void main()
{
    int fd;
    char buffer[BUFFER_SIZE];

    // Create the FIFO (named pipe) if it doesn't exist
    mkfifo(FIFO_FILE, 0666);

    printf("En attente...\n");
    // Fermeture brutale avec Ctrl+C
    if (access(FIFO_FILE, F_OK) == -1)
    {
        perror("Erreur de création du tube nommé");
        exit(EXIT_FAILURE);
    }

    fd = open(FIFO_FILE, O_RDONLY);

    printf("Shell connecté...\n");

    while (1)
    {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
        if (bytes_read > 0)
        {
            // buffer[bytes_read] = '\0'; // Retrait du caractère de fin de chaîne
            printf("%s", buffer);
        }
        else if (bytes_read == 0)
        {
            printf("Shell fermé. Fermeture...\n");
            break;
        }
        else
        {
            perror("Erreur de lecture du tube nommé");
            break;
        }
    }

    close(fd);
    unlink(FIFO_FILE);
}
