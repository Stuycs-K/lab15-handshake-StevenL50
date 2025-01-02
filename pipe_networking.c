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

  int error = mkfifo(WKP, 0650);
  int fd = open(WKP, O_RDONLY, 0650);

  char buffer[BUFFER_SIZE];
  int bytes = read(fd, buffer, BUFFER_SIZE);
  if (bytes < 0) {
    perror("read failed");
    exit(1);
  }

  from_client = atoi(buffer);
  error = unlink(WKP);
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
  // client -> server, RD
  int from_client = server_setup(); // also the pid

  // subserver

  int randInt;
  int bytes;

  char buffer[HANDSHAKE_BUFFER_SIZE];

  int rfd = open("dev/urandom/", O_RDONLY);
  bytes = read(rfd, &randInt, sizeof(int));
  snprintf(buffer, HANDSHAKE_BUFFER_SIZE, "%d", randInt);
  close(rfd);

  printf("random number 1: %d\n", randInt); // DEBUG

  // server -> client, WR
  bytes = write(from_client, buffer, HANDSHAKE_BUFFER_SIZE);
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }

  printf("random number 2: %s\n", buffer); // DEBUG

  // client -> server, RD
  bytes = read(from_client, buffer, HANDSHAKE_BUFFER_SIZE);
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }

  printf("pid+1: %s\n", buffer); // DEBUG

  if(atoi(buffer) != from_client+1) {
    printf("SOMETHING WENT WRONG\n");
    exit(1);
  }

  printf("HANDSHAKE COMPLETE\n");

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
  int bytes;
  int pid = getpid();

  char buffer[HANDSHAKE_BUFFER_SIZE];
  snprintf(buffer, HANDSHAKE_BUFFER_SIZE, "%d", pid);

  mkfifo(buffer, 0650); // create PP

  printf("pid: %s\n", buffer); // DEBUG

  int fdWKP = open(WKP, O_WRONLY, 0650);

  printf("WKP fd: %d\n", fdWKP); // DEBUG

  int from_server = open(buffer, O_RDONLY, 0650);

  printf("from_server fd: %d\n", from_server); // DEBUG

  // client -> server, WR
  bytes = write(fdWKP, buffer, HANDSHAKE_BUFFER_SIZE); // write PP to server
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }

  printf("pid: %s\n", buffer); // DEBUG

  // server -> client, RD
  bytes = read(from_server, buffer, HANDSHAKE_BUFFER_SIZE); // read randInt from server
  if (bytes < 0) {
    perror("read failed");
    exit(1);
  }

  printf("randInt: %s\n", buffer); // DEBUG

  int randInt = atoi(buffer);
  snprintf(buffer, HANDSHAKE_BUFFER_SIZE, "%d", pid);
  int error = unlink(buffer);
  if (error < 0) {
    perror("unlink failed");
    exit(3);
  }

  // client -> server, WR
  snprintf(buffer, HANDSHAKE_BUFFER_SIZE, "%d", randInt+1);
  bytes = write(fdWKP, buffer, HANDSHAKE_BUFFER_SIZE); // write randInt+1 to server
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }

  // server -> client, RD
  bytes = read(from_server, buffer, HANDSHAKE_BUFFER_SIZE); // 
  if (bytes < 0) {
    perror("read failed");
    exit(1);
  }

  if(atoi(buffer) != randInt+1) {
    printf("SOMETHING WENT WRONG\n");
    exit(1);
  }

  to_server = &fdWKP;
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
