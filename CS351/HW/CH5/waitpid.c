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
        printf("child (pid: %d) \n", getpid());
    }
    else
    {
        int status;
        int rw = waitpid(rc, &status, 0);
        printf("wait returns: %d (pid: %d) \n", rw, getpid());
    }
}