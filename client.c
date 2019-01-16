#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  char *message = calloc(200,1);
  char *response = calloc(200,1);
  
  while(1){
    printf("Enter something you wish to send to the server. \n");
    fflush(stdout);
    fgets(message, 100, stdin);
    message[strlen(message)-1] = "\0";
    write(to_server, message, 200);
    read(from_server, response, 200);
    printf("Server's reply: %s\n", response);
  }
}
