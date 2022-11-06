#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filters.h"

Filter_list *create_filter_list() {
    Filter_list *list = malloc(sizeof(Filter_list));
    list->data = NULL;
    list->next = NULL;
    return list;
}

void destroy_filter_list(Filter_list *list) {
    Filter_list *tmp;
    while (list != NULL) {
        tmp = list;
        list = list->next;
        free(tmp->data);
        free(tmp);
    }
}

void add_filter(Filter_list *list, filter_type type, void *value) {
    if (list->data == NULL) {
        Filter_data *data = malloc(sizeof(Filter_data));
        data->type = type;
        data->value = value;
        list->data = data;
    } else {
        Filter_list *tmp = list;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        Filter_list *next_filter = create_filter_list();
        add_filter(next_filter, type, value);
        tmp->next = next_filter;
    }
}

void remove_filter(Filter_list *list, filter_type type) {
    if (list->data == NULL) {
        return;
    }
    else{
        Filter_list *tmp = list;
        while (tmp != NULL) {
            if (tmp->data->type == type) {
                Filter_list *next = tmp->next;
                free(tmp->data);
                free(tmp);
                tmp = next;
            } else {
                tmp = tmp->next;
            }
        }
    }
}

bool filter_match(char *filename, Filter_data data) {
    switch (data.type) {
        case NAME:
            return strcmp(filename, data.value) == 0;
        case SIZE_EQ:
            // TODO
            return false;
        case SIZE_GT:
            // TODO
            return false;
        case SIZE_LT:
            // TODO
            return false;
        case DATE_GT:
            // TODO
            return false;
        case DATE_LT:
            // TODO
            return false;
        case MIME:
            // TODO
            return false;
        case CTC:
            // TODO
            return false;
        default:
            return false;
    }
}

bool filter_logical_match(enum {AND, OR} logical, char *filename, Filter_list *list) {
    // TODO
    return false;
}
