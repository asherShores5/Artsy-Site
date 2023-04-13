
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ircode.h"
#include "helper.h"
#include "semantic.h"
#include "symbolTable.h"
#include "AST.h"

// Declare a string variable for returning optimized IRcode
char * IRcodeOptimized;
char * copyVar;

// Standard program variables
#define MAX_LINE_LENGTH 10000
#define MAX_ARRAY_LENGTH 5000

// Constant declarations for generating variables and statements in the IRcode
int lastIndex = 0;
int lastArrayTempIndex = 0;
char outputId[50];
char buffer[50];
char * arrElVar;
char params[50][50];
int paramIndex = 0;

// Bool for checking if a comparison is within a logical statement
int isLogical = 0;
int currLogicalStatements = 0;
int totalLogicalStatements = 0;
char ** logicalTypes;

// Bool for checking if a call is within a while loop (optimized code only)
int inWhileLoop = 0;

// Bool for checking if type is a parameter
int isParam = 0;

// Variable for current scope and previous scope
char * currScope;
char ** prevScopes;
int totalIRScopes = 1;

// Variables to detect suboperation starts
int startSubOp = 0;
int startSubOpOptimized = 0;

// Struct to handle temporary variable construction
struct ConstantVar {
    char var[50];
    char scope[50];
    int val;
};

// Generate symbol table of temporary variables
struct ConstantVar cvTable[100];

// Indexes to traverse these tables
int cvIndex = 0;
int uvIndex = 0;

// Temporary variable struct to determine if a variable has been used in optimization
struct Var {
    char var[50];
    int boolVal;
};

// Generate symbol table for variable usage
struct Var uvTable[100];

char * updateArrayId(char id[50]) {
    // Split apart the phrase into a set of tokens by the "[" keyword
    char delimiter[] = "[], ";
    return strtok(id, delimiter);
}

// Use the cvTable to find the temporary variable for a given variable declaration
char* getVarConstant(char var[50]){
	for(int i=0; i<cvIndex; i++){
        if (strcmp(cvTable[i].var, var) == 0){
            memset(buffer, 0, 50);
			sprintf(buffer, "%d", cvTable[i].val);
            return buffer;
		}
	}
	return var;
}

// Function to add a variable to the unused variable table
void addUnusedVar(char var[50]) {
    strncpy(uvTable[uvIndex].var, var, 50);
    uvTable[uvIndex].boolVal = 0;
    uvIndex++;
}

// Function to update variable declarations if the variable is unused - part of the optimization process
void updateUnusedVar(char var[50]) {
	for(int i=0; i<uvIndex; i++){
        if (strcmp(uvTable[i].var, var) == 0) {
            uvTable[i].boolVal = 1;
            return;
		}
	}
}

// Checks the symbol table
// Returns true or false depending if the variable is used or not (for code optimization)
int isUsedVar(char var[50]) {
    for (int i = 0; i < uvIndex; i++){
        if (strcmp(uvTable[i].var, var) == 0) {
            return uvTable[i].boolVal;
		}
	}
    return 0;
}

// For unoptimized IRcode
// Generates the IRcode for assignments
char* emitBinaryOperation(char op[2], const char* id1, const char* id2){
    // Assign temporary variables for tracking base array variables
    char * token1 = malloc(strlen(id1)*sizeof(char));
    char * token2 = malloc(strlen(id2)*sizeof(char));
    strcpy(token1, id1);
    strcpy(token2, id2);

    // Create a temporary variable assignment using "T"
    memset(outputId, 0, 50);
    char currLabelIndexBuffer[50];
    sprintf(currLabelIndexBuffer, "%d", lastIndex);
    strcat(outputId, "T");
    strcat(outputId, currLabelIndexBuffer);

    // Updates the declaration if the variable is unused
    if (strncmp(getPrimaryType(token1), "var", 3) == 0) {
        updateUnusedVar(updateArrayId(token1));
    }
    if (strncmp(getPrimaryType(token2), "var", 3) == 0) {
        updateUnusedVar(updateArrayId(token2));
    }

    // Output suboperation type (for webgen parsing)
    if (!startSubOp) {
        char * opType = calloc(100, sizeof(char));
        
        if (strncmp(getPrimaryType(token1), "var", 3) == 0) {
            char * tempScope = findVarScope(token1, prevScopes, totalIRScopes);
            opType = getItemType(token1, tempScope, 1);
        } else {
            opType = getPrimaryType(token1);
        }

        startSubOp = 1;
    }

    // Print the unoptimized iRcode using outputIDs, the operator, and the other ids
    lastIndex += 1;

    // Returns output
    return outputId;
}

// For optimized IRcode but unoptimized generation for while loops
char* emitBinaryOperationUnoptimized(char op[2], const char* id1, const char* id2){
    // Assign temporary variables for tracking base array variables
    char * token1 = malloc(strlen(id1)*sizeof(char));
    strcpy(token1, id1);

    // Create a temporary variable assignment using "T"
    memset(outputId, 0, 50);
    char currLabelIndexBuffer[50];
    sprintf(currLabelIndexBuffer, "%d", lastIndex);
    strcat(outputId, "T");
    strcat(outputId, currLabelIndexBuffer);

    // Output suboperation type (for webgen parsing)
    if (!startSubOpOptimized) {
        char * opType = calloc(100, sizeof(char));

        if (strncmp(getPrimaryType(token1), "var", 3) == 0) {
            char * tempScope = findVarScope(token1, prevScopes, totalIRScopes);
            opType = getItemType(token1, tempScope, 1);
        } else {
            opType = getPrimaryType(token1);
        }

        sprintf(copyVar, "subop %s\n", opType);
        strcat(IRcodeOptimized, copyVar);
        startSubOpOptimized = 1;
    }

    // Print the unoptimized iRcode using outputIDs, the operator, and the other ids
    lastIndex += 1;

    // Returns output
    return outputId;
}

