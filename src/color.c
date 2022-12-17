#include "color.h"

void print_filename(const char *path) {
  if (app_context.color) {
    struct stat file_stat;
    if (stat(path, &file_stat) == -1) {
      printf("%s\n", path);
    }
    if (S_ISDIR(file_stat.st_mode)) {
      printf("\x1b[34m%s\x1b[0m\n", path);
    } else if (S_ISLNK(file_stat.st_mode)) {
      printf("\x1b[36m%s\x1b[0m\n", path);
    } else if (S_ISREG(file_stat.st_mode)) {
      if (file_stat.st_mode & S_IXUSR)
        printf("\x1b[32m%s\x1b[0m\n", path);
      else
        printf("\x1b[0m%s\x1b[0m\n", path);
    }
  } else
    printf("%s\n", path);
}
