// sfile.h
//
// Created on: Jan 1, 2021
//     Author: Jeff Manzione

#ifndef UTIL_FILE_SFILE_H_
#define UTIL_FILE_SFILE_H_

// Cross-platform solution for treating strings like FILEs.
typedef struct _SFILE SFILE;

// Creates a file-like string reader.
SFILE *sfile_open(const char *);

// Closes a file-like string reader.
void sfile_close(SFILE *);

// Reads characters from src and stores them as a C string into str until
// (num-1) characters have been read or either a newline or the end-of-file is
// reached, whichever happens first.
//
// A newline character makes fgets stop reading, but it is considered a valid
// character by the function and included in the string copied to str.
//
// A terminating null character is automatically appended after the characters
// copied to str.
//
// stream: Pointer to an array of chars where the string read is copied.
// num: Maximum number of characters to be copied into str (including the
// terminating null-character). src: The source string from which to read.

// On success, the function returns str.
char *sfile_gets(char *str, int num, SFILE *stream);

#endif /* UTIL_FILE_SFILE_H_ */