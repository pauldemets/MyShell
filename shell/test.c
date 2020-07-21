#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include "signal.h"



void fin_fils(int sig) {
  pid_t p = wait(NULL) ;
  printf("fin de %d !n", p) ;
}

int main()
{
  signal(SIGCHLD, fin_fils) ;
  if (fork() == 0) {
    printf("mabite\n" );
    exit(EXIT_SUCCESS);
  }
  sleep(2);
}
