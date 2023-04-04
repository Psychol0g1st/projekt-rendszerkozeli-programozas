/*****************************************/
/*   - Minimal UDP client for Linux -    */
/*             by Imre Varga             */
/*****************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define BUFSIZE 1024                 // Max length of buffer

int main(int argc, char *argv[]){    // Server's IPv4 address is needed as command line argument

   /********************** Declarations ************************/
   int s;                            // socket ID
   unsigned int server_size;         // length of the sockaddr_in server
   char on = 1;                      // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   struct sockaddr_in server;        // address of server

   /********************** Initialization **********************/
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr(argc==1 ? "127.0.0.1" : argv[1]);
   server.sin_port        = htons(2222);
   server_size            = sizeof server;

   /********************** Creating socket *********************/
   s = socket(AF_INET, SOCK_DGRAM, 0 );
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /********************** Sending data ************************/
   printf(" Message to send: ");
   fgets(buffer,BUFSIZE,stdin);
   sendto(s, buffer, strlen(buffer)+1, 0, (struct sockaddr *) &server, server_size);
   printf (" Message have been sent to server.\n");

   /********************** Receive data ************************/
   recvfrom(s, buffer, BUFSIZE, 0, (struct sockaddr *) &server, &server_size);
   printf(" Server's acknowledgement:\n  %s\n", buffer);

   /********************** Closing *****************************/
   close(s);
   puts("End");
   return 0;
   }

