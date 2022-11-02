#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <dirent.h>
#include <string.h>

#ifdef _WIN32
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif

typedef struct _FileTree {
  char *path;
  struct _FileTree *sibling;
  struct _FileTree *child;
} FileTree;

char *join_path(const char *base, const char *filename);

FileTree *construct_file_tree(char *base_path);

void destroy_file_tree(FileTree *tree);

#endif