// Optimized version of binary operations for IRcodeOptimized.ir
char* emitBinaryOperationOptimized(char op[1], const char* id1, const char* id2){
    memset(outputId, 0, 50);

    // Assign temporary variables for tracking base array variables
    char * token1 = malloc(strlen(id1)*sizeof(char));
    strcpy(token1, id1);

    // If it's an array, get the optype based on the token
    char * opType = getPrimaryType(updateArrayId(token1));

    if (strncmp(opType, "int", 3) == 0) {
        int num1 = (int) strtol(id1, (char **)NULL, 10);
        int num2 = (int) strtol(id2, (char **)NULL, 10);

        // Get optimized result
        int result;
        if (strncmp(op, "+", 1) == 0) {
            result = num1 + num2;
        } else if (strncmp(op, "-", 1) == 0) {
            result = num1 - num2;
        } else if (strncmp(op, "*", 1) == 0) {
            result = num1 * num2;
        } else if (strncmp(op, "/", 1) == 0) {
            result = num1 / num2;
        }

        sprintf(outputId, "%d", result);
        return outputId;
    } else if (strncmp(opType, "float", 5) == 0) {
        float num1 = atof(id1);
        float num2 = atof(id2);

        // Get optimized result
        float result;
        if (strncmp(op, "+", 1) == 0) {
            result = num1 + num2;
        } else if (strncmp(op, "-", 1) == 0) {
            result = num1 - num2;
        } else if (strncmp(op, "*", 1) == 0) {
            result = num1 * num2;
        } else if (strncmp(op, "/", 1) == 0) {
            result = num1 / num2;
        }

        sprintf(outputId, "%f", result);
        return outputId;
    }

    char currLabelIndexBuffer[50];
    sprintf(currLabelIndexBuffer, "%d", lastIndex);
    strcat(outputId, "T");
    strcat(outputId, currLabelIndexBuffer);
    
    // Output suboperation type (for webgen parsing)
    if (!startSubOpOptimized) {
        char * opType = calloc(100, sizeof(char));

        if (strncmp(getPrimaryType(token1), "var", 3) == 0) {
            char * tempScope = findVarScope(token1, prevScopes, totalIRScopes);
            opType = getItemType(token1, tempScope, 1);
        } else {
            opType = getPrimaryType(token1);
        }

        sprintf(copyVar, "subop %s\n", opType);
        strcat(IRcodeOptimized, copyVar);
        startSubOpOptimized = 1;
    }

    // Output optimized suboperation line
    sprintf(copyVar, "%s = %s %s %s\n", outputId, id1, op, id2);
    strcat(IRcodeOptimized, copyVar);
    lastIndex += 1;

    return outputId;
}

// Unoptimized IRcode operation for variable assignment
void emitAssignment(char * id1, char * id2){
    // Print the assignment statement using the two basic IDs
    // If the statement is a combined string, separate it as a set of array index assignments
    if (strncmp(getPrimaryType(id2), "string", 6) == 0) {
        // Get the number of escape characters
        // Used in calculating the new array length
        int numEscapeCharacters = countEscapeChars(id2);

        // If new array length is bigger, update it to that
        char * arrScope = findVarScope(id1, prevScopes, totalIRScopes);
        if (strlen(id2)-2-numEscapeCharacters > getArrayLength(id1, arrScope, 1)) {
            updateItemArrayLength(id1, arrScope, 0, strlen(id2)-2);
        }

        // Print each index assignment statement
        int loopEscapeChars = 0; // Tracks total escape chars encountered
        for (int i = 1; i < strlen(id2)-1-numEscapeCharacters; i++) {
            if (id2[i+loopEscapeChars] == '\\') {
                loopEscapeChars++;
            // If the character is a space
            } else if (id2[i+loopEscapeChars] == ' ') {
            } else {
            }
        }

        // Indicate end of full string with length
    } else {
    }

    // Indicate subop stop
    startSubOp = 0;
}

// Optimized assignment operation function for IRcodeOptimized.ir
void emitAssignmentOptimized(char * id1, char * id2){
    int flag = 1;
    if(isNumeric(id2)) {
        for(int i=0; i<cvIndex; i++){
            if (strcmp(cvTable[i].var, id1) == 0){
                cvTable[i].val = (int) strtol(id2, (char **)NULL, 10);
                flag = 0;
                break;
            }
	    }
        if(flag) {
            strcpy(cvTable[cvIndex].var, id1);
            cvTable[cvIndex].val = (int) strtol(id2, (char **)NULL, 10);
            cvIndex ++;
        }
    }

    // If the statement is a combined string,
    // separate it out into a set of array index assignments
    if (strncmp(getPrimaryType(id2), "string", 6) == 0) {
        // Get the number of escape characters
        // Used in calculating the new array length
        int numEscapeCharacters = countEscapeChars(id2);
        int loopEscapeChars = 0; // Tracks total escape chars encountered

        for (int i = 1; i < strlen(id2)-1-numEscapeCharacters; i++) {
            if (id2[i+loopEscapeChars] == '\\') {
                sprintf(copyVar, "%s[%d] = \"%s\"\n", id1, i-1, escapeCharType(id2[i+1+loopEscapeChars]));
                strcat(IRcodeOptimized, copyVar);
                loopEscapeChars++;
            // If the character is a space
            } else if (id2[i+loopEscapeChars] == ' ') {
                sprintf(copyVar, "%s[%d] = \"%s\"\n", id1, i-1, escapeCharType(id2[i+loopEscapeChars]));
                strcat(IRcodeOptimized, copyVar);
            } else {
                sprintf(copyVar, "%s[%d] = \"%c\"\n", id1, i-1, id2[i+loopEscapeChars]);
                strcat(IRcodeOptimized, copyVar);
            }
        }
        // Indicate end of full string with length
        sprintf(copyVar, "endstring %s %ld\n", id1, strlen(id2)-2-numEscapeCharacters);
        strcat(IRcodeOptimized, copyVar);
    } else {
        sprintf(copyVar, "%s = %s\n", id1, id2);
        strcat(IRcodeOptimized, copyVar);
    }
    
    // Indicate subop stop
    startSubOpOptimized = 0;

}

void emitAssignmentForElementOptimized(char *id1, char * elementNum, char * id2) {
    int flag = 1;

    if(isNumeric(id2)) {
        for(int i=0; i<cvIndex; i++){
            if (strcmp(cvTable[i].var, id1) == 0){
                cvTable[i].val = (int) strtol(id2, (char **)NULL, 10);
                flag = 0;
                break;
            }
	    }
        if(flag) {
            strcpy(cvTable[cvIndex].var, id1);
            cvTable[cvIndex].val = (int) strtol(id2, (char **)NULL, 10);
            cvIndex ++;
        }
    }

    // Check if the item uses an escape character or space, switch to new format
    if (strncmp(getPrimaryType(id2), "string", 6) == 0 && id2[1] == '\\' || id2[1] == ' ') {
        snprintf(id2, 10, "\"%s\"", escapeCharType(id2[1]));
    }

    // Print the assignment statement using the two basic IDs with an element number
    sprintf(copyVar, "%s[%s] = %s\n", id1, elementNum, id2);
    strcat(IRcodeOptimized, copyVar);
}

void emitWritePrimary(char * value) {
    sprintf(copyVar, "output %s\n", value);
    strcat(IRcodeOptimized, copyVar);
}

// Unoptimized IRcode operation for writing standard ids
void emitWriteId(char * id) {
    // Update variable in the unused variable table
    updateUnusedVar(updateArrayId(id));
}

// Optimized IRCode operation for writing standard ids
void emitWriteIdOptimized(char * id){
    // Print output keyword with the associated ID
    // Unused variables are already removed prior to this step, so it is redundant to include it here
    sprintf(copyVar, "output %s\n", id);
    strcat(IRcodeOptimized, copyVar);
}

// Unoptimized IRcode operation for writing array callouts
void emitWriteArrayId(char * id, char * elementNum) {
    // Update variable in the unused variable table
    updateUnusedVar(updateArrayId(id));
}

