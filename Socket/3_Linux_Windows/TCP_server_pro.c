/*******************************/
/* - TCP server professional - */
/*        by Imre Varga        */
/*******************************/

#include<stdio.h>
#include<string.h>
#if defined(linux) || defined(_linux) || defined(__linux__) || defined(__unix__) // if Linux
   #define WINDOWS 0
   #include<stdlib.h>
   #include<unistd.h>
   #include<sys/socket.h>
   #include<netinet/in.h>
   #include<arpa/inet.h>
   typedef int WSADATA;
   #define WSAGetLastError() -1
   #define WSAStartup(a,b) 0
   #define closesocket(a) ;
   #define WSACleanup() ;
#else // if Windows
   #define WINDOWS 1
   #include <winsock2.h>
   // Code::Blocks: Settings --> Compiler settings --> Linker settings --> Link libraries: Add: ws2_32
   #pragma comment(lib,"ws2_32.lib") // Winsock Library
   #define close(a) 1
#endif
#define BUFSIZE 1024                 // Max length of buffer
#define PORT_NO 2222                 // The port on which the server is listening
#define CQL     10                   // Connection Queue Length

int main(int argc, char *argv[]){

   /************************ Declarations **********************/
   int sc;                           // socket ID (for connects)
   int s;                            // socket ID (for messages)
   int bytes;                        // received/sent bytes
   int err;                          // error code
   int flag;                         // transmission flag
   char on;                          // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   unsigned int server_size;         // length of the sockaddr_in server
   unsigned int client_size;         // length of the sockaddr_in client
   struct sockaddr_in server;        // address of server
   struct sockaddr_in client;        // address of client

   /************************ Initialization ********************/
   on   = 1;
   flag = 0;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;
   client_size = sizeof client;
   if(WINDOWS){
      static WSADATA wsa;
      if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){ //Initialization of winsock
         fprintf(stderr, " %s: Initialization error (%d).\n", argv[0], WSAGetLastError());
         exit(1);
         }
      }

   /************************ Creating socket *******************/
   sc = socket(AF_INET, SOCK_STREAM, 0 );
   if ( sc < 0 ) {
      fprintf(stderr, " %s: Socket creation error (%d).\n", argv[0], WSAGetLastError());
      exit(2);
      }
   setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(sc, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /************************ Binding socket ********************/
   err = bind(sc, (struct sockaddr *) &server, server_size);
   if ( err < 0 ) {
      fprintf(stderr," %s: Binding error (%d).\n", argv[0], WSAGetLastError());
      exit(3);
      }

   /************************ Listening *************************/
   err = listen(sc, CQL);
   if ( err < 0 ) {
      fprintf(stderr, " %s: Listening error (%d).\n",argv[0], WSAGetLastError());
      exit(4);
      }

   while(1){ // Continuous server operation
      /************************ Accepting connection **************/
      printf("\n Waiting for connection request...\n");
      s = accept(sc, (struct sockaddr *) &client, &client_size);
      if ( s < 0 ) {
         fprintf(stderr, " %s: Accepting error (%d).\n",argv[0], WSAGetLastError());
         continue;
         }
      printf(" A client has connected.\n");

      /************************ Receive data **********************/
      printf(" Waiting for a message...\n");
      bytes = recv(s, buffer, BUFSIZE, flag );
      if ( bytes < 0 ) {
         fprintf(stderr, " %s: Receiving error (%d).\n", argv[0], WSAGetLastError());
         exit(6);
         }
      printf (" %d bytes have been received from the client (%s:%d).\n Client's message:\n  %s",
               bytes-1, inet_ntoa(client.sin_addr), ntohs(client.sin_port), buffer);

      /************************ Sending data **********************/
      sprintf(buffer,"I have received a %d bytes long message.", bytes-1);
      bytes = send(s, buffer, strlen(buffer)+1, flag );
      if ( bytes < 0 ) {
         fprintf(stderr, " %s: Sending error (%d).\n", argv[0], WSAGetLastError());
         exit(7);
         }
      printf(" Acknowledgement have been sent to client.\n");

      /************************ Closing connection ****************/
      if(WINDOWS) {closesocket(s);}
      else        {close(s);}
      }

   /************************ Closing ***************************/
   if(WINDOWS){
      closesocket(sc);
      WSACleanup();
      }
   else
      close(sc);

   return EXIT_SUCCESS;
   }

