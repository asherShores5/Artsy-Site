#ifndef SEMANTIC_H 
#define SEMANTIC_H
#include "AST.h"

// Error log file
extern FILE* errorFile;
extern int lines;

// Header file for all semantic check functions

void CheckGlobal(char* variableName, char* currentScope);
void checkIntLength(char * varName);
void checkFloatLength(char * varName);
void checkStringLength(char * varName);
void checkVariableLength(char * varName);
char* CheckPrimaryType(char*arg1, char scopeStack[50][50], int stackPointer);
void checkID(char * identifier, char scopeStack[50][50], int stackPointer);
void CheckComparisonType(struct AST * leftExprTreeNode, struct AST * rightExprTreeNode, char scopeStack[50][50], int stackPointer);
void CheckAssignmentType(char * identifier, char * exprType, char scopeStack[50][50], int stackPointer);
void CheckOperationType(char * leftExpr, char * rightExpr);
void CheckIndexOutOfBound(char * identifier, char * integer, char scopeStack[50][50], int stackPointer);
void checkIntDivisionError(int numerator, int denominator);
void checkFloatDivisionError(float numerator, float denominator);
void checkEscapeChars(char * phrase);
int countEscapeChars(char * phrase);
char * findVarScope(char * varName, char ** scopeList, int scopeListLength);

#endif


