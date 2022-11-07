#ifndef _FLAGS_H_
#define _FLAGS_H_

#include <stdbool.h>

typedef enum {
  TEST,
  NAME,
  SIZE,
  DATE,
  MIME,
  CTC,
  DIR_FLAG,
  COLOR,
  PERM,
  LINK,
  THREADS,
  OU
} flag_type;

typedef struct {
  flag_type type;
  char *value;
} Flag;

typedef struct _FlagsList {
  Flag *flag;
  struct _FlagsList *next;
} FlagsList;

FlagsList *create_flags_list();
void destroy_flags_list(FlagsList *list);
void add_flag(FlagsList *list, flag_type flag, char *value);
bool flag_exists(FlagsList *list, flag_type flag);
char *get_flag_value(FlagsList *list, flag_type flag);
FlagsList *parse_flags(int argc, char *argv[]);
void display_test_flag(FlagsList *list);

#endif
