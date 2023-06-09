#include <Arduino.h>
#include <unity.h>

#include "shell.h"



Shell myShell;

uint16_t gu16_cmd_not_found_called = 0;
uint16_t gu16_cmd_error_called = 0;
bool gb_cmd_help_called = false;
bool gb_cmd_error_called = false;
bool gb_cmd_testcommand1_called = false;
bool gb_cmd_testcommand2_called = false;
bool gb_cmd_testcommand3_called = false;
bool gb_cmd_testcommand4_called = false;


void cmd_not_found(char *pc_Cmd)
{
  gu16_cmd_not_found_called++;
  TEST_ASSERT_EQUAL_STRING("notfound", pc_Cmd);
  return;
}



int cmd_help(int argc, char *argv[])
{
  gb_cmd_help_called = true;
  TEST_ASSERT_EQUAL_INT16(1, argc);
  TEST_ASSERT_EQUAL_STRING("help", argv[0]);
  return 0;
}


void cmd_error(char *pc_Cmd, int rc)
{
  gu16_cmd_error_called++;
  TEST_ASSERT_EQUAL_INT(5, rc);
  return;
}



int cmd_testcommand1(int argc, char *argv[])
{
  gb_cmd_testcommand1_called = true;
  TEST_ASSERT_EQUAL_INT16(3, argc);
  TEST_ASSERT_EQUAL_STRING("test1", argv[0]);
  TEST_ASSERT_EQUAL_STRING("arg1", argv[1]);  
  TEST_ASSERT_EQUAL_STRING("arg2", argv[2]);
  return 0;
}


int cmd_testcommand2(int argc, char *argv[])
{
  gb_cmd_testcommand2_called = true;
  TEST_ASSERT_EQUAL_INT16(3, argc);
  TEST_ASSERT_EQUAL_STRING("test2", argv[0]);
  TEST_ASSERT_EQUAL_STRING("arg 1", argv[1]);  
  TEST_ASSERT_EQUAL_STRING("arg\\2", argv[2]);
  return 0;
}


int cmd_testcommand3(int argc, char *argv[])
{
  gb_cmd_testcommand3_called = true;
  TEST_ASSERT_EQUAL_INT16(3, argc);
  TEST_ASSERT_EQUAL_STRING("test3", argv[0]);
  TEST_ASSERT_EQUAL_STRING("", argv[1]);
  TEST_ASSERT_EQUAL_STRING(" ", argv[2]);
  return 0;
}


int cmd_testcommand4(int argc, char *argv[])
{
  gb_cmd_testcommand4_called = true;
  return 5;
}



void setUp(void) 
{
  // set stuff up here
  myShell.setCommandNotFoundCallback(&cmd_not_found);
  myShell.setCommandErrorCallback(&cmd_error);
  myShell.addCommandCallback("help", &cmd_help);
  myShell.addCommandCallback("test1", &cmd_testcommand1);
  myShell.addCommandCallback("test2", &cmd_testcommand2);
  myShell.addCommandCallback("test3", &cmd_testcommand3);
  myShell.addCommandCallback("test4", &cmd_testcommand4);

  myShell.begin();
}



void tearDown(void) 
{
  // clean stuff up here
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
  delay(2000); // service delay
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