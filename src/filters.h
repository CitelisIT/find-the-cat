#ifndef _FILTERS_H_
#define _FILTERS_H_

#include <stdbool.h>

typedef enum {NAME, SIZE_EQ, SIZE_GT, SIZE_LT, DATE_GT, DATE_LT, MIME, CTC} filter_type;

typedef struct _Filter_data {
    filter_type type;
    void *value;
} Filter_data;

typedef struct _Filter_list {
    Filter_data *data;
    struct _Filter_list *next;
} Filter_list;

Filter_list *create_filter_list();
void destroy_filter_list(Filter_list *list);
void add_filter(Filter_list *list, filter_type type, void *value);
void remove_filter(Filter_list *list, filter_type type);
bool filter_match(char *filename, Filter_data data);
bool filter_logical_match(enum {AND, OR} logical, char *filename, Filter_list *list);


#endif // _FILTERS_H_
