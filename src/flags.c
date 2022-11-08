#include "flags.h"
#include "errors.h"
#include <ctype.h>
#include <getopt.h>
#include <stddef.h>
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
    }
    FlagsList *next_flag = create_flags_list();
    add_flag(next_flag, type, value);
    tmp->next = next_flag;
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

#define NAME_FROM_OPT(opt) (options[(opt)].name)
#define FLAG_ERROR(flag) printf("Le flag %s n'est pas correct", (flag))
#define PRINT_FLAG_VALUE(flag, value)                                          \
  printf("La valeur du flag -%s est %s\n", (flag), (value))
#define ARG_IS_FLAG(arg) (arg[0] == '-')

FlagsList *parse_flags(int argc, char *argv[]) {
  FlagsList *flags = create_flags_list();
  static struct option options[] = {
      {"test", no_argument, 0, 't'},
      {"name", required_argument, 0, 'n'},
      {"size", required_argument, 0, 's'},
      {"date", required_argument, 0, 'd'},
      {"mime", required_argument, 0, 'm'},
      {"ctc", required_argument, 0, 'g'},
      {"dir", optional_argument, 0, 'f'},
      {"color", no_argument, 0, 'c'},
      {"perm", required_argument, 0, 'p'},
      {"link", no_argument, 0, 'l'},
      {"threads", required_argument, 0, 'x'},
      {"ou", no_argument, 0, 'o'},
      {0, 0, 0, 0},
  };
  opterr = 0;
  int opt;
  int opt_index = 0;
  while ((opt = getopt_long_only(argc, argv, "", options, &opt_index)) != -1) {
    switch (opt) {
    case 't':
      add_flag(flags, FLAG_TEST, "true");
      break;
    case 'n':
      if (ARG_IS_FLAG(optarg)) {
        FLAG_ERROR(NAME_FROM_OPT(opt_index));
        exit(1);
      }
      add_flag(flags, FLAG_NAME, optarg);
      break;
    case 's':
      if (ARG_IS_FLAG(optarg)) {
        FLAG_ERROR(NAME_FROM_OPT(opt_index));
        exit(1);
      }
      add_flag(flags, FLAG_SIZE, optarg);
      break;
    case 'd':
      if (ARG_IS_FLAG(optarg)) {
        FLAG_ERROR(NAME_FROM_OPT(opt_index));
        exit(1);
      }
      add_flag(flags, FLAG_DATE, optarg);
      break;
    case 'm':
      if (ARG_IS_FLAG(optarg)) {
        FLAG_ERROR(NAME_FROM_OPT(opt_index));
        exit(1);
      }
      add_flag(flags, FLAG_MIME, optarg);
      break;
    case 'g':
      if (ARG_IS_FLAG(optarg)) {
        FLAG_ERROR(NAME_FROM_OPT(opt_index));
        exit(1);
      }
      add_flag(flags, FLAG_CTC, optarg);
      break;
    case 'f':
      if (optarg == NULL && argv[optind] != NULL &&
          !ARG_IS_FLAG(argv[optind])) {
        add_flag(flags, FLAG_DIR, argv[optind]);
        optind++;
      } else {
        add_flag(flags, FLAG_DIR, "");
      }
      break;
    case 'c':
      add_flag(flags, FLAG_COLOR, "true");
      break;
    case 'p':
      if (ARG_IS_FLAG(optarg)) {
        FLAG_ERROR(NAME_FROM_OPT(opt_index));
        exit(1);
      }
      add_flag(flags, FLAG_PERM, optarg);
      break;
    case 'l':
      add_flag(flags, FLAG_LINK, "true");
      break;
    case 'x':
      if (ARG_IS_FLAG(optarg)) {
        FLAG_ERROR(NAME_FROM_OPT(opt_index));
        exit(1);
      }
      add_flag(flags, FLAG_THREADS, optarg);
      break;
    case 'o':
      add_flag(flags, FLAG_OU, "true");
      break;
    case '?':
      FLAG_ERROR(argv[optind - 1]);
      exit(1);
    }
  }
  return flags;
}
void display_test_flag(FlagsList *list) {
  if (list->next == NULL || list->next->flag == NULL) {
    return;
  } else {
    Flag *flag = list->next->flag;
    const char *flag_strings[] = {"-test", "-name", "-size",    "-date",
                                  "-mime", "-ctc",  "-dir",     "-color",
                                  "-perm", "-link", "-threads", "-ou"};
    printf("La valeur du flag %s est %s\n", flag_strings[flag->type],
           flag->value);
  }
}
