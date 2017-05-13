#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include "../src/fs.h"

using namespace std;

class FSTest : public ::testing::Test {
protected:
  FSTest() {
    system("rm -f disk0");
    system("rm -f disk1");
    system("./create_fs disk0");
    system("./create_fs disk1");
  }

  ~FSTest() {
    cout << "_________________________________" << endl;
  }
};

//test create_file return code 1 for success
TEST_F(FSTest, create_file_test) {
	myFileSystem f ((char*) "disk0");
	int code = f.create_file((char*)"test.c",1);
	ASSERT_EQ(1,code);

  f.ls();
}

//test delete_file return code -1 for failure
TEST_F(FSTest, delete_file_test_negative) {
	myFileSystem f ((char*) "disk0");
	int code = f.delete_file((char*)"test.c");
	ASSERT_EQ(-1,code);

  f.ls();
}

TEST_F(FSTest, delete_file_test_positive) {
  myFileSystem f ((char*) "disk0");
  int code = f.create_file((char*)"test.c",1);
  ASSERT_EQ(1,code);
  code = f.delete_file((char*)"test.c");
  ASSERT_EQ(1,code);

  f.ls();
}

TEST_F(FSTest, read_file_test_positive) {
  myFileSystem f ((char*) "disk0");
  int code = f.create_file((char*)"test.c",1);
  ASSERT_EQ(1,code);
  char buf[1024];
  code = f.read("test.c", 0, buf);

  char buf_expected[1024] = {0};

  for (int i = 0; i < 1024; i ++)
    ASSERT_EQ(buf[i], buf_expected[i]);

  ASSERT_EQ(1,code);

  buf[0] = 1;
  code = f.write("test.c", 0, buf);

  char new_buf[1024];
  code = f.read("test.c", 0, new_buf);

  ASSERT_EQ(new_buf[0], 1);

  for (int i = 1; i < 1024; i ++)
    ASSERT_EQ(new_buf[i], 0);

  f.ls();
}

TEST_F(FSTest, create_file_no_freeblocks_test){
  myFileSystem f ((char*) "disk0");
  int code = f.create_file((char*)"t0",8);
  ASSERT_EQ(1,code);
  string filename = "t";

  for(int i = 1; i < 15; i ++){
    std::ostringstream oss;
    oss << filename << i;
    code = f.create_file(strdup(oss.str().c_str()),8);
    ASSERT_EQ(1,code);
  }

  code = f.create_file("lastone",8);

  f.ls();
  
  ASSERT_EQ(-1, code);

  
  
}

TEST_F(FSTest, create_file_max_num_files_test){
  myFileSystem f ((char*) "disk0");
  int code = f.create_file((char*)"t0",8);
  ASSERT_EQ(1,code);
  string filename = "t";

  for(int i = 1; i < 16; i ++){
    std::ostringstream oss;
    oss << filename << i;
    code = f.create_file(strdup(oss.str().c_str()),1);
    ASSERT_EQ(1,code);
  }

  f.ls();

  code = f.create_file("nope", 2);
  ASSERT_EQ(-1, code);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
