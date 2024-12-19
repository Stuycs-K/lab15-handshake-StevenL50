#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;

  int error = mkfifo("WKP", 0650);
  if(error < 0) {
    perror("mkfifo failed");
    exit(2);
  }
  int fd = open("WKP", O_WRONLY, 0650);

  char buffer[BUFFER_SIZE];
  int bytes = read(fd, buffer, BUFFER_SIZE);
  if (bytes < 0) {
    perror("read failed");
    exit(1);
  }

  from_client = atoi(buffer);
  error = unlink("WKP");
  if (error < 0) {
    perror("unlink failed");
    exit(3);
  }
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client;

  server_setup();

  char pidBuffer[10];
  int pid = getpid();
  snprintf(pidBuffer, sizeof(pid), "%d", pid);
  int bytes = write(*to_client, pidBuffer, strlen(pidBuffer));
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }



  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
