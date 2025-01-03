#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  char buffer[BUFFER_SIZE];
  int fd = open("/dev/urandom", O_RDONLY);
  int num;
  int bytes;
  while(1) {
 	  bytes = read(fd, &num, sizeof(int));
 	  if (bytes < 0) {
 	  	perror("read failed");
 	  	exit(1);
 	  }
 	  printf("Sending %d to client\n", num);

		snprintf(buffer, BUFFER_SIZE, "%d", num);
 	  bytes = write(to_client, buffer, BUFFER_SIZE);
    if (bytes == 0) {
      printf("Client stopped connection\n");
      exit(0);
    }
 	  if (bytes < 0) {
 	  	perror("write failed");
 	  	exit(1);
 	  }

    bytes = read(from_client, buffer, BUFFER_SIZE);
    if (bytes < 0) {
	  	perror("read failed");
	  	exit(1);
	  }
	  printf("Receiving %s from client\n", buffer);
 	  sleep(1);
  }
}
