#ifndef ACTION_H
#define ACTION_H

#include "StringVector.h"

typedef void (*Action) (
  struct Shell *,
  const struct StringVector *
);
Action get_action(char * name);

#endif
