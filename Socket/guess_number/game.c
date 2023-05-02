#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#define BUFSIZE 4
#define PORT_NO 6666
int s; // socket ID

void stop(int sig)
{ // signal handler
    /************************ Closing ***************************/
    close(s);
    printf("\n Server is terminated.\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        puts("Client");
        /************************ Declarations **********************/
        int s;                     // socket ID
        int bytes;                 // received/sent bytes
        int flag;                  // transmission flag
        char on;                   // sockopt option
        int buffer;                // datagram buffer area
        unsigned int server_size;  // length of the sockaddr_in server
        struct sockaddr_in server; // address of server

        /************************ Initialization ********************/
        on = 1;
        flag = 0;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(argc == 1 ? "127.0.0.1" : argv[1]);
        server.sin_port = htons(PORT_NO);
        server_size = sizeof server;

        /************************ Creating socket *******************/
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
        {
            fprintf(stderr, " %s: Socket creation error.\n", argv[0]);
            exit(2);
        }
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
        setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);
        do
        {
            /************************ Sending data **********************/
            printf(" Number: ");
            scanf("%d", &buffer);
            bytes = sendto(s, &buffer, BUFSIZE, flag, (struct sockaddr *)&server, server_size);
            if (bytes <= 0)
            {
                fprintf(stderr, " %s: Sending error.\n", argv[0]);
                exit(3);
            }

            /************************ Receive data **********************/
            bytes = recvfrom(s, &buffer, BUFSIZE, flag, (struct sockaddr *)&server, &server_size);
            if (bytes < 0)
            {
                fprintf(stderr, " %s: Receiving error.\n", argv[0]);
                exit(4);
            }
            if (buffer == -1)
                printf("Your guess is too low\n");
            else if (buffer == 1)
                printf("Your guess is too high\n");
        } while (buffer != 0);
        /************************ Closing ***************************/
        puts("You win!");
        close(s);

        return EXIT_SUCCESS;
    }
    else if (argc == 1)
    {
        puts("Server");
        /************************ Declarations **********************/
        int bytes;                 // received/sent bytes
        int err;                   // error code
        int flag;                  // transmission flag
        char on;                   // sockopt option
        int buffer;                // datagram buffer area
        unsigned int server_size;  // length of the sockaddr_in server
        unsigned int client_size;  // length of the sockaddr_in client
        struct sockaddr_in server; // address of server
        struct sockaddr_in client; // address of client

        /************************ Initialization ********************/
        on = 1;
        flag = 0;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(PORT_NO);
        server_size = sizeof server;
        client_size = sizeof client;
        signal(SIGINT, stop);
        signal(SIGTERM, stop);

        /************************ Creating socket *******************/
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
        {
            fprintf(stderr, " %s: Socket creation error.\n", argv[0]);
            exit(2);
        }
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
        setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

        /************************ Binding socket ********************/
        err = bind(s, (struct sockaddr *)&server, server_size);
        if (err < 0)
        {
            fprintf(stderr, " %s: Binding error.\n", argv[0]);
            exit(3);
        }

        int is_guess = 0;
        int number_to_guess = 0;
        while (1)
        { // Continuous server operation
            if (is_guess)
            {
                /************************ Receive data **********************/
                printf("\n Waiting for a message...\n");
                bytes = recvfrom(s, &buffer, BUFSIZE, flag, (struct sockaddr *)&client, &client_size);
                if (bytes < 0)
                {
                    fprintf(stderr, " %s: Receiving error.\n", argv[0]);
                    exit(4);
                }
                printf("Client's guess is:  %d\n", buffer);
                if (buffer == number_to_guess)
                {
                    buffer = 0;
                    is_guess = 0;
                }
                else if (buffer < number_to_guess)
                    buffer = -1;
                else
                    buffer = 1;
                /************************ Sending data **********************/
                bytes = sendto(s, &buffer, BUFSIZE, flag, (struct sockaddr *)&client, client_size);
                if (bytes <= 0)
                {
                    fprintf(stderr, " %s: Sending error.\n", argv[0]);
                    exit(5);
                }
            }
            else
            {
                puts("Generating a random number...");
                number_to_guess = rand() % 100 + 1;
                printf("Number to guess is: %d", number_to_guess);
                is_guess = 1;
            }
        }

        return EXIT_SUCCESS;
    }
    else
    {
        printf("Invalid number of arguments");
        exit(1);
    }
}
