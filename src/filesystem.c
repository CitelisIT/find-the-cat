#define _DEFAULT_SOURCE
#include "filesystem.h"
#include <dirent.h>
#include <stdlib.h>

char *join_path(const char *base, const char *filename) {
  char *joined = malloc(strlen(base) + strlen(PATH_SEP) + strlen(filename) + 1);
  size_t filename_size = strlen(filename);
  strcpy(joined, base);
  strncat(joined, PATH_SEP, 1);
  strncat(joined, filename, filename_size);
  return joined;
}

FileTree *make_tree_node(char *path) {
  FileTree *tree = calloc(1, sizeof(FileTree));
  tree->path = path;
  return tree;
}

FileTree *_append_sibling(FileTree *root, char *path) {
  if (root->path == NULL) {
    root->path = path;
    return root;
  }
  FileTree *tmp = root;
  while (tmp->sibling != NULL) {
    tmp = tmp->sibling;
  }
  tmp->sibling = make_tree_node(path);
  return tmp->sibling;
}

FileTree *_list_directory(char *path) {
  DIR *dirp = opendir(path);
  if (dirp == NULL) {
    // TODO : throw dir_error
    exit(1);
  }
  FileTree *root = calloc(1, sizeof(FileTree));
  FileTree *curr_node;
  struct dirent *curr_dirent = readdir(dirp);
  while (curr_dirent != NULL) {
    if (strcmp(curr_dirent->d_name, ".") != 0 &&
        strcmp(curr_dirent->d_name, "..") != 0) {
      char *dir_name = join_path(path, curr_dirent->d_name);
      curr_node = _append_sibling(root, dir_name);
      if (curr_dirent->d_type == DT_DIR) {
        curr_node->child = _list_directory(dir_name);
      }
    }
    curr_dirent = readdir(dirp);
  }
  closedir(dirp);
  return root;
}

FileTree *construct_file_tree(char *base_path) {
  FileTree *tree = make_tree_node(base_path);
  tree->child = _list_directory(base_path);
  return tree;
}

void destroy_file_tree(FileTree *tree) {
  if (tree->child != NULL) {
    destroy_file_tree(tree->child);
  }
  if (tree->sibling != NULL) {
    destroy_file_tree(tree->sibling);
  }
  free(tree->path);
  free(tree);
}
