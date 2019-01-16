#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "pipe_networking.h"

static void sighandler(int signo){
  if(signo == SIGINT){
    //remove(stoc); how to remove this?? will figure out later
    exit(1);
  }
}

int main() {

  int to_client;
  int from_client;

  signal(SIGINT, sighandler);
  
  while(1){
    from_client = server_handshake( &to_client );
    char *input = calloc(100, 1);

    while(read(from_client, input, 100)){
      printf("Input from client: %s\n", input);

      //changing client output and sending to server
      for(int i=0; i<strlen(input); i++){
	input[i] = input[i] + 'A' - 'a';
      }
      write(to_client, input, 100);
    }
  }
  
}