// Optimized IRcode operation for writing array callouts
void emitWriteArrayIdOptimized(char * id, char * elementNum) {
    // Print output keyword with the associated ID and element num
    // Unused variables are already removed prior to this step, so it is redundant to include it here
    sprintf(copyVar, "output %s[%s]\n", id, elementNum);
    strcat(IRcodeOptimized, copyVar);
}

void emitWriteLn(){
    strcat(IRcodeOptimized, "addline\n");
}

// Outputs the variable and type for variable declaration (unoptimized)
void emitTypeDeclaration(char * type, char * id){
    // Flag variable for unused code optimization testing
    int flag = 1;
    
    // Char variable to determine if the variable is a type or a parameter
    char * varType = "type";
    
    // Check the whole UV table
    // If the type is already in the table, set the flag to 0
    for(int i=0; i<uvIndex; i++){
        if (strcmp(uvTable[i].var, id) == 0){ flag = 0; break; }
    }
    // If the flag is still true, add the new id to the unused variable table
    if (flag) {
        addUnusedVar(id);
    }

    if(isParam) {
        varType = "param";
    }
}

// Emit the type declaration for optimized IRcode file
void emitTypeDeclarationOptimized(char * type, char * id){
    // Char variable to determine if the variable is a type or a parameter
    char * varType = "type";

    if(isParam) {
        varType = "param";
    }

    // Already includes optimizations prior to this step, so doing optimization beforehand is redundant
    // Print variable declaration IRcode to file
    // If the type is a string, print a statement with a default size limit
    if (strncmp(type, "string", 6) == 0) {
        char * size = "100";
        sprintf(copyVar, "%s %s %s array %s\n", varType, type, id, size);
        strcat(IRcodeOptimized, copyVar);
    } else {
        // Else, print a standard type declaration statement
        sprintf(copyVar, "%s %s %s\n", varType, type, id);
        strcat(IRcodeOptimized, copyVar);
    }

}

// Outputs the variable and type for variable declaration (unoptimized) (for array)
void emitTypeArrayDeclaration(char * type, char * id, char * size){
    // Flag variable for unused code optimization testing
    int flag = 1;

    // Char variable to determine if the variable is a type or a parameter
    char * varType = "type";

    if(isParam) {
        varType = "param";
    }

    // Check the whole UV table
    // If the type is already in the table, set the flag to 0
    for(int i=0; i<uvIndex; i++){
        if (strcmp(uvTable[i].var, id) == 0){ flag = 0; break; }
    }

    // If the flag is still true, add the new id to the unused variable table
    if (flag) {
        addUnusedVar(id);
    }

}

// Outputs the variable and type for variable declaration (unoptimized) (for array)
void emitTypeArrayDeclarationOptimized(char * type, char * id, char * size){
    // Flag variable for unused code optimization testing
    int flag = 1;

    // Char variable to determine if the variable is a type or a parameter
    char * varType = "type";

    if(isParam) {
        varType = "param";
    }

    // Already includes optimizations prior to this step, so doing optimization beforehand is redundant
    // Print variable declaration IRcode to file
    sprintf(copyVar, "%s %s %s array %s\n", varType, type, id, size);
    strcat(IRcodeOptimized, copyVar);
}

void emitEntry(char * id) {
    int flag = 1;

    // Check the whole UV table
    // If the type is already in the table, set the flag to 0
    for(int i=0; i<uvIndex; i++){
        if (strcmp(uvTable[i].var, id) == 0){ flag = 0; break; }
    }

    // If the flag is still true, add the new id to the unused variable table
    if (flag) {
        addUnusedVar(id);
    }

    // Get type and update current scope
    char * type = getItemType(id, "global", 1);
    
    // Set new scope and retain history using previous scope
    currScope = malloc(1000 * sizeof(char));
    strcpy(currScope, id);
    strcpy(prevScopes[totalIRScopes], currScope);
    totalIRScopes++;

}

void emitEntryOptimized(char * id) {    
    char * type = getItemType(id, "global", 1);

    // Set new scope and retain history using previous scope
    currScope = malloc(1000 * sizeof(char));
    strcpy(currScope, id);
    strcpy(prevScopes[totalIRScopes], currScope);
    totalIRScopes++;

    sprintf(copyVar, "entry %s %s\n", type, id);
    strcat(IRcodeOptimized, copyVar);
}

void emitReturn(char * id) {
    // Create a temporary variable for array return support
    char * token = calloc(100, sizeof(char));
    strcpy(token, id);

    // Updates the variable if it is unused
    if (strncmp(getPrimaryType(token), "var", 3) == 0) {
        updateUnusedVar(updateArrayId(token));
    }

}

void emitReturnOptimized(char * id) {    
    sprintf(copyVar, "return %s\n", id);
    strcat(IRcodeOptimized, copyVar);
}

void emitVoidReturn() {
    strcat(IRcodeOptimized, "voidreturn\n");
}

void emitFinish() {
    strcat(IRcodeOptimized, "finish\n");
}

void emitExit() {
    // Set current scope as previous scope
    strcpy(currScope, prevScopes[totalIRScopes-2]);
    totalIRScopes--;

    strcat(IRcodeOptimized, "exit\n");
}

void emitLogicalExpression(char * opType) {
    sprintf(copyVar, " %s", opType);
    strcat(IRcodeOptimized, copyVar);
}

void emitComparisonExpression(char * leftExpr, char * compareType, char * rightExpr) {
    sprintf(copyVar, " %s %s %s", leftExpr, compareType, rightExpr);
    strcat(IRcodeOptimized, copyVar);
}

void emitIfConditionStatement() {
    strcat(IRcodeOptimized, "if");
}

void emitIfEndStatement() {
    strcat(IRcodeOptimized, "endif\n");
}

void emitElifConditionStatement() {
    strcat(IRcodeOptimized, "elif");
}

void emitElifEndStatement() {
    strcat(IRcodeOptimized, "endelif\n");
}

void emitElseConditionStatement() {
    strcat(IRcodeOptimized, "else");
}

void emitElseEndStatement() {
    strcat(IRcodeOptimized, "endelse\n");
}

void emitWhileConditionStatement() {
    strcat(IRcodeOptimized, "while");
}

void emitWhileEndStatement() {
    strcat(IRcodeOptimized, "endwhile\n");
}

void emitLogicEndStatement() {
    strcat(IRcodeOptimized, "endlogic\n");
}

