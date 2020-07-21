#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>


#include "Shell.h"

int main()
{
  struct Shell shell;
  shell_init(& shell);
  shell_run(& shell);
  shell_free(& shell);
  return (EXIT_SUCCESS);
}
