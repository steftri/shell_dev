#ifdef ARDUINO  
#include <Arduino.h>
#endif

#include <unity.h>

#include "TinyShell.h"




uint16_t gu16_cmd_not_found_called = 0;
uint16_t gu16_cmd_error_called = 0;
bool gb_cmd_help_called = false;
bool gb_cmd_error_called = false;
bool gb_cmd_testcommand1_called = false;
bool gb_cmd_testcommand2_called = false;
bool gb_cmd_testcommand3_called = false;
bool gb_cmd_testcommand4_called = false;


class MyTinyShell : public TinyShell
{
public:
  void printPrompt(void) override
  {
    // do nothing
  }

  void printCommandNotFound(const char *pc_Cmd) override
  {
    gu16_cmd_not_found_called++;
  }

  void printCommandError(const char *pc_Cmd, const int rc) override
  {
    gu16_cmd_error_called++;
    TEST_ASSERT_EQUAL_INT(5, rc);  
  }
};



class CmdHelp : public TinyShellCommand 
{
public:
  int exec(int argc, char *argv[]) override 
  {
    gb_cmd_help_called = true;
    TEST_ASSERT_EQUAL_INT16(1, argc);
    TEST_ASSERT_EQUAL_STRING("help", argv[0]);
    return 0;
  }
};


class CmdTest1 : public TinyShellCommand 
{
public:
  int exec(int argc, char *argv[]) override 
  {
    gb_cmd_testcommand1_called = true;
    TEST_ASSERT_EQUAL_INT16(3, argc);
    TEST_ASSERT_EQUAL_STRING("test1", argv[0]);
    TEST_ASSERT_EQUAL_STRING("arg1", argv[1]);  
    TEST_ASSERT_EQUAL_STRING("arg2", argv[2]);
    return 0;
  }
};


class CmdTest2 : public TinyShellCommand 
{
public:
  int exec(int argc, char *argv[]) override 
  {
    gb_cmd_testcommand2_called = true;
    TEST_ASSERT_EQUAL_INT16(3, argc);
    TEST_ASSERT_EQUAL_STRING("test2", argv[0]);
    TEST_ASSERT_EQUAL_STRING("arg 1", argv[1]);  
    TEST_ASSERT_EQUAL_STRING("arg\\2", argv[2]);
    return 0;
  }
};


class CmdTest3 : public TinyShellCommand 
{
public:
  int exec(int argc, char *argv[]) override 
  {
    gb_cmd_testcommand3_called = true;
    TEST_ASSERT_EQUAL_INT16(3, argc);
    TEST_ASSERT_EQUAL_STRING("test3", argv[0]);
    TEST_ASSERT_EQUAL_STRING("", argv[1]);  
    TEST_ASSERT_EQUAL_STRING(" ", argv[2]);
    return 0;
  }
};

class CmdTest4 : public TinyShellCommand 
{
public:
  int exec(int argc, char *argv[]) override 
  {
    gb_cmd_testcommand4_called = true;
    TEST_ASSERT_EQUAL_INT16(1, argc);
    TEST_ASSERT_EQUAL_STRING("test4", argv[0]);
    return 5;
  }
};


MyTinyShell myShell;

CmdHelp myHelpCmd;
CmdTest1 myTest1Cmd;
CmdTest2 myTest2Cmd;
CmdTest3 myTest3Cmd;
CmdTest4 myTest4Cmd;



void setUp(void) 
{
  // set stuff up here
  myShell.addCommand("help", &myHelpCmd);
  myShell.addCommand("test1", &myTest1Cmd);
  myShell.addCommand("test2", &myTest2Cmd);
  myShell.addCommand("test3", &myTest3Cmd);
  myShell.addCommand("test4", &myTest4Cmd);

  myShell.begin();
}



void tearDown(void) 
{
  // clean stuff up here
  myShell.end();
}



// helper 
void send_string(const char *pc_Str)
{
  for(auto i=0U; pc_Str[i]; i++)
    myShell.putChar(pc_Str[i]); 
}



// proceed unit test cases here

void test_not_found(void) 
{
  send_string("notfound\r");
  TEST_ASSERT_EQUAL(1, gu16_cmd_not_found_called);
}

void test_help(void) 
{
  send_string("help\r");
  TEST_ASSERT_EQUAL(true, gb_cmd_help_called);
}

void test_testcommand_normal(void) 
{
  send_string("test1 arg1 arg2\r");
  TEST_ASSERT_EQUAL(true, gb_cmd_testcommand1_called);
}

void test_testcommand_escaped(void) 
{
  send_string(" test2 \"arg 1\"\targ\\\\2\r");
  TEST_ASSERT_EQUAL(true, gb_cmd_testcommand2_called);
}

void test_testcommand_empty_quotes(void) 
{
  send_string("test3 \"\" \" \"\r");
  TEST_ASSERT_EQUAL(true, gb_cmd_testcommand3_called);
}


void test_command_error(void) 
{
  send_string("test4\r");
  TEST_ASSERT_EQUAL(1, gu16_cmd_error_called);
}


void setup()
{
#ifdef ARDUINO  
  delay(2000); // service delay
#endif  
  UNITY_BEGIN();

  RUN_TEST(test_not_found);
  RUN_TEST(test_help);
  RUN_TEST(test_testcommand_normal);  
  RUN_TEST(test_testcommand_escaped);
  RUN_TEST(test_testcommand_empty_quotes);
  RUN_TEST(test_command_error);

  UNITY_END(); // stop unit testing
}



void loop()
{
}



#ifndef ARDUINO
// only needed if unit test is running in an native environment
int main(int argc, char *argv[])
{
  setup();
  loop();
  return 0;
}
#endif