//Symbol table header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"

// Error log file
extern FILE* errorFile;
extern int lines;

// Header file for all semantic check functions
// This is a simplistic implementation, will be expanded on later

// Create a struct with a list of all items that variables within a scope will contain after variable declaration
struct Entry
{
	int itemID;
	char itemName[50];  //the name of the identifier
	char itemKind[15];  //is it a function or a variable?
	char itemType[8];  // Is it int, char, etc.?
	int arrayLength;
	char scope[50];     // global, or the name of the function
	int stackPointer;
	int blockNumber;
	char used;
	struct Entry *paramlist;
};


// Single scope and scope index declaration
extern struct Entry symTabItems[100];
extern int symTabIndex;

// Symbol table functions for adding items and accessing specific data types
void symTabAccess(void);

void addItem(char itemName[50], char itemKind[8], char itemType[8], int arrayLength, char scope[50], int stackPointer, int blockNumber);

void showSymTable();

void addFunction(char *type, char *id, struct AST* paramlist, char scopeStack[50][50], int stackPointer, int blockNumber);

void addLogic(char * itemName, char * itemKind, char * scopeStack, int stackPointer, int blockNumber);

int found(char itemName[50], char scopeStack[50][50], int stackPointer);

void updateItemArrayLength(char itemName[50], char scopeStack[50][50], int stackPointer, int newLen);

struct Entry* getItem(char itemName[50], char scopeStack[50][50], int stackPointer);

int getItemID(char itemName[50], char scopeStack[50][50], int stackPointer);

char* getItemKind(char itemName[50], char scopeStack[50][50], int stackPointer);

char* getItemType(char itemName[50], char scopeStack[50][50], int stackPointer);

int getArrayLength(char itemName[50], char scopeStack[50][50], int stackPointer);

char* getItemScope(char itemName[50], char scopeStack[50][50], int stackPointer);

int getItemStackPointer(char itemName[50], char scopeStack[50][50], int stackPointer);

int getItemBlockNumber(char itemName[50], char scopeStack[50][50], int stackPointer);

int compareTypes(char item1[50], char item2[50], char scope[50]);

int countParams(int itemID);

int getNumFuncParams(char funcName[50]);

char * getParamType(int itemID, int numParams, int searchIndex);

char * getFuncParamItemType(char funcName[50], int numParams, int index);