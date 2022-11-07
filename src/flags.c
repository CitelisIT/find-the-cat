#include "flags.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool _is_app_flag(char *flag) {
  return (strcmp(flag, "-test") == 0 || strcmp(flag, "-name") == 0 ||
          strcmp(flag, "-size") == 0 || strcmp(flag, "-date") == 0 ||
          strcmp(flag, "-mime") == 0 || strcmp(flag, "-ctc") == 0 ||
          strcmp(flag, "-dir") == 0 || strcmp(flag, "-color") == 0 ||
          strcmp(flag, "-perm") == 0 || strcmp(flag, "-link") == 0 ||
          strcmp(flag, "-threads") == 0 || strcmp(flag, "-ou") == 0);
}

FlagsList *create_flags_list() {
  FlagsList *list = calloc(1, sizeof(FlagsList));
  return list;
}

void destroy_flags_list(FlagsList *list) {
  FlagsList *tmp;
  while (list != NULL) {
    tmp = list;
    list = list->next;
    free(tmp->flag);
    free(tmp);
  }
}

void add_flag(FlagsList *list, flag_type type, char *value) {
  if (list->flag == NULL) {
    Flag *flag = calloc(1, sizeof(Flag));
    flag->type = type;
    flag->value = value;
    list->flag = flag;
  } else {
    FlagsList *tmp = list;
    while (tmp->next != NULL) {
      tmp = tmp->next;
      FlagsList *next_flag = create_flags_list();
      add_flag(next_flag, type, value);
      tmp->next = next_flag;
    }
  }
}

bool flag_exists(FlagsList *list, flag_type flag) {
  if (list->flag == NULL) {
    return false;
  } else {
    FlagsList *tmp = list;
    while (tmp != NULL) {
      if (tmp->flag->type == flag) {
        return true;
      } else {
        tmp = tmp->next;
      }
    }
    return false;
  }
}

char *get_flag_value(FlagsList *list, flag_type flag) {
  if (list->flag == NULL) {
    return NULL;
  } else {
    FlagsList *tmp = list;
    while (tmp != NULL) {
      if (tmp->flag->type == flag) {
        return tmp->flag->value;
      } else {
        tmp = tmp->next;
      }
    }
    return NULL;
  }
}

FlagsList *parse_flags(int argc, char *argv[]) {
  FlagsList *flags = create_flags_list();
  ErrorStack *error_stack = create_error_stack();
  for (int i = 2; i < argc; i++) {
    if (argv[i][0] == '-') { // The argv is a flag
      if (!_is_app_flag(argv[i])) {
        add_error(error_stack, create_error(FLAG_UNKNOWN, argv[i], NULL));
      } else {
        // Treating boolean flags first
        if (strcmp(argv[i], "-test") == 0) {
          add_flag(flags, TEST, "true");
        } else if (strcmp(argv[i], "-color") == 0) {
          add_flag(flags, COLOR, "true");
        } else if (strcmp(argv[i], "-ou") == 0) {
          add_flag(flags, OU, "true");
        }
        // Treating flags that take a parameter
        else {
          if (argv[i + 1] != NULL &&
              argv[i + 1][0] != '-') { // Next argv exists and is not a flag
            char *value = argv[i + 1];
            if (strcmp(argv[i], "-name") == 0) {
              add_flag(flags, NAME, value);
            } else if (strcmp(argv[i], "-size") == 0) {
              add_flag(flags, SIZE, value);
            } else if (strcmp(argv[i], "-date") == 0) {
              add_flag(flags, DATE, value);
            } else if (strcmp(argv[i], "-mime") == 0) {
              add_flag(flags, MIME, value);
            } else if (strcmp(argv[i], "-ctc") == 0) {
              add_flag(flags, CTC, value);
            } else if (strcmp(argv[i], "-dir") == 0) {
              add_flag(flags, DIR_FLAG, value);
            } else if (strcmp(argv[i], "-perm") == 0) {
              add_flag(flags, PERM, value);
            } else if (strcmp(argv[i], "-threads") == 0) {
              add_flag(flags, THREADS, value);
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
