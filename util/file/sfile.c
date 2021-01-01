// sfile.c
//
// Created on: Jan 1, 2021
//     Author: Jeff Manzione

#include "util/file/sfile.h"

#include <string.h>

#include "alloc/alloc.h"

struct _SFILE {
  const char *str;
  const char *pos;
};

SFILE *sfile_open(const char *source) {
  SFILE *file = ALLOC2(SFILE);
  file->str = source;
  file->pos = source;
  return file;
}

void sfile_close(SFILE *file) { DEALLOC(file); }

char *sgets(char *str, int num, SFILE *stream) {
  if (NULL == stream || NULL == stream->str || NULL == str || num <= 0 ||
      '\0' == *stream->pos) {
    return NULL;
  }
  int chars_read = 0;
  while (chars_read < (num - 1)) {
    const char c = stream->pos[chars_read];
    // EOS is not included.
    if (c == '\0') {
      break;
    }
    if (c == '\n') {
      chars_read++;
      break;
    }
    chars_read++;
  }
  memmove(str, stream->pos, sizeof(char) * chars_read);
  str[chars_read] = '\0';
  stream->pos += chars_read;
  return str;
}