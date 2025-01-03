#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  while(1) {
 		char buffer[BUFFER_SIZE];
 		
 		int fd = open("/dev/urandom", O_RDONLY);
 		int num;
 	  int bytes = read(fd, &num, sizeof(int));
 	  if (bytes < 0) {
 	  	perror("read failed");
 	  	exit(1);
 	  }
 	  close(fd);
 	  printf("Sending %d to client\n", num);

		snprintf(buffer, BUFFER_SIZE, "%d", num);
 	  bytes = write(to_client, buffer, BUFFER_SIZE);
 	  if (bytes < 0) {
 	  	perror("write failed");
 	  	exit(1);
 	  }
 	  sleep(1);
  }
}
