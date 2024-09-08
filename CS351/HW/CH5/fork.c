#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    printf("parent (pid: %d) \n", getpid());

    int rc = fork();
    int x = 44;
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        x = 50;
        printf("child value of x: %d (pid: %d) \n", x, getpid());
    }
    else
    {
        x = 66;
        printf("parent value of x: %d (pid: %d) \n", x, getpid());
    }
}