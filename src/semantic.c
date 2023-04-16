#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h> 
#include "semantic.h"
#include "helper.c"
#include "symbolTable.h"
#include "AST.h"

// References symbol table's function to find the time type for a scope
char* getItemType(char itemName[50], char scopeStack[50][50], int stackPointer);
int found(char itemName[50], char scopeStack[50][50], int stackPointer);

// NEW SEMANTIC CHANGE
// Checks to see if a variable has been declared globally first
void CheckGlobal(char* variableName, char* currentScope) {
    int nonGlobal = strcmp(currentScope, "global");
    if (nonGlobal != 0 && found(variableName, "global", 0) == 1) {
        fprintf(errorFile, "Semantic Error, line %d: Variable %s has already been declared globally.\n", lines, variableName);
        exit(1);
    }
}

// Find an item's primary type using getItemType
// Returns the actual primary token
char* CheckPrimaryType(char * variableName, char scopeStack[50][50], int stackPointer){
    // By default, return primary types
    if (strncmp(variableName, "int", 3) == 0 || strncmp(variableName, "string", 6) == 0 || strncmp(variableName, "float", 5) == 0) {
        return variableName;
    }

    if (strncmp(variableName, "inarray_", 8) == 0) {
        // Find the first occurrence of '_' in str
        // Return everything after that point
        char *substr;
        substr = strchr(variableName, '_');
        return substr + 1;
    }

    fflush(stdout);

    // If this item is not in the symbol table, return a Semantic Error. Else, return the primary type
    return getItemType(variableName, scopeStack, stackPointer);
}

void checkID(char* identifier, char scopeStack[50][50], int stackPointer) {
    if (found(identifier, scopeStack, stackPointer) == 0) {
        fprintf(errorFile, "Semantic Error, line %d: Variable %s does not exist.\n", lines, identifier);
        exit(1);
    }
}

void CheckComparisonType(struct AST * leftExprTreeNode, struct AST * rightExprTreeNode, char scopeStack[50][50], int stackPointer) {
    char* leftType;
    char* rightType;
    if(found(leftExprTreeNode->nodeType, scopeStack, stackPointer)) {
        leftType = getItemType(leftExprTreeNode->nodeType, scopeStack, stackPointer);
    } else {
        leftType = leftExprTreeNode->nodeType;
    }

    if(found(rightExprTreeNode->nodeType, scopeStack, stackPointer)) {
        rightType = getItemType(rightExprTreeNode->nodeType, scopeStack, stackPointer);
    } else {
        rightType = rightExprTreeNode->nodeType;
    }

    if(strcmp(leftType, rightType) != 0) {
        fprintf(errorFile, "Semantic Error, line %d: the type of leftExprTreeNode: %s does not match the type of the rightExprTreeNode: %s. \n", lines, leftExprTreeNode->nodeType, rightExprTreeNode->nodeType);
        exit(1);
    }
}

// Checks to see if the LHS matches the RHS for a given assignment expression
void CheckAssignmentType(char * identifier, char * exprType, char scopeStack[50][50], int stackPointer) {
    char * idType = getItemType(identifier, scopeStack, stackPointer);
    
    if (strncmp(idType, exprType, strlen(exprType)) != 0) {
        fprintf(errorFile, "Semantic Error, line %d: The type of \"%s\" does not match the type of \"%s\".\n", lines, identifier, exprType);
		exit(1);
    }
}

// Checks to see if the LHS matches the RHS for a given operation
void CheckOperationType(char * leftExpr, char * rightExpr) {
    if (strncmp(leftExpr, rightExpr, strlen(rightExpr)) != 0) {
        fprintf(errorFile, "Semantic Error, line %d: The type of \"%s\" does not match the type of \"%s\".\n", lines, leftExpr, rightExpr);
		exit(1);
    }
}

void CheckParamLength(char funcName[50], struct AST * funcCallParamList) {
    int funcParams = getNumFuncParams(funcName);
    int callParams = getNumExprs(funcCallParamList);

    if (funcParams != callParams) {
        fprintf(errorFile, "Semantic Error, line %d: The total number of call parameters for \"%s\" (%d) does not match function declaration (%d).", lines, funcName, callParams, funcParams);
        exit(1);
    }
}

