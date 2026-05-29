
#include "file-utils/string_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using testing::IsNull;

TEST(StringUtilsTest, EndsWith) {
  EXPECT_TRUE(ends_with("abcdef", ""));
  EXPECT_TRUE(ends_with("abcdef", "f"));
  EXPECT_TRUE(ends_with("abcdef", "ef"));
  EXPECT_TRUE(ends_with("abcdef", "def"));
  EXPECT_TRUE(ends_with("abcdef", "cdef"));
  EXPECT_TRUE(ends_with("abcdef", "bcdef"));
  EXPECT_TRUE(ends_with("abcdef", "abcdef"));
  EXPECT_FALSE(ends_with("abcdef", "zabcdef"));
  EXPECT_FALSE(ends_with("abcdef", "abcdefz"));
  EXPECT_FALSE(ends_with("abcdef", NULL));
  EXPECT_FALSE(ends_with(NULL, "abcdef"));
}

TEST(StringUtilsTest, StartsWith) {
  EXPECT_TRUE(starts_with("abcdef", ""));
  EXPECT_TRUE(starts_with("abcdef", "a"));
  EXPECT_TRUE(starts_with("abcdef", "ab"));
  EXPECT_TRUE(starts_with("abcdef", "abc"));
  EXPECT_TRUE(starts_with("abcdef", "abcd"));
  EXPECT_TRUE(starts_with("abcdef", "abcde"));
  EXPECT_TRUE(starts_with("abcdef", "abcdef"));
  EXPECT_FALSE(starts_with("abcdef", "zabcdef"));
  EXPECT_FALSE(starts_with("abcdef", "abcdefz"));
  EXPECT_FALSE(starts_with("abcdef", NULL));
  EXPECT_FALSE(starts_with(NULL, "abcdef"));
}

TEST(StringUtilsTest, ContainsChar) {
  EXPECT_TRUE(contains_char("abcdef", 'a'));
  EXPECT_TRUE(contains_char("abcdef", 'b'));
  EXPECT_TRUE(contains_char("abcdef", 'c'));
  EXPECT_TRUE(contains_char("abcdef", 'd'));
  EXPECT_TRUE(contains_char("abcdef", 'e'));
  EXPECT_TRUE(contains_char("abcdef", 'f'));
  EXPECT_FALSE(contains_char("abcdef", 'g'));
  EXPECT_FALSE(contains_char("abcdef", 0x0));
  EXPECT_FALSE(contains_char(NULL, 'g'));
}

TEST(StringUtilsTest, FindStr) {
  char *found_pos = find_str("abcdef", strlen("abcdef"), "de", strlen("de"));
  EXPECT_STREQ(found_pos, "def");

  found_pos = find_str("abcdef", strlen("abcdef"), "ef", strlen("ef"));
  EXPECT_STREQ(found_pos, "ef");

  found_pos = find_str("abcdef", strlen("abcdef"), "b", strlen("b"));
  EXPECT_STREQ(found_pos, "bcdef");

  found_pos = find_str("abcdef", strlen("abcdef"), "a", strlen("a"));
  EXPECT_STREQ(found_pos, "abcdef");

  found_pos = find_str("abcdef", strlen("abcdef"), "g", strlen("g"));
  EXPECT_THAT(found_pos, IsNull());
}

}  // namespace