#ifndef _FILTERS_H_
#define _FILTERS_H_

#include <stdbool.h>

typedef enum {FILTER_NAME, FILTER_SIZE_EQ, FILTER_SIZE_GT, FILTER_SIZE_LT, FILTER_DATE_GT, FILTER_DATE_LT, FILTER_MIME, FILTER_CTC} filter_type;

typedef enum {AND, OR} logical_op;

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
bool filter_match(char *filename, FilterData *data);
bool filter_logical_match(logical_op logical, char *filename, FilterList *list);


#endif // _FILTERS_H_
