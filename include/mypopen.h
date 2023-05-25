#ifndef B0436966_67FC_498E_A108_D6120BEAE275
#define B0436966_67FC_498E_A108_D6120BEAE275

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

FILE *mypopen(const char *command, const char *type);
int mypclose(FILE *stream);

#endif /* B0436966_67FC_498E_A108_D6120BEAE275 */
