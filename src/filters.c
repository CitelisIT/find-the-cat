#include "filters.h"
#include "../lib/MegaMimes.h"
#include "context.h"
#include "filesystem.h"
#include "flags.h"
#include <ctype.h>
#include <fts.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

FilterList *create_filter_list() {
  FilterList *list = malloc(sizeof(FilterList));
  list->data = NULL;
  list->next = NULL;
  return list;
}

void destroy_filter_list(FilterList *list) {
  FilterList *tmp;
  while (list != NULL) {
    tmp = list;
    list = list->next;
    if (tmp->data != NULL) {
      free(tmp->data->argument);
      free(tmp->data);
    }
    free(tmp);
  }
}

void add_filter_string(FilterList *list, filter_type type, char *value) {
  if (list->data == NULL) {
    FilterArgument *argument = calloc(1, sizeof(FilterArgument));
    FilterData *data = malloc(sizeof(FilterData));
    data->type = type;
    argument->string_value = value;
    data->argument = argument;
    list->data = data;
  } else {
    FilterList *tmp = list;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    FilterList *next_filter = create_filter_list();
    add_filter_string(next_filter, type, value);
    tmp->next = next_filter;
  }
}

void add_filter_numeric(FilterList *list, filter_type type, long value) {
  if (list->data == NULL) {
    FilterArgument *argument = calloc(1, sizeof(FilterArgument));
    FilterData *data = malloc(sizeof(FilterData));
    data->type = type;
    argument->numeric_value = value;
    data->argument = argument;
    list->data = data;
  } else {
    FilterList *tmp = list;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    FilterList *next_filter = create_filter_list();
    add_filter_numeric(next_filter, type, value);
    tmp->next = next_filter;
  }
}

void remove_filter(FilterList *list, filter_type type) {
  if (list->data == NULL) {
    return;
  } else {
    FilterList *tmp = list;
    while (tmp != NULL) {
      if (tmp->data->type == type) {
        FilterList *next = tmp->next;
        free(tmp->data->argument);
        free(tmp->data);
        free(tmp);
        tmp = next;
      } else {
        tmp = tmp->next;
      }
    }
  }
}

bool _is_number(char *value) {
  for (size_t i = 0; i < strlen(value); i++) {
    if (!isdigit(value[i]))
      return false;
  }
  return true;
}

bool _is_perm(char *value) {
  if (strlen(value) != 3)
    return false;
  else {
    for (int i = 0; i < 3; i++) {
      if (value[i] < 48 || value[i] > 55)
        return false;
    }
    return true;
  }
}