void CheckIndexOutOfBound(char * identifier, char * integer, char scopeStack[50][50], int stackPointer) {
    struct Entry * itemObj = getItem(identifier, scopeStack, stackPointer);
    if (itemObj == NULL) {
        fprintf(errorFile, "Semantic Error, line %d: The total number of call parameters for \"%s\" (%s) does not match function declaration (%d).", lines, identifier, integer, scopeStack[stackPointer]);
        exit(1);
    }
    if (itemObj->arrayLength < atoi(integer) || atoi(integer) < 0) {
        fprintf(errorFile, "Semantic Error, line %d: Index out of bound for array %s with size %d\n", lines, identifier, itemObj->arrayLength);
        exit(1);
    }
}

void checkIntDivisionError(int numerator, int denominator) {
    if (denominator == 0) {
        fprintf(errorFile, "Division Error, line %d: Cannot divide %d by 0.\n", lines, numerator);
        exit(1);
    }
}

void checkFloatDivisionError(float numerator, float denominator) {
    if (denominator == 0.0) {
        fprintf(errorFile, "Division Error, line %d: Cannot divide %f by 0.0.\n", lines, numerator);
        exit(1);
    }
}

void checkEscapeChars(char * phrase) {
    int len = strlen(phrase);

    // Detect escape characters by looping through each char
    for (int i = 0; i < len; i++) {
        // Detect if the escape character is referenced
        if (phrase[i] == '\\') {
            // If the next character is the null character, throw a Semantic Error
            // Incomplete escape character
            if (phrase[i+1] == '\0') {
                fprintf(errorFile, "Semantic Error, line %d: Incomplete escape character reference.\n", lines);
                exit(1);
            }

            // Throw a Semantic Error if the next char does not make this a valid escape char combination
            if (phrase[i+1] != '\"' && phrase[i+1] != '\'' && phrase[i+1] != '\\' && phrase[i+1] != 'n' && phrase[i+1] != 't') {
                fprintf(errorFile, "Semantic Error, line %d: Invalid escape character combination (\\%c).\n", lines, phrase[i+1]);
                exit(1);
            }

            // Skip over index for loop
            i++;
        }
    }
}

int countEscapeChars(char * phrase) {
    int len = strlen(phrase);
    int numEscapeChars = 0;

    // Count the escape characters by looping through each char
    for (int i = 0; i < len; i++) {
        // Detect if the escape character is referenced
        if (phrase[i] == '\\') {
            // If the next character is the null character, throw a Semantic Error
            // Incomplete escape character
            if (phrase[i+1] == '\0') {
                fprintf(errorFile, "Semantic Error, line %d: Incomplete escape character reference.\n", lines);
                exit(1);
            }

            // Throw a Semantic Error if the next char does not make this a valid escape char combination
            if (phrase[i+1] != '\"' && phrase[i+1] != '\'' && phrase[i+1] != '\\' && phrase[i+1] != 'n' && phrase[i+1] != 't') {
                fprintf(errorFile, "Semantic Error, line %d: Invalid escape character combination (\\%c).\n", lines, phrase[i+1]);
                exit(1);
            }

            // If no Semantic Error occurs, add to the count
            numEscapeChars++;

            // Skip over index for loop
            i++;
        }
    }

    // Return final total
    return numEscapeChars;
}

// Helper function to find which scope a variable is part of, if nested within multiple scopes
char * findVarScope(char * varName, char ** scopeList, int scopeListLength) {
    for (int i = 0; i < scopeListLength; i++) {
        if (found(varName, scopeList[i], 1)) {
            return getItemScope(varName, scopeList[i], 1);
        }
    }
    // Else, it's not in this list; throw a Semantic Error
    fprintf(errorFile, "Semantic Error, line %d: Variable %s is not nested in any scope.\n", lines, varName);
    exit(1);
}

