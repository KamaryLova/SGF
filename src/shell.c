#include "noyau.h"
#include "shell.h"

int main()
{
    int pipefd_parent_to_child[2], pipefd_child_to_parent[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    if (pipe(pipefd_parent_to_child) == -1 || pipe(pipefd_child_to_parent) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Processus enfant: Noyau
        runNoyau(pipefd_parent_to_child, pipefd_child_to_parent);
    }
    else
    {
        // Processus parent: Shell
        close(pipefd_parent_to_child[0]); // Close unused read end
        close(pipefd_child_to_parent[1]); // Close unused write end

        int fd = openFifoScreen();

        printf("Shell> ");

        while (fgets(buffer, BUFFER_SIZE, stdin) != NULL && strcmp(buffer, "exit\n") != 0)
        {
            buffer[strcspn(buffer, "\n")] = 0; // Remove newline

            processCommand(buffer, pipefd_parent_to_child, pipefd_child_to_parent);

            char response[BUFFER_SIZE];
            strcpy(response, buffer);
            strcat(response, "\n");

            // printf("%s\n", buffer);
            sendToScreen(response, fd);

            printf("Shell> ");
        }
        printf("Fermeture...\n");
        closeFifoScreen(fd);
        close(pipefd_parent_to_child[1]);
        close(pipefd_child_to_parent[0]);
        wait(NULL); // Attendre la fin du processus enfant
    }

    return EXIT_SUCCESS;
}

void processCommand(char *command, int pipefd_parent_to_child[], int pipefd_child_to_parent[])
{

    if (write(pipefd_parent_to_child[1], command, strlen(command) + 1) == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Attendre la réponse du noyau
    if (read(pipefd_child_to_parent[0], command, BUFFER_SIZE) == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (strcmp(command, "Exiting...") == 0)
    {
        printf("Exiting...\n");
        exit(EXIT_SUCCESS);
    }
}

int openFifoScreen()
{
    int fd;
    fd = open(FIFO_FILE, O_WRONLY);
    if (fd == -1)
    {
        perror("open fifo file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void closeFifoScreen(int fd)
{
    close(fd);
}

void sendToScreen(char *message, int fd)
{
    if (write(fd, message, strlen(message) + 1) == -1)
    {
        perror("write to screen");
        exit(EXIT_FAILURE);
    }
}

void getInput()
{
    printf("prompt $ >");

    memset(cmd_str, '\0', MAX_COMMAND_SIZE);
    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin));
}

void execute(){
    // If the user just hits enter, do nothing
    if (cmd_str == NULL)
    {
        return;
    }
    mkfifo("communication", 0666); 
    int fd = open("communication", O_WRONLY);
    if (fd == -1 )
    {
        perror("Could not open named pipe ");
    }
    if (write(fd, cmd_str, sizeof(cmd_str)) == -1 )
    {
        perror("Error bad file descriptor");
    }
    close(fd);
}