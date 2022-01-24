#include "pipe_networking.h"

void get_input(char *string, int length) {
  fgets(string, length, stdin);
  while (*string) {
    if (*string == '\n') {
      *string = '\0';
    }
    else {
      string ++;
    }
  }
}

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char line[1000];
  do {
    get_input(line, sizeof line);
    write(to_server, line, sizeof line);
    read(from_server, line, sizeof line);
    printf("[%s]\n", line);
  } while (*line);
  return 0;
}
