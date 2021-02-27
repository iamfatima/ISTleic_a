#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>
#include <strings.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "fs/lockOps.h"
#include "fs/operations.h"

#define INDIM 30
#define OUTDIM 512
#define SERVER "/tmp/sock"

#define MAX_COMMANDS 10
#define MAX_INPUT_SIZE 100

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

/******* Socket Variables *******/
int sockfd;
struct sockaddr_un server_addr;
socklen_t serverAddrlen;
char *path;

/******* Number of Threads *******/
int numberThreads = 0;

/* 
 * Sets the number of threads, checking to see if it's valid 
 */
void set_numberThreads(char *arg_threads)
{
    int threads = atoi(arg_threads);

    if (threads > 0)
    {
        numberThreads = threads;
    }
    else
    {
        fprintf(stderr, "Invalid thread number.\n");
        exit(EXIT_FAILURE);
    }
}

void errorParse()
{
    fprintf(stderr, "Error: command invalid\n");
    exit(EXIT_FAILURE);
}

/*
 * Take commands from the input array and execute them.
 */
void applyCommands()
{
    while (1)
    {
        socklen_t addrlen;
        struct sockaddr_un client_addr;
        char in_buffer[MAX_INPUT_SIZE], out_buffer[2];
        int c;

        addrlen = sizeof(struct sockaddr_un);
        c = recvfrom(sockfd, in_buffer, sizeof(in_buffer) - 1, 0, (struct sockaddr *)&client_addr, &addrlen);
        if (c <= 0)
            continue;

        //Preventivo, caso o cliente nao tenha terminado a mensagem em '\0',
        in_buffer[c] = '\0';

        char token, type;
        char name[MAX_INPUT_SIZE], name2[MAX_INPUT_SIZE];
        int numTokens = sscanf(in_buffer, "%c %s %s", &token, name, name2);
        int commandRes;

        if (numTokens < 2)
        {
            fprintf(stderr, "Error: invalid command in Queue\n");
            exit(EXIT_FAILURE);
        }

        type = name2[0];

        int searchResult;
        switch (token)
        {
        case 'c':
            switch (type)
            {
            case 'f':
                printf("Create file: %s\n", name);
                commandRes = create(name, T_FILE);
                break;
            case 'd':
                printf("Create directory: %s\n", name);
                commandRes = create(name, T_DIRECTORY);
                break;
            default:
                fprintf(stderr, "Error: invalid node type\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'l':
            searchResult = lookup(name);
            commandRes = searchResult;
            if (searchResult >= 0)
                printf("Search: %s found\n", name);
            else
                printf("Search: %s not found\n", name);
            break;
        case 'd':
            printf("Delete: %s\n", name);
            commandRes = delete (name);
            break;
        case 'm':
            printf("Move: %s to %s.\n", name, name2);
            commandRes = move(name, name2);
            break;
        case 'p':
            printf("Print tecnicofs to: %s\n", name);
            commandRes = print_tecnicofs_tree(name);
            break;
        default:
        { /* error */
            fprintf(stderr, "Error: command to apply\n");
            exit(EXIT_FAILURE);
        }
        }

        c = sprintf(out_buffer, "%d", commandRes);

        sendto(sockfd, out_buffer, c + 1, 0, (struct sockaddr *)&client_addr, addrlen);
    }
}

/*
 * Starts up threads with the applycommands() function.
 */
void runThreadPool(pthread_t threads[numberThreads])
{

    for (int i = 0; i < numberThreads; i++)
    {
        if (pthread_create(&threads[i], NULL, (void *)applyCommands, NULL) != 0)
        {
            fprintf(stderr, "Error creating thread.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * Waits until all slave threads have finished.
 */
void endThreadPool(pthread_t *threads)
{

    for (int i = 0; i < numberThreads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Error joining thread.\n");
            exit(EXIT_FAILURE);
        }
    }
}

int setSockAddrUn(char *path, struct sockaddr_un *addr)
{

    if (addr == NULL)
        return 0;

    bzero((char *)addr, sizeof(struct sockaddr_un));
    addr->sun_family = AF_UNIX;
    strcpy(addr->sun_path, path);

    return SUN_LEN(addr);
}

int main(int argc, char *argv[])
{
    /* Checks to see if the command line input is valid */
    if (argc != 3)
    {
        fprintf(stderr, "Invalid argument number.\n");
        exit(EXIT_FAILURE);
    }

    /* Threads */

    set_numberThreads(argv[1]);
    pthread_t threads[numberThreads];

    /* Init filesystem */
    init_fs();

    /* Create Socket */
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "server: can't open socket\n");
        exit(EXIT_FAILURE);
    }

    path = argv[2];

    unlink(path);

    /* Name the Socket */

    serverAddrlen = setSockAddrUn(path, &server_addr);
    if (bind(sockfd, (struct sockaddr *)&server_addr, serverAddrlen) < 0)
    {
        fprintf(stderr, "server: bind error\n");
        exit(EXIT_FAILURE);
    }

    runThreadPool(threads);

    endThreadPool(threads);

    close(sockfd);
    unlink(argv[2]);

    /* release allocated memory */
    destroy_fs();
    exit(EXIT_SUCCESS);
}

/*
 * TODO
 * 1 - Perguntar ao professor se é para apagar a parte do tempo;
 * 2 - 
 */