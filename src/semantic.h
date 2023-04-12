#ifndef SEMANTIC_H 
#define SEMANTIC_H
#include "AST.h"

// Error logging
extern int lines;

// Header file for all semantic check functions

void CheckGlobal(char* variableName, char* currentScope);
char* CheckPrimaryType(char*arg1, char scopeStack[50][50], int stackPointer);
void CheckID(char * identifier, char scopeStack[50][50], int stackPointer);
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


