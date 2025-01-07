#include "pipe_networking.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    unlink(WKP);
    exit(0);
  }
}

//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  signal(SIGINT, sighandler);
  int error = mkfifo(WKP, 0650);
  int from_client = open(WKP, O_RDONLY, 0650);

  error = unlink(WKP);
  if (error < 0) {
    perror("unlink failed");
    exit(3);
  }
  return from_client;
}

int server_handshake_half(int *to_client, int from_client) {
  char buffer[HANDSHAKE_BUFFER_SIZE];
  int bytes = read(from_client, buffer, HANDSHAKE_BUFFER_SIZE);
  if (bytes < 0) {
    perror("read failed");
    exit(1);
  }

  // printf("pid: %s\n", buffer); // DEBUG

  int cfd = open(buffer, O_WRONLY, 650);

  // printf("pp opened!\n"); // DEBUG

  short randInt;
  int rfd = open("/dev/urandom", O_RDONLY);
  bytes = read(rfd, &randInt, sizeof(short));
  if (bytes < 0) {
    perror("read failed");
    exit(1);
  }
  snprintf(buffer, HANDSHAKE_BUFFER_SIZE, "%d", randInt);
  close(rfd);

  // printf("random number 1: %d\n", randInt); // DEBUG

  // server -> client, WR
  bytes = write(cfd, buffer, HANDSHAKE_BUFFER_SIZE);
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }

  // printf("random number 2: %s\n", buffer); // DEBUG

  // client -> server, RD
  bytes = read(from_client, buffer, HANDSHAKE_BUFFER_SIZE);
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }

  // printf("randInt+1: %s\n", buffer); // DEBUG

  if(atoi(buffer) != randInt+1) {
    perror("HANDSHAKE FAILED\n");
    exit(1);
  }

  // printf("HANDSHAKE COMPLETE\n");
  // to_client = malloc(sizeof(int));
  // *to_client = cfd;
  *to_client = cfd;
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
  int from_client = server_setup(); // wkp fd
  return server_handshake_half(to_client, from_client);
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

  // printf("private pipe: %s\n", buffer); // DEBUG

  int fdWKP = open(WKP, O_WRONLY, 0650);

  // printf("WKP: %d\n", fdWKP); // DEBUG

  // client -> server, WR
  bytes = write(fdWKP, buffer, HANDSHAKE_BUFFER_SIZE); // write PP to server
  if (bytes < 0) {
    perror("write failed");
    exit(2);
  }

  // printf("pid: %s\n", buffer); // DEBUG

  int from_server = open(buffer, O_RDONLY, 0650);

  // printf("from_server (pp) fd: %d\n", from_server); // DEBUG

  // server -> client, RD
  bytes = read(from_server, buffer, HANDSHAKE_BUFFER_SIZE); // read randInt from server
  if (bytes < 0) {
    perror("read failed");
    exit(1);
  }

  // printf("randInt: %s\n", buffer); // DEBUG

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

  // printf("HANDSHAKE COMPLETE\n");

	// to_server = malloc(sizeof(int));
  // *to_server = fdWKP;
  *to_server = fdWKP;
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
