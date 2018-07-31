#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include "ac1800.h"

char * space_StepOver(char * where);

char cntAlpha(char * where);

const char * findMachineCode(char * name);

uint32_t assemble(const char * instrFormat);


char * parsePtr;

/* TO BE IMPLEMENTED  
int control(char ** parser) {


}

int findmacro(char ** parser) {


}

int createmacro(char ** parser){


}
*/

int main(int argC, char **argV) {

  char forbid68XX = 0;
  char forcePIC = 0;
  char myExampleBuf[256];
  const char * theInstruction;
  unsigned char * theBytes;
  uint32_t machineCode;
  for(;argC; argC--) {
  
  if(!strcmp(*argV++,"-S"))
  forbid68XX = 1;

  }
  puts("hello(I'm started up)");
  while(1) {
    while(1) {
      if(NULL == fgets(myExampleBuf, 256, stdin)) {
        puts("bye!");
        return(0);
      }
      if(myExampleBuf[0] != '\n') break;
      printf("please input something: ");
    }
    if (!strcmp(myExampleBuf,"exit\n")) {
      puts("bye!");
      return(0);
    }
    parsePtr = space_StepOver(myExampleBuf);
    theInstruction = findMachineCode(parsePtr);
    if(theInstruction) {
      printf("Instruction size: %d bytes\n",*theInstruction);
      unsigned int x = *theInstruction;
      if(forbid68XX && *(theInstruction+1) & EXTEND) {
        puts("180[4/5/6] instructions disabled because of -S flag");
      }
      else if(forcePIC && *(theInstruction+3) == ADDR16) {
        puts("long branches are disabled because of -pic flag"); 
      }
      machineCode = assemble(theInstruction);
      if(machineCode) {
        {
          theBytes = (uint8_t *)&machineCode;
          for(; x; x--) {
            printf("%.2X ",*theBytes++);
          }
          putc('\n',stdout);
        }
      }
    }
 } 
  
  return(0);
}
  
