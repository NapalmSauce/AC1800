
#include "ac1800.h"

  extern char * parsePtr;
  
uint32_t assemble(const unsigned char * definition) {

  uint32_t asmbuf = 0x000000ff;
  uint8_t * byteSequence = (uint8_t *)&asmbuf;
  int count;
  int nBytes;
  nBytes = *definition++;

  for(count = 0; count < nBytes;) {
  
    unsigned char formatCode;
    
    formatCode = *definition++;
    if(formatCode & EXTEND) {
      count++;
      formatCode &= 0x07;
//      printf("68");
      *byteSequence++ = 0x68;
    }
    
    switch(formatCode) {
      unsigned long constantValue;
    case ASIS:
//      printf("%.2X ",*definition);
      *byteSequence++ = *definition++;
      count++;
      break;
    case ADD_N:
      count++;
      {
      unsigned char x = 0;
      parsePtr = space_StepOver(parsePtr);
      if(*parsePtr == 'r' || *parsePtr == 'R') {
        x = *++parsePtr;  
        parsePtr++;
        if(isxdigit(x)) {
          if(x <= '9')
          x = *definition++ | (x - '0');
          else {
            x |= 0x20;
            x = *definition++ | (x - ('a' - 0xa));
          }
        }
        if(!x || isalnum(*parsePtr)) {
          puts("invalid register");
          return(0);
        }
      }
      else {
        puts("syntax error in register");
        return(0);
      }
//      printf("%.2X ",x);
      *byteSequence++ = x;
      }
      break;
    case IMM8:
      count++;
      if(getImm(parsePtr, &constantValue)) return(0);
      if(constantValue > 0xff) {
      printf("Immediate value%s",GT8BITS);
        return(0);
      }
//      printf("%.2X ",(int)constantValue);
      *byteSequence++ = constantValue;
      break;
    case ADDR8:
      count++;
      if(getAddr(parsePtr, &constantValue)) return(0);
      if(constantValue > 0xff) {
      printf("Short branch address%s",GT8BITS);
        return(0);
      /*if(curAddr & 0xff00 != constantValue & 0xff00) */
      //above is to be implemented later.
      }
//      printf("%.2X ",(int)constantValue);
      *byteSequence++ = constantValue;
      break;
    case IMM16:
      count += 2;
      if(getImm(parsePtr, &constantValue)) return(0);
      if(constantValue > 0xffff) {
        printf("Immediate value%s",GT16BITS);
        return(0);
      }
//      printf("%.4x ",(int)constantValue);
      *byteSequence++ = constantValue >> 8;
      *byteSequence++ = constantValue;
      break;
    case ADDR16:
      count += 2;
      if(getAddr(parsePtr, &constantValue)) return(0);
      if(constantValue > 0xffff) {
        printf("Long branch address%s",GT16BITS);

        return(0);
      }
//      printf("%.4x ",(int)constantValue);
      *byteSequence++ = constantValue >> 8;
      *byteSequence++ = constantValue;
      break;
    case IN_OUT:
      count++;
      parsePtr = space_StepOver(parsePtr);
      if(*parsePtr <= '7' && *parsePtr >= '1') {
//      printf("%.2x ", *parsePtr - '0' + *definition);
      *byteSequence++ = *parsePtr++ - '0' + *definition++;
      }
      else {
        puts("INP/OUT must range from 1 to 7");
        return(0);
      }
    }
    
    if(count == 2 && nBytes == 4) {
      if(*parsePtr != ',') {
        puts("missing separating comma between parameters");
        return(0);
      }
      parsePtr++;
    }
  }
//  putc('\n',stdout);
  return(asmbuf);
}

