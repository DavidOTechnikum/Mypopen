/**
 * @file mypopen.c
 * @author David Oberleitner
 * @brief Contains functions: mypopen, mypclose
 * @version 1
 * @date 2023-05-12
 */

#include "../include/mypopen.h"

/**
 *
 * @fn FILE *mypopen(const char *command, const char *type)
 * @brief Creates a pipe stream to or from a process.
 *
 * Opens a read or write stream to the shell.
 *
 * @param command string containing a shell command line
 * @param type 'w' for writing or 'r' for reading
 * @return FILE* standard I/O stream
 */

FILE *mypopen(const char *command, const char *type)
{
    int status;
    int fd[2];

    if ((pipe(fd)) != 0)
    {
        perror("pipe");
        exit;
    }
    pid_t pid = fork();                                         // pipe between parent and child
    if (pid == -1)
    {
        fprintf(stderr, "fork() unsuccessful");
        exit;
    }

    if (strcmp(type, "r") == 0)
    {
        if (pid == 0)                                           // child process
        {
            if ((close(fd[0])) != 0)
            {                                                   // does not need input from parent
                perror("r: child close fd[0]");
                exit;
            }
            if ((dup2(fd[1], 1)) == -1)
            {                                                   // 0: stdin, 1: stdout, 2: stderr; child fd[1] (to parent process) replaces stdout
                perror("r: dup2");
                exit;
            }
            if ((close(fd[1])) != 0)
            {                                                   // stdout is now file descriptor (pointer) to the pipe output, OG fd[1] is now obsolete, because we copied it
                perror("r: child close fd[1]");
                exit;
            }
            if ((execl("/bin/sh", "sh", "-c", command, NULL)) == -1)
            {                                                   // shell creates output for fd[1]
                perror("r: execl");
                exit;
            }
            return 0;
        }
        else                                                    // pid from fork > 0 means parent process
        {
            if ((close(fd[1])) != 0)
            {                                                   // fd[1] output to pipe (to child) is not needed, we expect data from the child
                perror("r: parent close fd[1]");
                exit;
            }
            return fdopen(fd[0], "r");                          // data from the child is now passed on (returned) to calling file handle
        }
    }
    else if (strcmp(type, "w") == 0)
    {
        if (pid == 0)
        {
            if ((close(fd[1])) != 0)
            {                                                   // child does not need pipe output to parent process
                perror("w: child close fd[1]");
                exit;
            }
            if ((dup2(fd[0], 0)) == -1)
            {                                                   // we now copy fd[0] to replace stdout
                perror("w: dup2");
                exit;
            }
            if ((close(fd[0])) != 0)
            {                                                   // fd[0] copied and OG "pointer" is not needed anymore
                perror("child close fd[0]");
                exit;
            }
            if ((execl("/bin/sh", "sh", "-c", command, NULL)) == -1)
            {                                                   // input goes to shell
                perror("w: execl");
                exit;
            }
            return 0;
        }
        else
        {
            if ((close(fd[0])) != 0)                            // parent closes pipe input
            {
                perror("parent close fd[0]");
                exit;
            }                                                   
            return fdopen(fd[1], "w");                          // returns pipe output to handle
        }
    }
    else
    {
        fprintf(stderr, "sh: type '%s' not supported\n", type);
        exit;
    }
}

/**
 * @fn int mypclose(FILE *stream)
 * @brief Closes a stream opened by mypopen.
 *
 * Closes a reading or writing stream.
 *
 * @param stream Stream to be closed.
 * @return int Return exit status of child process.
 */

int mypclose(FILE *stream)
{
    int status;
    if ((fclose(stream)) != 0)
    {
        perror("fclose stream");
        exit;
    }
    waitpid(-1, &status, 0);
    return status;
}