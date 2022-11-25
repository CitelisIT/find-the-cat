#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#define _DEFAULT_SOURCE

#include "filters.h"
#include <dirent.h>
#include <string.h>

#ifdef _WIN32
#define PATH_SEP "\\"
#define PATH_SEP_CHAR '\\'
#else
#define PATH_SEP "/"
#define PATH_SEP_CHAR '/'
#endif

char *join_path(const char *base, const char *filename);

void find_matching_files(char *path, FilterList *filters);

#endif
