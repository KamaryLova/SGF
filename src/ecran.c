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

#define MAX_COMMAND_SIZE 255
#define MAX_NUM_ARGUMENTS 10 // Mav shell only supports five arguments

#define WHITESPACE " \t\n" // We want to split our command line up into tokens \
                           // so we need to define what delimits our tokens.   \
                           // In this case  white space                        \
                           // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255 // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10 // Mav shell only supports five arguments


char cmd_str[MAX_COMMAND_SIZE];
char * token[MAX_NUM_ARGUMENTS];

void execute();
void getStream();


int main(){
    while(1)
    getStream();
    execute();
}

void getStream()
{   
    int fd = open("communication", O_RDONLY);

    if (fd == -1 )
    {
        perror("Could not open named pipe ");
    }
    if (read(fd, cmd_str, sizeof(cmd_str)) == -1 )
    {
        perror("Error bad file descriptor");
    }

    // parse message
    int token_count = 0;

    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;

    char *working_str = strdup(cmd_str);

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    memset(&token, '\0', MAX_NUM_ARGUMENTS);

    // Tokenize the input strings with whitespace used as the delimiter
    memset(&token, '\0', sizeof(MAX_NUM_ARGUMENTS));
    while (((arg_ptr = strsep(&working_str, WHITESPACE)) != NULL) &&
           (token_count < MAX_NUM_ARGUMENTS))
    {
        token[token_count] = strndup(arg_ptr, MAX_COMMAND_SIZE);
        if (strlen(token[token_count]) == 0)
        {
            token[token_count] = NULL;
        }
        token_count++;
    }

    free(working_root);

    printf("Token O %s\n", token[0]);
    printf("Token 1 %s\n", token[1]);
    printf("Token 2 %s\n", token[2]);
    close(fd);

}

// Function to be called 
void execute(){
    
}

