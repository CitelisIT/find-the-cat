#include "color.h"

void print_format(const char *format, const char *path) {
  printf("\x1b[%sm%s\x1b[0m\n", format, path);
}

const char *archive_extensions[] = {
    "tar",  "tgz", "arc",  "arj", "taz", "lzma", "t7z",  "zip", "z",   "dz",
    "gz",   "lrz", "lz",   "lzo", "xz",  "zst",  "tzs1", "bz2", "bz",  "tbz",
    "tbz2", "tz",  "deb",  "rpm", "jar", "war",  "ear",  "sar", "rar", "alz",
    "ace",  "zoo", "cpio", "7z",  "rz",  "cab",  "wim",  "swm", "dwm", "esd"};

const char *image_extensions[] = {
    "avif", "jpg",  "jpeg", "mjpg", "mjpeg", "gif",  "bmp", "pbm",  "pgm",
    "ppm",  "tga",  "xbm",  "xpm",  "tif",   "tiff", "png", "svg",  "svgz",
    "mng",  "pcx",  "mov",  "mpg",  "mpeg",  "m2v",  "mkv", "webm", "webp",
    "ogm",  "mp4",  "m4v",  "mp4v", "vob",   "qt",   "nuv", "wmv",  "asf",
    "rm",   "rmvb", "flc",  "avi",  "fli",   "flv",  "gl",  "dl",   "xcf",
    "xwd",  "yuv",  "cgm",  "emf",  "ogv",   "ogx"};

const char *audio_extensions[] = {"aac", "au",   "flac", "m4a", "mid", "midi",
                                  "mka", "mp3",  "mpc",  "ogg", "ra",  "wav",
                                  "oga", "opus", "spx",  "xspf"};

const char *backup_extensions[] = {
    "oga",     "opus",      "spx",      "xspf",     "bak",
    "old",     "orig",      "part",     "rej",      "swp",
    "tmp",     "dpkg-dist", "dpkg-old", "ucf-dist", "ucf-new",
    "ucf-old", "rpmnew",    "rpmorig",  "rpmsave"};

char *get_file_extension(const char *filename) {
  char *dot_index = strchr(filename, '.');
  if (dot_index) {
    return dot_index + 1;
  } else {
    return NULL;
  }
}

int is_archive(const char *file_extension) {
  for (int i = 0; i < 40; i++) {
    if (!strcmp(file_extension, archive_extensions[i])) {
      return 1;
    }
  }
  return 0;
}

int is_image(const char *file_extension) {
  for (int i = 0; i < 51; i++) {
    if (!strcmp(file_extension, image_extensions[i])) {
      return 1;
    }
  }
  return 0;
}

int is_audio(const char *file_extension) {
  for (int i = 0; i < 16; i++) {
    if (!strcmp(file_extension, audio_extensions[i])) {
      return 1;
    }
  }
  return 0;
}

int is_backup(const char *file_extension) {
  for (int i = 0; i < 19; i++) {
    if (!strcmp(file_extension, backup_extensions[i])) {
      return 1;
    }
  }
  return 0;
}

void print_filename(const char *path) {
  if (app_context.color) {
    struct stat file_stat;
    if (stat(path, &file_stat) == -1) {
      print_format("40;31;01", path);
    } else if (S_ISDIR(file_stat.st_mode)) {
      print_format("01;34", path);
    } else if (S_ISLNK(file_stat.st_mode)) {
      if (lstat(path, &file_stat) != 0) {
        print_format("40;31;01", path);
      } else {
        print_format("01;36", path);
      }
    } else if (S_ISFIFO(file_stat.st_mode)) {
      print_format("40;33", path);
    } else if (S_ISBLK(file_stat.st_mode)) {
      print_format("01;35", path);
    } else if (S_ISBLK(file_stat.st_mode) || S_ISCHR(file_stat.st_mode)) {
      print_format("40;33;01", path);
    } else if (S_ISREG(file_stat.st_mode)) {
      char *file_extension = get_file_extension(path);
      if (file_extension) {
        if (is_archive(file_extension)) {
          print_format("01;31", path);
        } else if (is_image(file_extension)) {
          print_format("01;35", path);
        } else if (is_audio(file_extension)) {
          print_format("00;36", path);
        } else if (is_backup(file_extension)) {
          print_format("00;90", path);
        } else if (S_IXUSR & file_stat.st_mode) {
          print_format("01;32", path);
        }
      }
    } else
      printf("%s\n", path);
  } else
    printf("%s\n", path);
}
