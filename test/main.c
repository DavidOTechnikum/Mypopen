#include <stdio.h>
#include <stdlib.h>
#include "../include/mypopen.h"

int main()
{
    // we create processes for both `ls` and `wc`
    // `popen` returns a FILE* that represents the process' stdout (`r`) or stdin (`w`)
    FILE *ls = mypopen("ls -la", "r");
    FILE *wc = mypopen("wc", "w");
    // we consume the output of `ls` and feed it to `wc` and we also print the lines
    char buf[1024];
    while (fgets(buf, sizeof(buf), ls) != NULL)
    {
        printf("%s", buf);
        fputs(buf, wc);
    }
    // once we're done, we close the streams
    mypclose(ls);
    mypclose(wc);
}