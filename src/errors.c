#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ErrorStack *create_error_stack() {
  ErrorStack *stack = calloc(1, sizeof(ErrorStack));
  stack->color = false;
  return stack;
}

void free_error_stack(ErrorStack *stack) {
  Error *error = stack->errors;
  Error *tmp;
  while (error != NULL) {
    tmp = error;
    error = error->next;
    free(tmp);
  }
  free(stack);
}

Error *create_error(app_error_t code, char *payload, int *errnum) {
  Error *error = calloc(1, sizeof(Error));
  error->code = code;
  error->payload = payload;
  error->errnum = errnum;
  return error;
}

void add_error(ErrorStack *stack, Error *error) {
  if (stack->errors == NULL) {
    stack->errors = error;
  } else {
    Error *tmp = stack->errors;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = error;
  }
}

bool no_error(ErrorStack *stack) { return stack->errors == NULL; }

void throw_indiv(Error *error, bool color) {
  switch (error->code) {
  case (FLAG_UNKNOWN):
    unknown_flag_error(error->payload, color);
    break;
  case (FLAG_NO_VALUE):
    flag_value_not_found_error(error->payload, color);
    break;
  case (DIR_ERROR):
    directory_error(error->payload, *(error->errnum), color);
    break;
  }
}

void throw_stack(ErrorStack *stack) {
  Error *error = stack->errors;
  while (error != NULL) {
    throw_indiv(error, stack->color);
    error = error->next;
  }
  free_error_stack(stack);
  exit(1);
}

void throw(app_error_t code, char *payload, int *errnum) {
  ErrorStack *stack = create_error_stack();
  add_error(stack, create_error(code, payload, errnum));
  throw_stack(stack);
}

void unknown_flag_error(char *flag, bool color) {
  if (color) {
    fprintf(stderr,
            "\x1b[1m\x1b[31mError:\x1b[0m flag \x1b[1m\x1b[33m%s\x1b[0m is "
            "unknown.\n",
            flag);
  } else {
    fprintf(stderr, "Le flag %s n'est pas correct\n", flag);
  }
}

void flag_value_not_found_error(char *flag, bool color) {
  if (color) {
    fprintf(stderr,
            "\x1b[1m\x1b[31mError:\x1b[0m no value was provided for flag "
            "\x1b[1m\x1b[33m%s\x1b[0m.\n",
            flag);
  } else {
    fprintf(stderr, "Le flag %s n'est pas correct\n", flag);
  }
}

void directory_error(char *dir, int errnum, bool color) {
  if (color) {
    fprintf(stderr,
            "\x1b[1m\x1b[31mError\x1b[0m: Could not open directory "
            "\x1b[1m\x1b[33m%s\x1b : %s\n",
            dir, strerror(errnum));
  } else {
    fprintf(stderr, "Error: Could not open directory %s : %s\n", dir,
            strerror(errnum));
  }
}
