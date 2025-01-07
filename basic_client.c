#include "pipe_networking.h"

int main() {
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char buffer[BUFFER_SIZE];
  int num = getpid();
  int bytes;
  while(1) {

		snprintf(buffer, BUFFER_SIZE, "%d", num);
 	  bytes = write(to_server, buffer, BUFFER_SIZE);
    if (bytes == 0) {
      printf("Server stopped connection\n");
      exit(0);
    }
 	  if (bytes < 0) {
 	  	perror("write failed");
 	  	exit(1);
 	  }
    printf("Sent pid %d to server\n", num);

    bytes = read(from_server, buffer, BUFFER_SIZE);
    if (bytes < 0) {
	  	perror("read failed");
	  	exit(1);
	  }
    printf("Received message %s from server\n", buffer);
 	  sleep(1);
  }
}
