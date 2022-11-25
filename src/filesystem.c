#define _GNU_SOURCE
#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>

char *join_path(const char *base, const char *filename) {

  char *last = strndupa(base + strlen(base) - 1, 1);
  if (strcmp(last, PATH_SEP)) {
    char *joined =
        malloc(strlen(base) + strlen(PATH_SEP) + strlen(filename) + 1);

    size_t filename_size = strlen(filename);
    strcpy(joined, base);
    strncat(joined, PATH_SEP, 1);
    strncat(joined, filename, filename_size);
    return joined;
  } else {
    char *joined =
        malloc(strlen(base) + strlen(PATH_SEP) + strlen(filename) + 1);
    size_t filename_size = strlen(filename);
    strcpy(joined, base);
    strncat(joined, filename, filename_size);
    return joined;
  }
}

void find_matching_files(char *path, FilterList *filters) {
  if (all_filters_match(path, filters))
    printf("%s\n", path);
  DIR *dirp = opendir(path);
  char *filename;
  if (dirp == NULL) {
    fprintf(stderr, "Could not open dir %s\n", path);
    exit(1);
  }
  struct dirent *curr_dirent = readdir(dirp);
  while (curr_dirent != NULL) {
    filename = curr_dirent->d_name;
    if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0) {
      char *joined_path = join_path(path, filename);
      if (curr_dirent->d_type == DT_DIR) {
        find_matching_files(joined_path, filters);
      } else {
        if (all_filters_match(joined_path, filters)) {
          printf("%s\n", joined_path);
        }

        free(joined_path);
      }
    }
    curr_dirent = readdir(dirp);
  }
  closedir(dirp);
  free(path);
}
