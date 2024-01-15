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

#define MAX_PROMPT_SIZE 256

int main()
{
    char prompt_message[MAX_PROMPT_SIZE];
    while(1)
    {
        printf("prompt $ ");
        fgets(prompt_message, MAX_PROMPT_SIZE, stdin);
    }
    return 0;
}