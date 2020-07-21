#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "signal.h"


#include "Shell.h"
#include "StringVector.h"
#include "commands.h"


void shell_init(struct Shell *this)
{
  this->running = false;
  this->buffer = NULL;
  this->line_number = 0;
}

void shell_free(struct Shell *this)
{
  free(this->buffer);
  this->buffer = NULL;
  this->buffer_size = 0;
}

void shell_run(struct Shell *this)
{
  this->running = true;
  printf("* Début du Shell\n");
  while (this->running) {
    shell_read_line(this);
    shell_execute_line(this);
  }
  printf("* Arrêt du Shell\n");
}

void shell_read_line(struct Shell *this)
{
  this->line_number++;
  printf("%d> ", this->line_number);
  clearerr(stdin);
  ssize_t line_length = getline(&this->buffer,
    &this->buffer_size, stdin);
    (void)line_length;
  }

  void shell_execute_line(struct Shell *this)
  {
    struct StringVector tokens = split_line(this->buffer);
    int nb_tokens = string_vector_size(&tokens);

    if (nb_tokens == 0)
    {
      printf("-> Rien à faire !\n");
    }
    else
    {
      char *name = string_vector_get(&tokens, 0);
      Action action = get_action(name);
      action(this, &tokens);
      free(this->buffer);

    }
  }
