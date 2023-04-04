/*****************************************/
/*   - Minimal TCP client for Linux -    */
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
   char on = 1;                      // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   struct sockaddr_in server;        // address of server

   /********************** Initialization **********************/
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr(argc==1 ? "127.0.0.1" : argv[1]);
   server.sin_port        = htons(2222);

   /********************** Creating socket *********************/
   s = socket(AF_INET, SOCK_STREAM, 0 );
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /********************** Connecting **************************/
   connect(s, (struct sockaddr *) &server, sizeof server);
   printf(" Connection is OK.\n");

   /********************** Sending data ************************/
   printf(" Message to send: ");
   fgets(buffer,BUFSIZE,stdin);
   send(s, buffer, strlen(buffer)+1, 0);
   printf (" Massage have been sent to server.\n");

   /********************** Receive data ************************/
   recv(s, buffer, BUFSIZE, 0);
   printf(" Server's acknowledgement:\n  %s\n", buffer);

   /********************** Closing *****************************/
   close(s);

   return 0;
   }

