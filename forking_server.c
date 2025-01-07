#include "pipe_networking.h"

void subserver(int from_client, int to_client) {
  char buffer[BUFFER_SIZE];
  int bytes;
  int pid;
  while(1) {
    bytes = read(from_client, buffer, BUFFER_SIZE);
    if (bytes < 0) {
	  	perror("read failed");
	  	exit(1);
	  }
    printf("Received pid %s from client\n", buffer);
    pid = atoi(buffer);

    snprintf(buffer, BUFFER_SIZE, "=== VERIFIED %d ===", pid);
    bytes = write(to_client, buffer, BUFFER_SIZE);
 	  if (bytes < 0) {
 	  	perror("write failed");
 	  	exit(1);
 	  }
    printf("Sent message %s to client\n", buffer);
 	  sleep(1);
  }
}

int main() {
  int to_client;
  int from_client;
  int f;
  while(1) {
 	  from_client = server_setup();
    f = fork();
    if(f == 0) {
      server_handshake_half(&to_client, from_client);
      subserver(from_client, to_client);
      exit(0);
    }
    else {
      close(from_client);
      close(to_client);
    }
  }
}
