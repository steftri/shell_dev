#include <gtest/gtest.h>

#include "TinyShell.h"



class TinyShellUnitTest : public ::testing::Test 
{
protected:
  TinyShell myTinyShell;
    
  void SetUp() override 
  {
    myTinyShell.begin();
  }

  void TearDown() override 
  {
    myTinyShell.end();
  }
};



TEST_F(TinyShellUnitTest, DefaultValueTest) 
{
//  EXPECT_EQ(myTinyShell.getValue(), 0u);
}



TEST_F(TinyShellUnitTest, SetValueTest) 
{
//  myTinyShell.setValue(4711);
//  EXPECT_EQ(myArduinoLibrary.getValue(), 4711u);
}



int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
