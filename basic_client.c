#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char buffer[BUFFER_SIZE];
  int fd = open("/dev/urandom", O_RDONLY);
  int num;
  int bytes;
  while(1) {
    bytes = read(from_server, buffer, BUFFER_SIZE);
    if (bytes < 0) {
	  	perror("read failed");
	  	exit(1);
	  }
	  printf("Receiving %s from server\n", buffer);

 	  bytes = read(fd, &num, sizeof(int));
 	  if (bytes < 0) {
 	  	perror("read failed");
 	  	exit(1);
 	  }
 	  printf("Sending %d to server\n", num);

		snprintf(buffer, BUFFER_SIZE, "%d", num);
 	  bytes = write(to_server, buffer, BUFFER_SIZE);
    if (bytes == 0) {
      printf("Client stopped connection\n");
      exit(0);
    }
 	  if (bytes < 0) {
 	  	perror("write failed");
 	  	exit(1);
 	  }
 	  sleep(1);
  }
}
