#include <gtest/gtest.h>
#include "TinyShell.h"

// Global variables for test tracking
uint16_t gu16_cmd_not_found_called = 0;
uint16_t gu16_cmd_error_called = 0;
bool gb_cmd_help_called = false;
bool gb_cmd_testcommand1_called = false;
bool gb_cmd_testcommand2_called = false;
bool gb_cmd_testcommand3_called = false;
bool gb_cmd_testcommand4_called = false;

// Custom TinyShell implementation for testing
class MyTinyShell : public TinyShell {
public:
  void printPrompt(void) override {
    // Do nothing
  }

  void printCommandNotFound(const char *pc_Cmd) override {
    gu16_cmd_not_found_called++;
  }

  void printCommandError(const char *pc_Cmd, const int rc) override {
    gu16_cmd_error_called++;
    EXPECT_EQ(5, rc);
  }
};

// Command implementations
class CmdHelp : public TinyShellCommand {
public:
  int exec(int argc, char *argv[]) override {
    gb_cmd_help_called = true;
    EXPECT_EQ(1, argc);
    EXPECT_STREQ("help", argv[0]);
    return 0;
  }
};

class CmdTest1 : public TinyShellCommand {
public:
  int exec(int argc, char *argv[]) override {
    gb_cmd_testcommand1_called = true;
    EXPECT_EQ(3, argc);
    EXPECT_STREQ("test1", argv[0]);
    EXPECT_STREQ("arg1", argv[1]);
    EXPECT_STREQ("arg2", argv[2]);
    return 0;
  }
};

class CmdTest2 : public TinyShellCommand {
public:
  int exec(int argc, char *argv[]) override {
    gb_cmd_testcommand2_called = true;
    EXPECT_EQ(3, argc);
    EXPECT_STREQ("test2", argv[0]);
    EXPECT_STREQ("arg 1", argv[1]);
    EXPECT_STREQ("arg\\2", argv[2]);
    return 0;
  }
};

class CmdTest3 : public TinyShellCommand {
public:
  int exec(int argc, char *argv[]) override {
    gb_cmd_testcommand3_called = true;
    EXPECT_EQ(3, argc);
    EXPECT_STREQ("test3", argv[0]);
    EXPECT_STREQ("", argv[1]);
    EXPECT_STREQ(" ", argv[2]);
    return 0;
  }
};

class CmdTest4 : public TinyShellCommand {
public:
  int exec(int argc, char *argv[]) override {
    gb_cmd_testcommand4_called = true;
    EXPECT_EQ(1, argc);
    EXPECT_STREQ("test4", argv[0]);
    return 5;
  }
};

// Test fixture
class TinyShellTest : public ::testing::Test {
protected:
  MyTinyShell myShell;
  CmdHelp myHelpCmd;
  CmdTest1 myTest1Cmd;
  CmdTest2 myTest2Cmd;
  CmdTest3 myTest3Cmd;
  CmdTest4 myTest4Cmd;

  void SetUp() override {
    myShell.addCommand("help", &myHelpCmd);
    myShell.addCommand("test1", &myTest1Cmd);
    myShell.addCommand("test2", &myTest2Cmd);
    myShell.addCommand("test3", &myTest3Cmd);
    myShell.addCommand("test4", &myTest4Cmd);
    myShell.begin();
  }

  void TearDown() override {
    myShell.end();
  }

  void send_string(const char *pc_Str) {
    for (auto i = 0U; pc_Str[i]; i++) {
      myShell.putChar(pc_Str[i]);
    }
  }
};

// Test cases
TEST_F(TinyShellTest, TestNotFound) {
  send_string("notfound\r");
  EXPECT_EQ(1, gu16_cmd_not_found_called);
}

TEST_F(TinyShellTest, TestHelp) {
  send_string("help\r");
  EXPECT_TRUE(gb_cmd_help_called);
}

TEST_F(TinyShellTest, TestCommandNormal) {
  send_string("test1 arg1 arg2\r");
  EXPECT_TRUE(gb_cmd_testcommand1_called);
}

TEST_F(TinyShellTest, TestCommandEscaped) {
  send_string(" test2 \"arg 1\"\targ\\\\2\r");
  EXPECT_TRUE(gb_cmd_testcommand2_called);
}

TEST_F(TinyShellTest, TestCommandEmptyQuotes) {
  send_string("test3 \"\" \" \"\r");
  EXPECT_TRUE(gb_cmd_testcommand3_called);
}

TEST_F(TinyShellTest, TestCommandError) {
  send_string("test4\r");
  EXPECT_EQ(1, gu16_cmd_error_called);
}

// Main function for GoogleTest
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}