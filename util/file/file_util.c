// file_util.c
//
// Created on: Jun 6, 2020
//     Author: Jeff Manzione

#include "util/file/file_util.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "alloc/alloc.h"
#include "debug/debug.h"
#include "util/string.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define SLASH_CHAR '\\'
#else
#define SLASH_CHAR '/'
#include <unistd.h>
#endif

FILE *file_fn(const char fn[], const char op_type[], int line_num,
              const char func_name[], const char file_name[]) {
  if (NULL == fn) {
    __error(line_num, func_name, file_name, "Unable to read file.");
  }
  if (NULL == op_type) {
    __error(line_num, func_name, file_name, "Unable to read file '%s'.", fn);
  }
  FILE *file = fopen(fn, op_type);
  if (NULL == file) {
    __error(line_num, func_name, file_name,
            "Unable to read file '%s' with op '%s'.", fn, op_type);
  }
  return file;
}

bool make_dir_if_does_not_exist(const char path[]) {
  ASSERT(NOT_NULL(path));
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
  while ((next = strpbrk(slash + 1, "\\/")))
    slash = next;
  if (full_path != slash)
    slash++;
  int path_len = slash - full_path;
  *dir_path = ALLOC_STRNDUP(full_path, path_len);
  char *dot = (ext == NULL) ? NULL : strchr(slash + 1, '.');
  int filename_len = (dot == NULL) ? strlen(full_path) - path_len : dot - slash;
  *file_name = ALLOC_STRNDUP(slash, filename_len);
  if (dot != NULL) {
    *ext = ALLOC_STRDUP(dot);
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
  char *tmp = ALLOC_ARRAY2(char, full_len);
  memmove(tmp, path, path_len);
  tmp[path_len] = SLASH_CHAR;
  memmove(tmp + path_len + 1, file_name, filename_len);
  if (NULL != ext) {
    memmove(tmp + path_len + 1 + filename_len, ext, ext_len);
  }
  char *result = ALLOC_STRNDUP(tmp, full_len);
  DEALLOC(tmp);
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
    bufptr = ALLOC_ARRAY2(char, 128);
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
      bufptr = REALLOC(bufptr, char, size);
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
  char *string = ALLOC_ARRAY2(char, fsize + 1);
  fread(string, fsize, 1, f);
  fclose(f);
  string[fsize] = '\0';
  *target = string;
  return fsize + 1;
}

FILE *file_from_string(const char string[]) {
  FILE *tmp = tmpfile();
  if (NULL == tmp) {
    FATALF("Unable to create temp file.");
    return NULL;
  }
  fprintf(tmp, "%s", string);
  rewind(tmp);
  return tmp;
}