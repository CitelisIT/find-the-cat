#include "errors.h"
#include "filesystem.h"
#include "flags.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *help =
    "Usage:\n"
    "  ftc <dir> [options]\n\n"
    "Finds a file matching the specified options under the <dir> "
    "directory.\n"
    "If no options are provided, it will list all the contents of the "
    "<dir> directory recursively.\n\n"
    "Options: \n"
    "  -test               Display the value of each flag for testing "
    "purposes.\n"
    "  -name     <NAME>    Returns files matching the name (or regex).\n"
    "  -size     <SIZE>    Returns files with the corresponding size.\n"
    "  -date     <DATE>    Returns file modified during the provided "
    "timeframe.\n"
    "  -mime     <MIME>    Returns file with the corresponding mime type.\n"
    "  -ctc      <TEXT>    Returns file containing the given text or regex.\n"
    "  -dir      [DIR]     Returns matching directories instead of file.\n"
    "                      If no argument is given, it will recusively list "
    "directories under <dir>.\n"
    "  -color              Colors the output of the program.\n"
    "  -perm     <PERM>    Returns file with the corresponding permissions "
    "(given in hex format).\n"
    "  -link               Follows symbolic links.\n"
    "  -threads  <NUM>     Starts the application using NUM threads.\n"
    "  -ou                 Returns files matching any of the options instead "
    "of all of them.\n\n"
    "The SIZE argument accepts the same format as the 'find' command: an "
    "optional '+' or '-' sign, an integer value and an optional unit.\nThe "
    "unit "
    "can be one of c, k, M or G, representing powers of 1024.\nBy default, ftc "
    "uses the 'c' unit, corresponding to a size in bytes.\n\n"
    "The DATE argument accepts an integer value and a unit of time.\nThe unit "
    "can be one of s, m, or h.\nOptionnally, you can add a '+' before the "
    "value "
    "to return files older than the specified date.\n\n"
    "The MIME argument can accept both the type (ex: 'text') or the type and "
    "subtype (ex: 'text/html').";

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("%s\n", help);
    return 0;
  } else {
    // Gets the path to run the program against and store it in a separate
    // variable
    char *path = calloc(1, strlen(argv[1]) + 1);
    strcpy(path, argv[1]);
    FlagsList *flags = parse_flags(argc, argv);
    // TODO : make app work
    if (flag_exists(flags, FLAG_TEST)) {
      display_test_flag(flags);
      free(path);
      destroy_flags_list(flags);
      return 0;
    }

    // Cleanup code here (destroying allocated structures)
    free(path);
    destroy_flags_list(flags);
  }
  return 0;
}
