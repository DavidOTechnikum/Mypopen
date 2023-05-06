#include "../include/mypopen.h"

int main () {
    FILE *ls = mypopen("ls", "r");
    FILE *wc = mypopen("wc", "w");

    char buf[1024];
    while (fgets(buf, sizeof(buf), ls) != NULL) {
        fputs(buf, wc);
    }

    mypclose(ls);
    mypclose(wc);

    return EXIT_SUCCESS;
}