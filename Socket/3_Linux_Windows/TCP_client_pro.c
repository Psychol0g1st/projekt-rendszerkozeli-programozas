/*******************************/
/* - TCP client professional - */
/*        by Imre Varga        */
/*******************************/

#include<stdio.h>
#include<string.h>
#include<netdb.h>
#if defined(linux) || defined(_linux) || defined(__linux__) || defined(__unix__) // if Linux OS
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
#else // if Windows OS
   #define WINDOWS 1
   #include <winsock2.h>
   // Code::Blocks: Settings --> Compiler settings --> Linker settings --> Link libraries: Add: ws2_32
   #pragma comment(lib,"ws2_32.lib") // Winsock Library
   #define close(a) 1
#endif
#define BUFSIZE 1024                 // Max length of buffer
#define PORT_NO 2222                 // The port on which the server is listening

int IPv4(char *addr){                // Is IPv4 address?
   unsigned int b1,b2,b3,b4,n;
   n=sscanf(addr,"%u.%u.%u.%u",&b1,&b2,&b3,&b4);
   if(n==4 && b1<256 && b2<256 && b3<256 && b4<256) return 1;
   return 0;
   }

int main(int argc, char *argv[]){    // Server's IPv4 address is needed as command line argument

   /************************ Declarations **********************/
   int s;                            // socket ID
   int bytes;                        // received/sent bytes
   int err;                          // error code
   int flag;                         // transmission flag
   char on;                          // sockopt option
   char IP[16];                      // IPv4 address as string
   char buffer[BUFSIZE];             // datagram buffer area
   unsigned int server_size;         // length of the sockaddr_in server
   struct sockaddr_in server;        // address of server
   struct hostent *computer;         // names and addresses of a computer

   /************************ Initialization ********************/
   on   = 1;
   flag = 0;
   if(WINDOWS){
      static WSADATA wsa;
      if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){ //Initialization of winsock
         fprintf(stderr, " %s: Initialization error (%d).\n", argv[0], WSAGetLastError());
         exit(1);
         }
      }
   if(argc==1)                       // no argument: localhost
      strcpy(IP,"127.0.0.1");
   else                              // argument is IPv4 address
      if(IPv4(argv[1]))
         strcpy(IP,argv[1]);
      else{                          // argument may be a hostname
         computer = gethostbyname(argv[1]);
         if ( computer == NULL ) {
            fprintf(stderr, " %s: Host name error (%d).\n", argv[0], WSAGetLastError());
            exit(2);
            }
         strcpy(IP,inet_ntoa(*(struct in_addr*)(*computer).h_addr));
         }
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr(IP);
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;

   /************************ Creating socket *******************/
   s = socket(AF_INET, SOCK_STREAM, 0);
   if ( s < 0 ) {
      fprintf(stderr, " %s: Socket creation error (%d).\n", argv[0], WSAGetLastError());
      exit(3);
      }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /************************ Connecting ************************/
   err = connect(s, (struct sockaddr *) &server, server_size);
   if ( err < 0 ) {
      fprintf(stderr, " %s: Connecting error (%d).\n", argv[0], WSAGetLastError());
      exit(4);
      }
   printf(" Connection is OK.\n");

   /************************ Sending data **********************/
   printf(" Message to send: ");
   fgets(buffer,BUFSIZE,stdin);
   bytes = send(s, buffer, strlen(buffer)+1, flag);
   if ( bytes <= 0 ) {
      fprintf(stderr, " %s: Sending error (%d).\n", argv[0], WSAGetLastError());
      exit(5);
      }
   printf (" %i bytes have been sent to server.\n", bytes-1);

   /************************ Receive data **********************/
   bytes = recv(s, buffer, BUFSIZE, flag);
   if ( bytes < 0 ) {
      fprintf(stderr, " %s: Receiving error (%d).\n", argv[0], WSAGetLastError());
      exit(6);
      }
   printf(" Server's (%s:%d) acknowledgement:\n  %s\n",
           inet_ntoa(server.sin_addr), ntohs(server.sin_port), buffer);

   /************************ Closing ***************************/
   if(WINDOWS){
      closesocket(s);
      WSACleanup();
      }
   else
      close(s);

   return EXIT_SUCCESS;
   }

