#include "pipe_networking.h"


/*=========================
  server_setup
  args:

  creates the WKP (upstream) and opens it, waiting for a
  connection.

  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;
  return from_client;
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


/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/

int rando() {
  int rand;
  int file = open("/dev/random", O_RDONLY);
  read(file, &rand, sizeof rand);
  close(file);
  return rand;
}

int server_handshake(int *prp) {
  mkfifo(WKP, 0644);
  printf("Server: just made well-known pipe %s\n", WKP);
  int wkp = open(WKP, O_RDONLY);
  printf("Server: just connected to well-known pipe %s\n", WKP);
  char prp_name[10];
  read(wkp, prp_name, sizeof prp_name);
  printf("Server: just learned that the private pipe is named %s\n", prp_name);
  remove(WKP);
  printf("Server: just removed well-known pipe %s\n", WKP);
  *prp = open(prp_name, O_WRONLY);
  printf("Server: just connected to private pipe %s\n", prp_name);
  int message = rando();
  write(*prp, &message, sizeof message);
  printf("Server: just sent secret message %d\n", message);
  int response;
  read(wkp, &response, sizeof response);
  printf("Server: just recieved response %d\n", response);
  if (response != message + 1) {
    printf("Server: I am not content with response %d as it should be %d\n", response, message + 1);
    exit(0);
  }
  printf("Server: I am content with response %d\n", response);
  return wkp;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *wkp) {
  char prp_name[10];
  sprintf(prp_name, "%d", getpid());
  mkfifo(prp_name, 0644);
  printf("Client: just made private pipe %s\n", prp_name);
  *wkp = open(WKP, O_WRONLY);
  printf("Client: just connected to well-known pipe %s\n", WKP);
  write(*wkp, prp_name, sizeof prp_name);
  int prp = open(prp_name, O_RDONLY);
  printf("Client: just connected to private pipe %s\n", prp_name);
  int message;
  read(prp, &message, sizeof message);
  printf("Client: just recieved secret message %d\n", message);
  remove(prp_name);
  printf("Client: just removed private pipe %s\n", prp_name);
  int response = message + 1;
  write(*wkp, &response, sizeof response);
  printf("Client: just sent response %d\n", response);
  return prp;
}
