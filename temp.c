#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAX_MESSAGE_SIZE 1024
#define FIFO_NAME "myfifo"

// Structure pour les données du thread d'envoi
typedef struct {
    int socket_fd;
    int fifo_fd;
} ThreadData;

// Fonction pour le thread d'envoi de messages au serveur
void *sendMessageThread(void *arg) {
    ThreadData *threadData = (ThreadData *)arg;
    char message[MAX_MESSAGE_SIZE];

    while (1) {
        // Lire le message depuis le terminal
        printf("Saisissez un message : ");
        fgets(message, MAX_MESSAGE_SIZE, stdin);

        // Écrire le message dans le tube nommé
        write(threadData->fifo_fd, message, strlen(message));
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <adresse_IP_serveur> <port_serveur>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialiser le socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configurer les informations du serveur
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Se connecter au serveur
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Créer le tube nommé
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("Erreur lors de la création du tube nommé");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Ouvrir le tube nommé en mode écriture
    int fifo_fd = open(FIFO_NAME, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Erreur lors de l'ouverture du tube nommé");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Créer la structure de données pour le thread d'envoi
    ThreadData threadData;
    threadData.socket_fd = socket_fd;
    threadData.fifo_fd = fifo_fd;

    // Créer le thread d'envoi
    pthread_t sendThread;
    if (pthread_create(&sendThread, NULL, sendMessageThread, (void *)&threadData) != 0) {
        perror("Erreur lors de la création du thread d'envoi");
        close(socket_fd);
        close(fifo_fd);
        exit(EXIT_FAILURE);
    }

    // Attendre la fin du thread d'envoi
    pthread_join(sendThread, NULL);

    // Fermer les descripteurs de fichiers
    close(socket_fd);
    close(fifo_fd);

    // Supprimer le tube nommé
    unlink(FIFO_NAME);

    return 0;
}

