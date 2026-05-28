// file_util.c
//
// Created on: Jun 6, 2020
//     Author: Jeff Manzione

#include "file-utils/file_utils.h"

#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>

#include "file-utils/string_utils.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define SLASH_CHAR '\\'
#else
#define SLASH_CHAR '/'
#include <unistd.h>
#endif

FILE *file_fn(const char fn[], const char op_type[], int line_num,
              const char func_name[], const char file_name[]) {
  if (NULL == fn) {
    fprintf(stderr, "%s:%d:%s: Unable to read file (null).\n", file_name,
            line_num, func_name);
    exit(1);
  }
  if (NULL == op_type) {
    fprintf(stderr, "%s:%d:%s: Unable to read file (%s as NULL).\n", file_name,
            line_num, func_name, fn);
    exit(1);
  }
  FILE *file = fopen(fn, op_type);
  if (NULL == file) {
    fprintf(stderr, "%s:%d:%s: Unable to read file (%s as '%s'): %s\n",
            file_name, line_num, func_name, fn, op_type, strerror(errno));
    exit(1);
  }
  return file;
}

bool make_dir_if_does_not_exist(const char path[]) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
#ifdef _WIN32
    mkdir(path);
#else
    mkdir(path, 0700);
#endif
    return true;
  }
  return false;
}

void split_path_file(const char full_path[], char **dir_path, char **file_name,
                     char **ext) {
  char *slash = (char *)full_path, *next;
  while ((next = strpbrk(slash + 1, "\\/"))) slash = next;
  if (full_path != slash) slash++;
  int path_len = slash - full_path;
  *dir_path = strndup(full_path, path_len);
  char *dot = (ext == NULL) ? NULL : strchr(slash + 1, '.');
  int filename_len = (dot == NULL) ? strlen(full_path) - path_len : dot - slash;
  *file_name = strndup(slash, filename_len);
  if (dot != NULL) {
    *ext = strdup(dot);
  } else if (ext != NULL) {
    *ext = NULL;
  }
}

char *combine_path_file(const char path[], const char file_name[],
                        const char ext[]) {
  int path_len = (ends_with(path, "/") || ends_with(path, "\\"))
                     ? strlen(path) - 1
                     : strlen(path);
  int filename_len = strlen(file_name);
  int ext_len = (NULL == ext) ? 0 : strlen(ext);
  int full_len = path_len + 1 + filename_len + ext_len;
  char *tmp = malloc(sizeof(char) * full_len);
  memmove(tmp, path, path_len);
  tmp[path_len] = SLASH_CHAR;
  memmove(tmp + path_len + 1, file_name, filename_len);
  if (NULL != ext) {
    memmove(tmp + path_len + 1 + filename_len, ext, ext_len);
  }
  char *result = strndup(tmp, full_len);
  free(tmp);
  return result;
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
  char *bufptr = NULL;
  char *p = bufptr;
  size_t size;
  int c;

  if (lineptr == NULL) {
    return -1;
  }
  if (stream == NULL) {
    return -1;
  }
  if (n == NULL) {
    return -1;
  }
  bufptr = *lineptr;
  size = *n;

  c = fgetc(stream);
  if (c == EOF) {
    return -1;
  }
  if (bufptr == NULL) {
    bufptr = malloc(sizeof(char) * 128);
    if (bufptr == NULL) {
      return -1;
    }
    size = 128;
  }
  p = bufptr;
  while (c != EOF) {
    if ((p - bufptr) > (size - 1)) {
      size = size + 128;
      // Handle realloc
      int offset = p - bufptr;
      bufptr = realloc(bufptr, sizeof(char) * size);
      if (bufptr == NULL) {
        return -1;
      }
      p = bufptr + offset;
    }
    *(p++) = c;
    if (c == '\n') {
      break;
    }
    c = fgetc(stream);
  }

  *(p++) = '\0';
  *lineptr = bufptr;
  *n = size;

  return p - bufptr - 1;
}

ssize_t getall(FILE *f, char **target) {
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *string = malloc(sizeof(char) * (fsize + 1));

#ifndef _WIN32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
  fread(string, fsize, 1, f);
#ifndef _WIN32
#pragma GCC diagnostic pop
#endif

  fclose(f);
  string[fsize] = '\0';
  *target = string;
  return fsize + 1;
}

FILE *file_from_string(const char string[]) {
  FILE *tmp = tmpfile();
  if (NULL == tmp) {
    fprintf(stderr, "Unable to create temp file.\n");
    exit(1);
    return NULL;
  }
  fprintf(tmp, "%s", string);
  rewind(tmp);
  return tmp;
}