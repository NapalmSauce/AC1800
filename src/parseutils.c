
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>


/*
* Returns pointer to next address
* that contains a char other than tabs
* or spaces starting from param 'where'
* return null if encounters \0
*/
char * space_StepOver(char * where) {
	char x;
	for(;;) {
		x = *where++;
		if(isspace(x) == 0 || x == '\n')
		return(--where);
	} 
}

/*
* Returns the number of successive
* ascii characters from the alphabet
* starting from arg where.
*/
int cntAlpha(char * where) {
	int y = 0;
	
	while(isalpha(*where++)) {
		y++;
	}
	return(y);
}

/*
* Returns the number of successive
* ascii characters from a-z,
* digits, and/or underscores (_)
* starting from arg where.
*/
int cntVoc(char * where) {
	int y = 0;
	
	while(isalnum(*where) || *where == '_') {
		y++;
		*where++;
	}
	return(y);
}

int cntGraf(char * where) {
	int y = 0;

	while(isgraph(*where++)) {
		y++;
	}
	//printf("%.1d graf",y);
	return(y);
}

int cntHex(char * where) {
	int y = 0;
	
	while(isxdigit(*where++)) {
		y++;
	}
	return y;
}

int cntDec(char * where) {
	int y = 0;
	
	while(isdigit(*where++)) {
		y++;
	}
	return y;

}

/*  Main routine to compute constant expressions. Right now it does
*   not support anything else than simple expressions in base 10 or 16.
*/
int express(char * parseHere, unsigned long *value) {

	char	lead;
	unsigned int	len;
	unsigned long	val = 0;
	
	parseHere = space_StepOver(parseHere);
	lead = *parseHere;
	
	if(lead == 0 || lead == '\n') {
		puts("missing constant parameter");	
	}
	if((lead == '0') || (lead == '$')) {
	
		parseHere++;
		if(tolower(*parseHere) == 'x') {		//both 0x and $ mean hexaecimal
			lead = '$';
			parseHere++;
		}
		for(;*parseHere == '0';parseHere++);	//skip any leading 0's
	}
	if(isdigit(lead)) {
			
		len = cntDec(parseHere);
		
		if(len > 10 || (len == 10 && strncmp(parseHere,"4294967295",10) > 0)) {

      puts("expression out of range");
			return(1);
		}
		
		
		for( ; len ; len--) {
			
			val = (val * 10) + (*parseHere++ - '0');
			
		}
		
	}
	
	else if(lead == '$') {
	
		len = cntHex(parseHere);
		if(len > 8) {
	
      puts("expression out of range");	
			return(1);
		}
		
		for( ; len; len--) {
			val = (val << 4) + ((*parseHere | 0x20) - '0');
			if(*parseHere++ > '9')
				val -= ('a'-'0' - 0xa);
		}
	}
	
/*	else if(isalpha(lead) != 0) {
	
    TO BE IMPLEMENTED: ADDRESS SYMBOLS

	} */
	
	else {
    puts("error in constant parameter syntax");
    return(1);
  }
  *value = val;
	return(0);
}

/* Wrapper function call to express() used for branch instructions.*/

int getAddr(char * parseHere, unsigned long *value) {
  parseHere = space_StepOver(parseHere);
  if(*parseHere == '@') {
    char x;
    x = toupper(*(parseHere+1));
    if( x == 'H' || x == 'L') {
      puts("info: [@H/@L] operator is ignored by branch instructions");
      parseHere += 2;
    }
  }
  if(express(parseHere, value)) return(1);
  if(*value > 0xffff) {
    puts("expression out of range");
    return(1);
  }
  return(0); 
}

/* Wrapper function call to express() used for immediate instructions.*/

int getImm(char * parseHere, unsigned long *value) {

  char x;
  char Half = 0;
  parseHere = space_StepOver(parseHere);
  if(*parseHere == '@') {
    x = toupper(*(parseHere+1));
    if( x == 'H' || x == 'L') {
      Half = x;
      parseHere += 2;
    }
  }
  if(express(parseHere, value)) return(1);
  if(*value > 0xffff) {
    puts("expression out of range");
    return(1);
  }
  if(Half == 'H')
  *value >>= 8;
  else if(Half)
  *value &= 0xff; 
  return(0);
}
