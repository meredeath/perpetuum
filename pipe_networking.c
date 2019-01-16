#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char * stoc = "stoc";
  mkfifo(stoc, 0666); //make a WKP

  int message = open(stoc, O_RDONLY); //waiting for a client to send a message, so it can read it and respond

  remove(stoc); //a client has connected, so remove name of pipe so other clients do not attempt to connect
  
  char *stuff = calloc(HANDSHAKE_BUFFER_SIZE, 1);
  read(message, stuff, HANDSHAKE_BUFFER_SIZE);//read the client message

  *to_client = open(stuff, O_WRONLY);//client sent private pipe name thru message; open pipe to write into it and send client stuff
  char *nmessage = "your message has been received!";
  write(*to_client, nmessage, sizeof(nmessage));//write to client message indicating message has been received

  char *more = calloc(HANDSHAKE_BUFFER_SIZE, 1);
  read(message, more, HANDSHAKE_BUFFER_SIZE);//read response from client
  
  close(*to_client);//close pipe server writes to client from
  close(message); //close pipe from client server reads from
  
  return 0;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char *ctos = "ctos";
  mkfifo(ctos, 0666); //client creates a pipe

  *to_server = open("stoc", O_WRONLY);
  write(*to_server, ctos, 4); //send message to server side

  int message = open(ctos, O_RDONLY);
  remove(ctos);//receives server message, remove pipe
  char *stuff = calloc(HANDSHAKE_BUFFER_SIZE, 1);
  read(message, stuff, HANDSHAKE_BUFFER_SIZE);
  printf("Server's message: %s\n", stuff);//print out message

  char *nmessage = "the message has been received!";
  write(*to_server, nmessage, sizeof(nmessage));//respond to server message

  close(*to_server); //close pipe going to server
  close(message); //close pipe reading from server
  return 0;
}
