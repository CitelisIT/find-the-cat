#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdbool.h>

typedef enum { FLAG_UNKNOWN, FLAG_NO_VALUE, DIR_ERROR } app_error_t;

typedef struct _Error {
  app_error_t code;
  char *payload;
  int *errnum;
  struct _Error *next;
} Error;

typedef struct {
  bool color;
  Error *errors;
} ErrorStack;

ErrorStack *create_error_stack();
void free_error_stack(ErrorStack *stack);
Error *create_error(app_error_t code, char *payload, int *errnum);
void add_error(ErrorStack *stack, Error *error);
bool no_error(ErrorStack *stack);

void throw_indiv(Error *error, bool color);
void throw_stack(ErrorStack *stack);
void throw(app_error_t code, char *payload, int *errnum);

void unknown_flag_error(char *flag, bool color);
void flag_value_not_found_error(char *flag, bool color);
void directory_error(char *dir, int errnum, bool color);

#endif