const unsigned char * findMachineCode(char * parseHere){
    
  static const uint32_t knownMnemonics[] = {  
    /* A */
    'AND',      /*AND*/
    'ADC',      /*ADC*/
    'ADCI',     /*ADCI*/
    'ADI',      /*ADI*/
    'ANI',      /*ANI*/
    'ADD',      /*ADD*/
    //End of mnemonics for A
    /* B */
    'BR',       /*BR*/
    'BQ',       /*BQ*/
    'BZ',       /*BZ*/
    'BDF',      /*BDF*/
    'BGE',      /*BGE*/
    'BPZ',      /*BPZ*/
    'B1',       /*B1*/
    'B1',       /*B2*/
    'B3',       /*B3*/
    'B4',       /*B4*/
    'BNQ',      /*BNQ*/
    'BNZ',      /*BNZ*/
    'BNF',      /*BNF*/
    'BM',       /*BM*/
    'BL',       /*BL*/
    'BN1',      /*BN1*/
    'BN2',      /*BN2*/
    'BN3',      /*BN3*/
    'BN4',      /*BN4*/
    'BCI',      /*BCI*/
    'BXI',      /*BXI*/
    //End of mnemonics for B
    /* C */
    'CIE',      /*CIE*/
    'CIE',      /*CID*/
    //End of mnemonics for C
    /* D */
    'DEC',      /*DEC*/
    'DTC',      /*DTC*/
    'DBNZ',     /*DBNZ*/
    'DADC',     /*DADC*/
    'DSAV',     /*DSAV*/
    'DSMB',     /*DSMB*/
    'DACI',     /*DACI*/
    'DSBI',     /*DSBI*/
    'DADD',     /*DADD*/
    'DSM',      /*DSM*/
    'DADI',     /*DADI*/
    'DSMI',     /*DSMI*/
    'DIS',      /*DIS*/
    //End of mnemonics for D
    /* E */
    'ETA',      /*ETA*/
    //End of mnemonics for E
    /* G */
    'GEC',      /*GEC*/
    'GLO',      /*GLO*/
    'GHI',      /*GHI*/
    //End of mnemonics for G
    /* I */
    
    'INP',      /*IN*/
    'IDL',      /*IDL*/
    'INC',      /*INC*/
    'IRX',      /*IRX*/
    //End of mnemonics for I
    /* L */
    'LDN',      /*LDN*/
    'LDA',      /*LDA*/
    'LDC',      /*LDC*/
    'LDXA',     /*LDXA*/
    'LBR',      /*LBR*/
    'LBQ',      /*LBQ*/
    'LBZ',      /*LBZ*/
    'LBDF',     /*LBDF*/
    'LSNQ',     /*LSNQ*/
    'LSNZ',     /*LSNZ*/
    'LSNF',     /*LSNF*/
    'LSKP',     /*LSKP*/
    'LBNQ',     /*LBNQ*/
    'LBNZ',     /*LBNZ*/
    'LBNF',     /*LBNF*/
    'LSIE',     /*LSIE*/
    'LSQ',      /*LSQ*/
    'LSZ',      /*LSZ*/
    'LSDF',     /*LSDF*/
    'LDX',      /*LDX*/
    'LDI',      /*LDI*/
    //End of mnemonics for L
    /* M */
    'MARK',     /*MARK*/
    //End of mnemonics for M
    /* N */
    'NOP',      /*NOP*/
    'NBR',      /*NBR*/
    'NLBR',     /*NLBR*/
    //End of mnemonics for N
    /* O */
    'OUT',      /*OUT*/
    'OR',       /*OR*/
    'ORI',      /*ORI*/
    //End of mnemonics for O
    /* P */
    'PLO',      /*PLO*/
    'PHI',      /*PHI*/
    //End of mnemonics for P
    /* R */
    'RSXD',     /*RSXD*/
    'RNX',      /*RNX*/
    'RLDI',     /*RLDI*/
    'RLXA',     /*RLXA*/
    'RET',      /*RET*/
    'REQ',      /*REQ*/
    'RSHL',     /*RSHL*/
    'RSHR',     /*RSHR*/
    //End of mnemonics for R
    /* S */
    'SKP',      /*SKP*/
    'STR',      /*STR*/
    'STPC',     /*STPC*/
    'SPM2',     /*SPM2*/
    'SCM2',     /*SCM2*/
    'SPM1',     /*SPM1*/
    'SCM1',     /*SCM1*/
    'STM',      /*STM*/
    'SCAL',     /*SCAL*/
    'SRET',     /*SRET*/
    'STXD',     /*STXD*/
    'SDB',      /*SDB*/
    'SHRC',     /*SHRC*/
    'SMB',      /*SMB*/
    'SAV',      /*SAV*/
    'SEQ',      /*SEQ*/
    'SDBI',     /*SDBI*/
    'SHLC',     /*SHLC*/
    'SMBI',     /*SMBI*/
    'SEP',      /*SEP*/
    'SEX',      /*SEX*/
    'SD',       /*SD*/
    'SHR',      /*SHR*/
    'SM',       /*SM*/
    'SDI',      /*SDI*/
    'SHR',      /*SHL*/
    'SMI',      /*SMI*/
    //End of mnemonics for S
    /* X */
    'XIE',      /*XIE*/
    'XID',      /*XID*/
    'XOR',      /*XOR*/
    'XRI',      /*XRI*/
    //End of mnemonics for X
  } ;
  
static const unsigned char opcode[][4] = {
  // A
    /*and*/   { 1, ASIS, 0xf2},
    /*adc*/   { 1, ASIS, 0x74},
    /*adci*/  { 2, ASIS, 0x7c, IMM8},
    /*adi*/   { 2, ASIS, 0xfc, IMM8},
    /*ani*/   { 2, ASIS, 0xfa, IMM8},
    /*add*/   { 1, ASIS, 0xf4},
  // B
    /*br*/    { 2, ASIS, 0x30, ADDR8},
    /*bq*/    { 2, ASIS, 0x31, ADDR8},
    /*bz*/    { 2, ASIS, 0x32, ADDR8},
    /*bdf*/   { 2, ASIS, 0x33, ADDR8},
    /*bge*/   { 2, ASIS, 0x33, ADDR8},
    /*bpz*/   { 2, ASIS, 0x33, ADDR8},
    /*b1*/    { 2, ASIS, 0x34, ADDR8},
    /*b2*/    { 2, ASIS, 0x35, ADDR8},
    /*b3*/    { 2, ASIS, 0x36, ADDR8},
    /*b4*/    { 2, ASIS, 0x37, ADDR8},
    /*bnq*/   { 2, ASIS, 0x39, ADDR8},
    /*bnz*/   { 2, ASIS, 0x3a, ADDR8},
    /*bnf*/   { 2, ASIS, 0x3b, ADDR8},
    /*bm*/    { 2, ASIS, 0x3b, ADDR8},
    /*bl*/    { 2, ASIS, 0x3b, ADDR8},
    /*bn1*/   { 2, ASIS, 0x3c, ADDR8},
    /*bn2*/   { 2, ASIS, 0x3d, ADDR8},
    /*bn3*/   { 2, ASIS, 0x3e, ADDR8},
    /*bn4*/   { 2, ASIS, 0x3f, ADDR8},
    /*bci*/   { 3, EXTEND | ASIS, 0x3e, ADDR8},
    /*bxi*/   { 3, EXTEND | ASIS, 0x3f, ADDR8},
  // C
    /*cie*/   { 2, EXTEND | ASIS, 0x0c},
    /*cid*/   { 2, EXTEND | ASIS, 0x0d},
  // D
    /*dec*/   { 1, ADD_N, 0x20},
    /*dtc*/   { 2, EXTEND | ASIS, 0x01},
    /*dbnz*/  { 4, EXTEND | ADD_N, 0x20, ADDR16},
    /*dadc*/  { 2, EXTEND | ASIS, 0x74},
    /*dsav*/  { 2, EXTEND | ASIS, 0x76},
    /*dsmb*/  { 2, EXTEND | ASIS, 0x77},
    /*daci*/  { 3, EXTEND | ASIS, 0x7c, IMM8},
    /*dsbi*/  { 3, EXTEND | ASIS, 0x7f, IMM8},
    /*dadd*/  { 2, EXTEND | ASIS, 0xf4},
    /*dsm*/   { 2, EXTEND | ASIS, 0xf7},
    /*dadi*/  { 3, EXTEND | ASIS, 0xfc, IMM8},
    /*dsmi*/  { 3, EXTEND | ASIS, 0xff, IMM8},
    /*dis*/   { 1, ASIS, 0x71},
  // E
    /*eta*/   { 2, EXTEND | ASIS, 0x09},
  // G
    /*gec*/   { 2, EXTEND | ASIS, 0x08},
    /*glo*/   { 1, ADD_N, 0x80},
    /*ghi*/   { 1, ADD_N, 0x90},
  // I
    /*inp*/   { 1, IN_OUT, 0x68},
    /*idl*/   { 1, ASIS, 0x00},
    /*inc*/   { 1, ADD_N, 0x10},
    /*irx*/   { 1, ASIS, 0x60},
  // L
    /*ldn*/   { 1, ADD_N, 0x00},
    /*lda*/   { 1, ADD_N, 0x40},
    /*ldc*/   { 2, EXTEND | ASIS, 0x06},
    /*ldxa*/  { 1, ASIS, 0x72},
    /*lbr*/   { 3, ASIS, 0xc0, ADDR16},
    /*lbq*/   { 3, ASIS, 0xc1, ADDR16},
    /*lbz*/   { 3, ASIS, 0xc2, ADDR16},
    /*lbdf*/  { 3, ASIS, 0xc3, ADDR16},
    /*lsnq*/  { 3, ASIS, 0xc5, ADDR16},
    /*lsnz*/  { 1, ASIS, 0xc6},
    /*lsnf*/  { 1, ASIS, 0xc7},
    /*lskp*/  { 1, ASIS, 0xc8},
    /*lbnq*/  { 3, ASIS, 0xc9, ADDR16},
    /*lbnz*/  { 3, ASIS, 0xca, ADDR16},
    /*lbnf*/  { 3, ASIS, 0xcb, ADDR16},
    /*lsie*/  { 1, ASIS, 0xcc},
    /*lsq*/   { 1, ASIS, 0xcd},
    /*lsz*/   { 1, ASIS, 0xce},
    /*lsdf*/  { 1, ASIS, 0xcf},
    /*ldx*/   { 1, ASIS, 0xf0},
    /*ldi*/   { 2, ASIS, 0xf8, IMM8},
  // M
    /*mark*/  { 1, ASIS, 0x79},
  // N
    /*nop*/   { 1, ASIS, 0xc4},
    /*nbr*/   { 1, ASIS, 0x38},
    /*nlbr*/  { 1, ASIS, 0xc8},
  // O
    /*out*/   { 1, IN_OUT, 0x60},
    /*or*/    { 1, ASIS, 0xf1},
    /*ori*/   { 2, ASIS, 0xf9, IMM8},
  // P
    /*plo*/   { 1, ADD_N, 0xa0},
    /*phi*/   { 1, ADD_N, 0xb0},
  // R
    /*rsxd*/  { 2, EXTEND | ADD_N, 0xa0},
    /*rnx*/   { 2, EXTEND | ADD_N, 0xb0},
    /*rldi*/  { 4, EXTEND | ADD_N, 0xc0, IMM16},
    /*rlxa*/  { 2, EXTEND | ADD_N, 0x60},
    /*ret*/   { 1, ASIS, 0x70},
    /*req*/   { 1, ASIS, 0x7a},
    /*rshl*/  { 1, ASIS, 0x7e},
    /*rshr*/  { 1, ASIS, 0x76},
  // S
    /*skp*/   { 1, ASIS, 0x38},
    /*str*/   { 1, ADD_N, 0x50},
    /*stpc*/  { 2, EXTEND | ASIS, 0x00},
    /*spm2*/  { 2, EXTEND | ASIS, 0x02},
    /*scm2*/  { 2, EXTEND | ASIS, 0x03},
    /*spm1*/  { 2, EXTEND | ASIS, 0x04},
    /*scm1*/  { 2, EXTEND | ASIS, 0x05},
    /*stm*/   { 2, EXTEND | ASIS, 0x07},
    /*scal*/  { 2, EXTEND | ADD_N, 0x80}, 
    /*sret*/  { 2, EXTEND | ADD_N, 0x90}, 
    /*stxd*/  { 1, ASIS, 0x73},
    /*sdb*/   { 1, ASIS, 0x75},
    /*shrc*/  { 1, ASIS, 0x76},
    /*smb*/   { 1, ASIS, 0x77},
    /*sav*/   { 1, ASIS, 0x78},
    /*seq*/   { 1, ASIS, 0x7b},
    /*sdbi*/  { 2, ASIS, 0x7d, IMM8},
    /*shlc*/  { 1, ASIS, 0x7e},
    /*smbi*/  { 1, ASIS, 0x7f},
    /*sep*/   { 1, ADD_N, 0xd0},
    /*sex*/   { 1, ADD_N, 0xe0},
    /*sd*/    { 1, ASIS, 0xf5},
    /*shr*/   { 1, ASIS, 0xf6},
    /*sm*/    { 1, ASIS, 0xf7},
    /*sdi*/   { 2, ASIS, 0xfd, IMM8},
    /*shl*/   { 1, ASIS, 0xfe},
    /*smi*/   { 2, ASIS, 0xff, IMM8},
  // X
    /*xie*/   { 2, EXTEND | ASIS, 0x0a},
    /*xid*/   { 2, EXTEND | ASIS, 0x0b},
    /*xor*/   { 1, ASIS, 0xf3},
    /*xri*/   { 2, ASIS, 0xfb, IMM8},
  };
  
 static const char AZ_Index[26][2] = {
      {A_START,A_COUNT},
      {B_START,B_COUNT},
      {C_START,C_COUNT},
      {D_START,D_COUNT},
      {E_START,E_COUNT},
      {F_START,F_COUNT},  
      {G_START,G_COUNT},
      {H_START,H_COUNT},
      {I_START,I_COUNT},
      {J_START,J_COUNT},  
      {K_START,K_COUNT},  
      {L_START,L_COUNT},
      {M_START,M_COUNT},
      {N_START,N_COUNT},
      {O_START,O_COUNT},
      {P_START,P_COUNT},
      {Q_START,Q_COUNT},
      {R_START,R_COUNT},
      {S_START,S_COUNT},
      {T_START,T_COUNT},  
      {U_START,U_COUNT},
      {V_START,V_COUNT},  
      {W_START,W_COUNT},  
      {X_START,X_COUNT},
      {Y_START,Y_COUNT},
      {Z_START,Z_COUNT},
  };
  
  unsigned char     firstLetter, len, comparesLeft, x;
  
  uint32_t currentItem, ourMnemonic = 0;
  
  len = cntAlpha(parseHere);

  if(len > MNEM_MAX_LEN || len < MNEM_MIN_LEN) {
    puts("type 2 to 4 characters.");
    return(NULL);
  }
  firstLetter = (*parseHere | 0x20) - 'a';  //first letter, interpret as [0 - 25].
  
  /* Load the mnemonic right-justified byte-wise into a 32-bit register */
  for (;len;len--) {
    ourMnemonic = (ourMnemonic << 8) | *parseHere++;
  }
  ourMnemonic &= TOUPPER_MASK;
  
  x = AZ_Index[firstLetter][0]; //get offset to mnemonics that start with matching letter
  comparesLeft = AZ_Index[firstLetter][1];
  if(x != NOTHING) {    
    
    const uint32_t * list = &knownMnemonics[x]; //go to start of matching letter
    
    while(comparesLeft) {
      currentItem = *list++;
      if(currentItem == ourMnemonic) {
        parsePtr = parseHere;
        return(&opcode[--list - knownMnemonics][0]);
      }
      comparesLeft--;
    }
  }
  puts("instruction doesn't exist");
  return(NULL);
}