char * emitFunctionCall(char *id) {
    memset(outputId, 0, 50);
    char currLabelIndexBuffer[50];
    sprintf(currLabelIndexBuffer, "%d", lastIndex);
    strcat(outputId, "T");
    strcat(outputId, currLabelIndexBuffer);

    // Update the code if it is unused
    if (strncmp(getPrimaryType(id), "var", 3) == 0) {
        updateUnusedVar(id);
    }

    // Output suboperation type (for webgen parsing)
    char * opType = calloc(100, sizeof(char));
    if (!startSubOp) {        
        if (strncmp(getPrimaryType(id), "var", 3) == 0) {
            strcpy(opType, getItemType(id, "global", 1));
        } else {
            strcpy(opType, getPrimaryType(id));
        }

        // Do not generate a subop if its a void function call
        if (strncmp(opType, "void", 4) != 0) {
            startSubOp = 1;
        }
    }
    
    // Print arguments
    for(int i = 0; i < paramIndex; i ++) {
        // Update variable if unused
        if (strncmp(getPrimaryType(params[i]), "var", 3) == 0) {
            updateUnusedVar(params[i]);
        }

    }
    lastIndex += 1;

    return outputId;
}

char * emitFunctionCallOptimized(char *id) {
    memset(outputId, 0, 50);
    char currLabelIndexBuffer[50];
    sprintf(currLabelIndexBuffer, "%d", lastIndex);
    strcat(outputId, "T");
    strcat(outputId, currLabelIndexBuffer);

    // Output suboperation type (for webgen parsing)
    char * opType;
    if (!startSubOpOptimized) {
        if (strncmp(getPrimaryType(id), "var", 3) == 0) {
            opType = getItemType(id, "global", 1);
        } else {
            opType = getPrimaryType(id);
        }

        // Do not generate a subop if its a void function call
        if (strncmp(opType, "void", 4) != 0) {
            sprintf(copyVar, "subop %s\n", opType);
            strcat(IRcodeOptimized, copyVar);
            startSubOpOptimized = 1;
        }
    }

    // Generate function call based on if there is a void keyword
    if (strncmp(opType, "void", 4) != 0) {
        sprintf(copyVar, "%s = call %s args", outputId, id);
        strcat(IRcodeOptimized, copyVar);
    } else {
        sprintf(copyVar, "call %s args", id);
        strcat(IRcodeOptimized, copyVar);
    }

    // Generate arguments
    for(int i = 0; i < paramIndex; i ++) {
        sprintf(copyVar, " %s", params[i]);
        strcat(IRcodeOptimized, copyVar);
    }
    strcat(IRcodeOptimized, "\n");
    lastIndex += 1;

    return outputId;
}

