
#include "file-utils/sfile.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using testing::IsNull;
using testing::NotNull;

TEST(SfileTest, OpenClose) {
  SFILE *file = sfile_open("abcdef");
  EXPECT_THAT(file, NotNull());
  sfile_close(file);
}

TEST(SfileTest, GetsAll) {
  SFILE *file = sfile_open("abcdef");
  EXPECT_THAT(file, NotNull());

  char buf[20];

  EXPECT_EQ(sfile_gets(buf, 20, file), buf);
  EXPECT_STREQ(buf, "abcdef");

  sfile_close(file);
}

TEST(SfileTest, GetsSome) {
  SFILE *file = sfile_open("abcdef");
  EXPECT_THAT(file, NotNull());

  char buf[4];

  EXPECT_EQ(sfile_gets(buf, 4, file), buf);
  EXPECT_STREQ(buf, "abc");

  EXPECT_EQ(sfile_gets(buf, 4, file), buf);
  EXPECT_STREQ(buf, "def");

  sfile_close(file);
}

}  // namespace