#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mypopen.h"

FILE *mypopen(const char *command, const char *type)
{
    int status;
    int fd[2];

    pipe(fd);
    pid_t pid = fork();                                 // pipe between parent and child

    if (strcmp(type, "r") == 0)
    {
        if (pid == 0)                                   // child process
        {
            close(fd[0]);                               // does not need input from parent
            dup2(fd[1], 1);                             // 0: stdin, 1: stdout, 2: stderr; child fd[1] (to parent process) replaces stdout
            close(fd[1]);                               // stdout is now file descriptor (pointer) to the pipe output, OG fd[1] is now obsolete, because we copied it
            execl("/bin/sh", "sh", "-c", command, NULL);// shell creates output for fd[1]
            return 0;
        }
        else if (pid == -1)
        {
            fprintf(stderr, "fork() unsuccessful");
        }
        else                                            // pid from fork > 0 means parent process 
        {
            close(fd[1]);                               // fd[1] output to pipe (to child) is not needed, we expect data from the child
            return fdopen(fd[0], "r");                  // data from the child is now passed on (returned) to calling file handle 
        }
    }
    else if (strcmp(type, "w") == 0)
    {
        if (pid == 0)
        {
            close(fd[1]);                               // child does not need pipe output to parent process
            dup2(fd[0], 0);                             // we now copy fd[0] to replace stdout
            close(fd[0]);                               // fd[0] copied and OG "pointer" is not needed anymore
            execl("/bin/sh", "sh", "-c", command, NULL);// input goes to shell
            return 0;
        }
        else if (pid == -1)
        {
            fprintf(stderr, "fork() unsuccessful");
        }
        else
        {
            close(fd[0]);                               // parent closes pipe input
            return fdopen(fd[1], "w");                  // returns pipe output to handle
        }
    }
    else
    {
        fprintf(stderr, "sh: type '%s' not supported\n", type);
    }
}

int mypclose(FILE *stream)
{
    int status;
    fclose(stream);
    waitpid(-1, &status, 0);
    return status;
}