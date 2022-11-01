#ifndef _FLAGS_H_
#define _FLAGS_H_

#include <stdbool.h>

typedef struct {
  bool test;
  char *name;
  char *size;
  char *date;
  char *mime;
  char *ctc;
  char *dir;
  bool color;
  char *perm;
  bool link;
  int threads;
  bool ou;
} Flags;

Flags *init_flags();
Flags *parse(int argc, char *argv[]);

#endif
