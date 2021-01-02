// file_info.h
//
// Created on: Jan 6, 2016
//     Author: Jeff Manzione

#ifndef UTIL_FILE_FILE_INFO_H_
#define UTIL_FILE_FILE_INFO_H_

#include <stdio.h>

#include "util/file/file_util.h"
#include "util/file/sfile.h"

// Represents a line from a file.
typedef struct LineInfo_ {
  char *line_text;
  int line_num;
} LineInfo;

// Represents a buffered FILE.
typedef struct FileInfo_ FileInfo;

// Creates a FileInfo for the specified file name, [fn].
FileInfo *file_info(const char fn[]);
// Creates a FileInfo for the specified [file].
FileInfo *file_info_file(FILE *file);
// Creates a FileInfo or the specified [file].
FileInfo *file_info_sfile(SFILE *file);

// Sets the name for the file if not already provided by file_info().
void file_info_set_name(FileInfo *fi, const char fn[]);

// Reads one line from the file and returns it.
LineInfo *file_info_getline(FileInfo *fi);

// Looks up the line info for the specified [line_num].
const LineInfo *file_info_lookup(const FileInfo *fi, int line_num);

// Returns the current number of read lines for the file.
int file_info_len(const FileInfo *fi);

// Returns the name of the file or NULL if there is no name set.
const char *file_info_name(const FileInfo *fi);

// Deletes [fi], deallocating all lines and closing the underlying FILE if
// necessary.
void file_info_delete(FileInfo *fi);

// Closes the underlying FILE if there is one.
void file_info_close_file(FileInfo *fi);

// Appends the contents of child to parent, destroying child in the process.
//
// If the parent has not fully consumed its stream, it is closed.
void file_info_append(FileInfo *parent, FileInfo *child);

#endif /* UTIL_FILE_FILE_INFO_H_ */