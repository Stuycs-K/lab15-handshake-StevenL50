#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;
  int f;
  while(1) {
 	  from_client = server_setup();
    f = fork();
    if(f == 0) {
      // subserver stuff
    }
    else {
      close(from_client);
      close(to_client);
    }
  }
}
