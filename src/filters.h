#ifndef _FILTERS_H_
#define _FILTERS_H_

#define _GNU_SOURCE

#include "flags.h"
#include <stdbool.h>
#include <time.h>

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

typedef struct {
  char *string_value;
  long numeric_value;
} FilterArgument;

typedef struct _FilterData {
  filter_type type;
  FilterArgument *argument;
} FilterData;

typedef struct _FilterList {
  FilterData *data;
  struct _FilterList *next;
} FilterList;

FilterList *create_filter_list();
void destroy_filter_list(FilterList *list);
void add_filter_string(FilterList *list, filter_type type, char *value);
void add_filter_numeric(FilterList *list, filter_type type, long value);
void remove_filter(FilterList *list, filter_type type);
FilterList *flags_to_filters(FlagsList *flags);
bool filter_match(char *filename, FilterData *data);
bool all_filters_match(char *filename, FilterList *list);

bool filter_name(char *filename, char *value);
bool filter_size_eq(char *filename, long value);
bool filter_size_gt(char *filename, long value);
bool filter_size_lt(char *filename, long value);

bool filter_date_gt(char *filename, time_t date);
bool filter_date_lt(char *filename, time_t date);

bool filter_mime(char *filename, char *mimetype);

bool filter_ctc(char *filename, char *ctc);

bool filter_dir(char *dirname, char *value);

bool filter_perms(char *filename, long value);

#endif // _FILTERS_H_
