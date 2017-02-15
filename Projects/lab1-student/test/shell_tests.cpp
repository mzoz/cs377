#include "../src/tsh.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

class ShellTest : public ::testing::Test
{
    protected:
        ShellTest(){} //constructor runs before each test
        virtual ~ShellTest(){} //destructor cleans up after tests
        virtual void SetUp(){} //sets up before each test (after constructor)
        virtual void TearDown(){} //clean up after each test, (before destructor)
};

//Testing isQuit for "quit"
TEST(ShellTest, Test1) {
    simple_shell s;
    EXPECT_TRUE(s.isQuit((char *)"quit"));
}

//Testing isQuit for "quit"
TEST(ShellTest, Test2) {
    simple_shell s;
    char *cmdTokens[25];
    s.parse_command((char *)"testing first second third", cmdTokens);
    printf("TOKENS: %s %s %s %s\n", cmdTokens[0], cmdTokens[1], cmdTokens[2], cmdTokens[3]);
    EXPECT_TRUE(strcmp((char *)"third", cmdTokens[3]) == 0);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
