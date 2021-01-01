// file_info.h
//
// Created on: Jan 6, 2016
//     Author: Jeff Manzione

#ifndef UTIL_FILE_FILE_INFO_H_
#define UTIL_FILE_FILE_INFO_H_

#include <stdio.h>

#include "util/file/file_util.h"
#include "util/file/sfile.h"

typedef struct LineInfo_ {
  char *line_text;
  int line_num;
} LineInfo;

typedef struct FileInfo_ FileInfo;

FileInfo *file_info(const char fn[]);
FileInfo *file_info_file(FILE *file);
FileInfo *file_info_sfile(SFILE *file);
void file_info_set_name(FileInfo *fi, const char fn[]);
LineInfo *file_info_getline(FileInfo *fi);
const LineInfo *file_info_lookup(const FileInfo *fi, int line_num);
int file_info_len(const FileInfo *fi);
const char *file_info_name(const FileInfo *fi);
void file_info_delete(FileInfo *fi);
void file_info_close_file(FileInfo *fi);
FILE *file_info_fp(FileInfo *fi);

#endif /* UTIL_FILE_FILE_INFO_H_ */