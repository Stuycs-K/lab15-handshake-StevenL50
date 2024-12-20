#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  printf("to_server: %d ||||| from_server: %d", to_server, from_server);
}
