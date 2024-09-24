#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int p1 = fork();
  int p[2];

  if (p1 < 0) {
    fprintf(stderr, "Fork failed");
  } else if (p1 == 0) {
    close(p[0]);
    dup2(p[1], STDOUT_FILENO);

    printf("hello, world");
  } else {
    int p2 = fork();

    if (p2 < 0) {
      fprintf(stderr, "fork failed");
    } else if (p2 == 0) {
      close(p[1]);
      dup2(p[0], STDIN_FILENO);

      char buffer[100];
      fgets(buffer, 100, stdin);

      printf("%s", buffer);
    } else {
      close(p[0]);
      close(p[1]);

      wait(NULL);
      wait(NULL);
    }
  }
  return 0;
}