FilterList *flags_to_filters(FlagsList *flags) {
  FilterList *filters = create_filter_list();
  int _true = 1;
  long num_value;
  char *unit;
  char *value;
  while (flags != NULL && flags->flag != NULL) {
    value = flags->flag->value;
    switch (flags->flag->type) {
    case FLAG_TEST:
      break;
    case FLAG_NAME:
      add_filter_string(filters, FILTER_NAME, value);
      break;
    case FLAG_SIZE:
      num_value = labs(strtol(value, &unit, 10));
      if (strlen(unit) != 1) {
        fprintf(stderr, "%s is not a valide size value\n", value);
        exit(1);
      } else {
        switch (unit[0]) {
        case 'c':
          // size is in bytes so don't modify the value of the variable
          break;
        case 'k':
          num_value *= 1024;
          break;
        case 'M':
          num_value *= 1048576;
          break;
        case 'G':
          num_value *= 1073741824;
          break;
        default:
          fprintf(stderr, "%s is not a valid size value\n", value);
          exit(1);
        }
        switch (value[0]) {
        case '+':
          add_filter_numeric(filters, FILTER_SIZE_GT, num_value);
          break;
        case '-':
          add_filter_numeric(filters, FILTER_SIZE_LT, num_value);
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          add_filter_numeric(filters, FILTER_SIZE_EQ, num_value);
          break;
        default:
          fprintf(stderr, "%s is not a valid size value\n", value);
          exit(1);
        }
      }
      break;
    case FLAG_DATE:
      num_value = labs(strtol(value, &unit, 10));
      if (strlen(unit) != 1) {
        fprintf(stderr, "%s is not a valid date value\n", value);
        exit(1);
      } else {
        switch (unit[0]) {
        case 'm':
          num_value *= 60;
          break;
        case 'h':
          num_value *= 3600;
          break;
        case 'j':
          num_value *= 86400;
          break;
        default:
          fprintf(stderr, "%s is not a valid date value\n", value);
        }
        switch (value[0]) {
        case '+':
          add_filter_numeric(filters, FILTER_DATE_GT, num_value);
          break;
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          add_filter_numeric(filters, FILTER_DATE_LT, num_value);
          break;
        default:
          fprintf(stderr, "%s is not a valid date value\n", value);
          exit(1);
        }
        break;
      }
    case FLAG_MIME:
      add_filter_string(filters, FILTER_MIME, value);
      break;
    case FLAG_CTC:
      add_filter_string(filters, FILTER_CTC, value);
      break;
    case FLAG_DIR:
      add_filter_string(filters, FILTER_DIR, value);
      break;
    case FLAG_COLOR:
      set_color(true);
      break;
    case FLAG_PERM:
      if (!_is_perm(value)) {
        fprintf(stderr, "%s is not a valid permission value\n", value);
        exit(1);
      } else {
        num_value = strtol(value, NULL, 8);
        add_filter_numeric(filters, FILTER_PERMS, num_value);
      }
      break;
    case FLAG_LINK:
      set_follow_links();
      break;
    case FLAG_THREADS:
      if (!_is_number(value)) {
        fprintf(stderr, "%s us not a valid thread number\n", value);
        exit(1);
      } else {
        set_threads(atoi(value));
      }
      break;
    case FLAG_OU:
      set_filter_type(OR);
      break;
    }
    flags = flags->next;
  }
  return filters;
}

bool filter_match(char *filename, FilterData *data) {
  FilterArgument *argument = data->argument;
  switch (data->type) {
  case FILTER_NAME:
    return filter_name(filename, argument->string_value);
  case FILTER_SIZE_EQ:
    return filter_size_eq(filename, argument->numeric_value);
  case FILTER_SIZE_GT:
    return filter_size_gt(filename, argument->numeric_value);
  case FILTER_SIZE_LT:
    return filter_size_lt(filename, argument->numeric_value);
  case FILTER_DATE_GT:
    return filter_date_gt(filename, argument->numeric_value);
  case FILTER_DATE_LT:
    return filter_date_lt(filename, argument->numeric_value);
  case FILTER_MIME:
    return filter_mime(filename, argument->string_value);
  case FILTER_CTC:
    return filter_ctc(filename, argument->string_value);
  case FILTER_DIR:
    return filter_dir(filename, argument->string_value);
  case FILTER_PERMS:
    return filter_perms(filename, argument->numeric_value);
  default:
    return false;
  }
}

bool all_filters_match(char *filename, FilterList *list) {
  switch (app_context.filter_type) {
  case AND:
    while (list != NULL && list->data != NULL) {
      if (!filter_match(filename, list->data))
        return false;
      else
        list = list->next;
    }
    return true;
    break;
  case OR:
    while (list != NULL && list->data != NULL) {
      if (filter_match(filename, list->data))
        return true;
      else
        list = list->next;
    }
    return false;
    break;
  default:
    // Cases where filter_type is not set, throws an error
    fprintf(stderr, "App error: filtering type is not set\n");
    exit(1);
    break;
  }
  return true;
}

