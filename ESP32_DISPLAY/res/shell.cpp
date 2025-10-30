
#include "shell.h"


CMD_SHELL shell_commands[10] = {

  { "push",      "Upload a file",           push      },
  { "disp_test", "Perform a display test",  displTest },
  { "sys_info",  "Display the system info", sysInfo   },
  { "help",      "Display this table",      help      },

};

 /*const size_t BUFFER_SIZE = 256;
  size_t bufferIndex       = 0;
  char isBinData           = 0;
  uint8_t buffer[BUFFER_SIZE];*/

/**
 Uploading files and all kind of binary data.
 */
void push(void){

  isBinData = true;


  //isBinData = false;
}
/*
void init_shell(void){

    Serial.println("ESP-32 Shell terminal. Type 'help' to see the available commands. \n");
    Serial.println("Shell:> ");

    bufferIndex = 0;
    memset(buffer, 0, sizeof buffer);
  }
}

void shell(void){

  //Wait until a data is received over the serial bus
  while(Serial.available() > 0 && bufferIndex < BUFFER_SIZE) {
    buffer[bufferIndex++] = Serial.read();
  }

  //If buffer filled or message complete (define your condition), process it
  if(bufferIndex == 0 || isBinData){
    return;
  }

  //Find the first non-printable character(if any) and replace it by '\0'. i.e end of string
  for(int i=0;i < strlen((char*)buffer); i++){
    if(buffer[i] < 30 ){
      buffer[i]=0;
        break;
      }
  }

  //Search for a valid shell command within our CMD SHELL list and execute the corresponding function
  int size = sizeof(shell_commands)/sizeof(shell_commands[0]);

  for(int i=0; i<size; i++){
    if(strcmp((char*)buffer, shell_commands[i].command ) == 0){
      shell_commands[i].f_ptr();
    }
  }

  Serial.printf("Shell:> Received: %s(%d) bytes \n",buffer, bufferIndex);
  Serial.println("Shell:> ");

  bufferIndex = 0;
  memset(buffer, 0, sizeof buffer);
}


void help(void){

  Serial.println(" \n\n === Available commands === \n");

  int size = sizeof(shell_commands)/sizeof(shell_commands[0]);

  for(int i=0; i<size; i++){
    Serial.printf(" %s \t %s \n", shell_commands[i].command, shell_commands[i].description);
  }
  Serial.println("\n");
}

void displTest(void){
  Serial.println(" Display test....");
}

void sysInfo(void){
   printCpuInfo();
}
