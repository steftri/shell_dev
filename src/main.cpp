/*
    echo.cpp

    This example registeres an example command ("echo") as well as the 
    three default callback functions which are called 
     - when a command prompt ("> ") shall be displayed
     - when an unknown command is recognized
     - when a command returned an error

    Created 2023-05-04
    By Stefan Trippler

    https://github.com/steftri/shell/

*/

#include <Arduino.h>
#include <shell.h>


// An object of type "Shell" is needed for operation.
Shell myShell;

// declarations of callbacks needed for the shell; see below for implementation
void shell_display_prompt(void);
void shell_cmd_not_found(char *pc_Cmd);
void shell_cmd_error(char *pc_Cmd, int rc);
int shell_cmd_echo(int argc, char *argv[]);



void setup()
{
  Serial.begin(115200);

  // registers the function which is to 
  // be called when a unknown command is recognized
  // (the function is implemented below)
  myShell.setCommandNotFoundCallback(&shell_cmd_not_found);

  // registers the function which is to 
  // be called when a command returned an error
  // (the function is implemented below)
  myShell.setCommandErrorCallback(&shell_cmd_error);

  // registers the function which outputs the command prompt
  // (the function is implemented below)
  myShell.setPromptCallback(&shell_display_prompt);

  // registers an example command (in this case "echo")
  // (the function is implemented below)
  // By default, up to 32 different commands can be registered. More
  // can be configured within the include file shell.h.
  myShell.addCommandCallback("echo", &shell_cmd_echo);

  // needed for outputting the initial command prompt
  myShell.begin();
}



void loop()
{
  if(Serial.available()) {
    char c = Serial.read();

    // mirror back entered characters
    if(c == '\r') {
      // if return, also send a complete newline sequence  
      Serial.println(); 
    } 
    else {
      Serial.write(c);
    }

    // handover the received character from the serial device 
    // to the shell command interface (this is important for the shell to work)
    myShell.putChar(c);
  }
}


// This callback is needed to output the command 
// prompt ("> " in this case) to the serial device. 
void shell_display_prompt(void)
{
  Serial.print("> ");
  return;
}


// This callback is called when an unknown command is received. 
void shell_cmd_not_found(char *pc_Cmd)
{
  Serial.print("Command '");
  Serial.print(pc_Cmd);
  Serial.println("' not found (try command 'echo')");
  return;
}



// This callback is called when a command returned an error code. 
void shell_cmd_error(char *pc_Cmd, int rc)
{
  Serial.print(pc_Cmd);
  Serial.print(" failed returncode ");
  Serial.print(rc);
  Serial.println();
}



// This is the callback for the command "echo". 
// It is called everytime the command is received.
// The parameter "argc" holds the number of arguments, 
// the parameter argv hold an array of arguments  
// (including the command itself as the first parameter) 
int shell_cmd_echo(int argc, char *argv[])
{
  if(argc<2) {
    Serial.println("usage: echo <text>");
    return 5; // an error occured - return anything different to 0
  }

  for(int i=1; i<argc; i++) {
    Serial.print(argv[i]);
    Serial.print(" ");
  }
  Serial.println();

  return 0; // command executed sucessfully
}
