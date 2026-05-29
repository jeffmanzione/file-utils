
#include "file-utils/file_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "absl/strings/str_format.h"

namespace {
using testing::IsNull;
using testing::NotNull;

TEST(FileUtilsTest, FnFile) {
  char file_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkstemp(file_name);

  FILE *file = FILE_FN(file_name, "w");
  fprintf(file, "Hello, world!\n");
  fclose(file);
  file = FILE_FN(file_name, "r");

  char buffer[100];
  EXPECT_THAT(fgets(buffer, 100, file), NotNull());
  EXPECT_STREQ(buffer, "Hello, world!\n");
  fclose(file);
}

TEST(FileUtilsTest, MakeDirIfDoesNotExist) {
  char dir_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkdtemp(dir_name);

  EXPECT_TRUE(make_dir_if_does_not_exist(
      absl::StrFormat("%s/%s", dir_name, "test").c_str()));
}

TEST(FileUtilsTest, MakeDirIfDoesNotExistAlreadyPresent) {
  char dir_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkdtemp(dir_name);

#ifdef _WIN32
  mkdir(absl::StrFormat("%s/%s", dir_name, "test").c_str());
#else
  mkdir(absl::StrFormat("%s/%s", dir_name, "test").c_str(), 0700);
#endif

  EXPECT_FALSE(make_dir_if_does_not_exist(
      absl::StrFormat("%s/%s", dir_name, "test").c_str()));
}

TEST(FileUtilsTest, SplitPathFile) {
  char path[] = "alpha/beta/gamma/file.test";
  char *dir_path, *file_name, *ext;
  split_path_file(path, &dir_path, &file_name, &ext);
  EXPECT_STREQ(dir_path, "alpha/beta/gamma/");
  EXPECT_STREQ(file_name, "file");
  EXPECT_STREQ(ext, ".test");

  free(dir_path);
  free(file_name);
  free(ext);
}

TEST(FileUtilsTest, SplitPathFileNoExt) {
  char path[] = "alpha/beta/gamma/file";
  char *dir_path, *file_name, *ext;
  split_path_file(path, &dir_path, &file_name, &ext);
  EXPECT_STREQ(dir_path, "alpha/beta/gamma/");
  EXPECT_STREQ(file_name, "file");
  EXPECT_THAT(ext, IsNull());

  free(dir_path);
  free(file_name);
}

TEST(FileUtilsTest, SplitPathFileNoPath) {
  char path[] = "file.test";
  char *dir_path, *file_name, *ext;
  split_path_file(path, &dir_path, &file_name, &ext);
  EXPECT_STREQ(dir_path, "");
  EXPECT_STREQ(file_name, "file");
  EXPECT_STREQ(ext, ".test");

  free(dir_path);
  free(file_name);
  free(ext);
}

TEST(FileUtilsTest, SplitPathFileNoPathNoExt) {
  char path[] = "file";
  char *dir_path, *file_name, *ext;
  split_path_file(path, &dir_path, &file_name, &ext);
  EXPECT_STREQ(dir_path, "");
  EXPECT_STREQ(file_name, "file");
  EXPECT_THAT(ext, IsNull());

  free(dir_path);
  free(file_name);
}

TEST(FileUtilsTest, CombinePathFile) {
  char *combo = combine_path_file("path/to/a/", "file", ".test");
  EXPECT_STREQ(combo, "path/to/a/file.test");
  free(combo);
}

TEST(FileUtilsTest, CombinePathFileNoExt) {
  char *combo = combine_path_file("path/to/a/", "file", nullptr);
  EXPECT_STREQ(combo, "path/to/a/file");
  free(combo);
}

TEST(FileUtilsTest, GetLine) {
  char file_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkstemp(file_name);

  FILE *file = FILE_FN(file_name, "w");
  fprintf(file, "Hello, world 1!\nHello, world 2!\n");
  fclose(file);

  file = FILE_FN(file_name, "r");

  char *line = nullptr;
  size_t n = 100;
  EXPECT_EQ(getline(&line, &n, file), 16);
  EXPECT_STREQ(line, "Hello, world 1!\n");

  free(line);
  line = nullptr;

  EXPECT_EQ(getline(&line, &n, file), 16);
  EXPECT_STREQ(line, "Hello, world 2!\n");

  free(line);
  line = nullptr;

  EXPECT_EQ(getline(&line, &n, file), -1);

  fclose(file);
}

TEST(FileUtilsTest, GetAll) {
  char file_name[16] = "/tmp/fileXXXXXX";
  std::ignore = mkstemp(file_name);

  FILE *file = FILE_FN(file_name, "w");
  fprintf(file, "Hello, world 1!\nHello, world 2!\n");
  fclose(file);

  file = FILE_FN(file_name, "r");

  char *full_text = nullptr;
  EXPECT_EQ(getall(file, &full_text), 33);
  EXPECT_STREQ(full_text, "Hello, world 1!\nHello, world 2!\n");
  free(full_text);
}

TEST(FileUtilsTest, FileFromString) {
  FILE *file = file_from_string("Hello, world 1!\nHello, world 2!\n");

  char *full_text = nullptr;
  EXPECT_EQ(getall(file, &full_text), 33);
  EXPECT_STREQ(full_text, "Hello, world 1!\nHello, world 2!\n");
  free(full_text);
}

}  // namespace