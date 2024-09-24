#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    else if (pid == 0)
    { // Child process

        // execl
        // execl("/bin/ls", "ls", "-l", NULL);

        // execle
        // char *const envp[] = { "PATH=/bin:/usr/bin", NULL };
        // execle("/bin/ls", "ls", "-l", NULL, envp);

        // execlp
        // execlp("ls", "ls", "-l", NULL);

        // execv
        // char *const argv[] = { "ls", "-l", NULL };
        // execv("/bin/ls", argv);

        // execvp
        // char *const argv[] = { "ls", "-l", NULL };
        // execvp("ls", argv);

        // execvpe 
        // char *const argv[] = { "ls", "-l", NULL };
        // char *const envp[] = { "PATH=/bin:/usr/bin", NULL };
        // execvpe("ls", argv, envp);

        // If exec() succeeds, this code won't be reached
        fprintf(stderr, "Exec failed\n");
        return 1;
    }
    else
    {               // Parent process
        wait(NULL); // Wait for child to finish
        printf("Child process completed\n");
    }

    return 0;
}