bool filter_name(char *filename, char *value) {
  regex_t regex;
  int match_res;
  char buff[128];
  char *name_position = strrchr(filename, PATH_SEP_CHAR);
  char *name;
  if (name_position == NULL || strcmp(name_position, "\0") == 0) {
    name = filename;
  } else {
    name = name_position + 1;
  }
  if ((match_res = regcomp(&regex, value, 0))) {
    regerror(match_res, &regex, buff, 128);
    fprintf(stderr, "Regex not compiled : %s\n", buff);
    exit(1);
  }
  match_res = regexec(&regex, name, 0, NULL, 0);
  regfree(&regex);
  if (match_res == REG_NOMATCH) {
    return false;
  } else {
    return true;
  }
}

bool filter_size_eq(char *path, long value) {
  struct stat file_stat;
  if (stat(path, &file_stat) == -1) {
    fprintf(stderr, "Error while getting file stats \n");
    exit(1);
  }
  return file_stat.st_size == value;
}

bool filter_size_gt(char *path, long value) {
  struct stat file_stat;
  if (stat(path, &file_stat) == -1) {
    fprintf(stderr, "Error while getting file stats \n");
    exit(1);
  }
  return file_stat.st_size > value;
}

bool filter_size_lt(char *path, long value) {
  struct stat file_stat;
  if (stat(path, &file_stat) == -1) {
    fprintf(stderr, "Error while getting file stats \n");
    exit(1);
  }
  return file_stat.st_size < value;
}

bool filter_date_gt(char *path, time_t date) {
  struct stat file_stat;
  if (stat(path, &file_stat) == -1) {
    fprintf(stderr, "Error while getting file stats \n");
    exit(1);
  }
  time_t actual_time = time(NULL);
  return (actual_time - file_stat.st_mtime) > date;
}

bool filter_date_lt(char *path, time_t date) {
  struct stat file_stat;
  if (stat(path, &file_stat) == -1) {
    fprintf(stderr, "Error while getting file stats \n");
    exit(1);
  }
  time_t actual_time = time(NULL);
  return (actual_time - file_stat.st_mtime) < date;
}

bool filter_mime(char *path, char *mimetype) {

  char *mime = getMegaMimeType(path);
  if (mime == NULL) {
    return false;
  } else {
    int slash_index = strchrnul(mime, '/') - mime;
    char *base = strndupa(mime, slash_index);
    return (strcmp(base, mimetype) == 0) || (strcmp(mime, mimetype) == 0);
  }
}

bool filter_ctc(char *path, char *ctc) {
  char *buffer = 0;
  long length;

  FILE *file = fopen(path, "rb");

  if (file) {
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, file);
    }
    fclose(file);
  } else {
    fprintf(stderr, "Error while opening file %s\n", path);
    exit(1);
  }

  if (buffer) {
    regex_t regex;
    int match_res;
    char buff[128];

    if ((match_res = regcomp(&regex, ctc, 0))) {
      regerror(match_res, &regex, buff, 128);
      fprintf(stderr, "Regex not compiled : %s\n", buff);
      exit(1);
    }
    match_res = regexec(&regex, path, 0, NULL, 0);
    regfree(&regex);
    free(buffer);
    if (match_res == REG_NOMATCH) {
      return false;
    } else {
      return true;
    }
  }

  else {
    fprintf(stderr, "Error while reading file %s\n", path);
    exit(1);
  }
}

bool filter_dir(char *dirname, char *value) {
  struct stat dir_stat;
  if (stat(dirname, &dir_stat) == -1) {
    fprintf(stderr, "Error while getting file stats \n");
    exit(1);
  }
  if (strlen(value) == 0) {
    return S_ISDIR(dir_stat.st_mode);
  } else {
    return (S_ISDIR(dir_stat.st_mode) && filter_name(dirname, value));
  }
}

bool filter_perms(char *path, long value) {
  struct stat file_stat;
  if (stat(path, &file_stat) == -1) {
    fprintf(stderr, "Error while getting file stats \n");
    exit(1);
  }
  return ((long)(file_stat.st_mode & 0xfff) == value);
}
