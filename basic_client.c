#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

	while(1) {
		char buffer[BUFFER_SIZE];
	  int bytes = read(from_server, buffer, BUFFER_SIZE);
	  if (bytes < 0) {
	  	perror("read failed");
	  	exit(1);
	  }
	  printf("%s\n", buffer);
	  sleep(1);
  }
}
