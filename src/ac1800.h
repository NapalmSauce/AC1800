#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

/*How many instructions there are, alphabetically organized.*/

#define A_COUNT 6
#define B_COUNT 21
#define C_COUNT 2
#define D_COUNT 13
#define E_COUNT 1
#define F_COUNT 0
#define G_COUNT 3
#define H_COUNT 0
#define I_COUNT 4
#define J_COUNT 0
#define K_COUNT 0
#define L_COUNT 21
#define M_COUNT 1
#define N_COUNT 3
#define O_COUNT 3
#define P_COUNT 2
#define Q_COUNT 0
#define R_COUNT 8
#define S_COUNT 27
#define T_COUNT 0
#define U_COUNT 0
#define V_COUNT 0
#define W_COUNT 0
#define X_COUNT 4
#define Y_COUNT 0
#define Z_COUNT 0


/*  Below are the increment values from mnemonics[]
*   to each corresponding letter. If any instruction
*   exist for the matching letter it'll always evaluate to < 255.
*/

#define NOTHING 0xFF

#define A_START 0
#define B_START A_COUNT
#define C_START B_START + B_COUNT
#define D_START C_START + C_COUNT
#define E_START D_START + D_COUNT
#define F_START NOTHING
#define G_START E_START + E_COUNT
#define H_START NOTHING
#define I_START G_START + G_COUNT
#define J_START NOTHING
#define K_START NOTHING
#define L_START I_START + I_COUNT
#define M_START L_START + L_COUNT
#define N_START M_START + M_COUNT
#define O_START N_START + N_COUNT
#define P_START O_START + O_COUNT
#define Q_START NOTHING
#define R_START P_START + P_COUNT
#define S_START R_START + R_COUNT
#define T_START NOTHING
#define U_START NOTHING
#define V_START NOTHING
#define W_START NOTHING
#define X_START S_START + S_COUNT
#define Y_START NOTHING
#define Z_START NOTHING

#define MNEM_MIN_LEN 2
#define MNEM_MAX_LEN 4

#define TOUPPER_MASK  0xDFDFDFDF

/*  Constants used for instructions definition in a sequence of up to three
*   bytes; they tell what parameter(s) to expect
*   and how to produce code that corresponds the provided input.
*/
#define ASIS		0x01
#define ADD_N		0x02
#define IMM8		0x03
#define ADDR8		0x04
#define IMM16		0x05
#define ADDR16	0x06
#define IN_OUT	0x07

/*  EXTEND when present must be OR'd with the first definition byte
*   (just after the instruction size byte), which will be one of the
*   seven constants above.
*/
#define EXTEND	0x08 

#define GT8BITS " cannot be represented within 8 bits.\n"
#define GT16BITS " cannot be represented within 16 bits.\n"

char * space_StepOver(char * where);
char cntAlpha(char * where);
char cntGraf(char * where);

int express(char * parseHere, unsigned long * value);
int getAddr(char * parseHere, unsigned long *value);
int getImm(char * parseHere, unsigned long *value);
