#include "filters.h"
#include "context.h"
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
    // TODO
    break;

  case OR:
    // TODO
    break;

  default:
    // Cases where filter_type is not set, throws an error
    // TODO
    break;
  }
  return true;
}
bool filter_name(char *filename, char *value) {
  regex_t regex;
  int match_res;

  if (regcomp(&regex, value, REG_EXTENDED))
    fprintf(stderr, "Regex not compiled");
  match_res = regexec(&regex, filename, 0, NULL, 0);
  regfree(&regex);
  if (match_res == REG_NOMATCH) {
    return false;
  } else {
    return true;
  }
}
