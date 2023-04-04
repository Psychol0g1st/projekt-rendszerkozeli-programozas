/*****************************************/
/*   - Minimal TCP server for Linux -    */
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
   int sc;                           // socket ID (for connects)
   int s;                            // socket ID (for messages)
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
   sc = socket(AF_INET, SOCK_STREAM, 0 );
   setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(sc, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /********************** Binding socket **********************/
   bind(sc, (struct sockaddr *) &server, sizeof server);

   /********************** Listening ***************************/
   listen(sc, 10);

   while(1){ // Continuous server operation
      /********************** Accepting connection ****************/
      printf("\n Waiting for connection request...\n");
      s = accept(sc, (struct sockaddr *) &client, &client_size);
      printf(" A client has connected.\n");

      /********************** Receive data ************************/
      printf(" Waiting for a message...\n");
      recv(s, buffer, BUFSIZE, 0 );
      printf (" Client's message:\n  %s", buffer);

      /********************** Sending data ************************/
      sprintf(buffer,"I have received a message.");
      send(s, buffer, strlen(buffer)+1, 0 );
      printf(" Acknowledgement have been sent to client.\n");

      /********************** Closing connection ******************/
      close(s);
      }

   return 0;
   }

