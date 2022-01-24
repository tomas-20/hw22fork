#include "pipe_networking.h"

void capitalize(char *string) {
  while (*string) {
    if ('a' <= *string && *string <= 'z') {
      *string -= 32;
    }
    string ++;
  }
}

int main() {

  int to_client;
  int from_client;

  char line[1000];
  while (1) {
    from_client = server_handshake( &to_client );
    do {
      read(from_client, line, sizeof line);
      printf("just converted [%s]", line);
      capitalize(line);
      printf(" to [%s]\n", line);
      write(to_client, line, sizeof line);
    } while (*line);
  }
  return 0;
}
