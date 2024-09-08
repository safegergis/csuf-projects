#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        close(STDOUT_FILENO);

        printf("This output will not be seen\n");
    }
    else
    {
        wait(NULL);
        printf("Child process completed\n");
    }

    return 0;
}