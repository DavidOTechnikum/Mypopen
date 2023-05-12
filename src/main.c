#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/mypopen.h"

/*
int main()
{
    FILE *a = mypopen("ls", "r");
    FILE *b = mypopen("wc", "w");
    char buf[1024];
    while(fgets(buf, sizeof(buf), a)) {
        fputs(buf, b);
        //printf(" -> %s", buf);
    }

return 0;
}
*/

int main()
{
// we create processes for both `ls` and `wc`
// `popen` returns a FILE* that represents the process' stdout (`r`) or stdin (`w`)
FILE *ls = mypopen("ls -la", "r");
FILE *wc = mypopen("wc", "w");
// we consume the output of `ls` and feed it to `wc`
char buf[1024];
while (fgets(buf, sizeof(buf), ls) != NULL)
fputs(buf, wc);
// once we're done, we close the streams
mypclose(ls);
mypclose(wc);
}

/*
int main() {
    FILE *a = mypopen("ls -l", "w");
    char string[] = "main.c";
    fputs(string, a);
}
*/