#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "signal.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <assert.h>

#include "Shell.h"
#include "commands.h"


static void do_unknown_command(struct Shell *this, const struct StringVector *args);
static void do_help(struct Shell *this, const struct StringVector *args);
static void do_exit(struct Shell *this, const struct StringVector *args);
static void do_cd(struct Shell *this, const struct StringVector *args);
static void do_pwd(struct Shell *this, const struct StringVector *args);
static void do_bash(struct Shell *this, const struct StringVector *args);
static void do_jobs(struct Shell *this, const struct StringVector *args);
static void modifTabJobs(int pid);


char *jobs[20];
pid_t jPid[20];
int cpt = 0;
int pid = 0;


static struct
{
  const char *name;
  Action action;
} actions[] = {
  {.name = "exit", .action = do_exit},
  {.name = "cd", .action = do_cd},
  {.name = "pwd", .action = do_pwd},
  {.name = "help", .action = do_help},
  {.name = "?", .action = do_help},
  {.name = "!", .action = do_bash},
  {.name = "jobs", .action = do_jobs},
  {.name = NULL, .action = do_unknown_command}};


  void modifJobs(){
    pid_t pid =  waitpid(-1, NULL, WNOHANG);
    if (pid != -1){
      modifTabJobs(pid);
    }
    signal(SIGCHLD, modifJobs);
  }



  Action get_action(char *name)
  {
    signal(SIGCHLD, modifJobs);

    int i = 0;
    while (actions[i].name != NULL && strcmp(actions[i].name, name) != 0)
    {
      i++;
    }
    return actions[i].action;
  }



  static void do_exit(struct Shell *this, const struct StringVector *args)
  {
    this->running = false;
  }

  static void do_help(struct Shell *this, const struct StringVector *args)
  {
    printf("-> Besoin d'aide ?\n Commandes possibles dans ce Shell :\n - exit\n - cd\n - help/?\n");
  }

  static void do_pwd(struct Shell *this, const struct StringVector *args){
    system("pwd");
  }

  static void do_cd(struct Shell *this, const struct StringVector *args)
  {
    char *chemin = string_vector_get(args, 1);

    if (NULL != chemin){
      chdir(chemin);
    }
    else
    {
      char *home = getenv("HOME");
      chdir(home);
    }
  }

  static void do_bash(struct Shell *this, const struct StringVector *args)
  {
    system("/bin/bash");
  }



  static void do_unknown_commandBackground(struct Shell *this, const struct StringVector *args){
    struct StringVector tokens = split_line(this->buffer);
    char *name = string_vector_get(& tokens, 0);

    pid_t fils = fork();

    if (fils == 0){
      int size = string_vector_size(& tokens) - 1;
      char *argu[size];
      printf("[%d] ",getpid());

      for (int i = 0; i < size; i++){
        malloc(strlen(string_vector_get(args, i)) * sizeof (char *[i]));
        argu[i] = string_vector_get(&tokens, i);
        printf("%s ", argu[i]);
      }
      printf("\n");
      argu[size] = 0;
      execvp(name, argu);
      exit(EXIT_SUCCESS);
    }

    jobs[cpt] = name;
    jPid[cpt] = fils;
    sleep(1);
    cpt ++;
    jobs[cpt] = NULL;
    jPid[cpt] = NULL;
  }

  static void do_unknown_command(struct Shell *this, const struct StringVector *args){
    struct StringVector tokens = split_line(this->buffer);
    char *name = string_vector_get(& tokens, 0);

    if (strcmp(string_vector_get(&tokens ,string_vector_size(& tokens) - 1), "&") == 0){
      do_unknown_commandBackground(this, args);
    }
    else {

      pid_t fils = fork();

      if (fils == 0){

        int size = string_vector_size(& tokens);
        char *argu[size];
        for (int i = 0; i < size; i++){
          malloc(strlen(string_vector_get(args, i)) * sizeof (char *[i]));
          argu[i] = string_vector_get(&tokens, i);
        }
        argu[size] = 0;
        execvp(name, argu);
        exit(EXIT_SUCCESS);
      }
      int status;
      waitpid(fils, &status, 0);
    }
  }

  static void modifTabJobs(int pid){
    for(int i = 0; i < 20 ; i ++){
      if(jPid[i] == pid){
        jPid[i] = NULL;
        jobs[i] = NULL;
      }
    }
  }

  static void do_jobs(struct Shell *this, const struct StringVector *args){
    for(int i = 0; i < 20 ; i ++){
      if(jPid[i] != NULL){
        printf("[%d] %s\n", jPid[i], jobs[i]);
      }
    }
  }
