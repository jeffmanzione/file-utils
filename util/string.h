// string.h
//
// Created on: Jun 6, 2020
//     Author: Jeff Manzione

#ifndef UTIL_STRING_H_
#define UTIL_STRING_H_

#include <stdbool.h>
#include <stdlib.h>

// Returns true if [str] ends with [suffix], otherwise false.
//
// If either [str] or [suffix] are NULL, returns false. This function assumes
// that [str] and [suffix] are null-terminated strings.
bool ends_with(const char *str, const char *suffix);

// Returns true if [str] starts with [prefix], otherwise false.
//
// If either [str] or [prefix] are NULL, returns false. This function assumes
// that [str] and [prefix] are null-terminated strings.
bool starts_with(const char *str, const char *prefix);

// Returns true if char [c] is within the null-terminated string [str],
// otherwise false.
bool contains_char(const char str[], char c);

// Returns a character pointer to the first position of [needle] found in
// [haystack] and null if no match is found.
//
// [haystack_len] The max number of characters to be scanned in [haystack].
// [needle_len] The number of characters from [needle] with which to search.
//
// In the case that [haystack] and [needle] are null-terminated strings, the
// function can be used like this:
//
// char *pos = find_str(haystack, strlen(haystack), needle, strlen(needle));
char *find_str(char *haystack, size_t haystack_len, const char *needle,
               size_t needle_len);

#endif /* UTIL_STRING_H_ */