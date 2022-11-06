#include "flags.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Flags *init_flags() {
  Flags *flags = calloc(1, sizeof(Flags));
  flags->test = false;
  flags->name = NULL;
  flags->size = NULL;
  flags->date = NULL;
  flags->mime = NULL;
  flags->ctc = NULL;
  flags->dir = NULL;
  flags->color = false;
  flags->perm = NULL;
  flags->link = false;
  flags->threads = 1;
  flags->ou = false;
  return flags;
}

bool _is_app_flag(char *flag) {
  return (strcmp(flag, "-test") == 0 || strcmp(flag, "-name") == 0 ||
          strcmp(flag, "-size") == 0 || strcmp(flag, "-date") == 0 ||
          strcmp(flag, "-mime") == 0 || strcmp(flag, "-ctc") == 0 ||
          strcmp(flag, "-dir") == 0 || strcmp(flag, "-color") == 0 ||
          strcmp(flag, "-perm") == 0 || strcmp(flag, "-link") == 0 ||
          strcmp(flag, "-threads") == 0 || strcmp(flag, "-ou") == 0);
}

Flags *parse(int argc, char *argv[]) {
  Flags *flags = init_flags();
  ErrorStack *error_stack = create_error_stack();
  for (int i = 2; i < argc; i++) {
    if (argv[i][0] == '-') { // The argv is a flag
      if (!_is_app_flag(argv[i])) {
        add_error(error_stack, create_error(FLAG_UNKNOWN, argv[i], NULL));
      } else {
        // Treating boolean flags first
        if (strcmp(argv[i], "-test") == 0) {
          flags->test = true;
        } else if (strcmp(argv[i], "-color") == 0) {
          error_stack->color = true;
          flags->color = true;
        } else if (strcmp(argv[i], "-ou") == 0) {
          flags->ou = true;
        }
        // Treating flags that take a parameter
        else {
          if (argv[i + 1] != NULL &&
              argv[i + 1][0] != '-') { // Next argv exists and is not a flag
            char *value = argv[i + 1];
            if (strcmp(argv[i], "-name") == 0) {
              flags->name = value;
            } else if (strcmp(argv[i], "-size") == 0) {
              flags->size = value;
            } else if (strcmp(argv[i], "-date") == 0) {
              flags->date = value;
            } else if (strcmp(argv[i], "-mime") == 0) {
              flags->mime = value;
            } else if (strcmp(argv[i], "-ctc") == 0) {
              flags->ctc = value;
            } else if (strcmp(argv[i], "-dir") == 0) {
              flags->dir = value;
            } else if (strcmp(argv[i], "-perm") == 0) {
              flags->dir = value;
            } else if (strcmp(argv[i], "-threads") == 0) {
              flags->threads = atoi(value);
            }
          } else {
            // TODO : throw flag_value_not_found error
            add_error(error_stack, create_error(FLAG_NO_VALUE, argv[i], NULL));
          }
        }
      }
    }
  }
  if (no_error(error_stack)) {
    free_error_stack(error_stack);
    return flags;
  } else {
    throw_stack(error_stack);
    return NULL;
  }
}
