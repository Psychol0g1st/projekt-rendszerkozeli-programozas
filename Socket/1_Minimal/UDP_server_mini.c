/*****************************************/
/*   - Minimal UDP server for Linux -    */
/*             by Imre Varga             */
/*****************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define BUFSIZE 1024                 // Max length of buffer

int main(){

   /********************** Declarations ************************/
   int s;                            // socket ID
   char on = 1;                      // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   unsigned int client_size;         // length of the sockaddr_in client
   struct sockaddr_in server;        // address of server
   struct sockaddr_in client;        // address of client

   /********************** Initialization **********************/
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(2222);

   /********************** Creating socket *********************/
   s = socket(AF_INET, SOCK_DGRAM, 0 );
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /********************** Binding socket **********************/
   bind(s, (struct sockaddr *) &server, sizeof server);

   while(1){ // Continuous server operation
      /********************** Receive data ************************/
      printf("\n Waiting for a message...\n");
      recvfrom(s, buffer, BUFSIZE, 0, (struct sockaddr *) &client, &client_size);
      printf (" Client's message:\n  %s", buffer);

      /********************** Sending data ************************/
      sprintf(buffer,"I have received a message.");
      sendto(s, buffer, strlen(buffer)+1, 0, (struct sockaddr *) &client, client_size);
      printf(" Acknowledgement have been sent to client.\n");
      }

   /*********************** Closing ****************************/
   close(s);
   
   return 0;
   }

