// file_info.c
//
// Created on: Jan 6, 2016
//     Author: Jeff Manzione

#include "util/file/file_info.h"

#include <stdio.h>

#include "alloc/alloc.h"
#include "debug/debug.h"

#define DEFAULT_NUM_LINES 128
#define MAX_LINE_LENGTH 512

struct FileInfo_ {
  char *name;
  FILE *fp;
  SFILE *sfp;
  LineInfo **lines;
  int num_lines;
  int array_len;
};

LineInfo *line_info(FileInfo *fi, char line_text[], int line_num) {
  LineInfo *li = ALLOC(LineInfo);
  li->line_text = ALLOC_STRDUP(line_text);
  li->line_num = line_num;
  return li;
}

void _line_info_delete(LineInfo *li) {
  DEALLOC(li->line_text);
  DEALLOC(li);
}

FileInfo *file_info(const char fn[]) {
  FILE *file = FILE_FN(fn, "r");
  FileInfo *fi = file_info_file(file);
  file_info_set_name(fi, fn);
  return fi;
}

inline void file_info_set_name(FileInfo *fi, const char fn[]) {
  ASSERT(NOT_NULL(fi), NOT_NULL(fn));
  fi->name = ALLOC_STRDUP(fn);
}

FileInfo *file_info_file(FILE *file) {
  FileInfo *fi = ALLOC(FileInfo);
  fi->name = NULL;
  fi->fp = file;
  fi->sfp = NULL;
  ASSERT_NOT_NULL(fi->fp);
  fi->num_lines = 0;
  fi->array_len = DEFAULT_NUM_LINES;
  fi->lines = ALLOC_ARRAY(LineInfo *, fi->array_len);
  return fi;
}

FileInfo *file_info_sfile(SFILE *file) {
  FileInfo *fi = ALLOC(FileInfo);
  fi->name = NULL;
  fi->fp = NULL;
  fi->sfp = file;
  fi->num_lines = 0;
  fi->array_len = DEFAULT_NUM_LINES;
  fi->lines = ALLOC_ARRAY(LineInfo *, fi->array_len);
  return fi;
}

inline void file_info_close_file(FileInfo *fi) {
  if (NULL == fi->fp) {
    return;
  }
  fclose(fi->fp);
  fi->fp = NULL;
}

void file_info_delete(FileInfo *fi) {
  ASSERT_NOT_NULL(fi);
  ASSERT_NOT_NULL(fi->lines);
  if (NULL != fi->name) {
    DEALLOC(fi->name);
    fi->name = NULL;
  }
  int i;
  for (i = 0; i < fi->num_lines; i++) {
    _line_info_delete(fi->lines[i]);
  }
  DEALLOC(fi->lines);
  file_info_close_file(fi);
  DEALLOC(fi);
}

void file_info_reset_lines(FileInfo *fi, int32_t num_lines) {
  fi->num_lines = num_lines;
}

char *_file_info_gets(FileInfo *fi, char *target, size_t num) {
  if (fi->fp != NULL) {
    return fgets(target, num, fi->fp);
  }
  if (fi->sfp != NULL) {
    return sfile_gets(target, num, fi->sfp);
  }
  ERROR("FileInfo missing FILE OR SFILE.");
  return NULL;
}

LineInfo *_file_info_append_line(FileInfo *fi, LineInfo *li) {
  fi->lines[fi->num_lines++] = li;
  if (fi->num_lines >= fi->array_len) {
    fi->array_len += DEFAULT_NUM_LINES;
    fi->lines = REALLOC(fi->lines, LineInfo *, fi->array_len);
  }
  return li;
}

LineInfo *_file_info_append_line_text(FileInfo *fi, char line_text[]) {
  return _file_info_append_line(fi, line_info(fi, line_text, fi->num_lines));
}

LineInfo *file_info_getline(FileInfo *fi) {
  char line[MAX_LINE_LENGTH];
  if (NULL == _file_info_gets(fi, line, MAX_LINE_LENGTH)) {
    return NULL;
  }
  return _file_info_append_line_text(fi, line);
}

inline const LineInfo *file_info_lookup(const FileInfo *fi, int line_num) {
  if (line_num < 1 || line_num > fi->num_lines) {
    return NULL;
  }
  return fi->lines[line_num - 1];
}

inline int file_info_len(const FileInfo *fi) { return fi->num_lines; }

inline const char *file_info_name(const FileInfo *fi) { return fi->name; }

void file_info_append(FileInfo *parent, FileInfo *child) {
  int i;
  for (i = 0; i < child->num_lines; ++i) {
    LineInfo *li = child->lines[i];
    li->line_num = parent->num_lines;
    _file_info_append_line(parent, li);
  }
  file_info_close_file(parent);
  parent->fp = child->fp;
  parent->sfp = child->sfp;
  DEALLOC(child->lines);
  DEALLOC(child);
}