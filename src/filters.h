#ifndef _FILTERS_H_
#define _FILTERS_H_

#include "flags.h"
#include <stdbool.h>

typedef enum {
  FILTER_NAME,
  FILTER_SIZE_EQ,
  FILTER_SIZE_GT,
  FILTER_SIZE_LT,
  FILTER_DATE_GT,
  FILTER_DATE_LT,
  FILTER_MIME,
  FILTER_CTC,
  FILTER_DIR,
  FILTER_PERMS,
} filter_type;

typedef struct _FilterData {
  filter_type type;
  void *value;
} FilterData;

typedef struct _FilterList {
  FilterData *data;
  struct _FilterList *next;
} FilterList;

FilterList *create_filter_list();
void destroy_filter_list(FilterList *list);
void add_filter(FilterList *list, filter_type type, void *value);
void remove_filter(FilterList *list, filter_type type);
FilterList *flags_to_filters(FlagsList *flags);
bool filter_match(char *filename, FilterData *data);
bool all_filters_match(char *filename, FilterList *list);

bool filter_name(char *filename, char *value);
bool filter_size_eq(char *filename, long value);
bool filter_size_gt(char *filename, long value);
bool filter_size_lt(char *filename, long value);

#endif // _FILTERS_H_
