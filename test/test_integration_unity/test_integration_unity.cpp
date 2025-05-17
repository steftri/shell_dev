#include <unity.h>

#ifdef ARDUINO  
#include <Arduino.h>   // needed for service delay
#else
#include <string.h>    // needed for strlen
#endif

#include "TinyShell.h"



TinyShell myShell;

bool gb_cmd_testcommand1_called = false;


class MyTinyShellCommand : public TinyShellCommand 
{
public:
  int exec(int argc, char *argv[]) override 
  {
    gb_cmd_testcommand1_called = true;
    return 0; // Success
  }
};




void setUp(void) 
{
  // set stuff up here
}

void tearDown(void) 
{
  // clean stuff up here
}



void test_add_command(void) 
{
  MyTinyShellCommand myCommand1;

  (void)myShell.begin();
  
  // Add a command to the shell  
  TEST_ASSERT_EQUAL_UINT8((uint8_t)myShell.addCommand("mycommand1", &myCommand1), (uint8_t)TinyShell::ERc::OK);

  TEST_ASSERT_FALSE(gb_cmd_testcommand1_called);

  // Simulate input for the command
  // Note: The command is terminated with a carriage return '\r'
  const char *input = "mycommand1\r";
  for (size_t i = 0; i < strlen(input); ++i) 
  {
    myShell.putChar(input[i]);
  }

  TEST_ASSERT_TRUE(gb_cmd_testcommand1_called);

  (void)myShell.end();
}



void setup()
{
#ifdef ARDUINO  
    delay(2000); // service delay
#endif    
    UNITY_BEGIN();

    RUN_TEST(test_add_command);

    UNITY_END(); // stop unit testing
}



void loop()
{
  // nothing to do here, since unit test is finished
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