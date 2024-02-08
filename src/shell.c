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

#define MAX_COMMAND_SIZE 255 // The maximum command-line size

char cmd_str[MAX_COMMAND_SIZE];//Entire string inputted by the user. It will be parsed into multiple tokens (47)

void getInput();
void execute();

int main()
{   
    while (1)
    {
        getInput();
        execute();
    }
    return 0;
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