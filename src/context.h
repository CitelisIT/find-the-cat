#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { AND, OR } logical_op;

typedef struct {
  bool color;
  bool follow_links;
  int threads;
  logical_op filter_type;
} AppContext;

extern AppContext app_context;

void set_color();
void set_follow_links();
void set_threads(int n);
void set_filter_type(logical_op op);
#endif
