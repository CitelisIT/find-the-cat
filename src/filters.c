#include "filters.h"
#include "context.h"
#include "flags.h"
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

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
    free(tmp->data);
    free(tmp);
  }
}

void add_filter(FilterList *list, filter_type type, void *value) {
  if (list->data == NULL) {
    FilterData *data = malloc(sizeof(FilterData));
    data->type = type;
    data->value = value;
    list->data = data;
  } else {
    FilterList *tmp = list;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    FilterList *next_filter = create_filter_list();
    add_filter(next_filter, type, value);
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
  long size;
  long date;
  char *unit;
  int perms;
  char *value;
  while (flags != NULL) {
    value = flags->flag->value;
    switch (flags->flag->type) {
    case FLAG_TEST:
      break;
    case FLAG_NAME:
      add_filter(filters, FILTER_NAME, value);
      break;
    case FLAG_SIZE:
      size = labs(strtol(value, &unit, 10));
      if (strlen(unit) != 1) {
        fprintf(stderr, "%s is not a valide size value\n", value);
        exit(1);
      } else {
        switch (unit[0]) {
        case 'c':
          // size is in bytes so don't modify the value of the variable
          break;
        case 'k':
          size *= 1024;
          break;
        case 'M':
          size *= 1048576;
          break;
        case 'G':
          size *= 1073741824;
          break;
        default:
          fprintf(stderr, "%s is not a valid size value\n", value);
          exit(1);
        }
        switch (value[0]) {
        case '+':
          add_filter(filters, FILTER_SIZE_GT, &size);
          break;
        case '-':
          add_filter(filters, FILTER_SIZE_LT, &size);
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
          add_filter(filters, FILTER_SIZE_EQ, &size);
          break;
        default:
          fprintf(stderr, "%s is not a valid size value\n", value);
          exit(1);
        }
      }
      break;
    case FLAG_DATE:
      date = labs(strtol(value, &unit, 10));
      if (strlen(unit) != 1) {
        fprintf(stderr, "%s is not a valid date value\n", value);
        exit(1);
      } else {
        switch (unit[0]) {
        case 'm':
          date *= 60;
          break;
        case 'h':
          date *= 3600;
          break;
        case 'j':
          date *= 86400;
          break;
        default:
          fprintf(stderr, "%s is not a valid date value\n", value);
        }
        switch (value[0]) {
        case '+':
          add_filter(filters, FILTER_DATE_GT, &date);
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
          add_filter(filters, FILTER_DATE_LT, &date);
          break;
        default:
          fprintf(stderr, "%s is not a valid date value\n", value);
          exit(1);
        }
        break;
      }
    case FLAG_MIME:
      add_filter(filters, FILTER_MIME, value);
      break;
    case FLAG_CTC:
      add_filter(filters, FILTER_CTC, value);
      break;
    case FLAG_DIR:
      add_filter(filters, FILTER_DIR, value);
      break;
    case FLAG_COLOR:
      set_color(true);
      break;
    case FLAG_PERM:
      if (!_is_perm(value)) {
        fprintf(stderr, "%s is not a valid permission value\n", value);
        exit(1);
      } else {
        perms = atoi(value);
        add_filter(filters, FILTER_PERMS, &perms);
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
  switch (data->type) {
  case FILTER_NAME:
    return filter_name(filename, (char *)data->value);
  case FILTER_SIZE_EQ:
    // TODO
    return false;
  case FILTER_SIZE_GT:
    // TODO
    return false;
  case FILTER_SIZE_LT:
    // TODO
    return false;
  case FILTER_DATE_GT:
    // TODO
    return false;
  case FILTER_DATE_LT:
    // TODO
    return false;
  case FILTER_MIME:
    // TODO
    return false;
  case FILTER_CTC:
    // TODO
    return false;
  case FILTER_DIR:
    // TODO
    return false;
  case FILTER_PERMS:
    // TODO
    return false;
  default:
    return false;
  }
}

bool all_filters_match(char *filename, FilterList *list) {
  switch (app_context.filter_type) {
  case AND:
    while (list != NULL) {
      if (!filter_match(filename, list->data))
        return false;
      else
        list = list->next;
    }
    return true;
    break;
  case OR:
    while (list != NULL) {
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
}

bool filter_name(char *filename, char *value) {
  regex_t regex;
  int match_res;
  char buff[128];

  if ((match_res = regcomp(&regex, value, 0))) {
    regerror(match_res, &regex, buff, 128);
    fprintf(stderr, "Regex not compiled : %s\n", buff);
    exit(1);
  }
  match_res = regexec(&regex, filename, 0, NULL, 0);
  regfree(&regex);
  if (match_res == REG_NOMATCH) {
    return false;
  } else {
    return true;
  }
}
