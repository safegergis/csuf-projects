#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    printf("parent (pid: %d) \n", getpid());

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        int rw = wait(NULL);
        printf("wait returns: %d (pid: %d) \n", rw, getpid());
    }
    else
    {
        printf("parent (pid: %d) \n", getpid());
    }
}