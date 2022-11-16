// file_util.h
//
// Created on: Jun 6, 2020
//     Author: Jeff Manzione

#ifndef UTIL_FILE_FILE_UTIL_H_
#define UTIL_FILE_FILE_UTIL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#ifdef _WIN32
#define ssize_t int
#endif

// Opens a [file] with the specified [op_type], handling potential issues with
// opening the file.
//
// Usage:
//   FILE *file = FILE_FN("myfile.txt", "r");
#define FILE_FN(fn, op_type) file_fn(fn, op_type, __LINE__, __func__, __FILE__)
FILE *file_fn(const char fn[], const char op_type[], int line_num,
              const char func_name[], const char file_name[]);

// Creates a directory at [path] if it does not exist, otherwise it has no
// effect.
//
// Returns true if the directory was created by this function, otherwise false.
bool make_dir_if_does_not_exist(const char path[]);

// Splits [full_path] into the directory path, file name, and extension.
//
// Usage:
//   char *dir_path, *file_name, *ext;
//   split_path_file("/path/to/my/file.ext", &dir_path, &file_name, &ext);
//   printf("%s", dir_path);  // "path/to/my"
//   printf("%s", file_name);  // "file"
//   printf("%s", ext);  // ".ext"
void split_path_file(const char full_path[], char **dir_path, char **file_name,
                     char **ext);

// Combines the provided [path], [file_name], and [ext] into a valid full path.
//
// Usage:
//   char *full_path = combine_path_file("path/to", "file", ".txt");
//   printf("%s", full_path);  // "path/to/file.txt"
//   full_path = combine_path_file("path/to", "file", NULL);
//   printf("%s", full_path);  // "path/to/file"
//   full_path = combine_path_file("path/to/", "file", NULL);
//   printf("%s", full_path);  // "path/to/file"
char *combine_path_file(const char path[], const char file_name[],
                        const char ext[]);

// Reads an entire line from stream, storing the address of the buffer
// containing the text into *lineptr.  The buffer is null-terminated and
// includes the newline character, if one was found.
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

// Reads the entire content of a file and returns sets *target to it.
// Allocates memory that must be freed. Returns the total length of the
// allocated string.
ssize_t getall(FILE *f, char **target);

#endif /* UTIL_FILE_FILE_UTIL_H_ */