#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;
  while(1) {
 	  from_client = server_handshake( &to_client );

    printf("CONNECTION ESTABLISHED ||| fds %d %d\n", from_client, to_client);

    close(from_client);
    close(to_client);
  }
}
