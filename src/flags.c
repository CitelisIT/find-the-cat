#include "flags.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Flags *init_flags() {
  Flags *flags = calloc(1, sizeof(Flags));
  flags->test = false;
  flags->name = NULL;
  flags->size = NULL;
  flags->date = NULL;
  flags->mime = NULL;
  flags->ctc = NULL;
  flags->dir = NULL;
  flags->color = false;
  flags->perm = NULL;
  flags->link = false;
  flags->threads = 1;
  flags->ou = false;
  return flags;
}

Flags *parse(int argc, char *argv[]) {
  Flags *flags = init_flags();
  for (int i = 0; i < argc; i++) {
    if (argv[i][0] == '-') { // The argv is a flag
      // Treating boolean flags first
      if (strcmp(argv[i], "-test") == 0) {
        flags->test = true;
      } else if (strcmp(argv[i], "-color") == 0) {
        flags->color = true;
      } else if (strcmp(argv[i], "-ou") == 0) {
        flags->ou = true;
      }
      // Treating flags that take a parameter
      else {
        if (argv[i + 1] != NULL &&
            argv[i + 1][0] != '-') { // Next argv exists and is not a flag
          char *value = argv[i + 1];
          if (strcmp(argv[i], "-name") == 0) {
            flags->name = value;
          } else if (strcmp(argv[i], "-size") == 0) {
            flags->size = value;
          } else if (strcmp(argv[i], "-date") == 0) {
            flags->date = value;
          } else if (strcmp(argv[i], "-mime") == 0) {
            flags->mime = value;
          } else if (strcmp(argv[i], "-ctc") == 0) {
            flags->ctc = value;
          } else if (strcmp(argv[i], "-dir") == 0) {
            flags->dir = value;
          } else if (strcmp(argv[i], "-perm") == 0) {
            flags->dir = value;
          } else if (strcmp(argv[i], "-threads") == 0) {
            flags->threads = atoi(value);
          } else {
            // The flag given is not a valid flag for our application
            // TODO : throw flag_not_found error
            printf("Unknown flag");
            free(flags);
            return NULL;
          }
        } else {
          // TODO : throw flag_value_not_found error
          printf("No flag value provided");
          free(flags);
          return NULL;
        }
      }
    }
  }
  return flags;
}