// Function to traverse the AST tree
// This initializes creating all of the IRcode for unoptimized IRcode
char* ASTTraversal(struct AST* root) {
    if(root != NULL) {
        // printf("root->nodeType: %s\n", root->nodeType);
        // if(root->LHS != NULL)
        // printf("root->LHS: %s\n", root->LHS);
        // if(root->RHS != NULL)
        // printf("root->RHS: %s\n", root->RHS);
        fflush(stdout);
        char rightVar[50];
        char leftVar[50];
        if(strcmp(root->nodeType, "int") == 0
            || strcmp(root->nodeType, "float") == 0
            || strcmp(root->nodeType, "string") == 0
            || strcmp(root->nodeType, "flag") == 0) {
                return root->RHS;
        }
        if(strcmp(root->nodeType, "type") == 0) {
            if(root->right != NULL && strcmp(root->right->LHS, "array") == 0) {
                emitTypeArrayDeclaration(root->LHS, root->RHS, root->right->right->RHS);
            } else {
                emitTypeDeclaration(root->LHS, root->RHS);
            }
        }
        if(strcmp(root->nodeType, "variable parm") == 0){
            isParam = 1;
            emitTypeDeclaration(root->LHS, root->RHS);
            isParam = 0;
        }
        if(strcmp(root->nodeType, "array parm") == 0) {
            emitTypeArrayDeclaration(root->LHS, root->RHS, "-1");
        }
        // Handle Logical Statements
        if(strcmp(root->nodeType, "Logical") == 0) {
            // Set logical operation type variables
            if (strcmp(root->RHS, "Logical") == 0) {
                strcpy(logicalTypes[totalLogicalStatements], root->right->RHS);
            } else {
                strcpy(logicalTypes[totalLogicalStatements], root->RHS);
            }
            totalLogicalStatements++;

            // Traverse to both conditionals
            ASTTraversal(root->right->left);
            ASTTraversal(root->right->right);
        }
        if(strcmp(root->nodeType, "Comparison") == 0) {
            if (isLogical == 1 && currLogicalStatements != 0) {
                updateUnusedVar(ASTTraversal(root->right->left));
                updateUnusedVar(ASTTraversal(root->right->right));
                currLogicalStatements++;
            } else if (isLogical == 1) {
                updateUnusedVar(ASTTraversal(root->right->left));
                updateUnusedVar(ASTTraversal(root->right->right));
                currLogicalStatements++;
            }
            else {
                updateUnusedVar(ASTTraversal(root->right->right->left));
                updateUnusedVar(ASTTraversal(root->right->right->right));
            }
        }
        if(strcmp(root->nodeType, "program") == 0 
            || strcmp(root->nodeType, "vardec") == 0
            || strcmp(root->nodeType, "ActionDecl ActionDeclListTail") == 0
            || strcmp(root->nodeType, "ParaDecl comma ParaDeclListTail") == 0
            || strcmp(root->nodeType, "statements") == 0
            || strcmp(root->nodeType, "IfStmtTail continue") == 0
            || strcmp(root->nodeType, "IfStmt") == 0) { 
            ASTTraversal(root->left);
            ASTTraversal(root->right);
        }

        if(strcmp(root->nodeType, "WhileL") == 0) {
            // Change logical statement indicator variable to active
            isLogical = !strcmp(root->LHS, "Logical");

            // Set memory for logical type string array
            logicalTypes = calloc(2500, sizeof(char));
            for (int i = 0; i < 50; i++) { logicalTypes[i] = malloc(5 * sizeof(char));}

            // Traverse to the left to generate logical and conditional statements
            ASTTraversal(root->left);

            // Reset Logical Statement Variables
            isLogical = 0;
            currLogicalStatements = 0;
            totalLogicalStatements = 0;

            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "while %s %d", currScope, getItemBlockNumber("while", findVarScope("while", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;


            // Traverse to the right to generate block code
            ASTTraversal(root->right);

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }

        if(strcmp(root->nodeType, "If") == 0) {
            // Change logical statement indicator variable to active
            isLogical = !strcmp(root->LHS, "Logical");

            // Set memory for logical type string array
            logicalTypes = calloc(2500, sizeof(char));
            for (int i = 0; i < 50; i++) { logicalTypes[i] = malloc(5 * sizeof(char));}

            // Traverse to the left
            ASTTraversal(root->left);

            // Reset Logical Statement Variables
            isLogical = 0;
            currLogicalStatements = 0;
            totalLogicalStatements = 0;
            
            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "if %s %d", currScope, getItemBlockNumber("if", findVarScope("if", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;


            // Traverse to the right to generate block code
            ASTTraversal(root->right);

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }

        if (strcmp(root->nodeType, "Elif") == 0) {
            // Change logical statement indicator variable to active
            isLogical = !strcmp(root->LHS, "Logical");

            // Set memory for logical type string array
            logicalTypes = calloc(2500, sizeof(char));
            for (int i = 0; i < 50; i++) { logicalTypes[i] = malloc(5 * sizeof(char));}

            // Traverse to the left
            ASTTraversal(root->left);

            // Reset Logical Statement Variables
            isLogical = 0;
            currLogicalStatements = 0;
            totalLogicalStatements = 0;

            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "elif %s %d", currScope, getItemBlockNumber("elif", findVarScope("elif", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;


            // Traverse to the right to generate block code
            ASTTraversal(root->right);

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }

        if(strcmp(root->nodeType, "IfStmtTail else end") == 0) {
            // Return an empty string 
            if(strcmp(root->right, "") == 0) {
                // Generate Logic Ending Statement if there's no else statement
                return "";
            }

            // If there is an Else statement, traverse right, then print logic ending statement
            ASTTraversal(root->right);
        }
        if(strcmp(root->nodeType, "Else") == 0) {
            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "else %s %d", currScope, getItemBlockNumber("else", findVarScope("else", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;

            // Traverse to the right to generate block code
            ASTTraversal(root->right);

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }
        if(strcmp(root->nodeType, "and") == 0) {
            ASTTraversal(root->left);
            ASTTraversal(root->right);
        }
        if(strcmp(root->nodeType, "or") == 0) {
            ASTTraversal(root->left);
            ASTTraversal(root->right);
        }
        if(strcmp(root->nodeType, "write") == 0) {
            if (root->right->LHS != NULL && strncmp(getPrimaryType(root->right->LHS), "var", 3) == 0 && strncmp(root->right->LHS, "int", 3) != 0 && strncmp(root->right->LHS, "float", 5) != 0) {
                // Write id in array callout format
                emitWriteArrayId(root->right->LHS, root->right->RHS);
            } else if (strcmp(root->RHS, "int") == 0
            || strcmp(root->RHS, "float") == 0
            || strcmp(root->RHS, "string") == 0) {
                updateUnusedVar(root->right->RHS);
            } else if (strncmp(getPrimaryType(root->RHS), "var", 3) == 0) {
                emitWriteId(root->RHS);
            } else {
                // Get tempVar from ASTtraversal
                char * tempVar = ASTTraversal(root->right);

                // Emit write operation using temporary variable
                emitWriteId(tempVar);
            }
        }
        if(strcmp(root->nodeType, "addline") == 0) {
        }
        if(strcmp(root->nodeType, "action context") == 0) {
            emitEntry(root->LHS);
            ASTTraversal(root->right);
        }
        if(strcmp(root->nodeType, "action") == 0) {
            ASTTraversal(root->left);
            ASTTraversal(root->right);
            
            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }
        if(strcmp(root->nodeType, "exprlist end") == 0) {
            if (root->RHS != NULL && root->RHS[0] != '\0' && root->RHS[0] != '\n') {
                strcpy(params[paramIndex], root->right->RHS);
                paramIndex += 1;
            }
        }
        if(strcmp(root->nodeType, "exprlist exprtail") == 0) {
            strcpy(params[paramIndex], root->left->RHS);
            paramIndex += 1;
            ASTTraversal(root->right);
        }
        if(strncmp(root->nodeType, "action call param list", 24) == 0) {
            memset(params, 0, 50 * 50);
            ASTTraversal(root->right);
        }
        if(strncmp(root->nodeType, "action call", 14) == 0) {
            char * funcVar = malloc(100*sizeof(char));
            ASTTraversal(root->right);
            strncpy(funcVar, root->LHS, 100);
            memset(buffer, 0, 50);
            strcpy(buffer, emitFunctionCall(funcVar));
            paramIndex = 0;
            return buffer;
        }
        if(strcmp(root->nodeType, "report") == 0) {
            // Traverse and report the RHS if the action has a return type
            strcpy(rightVar, ASTTraversal(root->right));
            emitReturn(rightVar);
        }
        if(strcmp(root->nodeType, "voidreport") == 0) {
        }
        if(strcmp(root->nodeType, "finish") == 0) {
        }
        if(strcmp(root->nodeType, "=") == 0) {
            strcpy(rightVar, ASTTraversal(root->right));
            if(strcmp(root->right->nodeType, "element assignment") == 0) {
                if(strcmp(root->right->RHS, "+") == 0
                    || strcmp(root->right->RHS, "-") == 0
                    || strcmp(root->right->RHS, "*") == 0
                    || strcmp(root->right->RHS, "/") == 0) {
                        // Check to see if the operation includes any array indexes
                        // If so, include the array callout in the binary operation
                        // Form leftVar variable
                        if (strncmp(getPrimaryType(root->right->right->left->LHS), "var", 3) == 0) {
                            sprintf(leftVar, "%s[%s]", root->right->right->left->LHS, root->right->right->left->RHS);
                        } else {
                            strcpy(leftVar, root->right->right->left->RHS);
                        }

                        // Form rightVar variable
                        if (strncmp(getPrimaryType(root->right->right->right->LHS), "var", 3) == 0) {
                            sprintf(rightVar, "%s[%s]", root->right->right->right->LHS, root->right->right->right->RHS);
                        } else {
                            strcpy(rightVar, root->right->right->right->RHS);
                        }
                        return emitBinaryOperation(root->right->RHS, leftVar, rightVar);
                    } else {
                        if (strncmp(getPrimaryType(root->right->right->LHS), "var", 3) == 0) {
                            sprintf(rightVar, "%s[%s]", root->right->right->LHS, root->right->right->RHS);
                        } else {
                            strcpy(rightVar, root->right->right->RHS);
                        }
                    }
            } else {
                emitAssignment(root->LHS, rightVar);
            }
        }
        if(strcmp(root->nodeType, "block") == 0) {
            return ASTTraversal(root->right);
        }
        if(strcmp(root->nodeType, "+") == 0
            || strcmp(root->nodeType, "-") == 0
            || strcmp(root->nodeType, "*") == 0
            || strcmp(root->nodeType, "/") == 0
            || strcmp(root->nodeType, ">") == 0
            || strcmp(root->nodeType, ">=") == 0
            || strcmp(root->nodeType, "<") == 0
            || strcmp(root->nodeType, "<=") == 0
            || strcmp(root->nodeType, "==") == 0
            || strcmp(root->nodeType, "!=") == 0)
            {
                // Check to see if the operation includes any array indexes
                // If so, include the array callout in the binary operation
                // Form leftVar variable
                if (strncmp(getPrimaryType(root->left->LHS), "var", 3) == 0) {
                    // New changes here
                    if (strncmp(root->left->LHS, "action call", 14) == 0) {
                        strcpy(leftVar, ASTTraversal(root->left));
                    } else if (strncmp(root->left->LHS, "int", 3) == 0 || strncmp(root->left->LHS, "float", 5) == 0) {
                        strcpy(leftVar, root->left->left->RHS);
                    } else if (strncmp(root->left->RHS, "action call param list", 24) == 0) {
                        strcpy(leftVar, ASTTraversal(root->left));
                    } else {
                        sprintf(leftVar, "%s[%s]", root->left->LHS, root->left->RHS);
                    }
                } else {
                    strcpy(leftVar, ASTTraversal(root->left));
                }

                if (strncmp(getPrimaryType(root->right->LHS), "var", 3) == 0) {
                    // New changes here
                    if (strncmp(root->right->LHS, "action call", 14) == 0) {
                        strcpy(rightVar, ASTTraversal(root->right));
                    } else if (strncmp(root->right->LHS, "int", 3) == 0 || strncmp(root->right->LHS, "float", 5) == 0) {
                        strcpy(rightVar, root->right->left->RHS);
                    } else if (strncmp(root->right->RHS, "action call param list", 24) == 0) {
                        strcpy(rightVar, ASTTraversal(root->right));
                    } else {
                        sprintf(rightVar, "%s[%s]", root->right->LHS, root->right->RHS);
                    }
                } else {
                    strcpy(rightVar, ASTTraversal(root->right));
                }

                // Traverse to the left FIRST if there's an expression; avoids last missing expression problem
                if (strcmp(root->LHS, "+") == 0
                    || strcmp(root->LHS, "-") == 0
                    || strcmp(root->LHS, "*") == 0
                    || strcmp(root->LHS, "/") == 0
                    || strcmp(root->LHS, ">") == 0
                    || strcmp(root->LHS, ">=") == 0
                    || strcmp(root->LHS, "<") == 0
                    || strcmp(root->LHS, "<=") == 0
                    || strcmp(root->LHS, "==") == 0
                    || strcmp(root->LHS, "!=") == 0) {
                        return emitBinaryOperation(root->nodeType, ASTTraversal(root->left), rightVar);;
                }
                return emitBinaryOperation(root->nodeType, leftVar, rightVar);
        }

        if(strcmp(root->LHS, "array") == 0) {
            arrElVar = malloc(50*sizeof(char));
            strcat(arrElVar, root->nodeType);
            strcat(arrElVar, root->RHS);
            return arrElVar;
        }
        return "";
    }
}

// Function to traverse the AST tree
// This initializes creating all of the IRcode for optimized IRcode

/**
 * @brief Applied code Optimization techniques
 *          1. Constant Propogation
 *          2. Dead Code Elimination
 *          3. Constant Folding
 *          4. Copy propogation (Currently disabled)
 * @param root 
 * @return * Code* 
 */
char* ASTTraversalOptimized(struct AST* root) {
    if(root != NULL) {
        // printf("root->nodeType: %s\n", root->nodeType);
        // if(root->LHS != NULL)
        // printf("root->LHS: %s\n", root->LHS);
        // if(root->RHS != NULL)
        // printf("root->RHS: %s\n", root->RHS);
        fflush(stdout);

        char rightVar[50];
        char leftVar[50];
        
        if(strcmp(root->nodeType, "int") == 0
            || strcmp(root->nodeType, "float") == 0
            || strcmp(root->nodeType, "string") == 0
            || strcmp(root->nodeType, "flag") == 0) {
                return root->RHS;
        }
        if(strcmp(root->nodeType, "type") == 0) {
            if (isUsedVar(root->RHS)) {
                if (root->right != NULL && strncmp(root->right->LHS, "array", 5) == 0) {
                    emitTypeArrayDeclarationOptimized(root->LHS, root->RHS, root->right->right->RHS);
                } else {
                    emitTypeDeclarationOptimized(root->LHS, root->RHS);
                }
            }
        }
        // Handle Logical Statements
        if(strcmp(root->nodeType, "Logical") == 0) {
            // Set logical operation type variables
            if (strcmp(root->RHS, "Logical") == 0) {
                strcpy(logicalTypes[totalLogicalStatements], root->right->RHS);
            } else {
                strcpy(logicalTypes[totalLogicalStatements], root->RHS);
            }
            totalLogicalStatements++;

            // Traverse to both conditionals
            ASTTraversalOptimized(root->right->left);
            ASTTraversalOptimized(root->right->right);
        }
        if(strcmp(root->nodeType, "Comparison") == 0) {
            if (isLogical == 1 && currLogicalStatements != 0) {
                emitLogicalExpression(logicalTypes[totalLogicalStatements-currLogicalStatements]);
                emitComparisonExpression(ASTTraversalOptimized(root->right->left), root->right, ASTTraversalOptimized(root->right->right));
                currLogicalStatements++;
            } else if (isLogical == 1) {
                emitComparisonExpression(ASTTraversalOptimized(root->right->left), root->right, ASTTraversalOptimized(root->right->right));
                currLogicalStatements++;
            }
            else {
                emitComparisonExpression(ASTTraversalOptimized(root->right->right->left), root->right->RHS, ASTTraversalOptimized(root->right->right->right));
            }
        }
        if(strcmp(root->nodeType, "program") == 0 
            || strcmp(root->nodeType, "vardec") == 0
            || strcmp(root->nodeType, "ActionDecl ActionDeclListTail") == 0
            || strcmp(root->nodeType, "ParaDecl comma ParaDeclListTail") == 0
            || strcmp(root->nodeType, "statements") == 0
            || strcmp(root->nodeType, "IfStmtTail continue") == 0
            || strcmp(root->nodeType, "IfStmt") == 0) { 
            ASTTraversalOptimized(root->left);
            ASTTraversalOptimized(root->right);
        }
        if(strcmp(root->nodeType, "WhileL") == 0) {
            // Start While Loop statement
            inWhileLoop = 1;
            emitWhileConditionStatement();

            // Change logical statement indicator variable to active
            isLogical = !strcmp(root->LHS, "Logical");

            // Set memory for logical type string array
            logicalTypes = calloc(2500, sizeof(char));
            for (int i = 0; i < 50; i++) { logicalTypes[i] = malloc(5 * sizeof(char));}

            // Traverse to the left to generate logical and conditional statements
            ASTTraversalOptimized(root->left);

            // Reset Logical Statement Variables
            isLogical = 0;
            currLogicalStatements = 0;
            totalLogicalStatements = 0;
            
            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "while %s %d", currScope, getItemBlockNumber("while", findVarScope("while", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;

            strcat(IRcodeOptimized, "\n");

            // Traverse to the right to generate block code
            ASTTraversalOptimized(root->right);

            // End the While statement
            emitWhileEndStatement();
            inWhileLoop = 0;
            
            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }

        if(strcmp(root->nodeType, "If") == 0) {
            // Start If Statement Condition
            emitIfConditionStatement();

            // Change logical statement indicator variable to active
            isLogical = !strcmp(root->LHS, "Logical");

            // Set memory for logical type string array
            logicalTypes = calloc(2500, sizeof(char));
            for (int i = 0; i < 50; i++) { logicalTypes[i] = malloc(5 * sizeof(char));}

            // Traverse to the left to generate logical and conditional statements
            ASTTraversalOptimized(root->left);

            // Reset Logical Statement Variables
            isLogical = 0;
            currLogicalStatements = 0;
            totalLogicalStatements = 0;
            
            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "if %s %d", currScope, getItemBlockNumber("if", findVarScope("if", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;

            strcat(IRcodeOptimized, "\n");

            // Traverse to the right to generate block code
            ASTTraversalOptimized(root->right);

            // End the If statement
            emitIfEndStatement();
            
            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }

        if (strcmp(root->nodeType, "Elif") == 0) {
            // Start Elif Statement Condition
            emitElifConditionStatement();

            // Change logical statement indicator variable to active
            isLogical = !strcmp(root->LHS, "Logical");

            // Set memory for logical type string array
            logicalTypes = calloc(2500, sizeof(char));
            for (int i = 0; i < 50; i++) { logicalTypes[i] = malloc(5 * sizeof(char));}

            // Traverse to the left to generate logical and conditional statements
            ASTTraversalOptimized(root->left);

            // Reset Logical Statement Variables
            isLogical = 0;
            currLogicalStatements = 0;
            totalLogicalStatements = 0;
            
            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "elif %s %d", currScope, getItemBlockNumber("elif", findVarScope("elif", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;

            strcat(IRcodeOptimized, "\n");

            // Traverse to the right to generate block code
            ASTTraversalOptimized(root->right);

            // End the Elif statement
            emitElifEndStatement();
            
            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }

        if(strcmp(root->nodeType, "IfStmtTail else end") == 0) {
            // Return an empty string 
            if(strcmp(root->right, "") == 0) {
                // Generate Logic Ending Statement if there's no else statement
                emitLogicEndStatement();
                return "";
            }

            // If there is an Else statement, traverse right, then print logic ending statement
            ASTTraversalOptimized(root->right);
            emitLogicEndStatement();
        }
        if(strcmp(root->nodeType, "Else") == 0) {
            // Start Else Statement Condition
            emitElseConditionStatement();
            
            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "else %s %d", currScope, getItemBlockNumber("else", findVarScope("else", prevScopes, totalIRScopes), 1));
            strcpy(prevScopes[totalIRScopes], currScope);
            strcpy(currScope, newScope);
            totalIRScopes++;

            strcat(IRcodeOptimized, "\n");

            // Traverse to the right to generate block code
            ASTTraversalOptimized(root->right);

            // End the Elif statement
            emitElseEndStatement();
            
            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalIRScopes-2]);
            totalIRScopes--;
        }
        if(strcmp(root->nodeType, "and") == 0
            || strcmp(root->nodeType, "or") == 0) {
                ASTTraversalOptimized(root->left);
                ASTTraversalOptimized(root->right);
        }
        if(strcmp(root->nodeType, "variable parm") == 0){
            isParam = 1;
            emitTypeDeclarationOptimized(root->LHS, root->RHS);
            isParam = 0;
        }
        if(strcmp(root->nodeType, "array parm") == 0) {
            emitTypeArrayDeclarationOptimized(root->LHS, root->RHS, "-1");
        }
        if(strcmp(root->nodeType, "write") == 0) {
            if (root->right->LHS != NULL && strncmp(getPrimaryType(root->right->LHS), "var", 3) == 0 && strncmp(root->right->LHS, "int", 3) != 0 && strncmp(root->right->LHS, "float", 5) != 0) {
                // Write id in array callout format
                if (isUsedVar(root->right->LHS)) {
                    emitWriteArrayIdOptimized(root->right->LHS, root->right->RHS);
                }
            } else if (strcmp(root->RHS, "int") == 0
            || strcmp(root->RHS, "float") == 0
            || strcmp(root->RHS, "string") == 0) {
                emitWritePrimary(root->right->RHS);
            } else if (strncmp(getPrimaryType(root->RHS), "var", 3) == 0) {
                if (isUsedVar(root->RHS)) {
                    emitWriteIdOptimized(root->RHS);
                }
            } else {
                // Get tempVar from ASTtraversal
                char * tempVar = ASTTraversalOptimized(root->right);

                // Emit write operation using temporary variable
                emitWriteIdOptimized(tempVar);
            }
        }
        if(strcmp(root->nodeType, "addline") == 0) {
            emitWriteLn();
        }
        if(strcmp(root->nodeType, "action context") == 0) {
            emitEntryOptimized(root->LHS);
            ASTTraversalOptimized(root->right);
        }
        if(strcmp(root->nodeType, "action") == 0) {
            if(isUsedVar(root->left->LHS)) {
                ASTTraversalOptimized(root->left);
                ASTTraversalOptimized(root->right);
                emitExit();
            }
        }
        if(strcmp(root->nodeType, "exprlist end") == 0) {
            if (root->RHS != NULL && root->RHS[0] != '\0' && root->RHS[0] != '\n') {
                strcpy(params[paramIndex], root->right->RHS);
                paramIndex += 1;
            }
        }
        if(strcmp(root->nodeType, "exprlist exprtail") == 0) {
            strcpy(params[paramIndex], root->left->RHS);
            paramIndex += 1;
            if (isUsedVar(root->RHS)) {
                if (root->right != NULL && strcmp(root->right->LHS, "array") == 0) {
                    emitTypeArrayDeclarationOptimized(root->LHS, root->right->right->RHS, root->RHS);
                } else {
                    emitTypeDeclarationOptimized(root->LHS, root->RHS);
                }
            }
            ASTTraversalOptimized(root->right);
        }
        if(strncmp(root->nodeType, "action call param list", 24) == 0) {
            memset(params, 0, 50 * 50);
            if(isUsedVar(root->RHS)) {
                if(root->right != NULL && strcmp(root->right->LHS, "array") == 0) {
                    emitTypeArrayDeclarationOptimized(root->LHS, root->right->right->RHS, root->RHS);
                } else {
                    emitTypeDeclarationOptimized(root->LHS, root->RHS);
                }
            }
            ASTTraversalOptimized(root->right);
        }
        if(strncmp(root->nodeType, "action call", 14) == 0) {
            ASTTraversalOptimized(root->right);
            memset(buffer, 0, 50);
            strcpy(buffer, emitFunctionCallOptimized(root->LHS));
            paramIndex = 0;
            return buffer;
        }
        if(strcmp(root->nodeType, "report") == 0) {
            strcpy(rightVar, ASTTraversalOptimized(root->right));
            emitReturnOptimized(rightVar);
        }
        if(strcmp(root->nodeType, "voidreport") == 0) {
            emitVoidReturn();
        }
        if(strcmp(root->nodeType, "finish") == 0) {
            emitFinish();
        }
        if(strcmp(root->nodeType, "block") == 0) {
            return ASTTraversalOptimized(root->right);
        }
        if(strcmp(root->nodeType, "=") == 0) {
            if (isUsedVar(root->LHS)) {
                strcpy(rightVar, ASTTraversalOptimized(root->right));
                if (strcmp(root->right->nodeType, "element assignment") == 0) {
                    if(strcmp(root->right->RHS, "+") == 0
                        || strcmp(root->right->RHS, "-") == 0
                        || strcmp(root->right->RHS, "*") == 0
                        || strcmp(root->right->RHS, "/") == 0) {
                            // Check to see if the operation includes any array indexes
                            // If so, include the array callout in the binary operation
                            // Form leftVar variable
                            if (strncmp(getPrimaryType(root->right->right->left->LHS), "var", 3) == 0) {
                                sprintf(leftVar, "%s[%s]", root->right->right->left->LHS, root->right->right->left->RHS);
                            } else {
                                strcpy(leftVar, root->right->right->left->RHS);
                            }

                            // Form rightVar variable
                            if (strncmp(getPrimaryType(root->right->right->right->LHS), "var", 3) == 0) {
                                sprintf(rightVar, "%s[%s]", root->right->right->right->LHS, root->right->right->right->RHS);
                            } else {
                                strcpy(rightVar, root->right->right->right->RHS);
                            }

                            return inWhileLoop ? emitBinaryOperationUnoptimized(root->right->RHS, leftVar, rightVar) : emitBinaryOperationOptimized(root->right->RHS, leftVar, rightVar);
                        } else {
                            if (strncmp(getPrimaryType(root->right->right->LHS), "var", 3) == 0) {
                                sprintf(rightVar, "%s[%s]", root->right->right->LHS, root->right->right->RHS);
                            } else {
                                strcpy(rightVar, root->right->right->RHS);
                            }
                            emitAssignmentForElementOptimized(root->LHS, root->right->LHS, rightVar);
                        }
                } else {
                    char rightVarOptimized[50];
                    strcpy(rightVarOptimized, getVarConstant(rightVar));
                    emitAssignmentOptimized(root->LHS, rightVarOptimized);
                }
            }
        }
        if(strcmp(root->nodeType, "+") == 0
            || strcmp(root->nodeType, "-") == 0
            || strcmp(root->nodeType, "*") == 0
            || strcmp(root->nodeType, "/") == 0
            || strcmp(root->nodeType, ">") == 0
            || strcmp(root->nodeType, ">=") == 0
            || strcmp(root->nodeType, "<") == 0
            || strcmp(root->nodeType, "<=") == 0
            || strcmp(root->nodeType, "==") == 0
            || strcmp(root->nodeType, "!=") == 0) {
                char rightVarOptimized[50];
                char leftVarOptimized[50];

                // Check to see if the operation includes any array indexes
                // If so, include the array callout in the binary operation
                // Form leftVar variable
                if (strncmp(getPrimaryType(root->left->LHS), "var", 3) == 0) {
                    // New changes here
                    if (strncmp(root->left->LHS, "action call", 14) == 0) {
                        strcpy(leftVar, ASTTraversalOptimized(root->left));
                    } else if (strncmp(root->left->LHS, "int", 3) == 0 || strncmp(root->left->LHS, "float", 5) == 0) {
                        strcpy(leftVar, root->left->left->RHS);
                    } else if (strncmp(root->left->RHS, "action call param list", 24) == 0) {
                        strcpy(leftVar, ASTTraversalOptimized(root->left));
                    } else {
                        sprintf(leftVar, "%s[%s]", root->left->LHS, root->left->RHS);
                    }
                } else {
                    strcpy(leftVar, ASTTraversalOptimized(root->left));
                }

                // Form rightVar variable
                if (strncmp(getPrimaryType(root->right->LHS), "var", 3) == 0) {
                    // New changes here
                    if (strncmp(root->right->LHS, "action call", 14) == 0) {
                        strcpy(rightVar, ASTTraversalOptimized(root->right));
                    } else if (strncmp(root->right->LHS, "int", 3) == 0 || strncmp(root->right->LHS, "float", 5) == 0) {
                        strcpy(rightVar, root->right->left->RHS);
                    } else if (strncmp(root->right->RHS, "action call param list", 24) == 0) {
                        strcpy(rightVar, ASTTraversalOptimized(root->right));
                    } else {
                        sprintf(rightVar, "%s[%s]", root->right->LHS, root->right->RHS);
                    }
                } else {
                    strcpy(rightVar, ASTTraversalOptimized(root->right));
                }

                // Traverse to the left FIRST if there's an expression; avoids last missing expression problem
                if (strcmp(root->LHS, "+") == 0
                    || strcmp(root->LHS, "-") == 0
                    || strcmp(root->LHS, "*") == 0
                    || strcmp(root->LHS, "/") == 0
                    || strcmp(root->LHS, ">") == 0
                    || strcmp(root->LHS, ">=") == 0
                    || strcmp(root->LHS, "<") == 0
                    || strcmp(root->LHS, "<=") == 0
                    || strcmp(root->LHS, "==") == 0
                    || strcmp(root->LHS, "!=") == 0) {
                        char * newVar = getVarConstant(ASTTraversalOptimized(root->left));
                        strcpy(rightVarOptimized, getVarConstant(newVar));
                        strcpy(leftVarOptimized, getVarConstant(rightVar));
                        return inWhileLoop ? emitBinaryOperationUnoptimized(root->nodeType, leftVar, rightVar) : emitBinaryOperationOptimized(root->nodeType, rightVarOptimized, leftVarOptimized);
                }

                strcpy(rightVarOptimized, getVarConstant(leftVar));
                strcpy(leftVarOptimized, getVarConstant(rightVar));

                return inWhileLoop ? emitBinaryOperationUnoptimized(root->nodeType, leftVar, rightVar) : emitBinaryOperationOptimized(root->nodeType, rightVarOptimized, leftVarOptimized);
        }
        if(strcmp(root->LHS, "array") == 0) {
            memcpy(arrElVar, 0, 50);
            strcat(arrElVar, root->nodeType);
            strcat(arrElVar, root->RHS);
            return arrElVar;
        }
        return "";
    }
}

// Main functions for IRcode generation
void generateIRCode() {
    // Set Scope Memory
    currScope = calloc(1000, sizeof(char));
    strcpy(currScope, "global");
    prevScopes = malloc(MAX_ARRAY_LENGTH * sizeof(char*));
    for (int i = 0; i < MAX_ARRAY_LENGTH; i++) { prevScopes[i] = malloc(100 * sizeof(char)); }
    strcpy(prevScopes[0], "global");

    // Start AST Traversal
    ASTTraversal(ast);
}

// Main function for optimized IRcode generation
char * generateIRCodeOptimized() {
    // Reset temp variable number
    lastIndex = 0;

    // Set IRcodeOptimized String memory
    IRcodeOptimized = malloc(1000000 * sizeof(char));

    // Set copy variable string memory
    copyVar = malloc(MAX_LINE_LENGTH * sizeof(char));

    // Set Scope Memory
    strcpy(currScope, "global");

    // Start Optimized AST Traversal
    ASTTraversalOptimized(ast);
    fflush(stdout);

    return IRcodeOptimized;
}