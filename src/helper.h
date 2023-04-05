#ifndef HELPER_H 
#define HELPER_H
#include "AST.h"

// Header file for all helper functions
char * getPrimaryType(char * phrase);
int isAlpha(char * phrase);
int isInt(char * phrase);
int isFloat(char * phrase);
int isVar(char * phrase);
int isNumeric(char * var);
char * escapeCharType(char c);
char * getWATType(char * phrase);
char * getCompareWATType(char * symbol, char * WATType, char * logicType);
int getMoveAmount(char * phrase);
char * convertToASCII(char * phrase);

#endif