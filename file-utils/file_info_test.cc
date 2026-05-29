
#include "file-utils/file_info.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>

namespace {

using testing::IsNull;
using testing::NotNull;

TEST(FileInfoTest, FileInfoGetLine) {
  char file_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkstemp(file_name);

  FILE *file = fopen(file_name, "w");
  fprintf(file, "Hello, world 1!\nHello, world 2!\nHello, world 3!\n");
  fclose(file);

  FileInfo *fi = file_info(file_name);

  LineInfo *line = file_info_getline(fi);
  EXPECT_STREQ(line->line_text, "Hello, world 1!\n");
  EXPECT_EQ(line->line_num, 0);

  line = file_info_getline(fi);
  EXPECT_STREQ(line->line_text, "Hello, world 2!\n");
  EXPECT_EQ(line->line_num, 1);

  line = file_info_getline(fi);
  EXPECT_STREQ(line->line_text, "Hello, world 3!\n");
  EXPECT_EQ(line->line_num, 2);

  EXPECT_THAT(file_info_getline(fi), IsNull());

  file_info_delete(fi);
}

TEST(FileInfoTest, FileInfoLookup) {
  char file_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkstemp(file_name);

  FILE *file = fopen(file_name, "w");
  fprintf(file, "Hello, world 1!\nHello, world 2!\nHello, world 3!\n");
  fclose(file);

  FileInfo *fi = file_info(file_name);

  while (file_info_getline(fi) != NULL)
    ;

  const LineInfo *line = file_info_lookup(fi, 1);
  EXPECT_THAT(line, NotNull());
  EXPECT_STREQ(line->line_text, "Hello, world 1!\n");

  line = file_info_lookup(fi, 2);
  EXPECT_THAT(line, NotNull());
  EXPECT_STREQ(line->line_text, "Hello, world 2!\n");

  line = file_info_lookup(fi, 3);
  EXPECT_THAT(line, NotNull());
  EXPECT_STREQ(line->line_text, "Hello, world 3!\n");

  file_info_delete(fi);
}

TEST(FileInfoTest, FileInfoLookupOutOfBounds) {
  char file_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkstemp(file_name);

  FILE *file = fopen(file_name, "w");
  fprintf(file, "Hello, world 1!\nHello, world 2!\nHello, world 3!\n");
  fclose(file);

  FileInfo *fi = file_info(file_name);

  while (file_info_getline(fi) != nullptr)
    ;

  EXPECT_THAT(file_info_lookup(fi, 0), IsNull());
  EXPECT_THAT(file_info_lookup(fi, 4), IsNull());

  file_info_delete(fi);
}

}  // namespace
