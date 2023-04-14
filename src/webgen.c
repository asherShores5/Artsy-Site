// Set of functions to emit WAT code
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "webgen.h"
#include "helper.h"
#include "semantic.h"
#include "symbolTable.h"

// All file string variables

// The WATcode file has the following purposes:
// 1. To place code directly first within the final file first (e.g., start module code)
// 2. To start functions, hold function parameters, and the function result parameter
// 3. To end functions with an export line
// 4. To declare global variables
// 5. To place temporary variables at the top of a local scope
// 6. Once the compilation process is complete, the other files will copy their contents into this file

char * WATcode; // Generates final WATcode string

// All other variables are helper strings used in the compilation process
char * MAINcode; // File used to place code within the main function
char * VARScode; // File used to place variables at the top of the main function
char * LOCALcode; // File used to place code within a local scope

// Copy variable
char * copyWAT;

// Standard program variables
#define MAX_LINE_LENGTH 10000
#define MAX_ARRAY_LENGTH 5000
char varDelimiter[] = "[], ";
char * returnType;
char * currOp;

// Scope Variables
char * currScope;
char ** prevScopes;
int totalWebScopes = 1;

// Variables to detect if declared types and params are within a function
int inMain = 1;
int inParams = 0;
int inLogic = 0;

// Variable to track finish indexes
int * finishIndexes;
int stackNumber = 0;

// File functions

// Function to open the files for the WebAssembly Generator
// Required before generating any WAT code
void initAssemblyFile() {
    WATcode = malloc(1000000 * sizeof(char));
    MAINcode = malloc(1000000 * sizeof(char));
    VARScode = malloc(1000000 * sizeof(char));
    LOCALcode = malloc(1000000 * sizeof(char));
    copyWAT = malloc(MAX_LINE_LENGTH * sizeof(char));
}

// Function to get file type from string
char * getFileType(char * fileType) {
    if (strncmp(fileType, "MAINcode", 8) == 0) {
        return MAINcode;
    } else if (strncmp(fileType, "LOCALcode", 9) == 0) {
        return LOCALcode;
    } else if (strncmp(fileType, "VARScode", 8) == 0) {
        return VARScode;
    } else if (strncmp(fileType, "WATcode", 7) == 0) {
        return WATcode;
    }
    return 0;
}

// Function to get file type from string
char * getHelperFileType(char * fileType) {
    if (strncmp(fileType, "MAINcode", 8) == 0) {
        return VARScode;
    } else if (strncmp(fileType, "LOCALcode", 9) == 0) {
        return WATcode;
    }
    return 0;
}


// String hash table functions

// Define iterator and default variables for updating array index calls by dedicated memory index
#define TABLE_SIZE 256
#define MAX_KEY_LENGTH 32
int currMaxStringIndex = 0;

// Define StringEntry struct for each table
struct StringEntry {
    char key[MAX_KEY_LENGTH];
    int value;
};

// Define the hash table
struct StringHashTable {
    struct StringEntry table[TABLE_SIZE];
};

// Get hash value from key
unsigned int hash(char* key) {
    unsigned int value = 0;
    for (int i = 0; i < strlen(key); i++) {
        value = value * 31 + key[i];
    }
    return value % TABLE_SIZE;
}

// Function to initialize a string hash table
void init(struct StringHashTable* dict) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        dict->table[i].key[0] = '\0';
        dict->table[i].value = 0;
    }
}

// Function to set an entry in a string hash table
void set(struct StringHashTable* dict, char * key, char * scope, int value) {
    // Get new key
    char * newKey = malloc(1000 * sizeof(char));
    strcpy(newKey, key);
    strcat(newKey, scope);

    // Set hash value
    unsigned int index = hash(key);
    strcpy(dict->table[index].key, key);
    dict->table[index].value = value;
}

// Function to get an entry from a string hash table
int get(struct StringHashTable* dict, char * key, char * scope) {
    // Get new key
    char * newKey = malloc(1000 * sizeof(char));
    strcpy(newKey, key);
    strcat(newKey, scope);

    // Get hash value
    unsigned int index = hash(key);
    if (strcmp(dict->table[index].key, key) == 0) {
        return dict->table[index].value;
    }
    return -1;
}

// Create String Hash Tables
struct StringHashTable stringAddresses;
struct StringHashTable stringSizes;

// Temporary Variable Table implementation

// Generate a struct for a temporary variable type
struct TempVar {
    char * name;
    char * type;
    char * kind;
};

// Generate a table of temporary variables
struct TempVar tempVarTable[TABLE_SIZE];

// Current temporary variable table index
int tempVarIndex = 0;

// Iterator variable for curr print string
int currprint_strings = 0;

// Add temporary variable item to the table
char * addTempVarItem(char * name, char * type, char * kind) {
    // Assign memory to each item field
    tempVarTable[tempVarIndex].name = malloc(sizeof(char)*100);
    tempVarTable[tempVarIndex].type = malloc(sizeof(char)*100);
    tempVarTable[tempVarIndex].kind = malloc(sizeof(char)*100);
    
    // Copy string contents to struct field
    strncpy(tempVarTable[tempVarIndex].name, name, 100);
    strncpy(tempVarTable[tempVarIndex].type, type, 100);
    strncpy(tempVarTable[tempVarIndex].kind, kind, 100);

    // Increase Temp Variable Table Index
    tempVarIndex++;
}

// Check if item name is in the temporary variable table
int isTempVar(char * name) {
    for (int i = 0; i < tempVarIndex; i++) {
        if (strncmp(tempVarTable[i].name, name, 1000) == 0) {
            return 1;
		}
	}
    return 0;
}

// Get the temporary variable type based on the name
char * getTempVarType(char * name) {
    for (int i = 0; i < tempVarIndex; i++){
        if (strcmp(tempVarTable[i].name, name) == 0) {
            return tempVarTable[i].type;
		}
	}
    return 0;
}

// Get the temporary variable kind based on the name
char * getTempVarKind(char * name) {
    for (int i = 0; i < tempVarIndex; i++){
        if (strcmp(tempVarTable[i].name, name) == 0) {
            return tempVarTable[i].kind;
		}
	}
    return 0;
}

// Helper function to find scopeType
char * getScopeType(char * varName) {
    // Goal: Remove any "[]" characters from the string, create a new variable as a token
    char * varToken = malloc(100*sizeof(char));
    strcpy(varToken, varName);
    varToken = strtok(varToken, varDelimiter);

    // Return based on whether it is found in global or not
    return found(varToken, "global", 0) ? "global" : "local";
}

// Helper function to switch context from within parameters to within a scope
void checkParamContext() {
    if (inParams) {
        inParams = 0;
        generateResultWAT(WATcode, returnType);
    }
}

// Helper function to get finish index
int getFinishIndex() {
    int newIndex = 0;
    for (int i = 0; i < stackNumber + 1; i++) {
        newIndex += finishIndexes[i];
    }
    return newIndex;
}

// WAT CODE PRINT MODULES

// Function to generate the initial set of lines required for proper WebAssembly output
void generateStartModule() {
    strcat(WATcode, "(module\n");
    strcat(WATcode, "\t;; WAT Setup Declarations\n");
    strcat(WATcode, "\t(import \"env\" \"print_int\" (func $print_int (param i32)))\n");
    strcat(WATcode, "\t(import \"env\" \"print_float\" (func $print_float (param f32)))\n");
	strcat(WATcode, "\t(import \"env\" \"print_string\" (func $print_string (param i32)))\n");
    strcat(WATcode, "\t(import \"env\" \"newline\" (func $newline))\n");
    strcat(WATcode, "\t(memory $0 100)\n");
    strcat(WATcode, "\t(export \"pagememory\" (memory $0))\n");
    strcat(WATcode, "\t(func $create_array (param $size i32) (result i32) (local $ptr i32) (set_local $ptr (i32.const 0)) (block (loop $loop (br_if $loop (i32.eq (get_local $size) (i32.const 0))) (set_local $ptr (i32.add (get_local $ptr) (i32.const 4))) (set_local $size (i32.sub (get_local $size) (i32.const 1))))) (get_local $ptr))\n");
    strcat(WATcode, "\t(export \"create_array\" (func $create_array))\n");
    strcat(WATcode, "\t(func $get_element_i32 (param $ptr i32) (param $index i32) (result i32) (i32.load (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4)))))\n");
    strcat(WATcode, "\t(export \"get_element_i32\" (func $get_element_i32))\n");
    strcat(WATcode, "\t(func $set_element_i32 (param $ptr i32) (param $index i32) (param $value i32) (i32.store (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))) (get_local $value)))\n");
    strcat(WATcode, "\t(export \"set_element_i32\" (func $set_element_i32))\n\n");
    strcat(WATcode, "\t(func $get_element_f32 (param $ptr i32) (param $index i32) (result f32) (f32.load (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4)))))\n");
    strcat(WATcode, "\t(export \"get_element_f32\" (func $get_element_f32))\n");
    strcat(WATcode, "\t(func $set_element_f32 (param $ptr i32) (param $index i32) (param $value f32) (f32.store (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))) (get_local $value)))\n");
    strcat(WATcode, "\t(export \"set_element_f32\" (func $set_element_f32))\n\n");
    strcat(WATcode, "\t;; Artsy Program in WAT\n");
}

// Function module for handling addline statements
void generateAddLineWAT(char * printFile) {
    strcat(printFile, "\t\t(call $newline)\n");
}

// Function module for declaring a variable
void generateVarDeclareStatementWAT(char * printFile, char * varType, char * varName) {
    // Step 1: Get the WAT Type, current scope, and files
    char * scopeType = getScopeType(varName);
    char * varScope = findVarScope(varName, prevScopes, totalWebScopes);
    char * WATType = strncmp(getItemKind(varName, varScope, 1), "Array", 5) == 0 ? getWATType("int") : getWATType(getItemType(varName, currScope, 1));
    char * printFileStr = inMain ? "MAINcode" : "LOCALcode";
    char * helperFile = getHelperFileType(printFileStr);

    // Step 2: Handle standard variable declarations
    if (inMain && inLogic == 0) {
        // Find the placeholder value, based on whether its a float or int
        char * placeholderVar = calloc(100, sizeof(char));
        placeholderVar = strncmp(WATType, "f32", 3) == 0 ? "0.0" : "0";

        // Output the type to the global section
        sprintf(copyWAT, "\t(global $%s (mut %s) (%s.const %s))\n", varName, WATType, WATType, placeholderVar);
        strcat(WATcode, copyWAT);
    } else {
        sprintf(copyWAT, "\t\t(local $%s %s)\n", varName, WATType);
        strcat(helperFile, copyWAT);
    }
}

void generateArrayDeclareStatementWAT(char * printFile, char * varType, char * varName, char * arrSize) {    
    // Step 1: Create string variable in hash table
    int indexEntry = currMaxStringIndex;
    char * varScope = findVarScope(varName, prevScopes, totalWebScopes);
    set(&stringAddresses, varName, varScope, indexEntry);
    set(&stringSizes, varName, varScope, atoi(arrSize));

    // Step 2: Update max string index for the next array entry
    currMaxStringIndex += atoi(arrSize);

    // Step 3: Find the scope type for the array variable
    char * scopeType = getScopeType(varName);

    // Step 3: Generate WAT code for creating the new array
    sprintf(copyWAT, "\t\t(%s.set $%s (call $create_array (i32.const %s)))", scopeType, varName, arrSize);
    strcat(printFile, copyWAT);
}

// Function module for printing get statements for all variable types in WAT
void generateGetStatementWAT(char * printFile, char * varName) {
    // Step 1: Get the primary type, the length, and the scope type of the variable name
    char * primaryType = getPrimaryType(varName);
    int len = strlen(varName);
    char * scopeType = getScopeType(varName);

    // Step 2a: Print a const get statement if its a primary
    if (strncmp(primaryType, "var", 3) != 0) {
        // Get WAT Type
        char * WATType = getWATType(primaryType);

        // Check if primaryVar references a char
        // If it does, convert it to an ASCII value
        if (strncmp(getPrimaryType(varName), "string", 6) == 0) {
            varName = convertToASCII(varName);
        }

        // Check if primaryVar equals zero as a string char (unidentified value)
        // If so, convert it to a space in ASCII
        if (strncmp(varName, "0", 1) == 0 && strncmp(primaryType, "string", 6) == 0) {
            varName = "32";
        }

        // Print the statement
        sprintf(copyWAT, " (%s.const %s)", WATType, varName);
        strcat(printFile, copyWAT);
    }

    // Step 2b: Generate an array index get statement if it's a variable with a "[]"
    else if (varName[len - 1] == ']') {
        // Goal: Return the get_element statement
        
        // Get the array variable and the index number
        char * arrName = calloc(100, sizeof(char));
        char * arrEl = calloc(100, sizeof(char));
        char * arrIndex = calloc(100, sizeof(char));

        // Assign variable name
        strcpy(arrName, varName);
        arrName = strtok(arrName, varDelimiter);

        // Get the array scope and asscoiated WATType
        char * arrScope = findVarScope(arrName, prevScopes, totalWebScopes);
        char * WATType = getWATType(getItemType(arrName, arrScope, 1));

        // Assign array index variable
        strcpy(arrEl, varName);
        arrEl = strtok(arrEl, varDelimiter);
        arrEl = strtok(NULL, varDelimiter);
        sprintf(arrIndex, "%d", get(&stringAddresses, arrName, arrScope) + atoi(arrEl));

        // Print array index return statement
        sprintf(copyWAT, " (call $get_element_%s (%s.get $%s) (i32.const %s))\n", WATType, scopeType, arrName, arrIndex);
        strcat(printFile, copyWAT);
    }
    
    // Step 2c: Generate a standard scope.get statment using the variable name
    else {
        sprintf(copyWAT, "\t\t(%s.get $%s)", scopeType, varName);
        strcat(printFile, copyWAT);
    }
}

void generateAssignmentWAT(char * printFile, char * assignVar, char * setVar) {
    // Step 1: Get the scope type and assignment variable length
    char * scopeType = getScopeType(assignVar);
    int len = strlen(assignVar);

    // Step 2: If a current operation is not assigned, get the current operation from the assignment statement
    if (strncmp(currOp, "\0", 1) == 0 || currOp == NULL) {
        currOp = getPrimaryType(setVar);
    }

    // Step 3: If the assignment variable is an array index callout variable, generate a unique starting set call
    if (assignVar[len - 1] == ']') {
        // Get the array variable and the index number
        char * arrName = calloc(100, sizeof(char));
        char * arrEl = calloc(100, sizeof(char));
        char * arrIndex = calloc(100, sizeof(char));
        char * WATType = calloc(100, sizeof(char));

        // Assign variable name
        strcpy(arrName, assignVar);
        arrName = strtok(arrName, varDelimiter);
        
        // Redetermine the scope type and scope name with the revised name
        scopeType = getScopeType(arrName);
        char * arrScope = findVarScope(arrName, prevScopes, totalWebScopes);

        // Get the WATType of the array
        WATType = getWATType(getItemType(arrName, arrScope, 1));

        // Assign array index variable
        strcpy(arrEl, assignVar);
        arrEl = strtok(arrEl, varDelimiter);
        arrEl = strtok(NULL, varDelimiter);
        sprintf(arrIndex, "%d", get(&stringAddresses, arrName, arrScope) + atoi(arrEl));

        // Print a starting set call for an array index callout
        sprintf(copyWAT, "\t\t(call $set_element_%s (%s.get $%s) (i32.const %s)", WATType, scopeType, arrName, arrIndex);
        strcat(printFile, copyWAT);

        // Generate an array index get call
        generateGetStatementWAT(printFile, setVar);
        strcat(printFile, ")\n");
    }
    
    // Step 4: If its an array assignment, copy all array indexes to the new array using a for-loop
    else if (strncmp(getItemKind(assignVar, findVarScope(assignVar, prevScopes, totalWebScopes), 1), "Array", 5) == 0 || strncmp(getItemType(assignVar, findVarScope(assignVar, prevScopes, totalWebScopes), 1), "string", 6) == 0) {
        // Get variable scopes and scope types
        char * assignmentScope = findVarScope(assignVar, prevScopes, totalWebScopes);
        char * setVarScope = findVarScope(setVar, prevScopes, totalWebScopes);
        char * setVarScopeType = getScopeType(setVar);

        // Update assignVar array size
        set(&stringSizes, assignVar, assignmentScope, get(&stringSizes, setVar, setVarScope));

        // Get the WATType for the operation
        char * WATType = getWATType(getItemType(assignVar, assignmentScope, 1));

        for (int i = 0; i < get(&stringSizes, assignVar, assignmentScope); i++) {
            // Print a starting set call for an array index callout
            sprintf(copyWAT, "\t\t(call $set_element_%s (%s.get $%s) (i32.const %d)", WATType, scopeType, assignVar, i + get(&stringAddresses, assignVar, assignmentScope));
            strcat(printFile, copyWAT);
            
            // Generate an array index get call for each element
            sprintf(copyWAT, " (call $get_element_%s (%s.get $%s) (i32.const %d))\n", WATType, setVarScopeType, setVar, i + get(&stringAddresses, setVar, setVarScope));
            strcat(printFile, copyWAT);
            strcat(printFile, ")\n");
        }
    }

    // Step 5: If its a standard assignment variable, generate standard starting call and get call
    else {
        sprintf(copyWAT, "\t\t(%s.set $%s\n", scopeType, assignVar);
        strcat(printFile, copyWAT);
        generateGetStatementWAT(printFile, setVar);
        strcat(printFile, ")\n");
    }
    
    // Step 7: Reset currOp variable
    currOp = calloc(100, sizeof(char));
}

// Function module for handling parameter statements
void generateParameterWAT(char * printFile, char * type, char * varName) {
    sprintf(copyWAT, "(param $%s %s) ", varName, getWATType(type));
    strcat(printFile, copyWAT);
}

// Function module for handling return parameters
void generateResultWAT(char * printFile, char * returnType) {
    if (strncmp(returnType, "void", 4) != 0) {
        sprintf(copyWAT, "(result %s)\n", returnType);
        strcat(printFile, copyWAT);
    } else {
        strcat(printFile, "\n");
    }
}

// Function module for handling action returns with a return type
void generateReturnWAT(char * varName, char * scopeType) {
    // Create starting return statement in WAT
    strcat(LOCALcode, "\t\t(return");

    // Generate the Get Statement in WAT
    generateGetStatementWAT(LOCALcode, varName);

    // Generate the return ending statement in WAT
    strcat(LOCALcode, ")\n");    
}

// Function module for handling action returns without a return type
void generateVoidReturnWAT() {
    // Create void return statement in WAT
    strcat(LOCALcode, "\t\t(return)\n");
}

void generateFinishWAT(char * printFile) {
    // Create finish statement in wat
    sprintf(copyWAT, "\t\t(br %d)\n", getFinishIndex());
    strcat(printFile, copyWAT);
}


void generateActionEndWAT(char * actionScope) {
    sprintf(copyWAT, "\t)\n\t(export \"%s\" (func $%s))\n", actionScope, actionScope);
    strcat(WATcode, copyWAT);
}

// Function module for handling set_element statements in WAT strings
void setStringElementWAT(char * printFile, char * WATType, char * scopeType, char * arrName, int arrIndex, int arrVal) {
    sprintf(copyWAT, "\t\t(call $set_element_%s (%s.get $%s) (i32.const %d) (%s.const %d))\n", WATType, scopeType, arrName, arrIndex, WATType, arrVal);
    strcat(printFile, copyWAT);
}

// Function module for generating print module calls
void generatePrintModuleWAT(char * printFile, char * printType) {
    sprintf(copyWAT, "\t\t(call $print_%s", printType);
    strcat(printFile, copyWAT);
}

// Function module for handling printing solo string in Artsy code
void generateSoloStringWAT(char * printFile, char * strVal) {
    // Step 1: Generate a unique print variable name
    char * printVar = malloc(sizeof(char)*100);
    snprintf(printVar, 100, "_printstr_%d", currprint_strings);
    currprint_strings++; // Iterate current number of solo print strings in the program

    // Step 2: Convert the strVal to a usable format that supports escape characters
    int numEscapeCharacters = countEscapeChars(strVal);
    int printStrArrSize = strlen(strVal)-2-numEscapeCharacters; // Iterator variable to keep track of size
    char ** printStrArr = (char **) malloc(printStrArrSize * 10 * sizeof(char)); // Create string array variable

    // Allocate memory for each string
    for (int i = 0; i < printStrArrSize; i++){
        printStrArr[i] = (char *) malloc(20 * sizeof(char));
    }

    // Populate printStrArr variable
    int loopEscapeChars = 0; // Tracks total escape chars encountered
    for (int i = 1; i < printStrArrSize + 1; i++) {
        // Declare insertStr variable
        char * insertStr = malloc(sizeof(char)*20);

        // Form insertStr variable
        if (strVal[i+loopEscapeChars] == '\\') { // Case for an escape character
            snprintf(insertStr, 20, "\"%s\"", escapeCharType(strVal[i+1+loopEscapeChars]));
            loopEscapeChars++;
        } else { // Standard case (no escape character)
            snprintf(insertStr, 20, "\"%c\"", strVal[i+loopEscapeChars]);
        }

        // Put the insertStr variable into the string array
        strncpy(printStrArr[i-1], insertStr, 20);
    }

    // Step 3: Add the string to the string hash table
    int indexEntry = currMaxStringIndex;
    int strSize = printStrArrSize;
    set(&stringAddresses, printVar, currScope, indexEntry);
    set(&stringSizes, printVar, currScope, strSize);
    currMaxStringIndex += strSize; // Update max string index for the next array entry

    // Step 4: Print the whole string in one area

    // Step 4A: Determine helper file type
    char * printFileStr = inMain ? "MAINcode" : "LOCALcode";
    char * helperFile = getHelperFileType(printFileStr);

    // Step 4B: Declare the new array variable
    char * scopeType = "local"; // Declare scope variable
    sprintf(copyWAT, "\t\t(%s $%s i32)\n", scopeType, printVar);
    strcat(helperFile, copyWAT);

    // Step 4C: Generate WAT code for creating the new array
    sprintf(copyWAT, "\t\t(%s.set $%s (call $create_array (i32.const %d)))\n", scopeType, printVar, strlen(strVal));
    strcat(printFile, copyWAT);

    // Step 4D: Get the WAT type for the solo string
    char * WATType = getWATType(getPrimaryType(strVal));

    // Step 4E: Set each element from the string into the new array using a for-loop
    for (int currArrIndex = 0; currArrIndex < strSize; currArrIndex++) {
        setStringElementWAT(printFile, WATType, scopeType, printVar, indexEntry + currArrIndex, atoi(convertToASCII(printStrArr[currArrIndex])));
    }

    // Step 4F: Print each element in the new string array using a for-loop
    for (int currArrIndex = 0; currArrIndex < strSize; currArrIndex++) {
        generatePrintModuleWAT(printFile, "string");
        sprintf(copyWAT, "\t\t (call $get_element_%s (%s.get $%s) (i32.const %d)))\n", WATType, scopeType, printVar, indexEntry + currArrIndex);
        strcat(printFile, copyWAT);
    }

}

void generateOutputStatementWAT(char * printFile, char * varName) {
    // Step 1a: Get the primary type, the length, and the scope type of the variable name
    char * primaryType = getPrimaryType(varName);
    int len = strlen(varName);
    char * scopeType = getScopeType(varName);

    // Step 1b: Declare scope, type, and item kind variables if it is a variable
    char * itemScope;
    char * varType;
    char * itemKind;
    char * token = calloc(100, sizeof(char));
    strcpy(token, varName);
    token = strtok(token, varDelimiter);

    // If the variable is a temporary assigned table, use the type & kind from the TempVar table
    if (strncmp(primaryType, "var", 3) == 0 && isTempVar(token)) {
        varType = getTempVarType(token);
        itemKind = getTempVarKind(token);
    }

    // Else, get the variable type and item kind from the symbol table
    else if (strncmp(primaryType, "var", 3) == 0) {
        itemScope = findVarScope(token, prevScopes, totalWebScopes);
        varType = getItemType(token, itemScope, 1);
        itemKind = getItemKind(token, itemScope, 1);
    }

    // Step 2: If the output statement uses a solo string, print the string
    if (strncmp(primaryType, "string", 6) == 0) {
        generateSoloStringWAT(printFile, varName);
    }

    // Step 3: If the output uses a primary type, print the primary
    else if (strncmp(primaryType, "var", 3) != 0) {
        generatePrintModuleWAT(printFile, primaryType);
        generateGetStatementWAT(printFile, varName);
        strcat(printFile, "\t\t)\n");
    } 
    
    // Step 4: If the type is an array index callout, get the statement and print it
    else if (varName[len - 1] == ']') {
        // Start call statement with primary type
        generatePrintModuleWAT(printFile, varType);

        // Generate the get statement
        generateGetStatementWAT(printFile, varName);

        // End the output statement
        strcat(printFile, ")\n");
    }

    // Step 5: If the type is a full array or a string variable, output all available indexes
    else if (strncmp(itemKind, "Array", 5) == 0 || strncmp(primaryType, "var", 3) == 0 && strncmp(varType, "string", 6) == 0) {
        // Assign array index variable
        int arrIndex = get(&stringAddresses, varName, itemScope);

        // Get the WATType of the array
        char * WATType = getWATType(varType);
        
        // Print out all available indexes using a for-loop
        for (int newIndex = 0; newIndex < get(&stringSizes, varName, itemScope); newIndex++) {
            generatePrintModuleWAT(printFile, varType);
            sprintf(copyWAT, " (call $get_element_%s (%s.get $%s) (i32.const %d)))\n", WATType, scopeType, varName, arrIndex + newIndex);
            strcat(printFile, copyWAT);
        }
    }

    // Step 6: If the type is a standard variable, get the statement and print it
    else {
        // Start call statement with primary type
        generatePrintModuleWAT(printFile, varType);

        // Generate the get statement
        generateGetStatementWAT(printFile, varName);

        // End the output statement
        strcat(printFile, ")\n");
    }
}

void generateActionCallStartWAT(char * printFile, char * assignVar, char * actionVar) {
    // Step 1: Get the file type, helper file type, scope type, and the operation type
    char * printFileStr = inMain ? "MAINcode" : "LOCALcode";
    char * helperFile = getHelperFileType(printFileStr);
    char * scopeType = getScopeType(assignVar);
    char * opType = getWATType(currOp);

    // Step 2: Print a temporary variable declaration line with the WAT type
    // Save the temporary variable to the table
    addTempVarItem(assignVar, currOp, "Var");
    sprintf(copyWAT, "\t\t(%s $%s %s)\n", scopeType, assignVar, opType);
    strcat(helperFile, copyWAT);

    // Step 3: Output the assignVar call line
    sprintf(copyWAT, "\t\t(%s.set $%s", scopeType, assignVar);
    strcat(printFile, copyWAT);

    // Step 4: Generate starting action call line
    sprintf(copyWAT, " (call $%s\n", actionVar);
    strcat(printFile, copyWAT);
}

void generateOperationWAT(char * printFile, char * assignVar, char * leftVar, char * opChar, char * rightVar) {
    // Step 1: Get the file type, helper file type, scope type, and the operation type
    char * printFileStr = inMain ? "MAINcode" : "LOCALcode";
    char * helperFile = getHelperFileType(printFileStr);
    char * scopeType = getScopeType(assignVar);
    char * opType = getWATType(currOp);

    // Step 2: Generate a temporary variable declaration line and add to the TempVar table
    addTempVarItem(assignVar, currOp, "Var");
    sprintf(copyWAT, "\t\t(%s $%s %s)\n", scopeType, assignVar, opType);
    strcat(helperFile, copyWAT);

    // Step 3: Output the assignment variable set line
    sprintf(copyWAT, "\t\t(%s.set $%s\n", scopeType, assignVar);
    strcat(printFile, copyWAT);

    // Step 4a: Determine operation call for WAT
    // "+" = "add", "-" = sub, "*" = mul, and "/" = "div" or "div_s" 
    char * opCall = calloc(10, sizeof(char));

    if (strncmp(opChar, "+", 1) == 0) {
        strcpy(opCall, "add");
    } else if (strncmp(opChar, "-", 1) == 0) {
        strcpy(opCall, "sub");
    } else if (strncmp(opChar, "*", 1) == 0) {
        strcpy(opCall, "mul");
    } else if (strncmp(opChar, "/", 1) == 0) {
        strcpy(opCall, "div");
    }

    // Step 4b: If the operation is an integer and uses division, specify "div_s"
    if (strncmp(opCall, "div", 3) == 0 && strncmp(opType, "i32", 3) == 0) {
        strncat(opCall, "_s", 2);
    }

    // Step 5: Start the operation call line
    sprintf(copyWAT, "\t\t(%s.%s", opType, opCall);
    strcat(printFile, copyWAT);

    // Step 6: Declare the get statements for the left and right variable
    generateGetStatementWAT(printFile, leftVar);
    generateGetStatementWAT(printFile, rightVar);

    // Step 7: End the operation call
    strcat(printFile, "\t\t))\n");
}

void generateComparisonWAT(char * printFile, char * leftTerm, char * compareType, char * rightTerm, char * logicType) {
    // Step 1: Determine Item Scope and WAT Type
    char * compareWATType = calloc(10, sizeof(char));
    
    if (strncmp(getPrimaryType(leftTerm), "var", 3) == 0) {
        char * itemScope = findVarScope(leftTerm, prevScopes, totalWebScopes);
        compareWATType = getWATType(getItemType(leftTerm, itemScope, 1));
    } else {
        compareWATType = getWATType(getPrimaryType(leftTerm));
    }

    // Step 2: Determine the WAT operation type from the compareType variable
    char * compareOpWAT = getCompareWATType(compareType, compareWATType, logicType);

    // Step 3a: Print the starting statement in WAT
    sprintf(copyWAT, " (%s.%s", compareWATType, compareOpWAT);
    strcat(printFile, copyWAT);

    // Step 3b: Print out the WAT format for both the terms
    generateGetStatementWAT(printFile, leftTerm);
    generateGetStatementWAT(printFile, rightTerm);

    // Step 3c: Finish the comparison statement in WAT
    strcat(printFile, ")");
}

// Function module for handling starting while loops
void generateWhileStartWAT(char * printFile) {
    strcat(printFile, "\t\t(block (loop\n\t\t(br_if 1");
}

// Function module for handling exiting from While Loops
void generateWhileExitWAT(char * printFile) {
    strcat(printFile, "\t\t(br 0)\n\t\t))\n");
}

// Function module for starting if statements
void generateIfStartWAT(char * printFile, char * ifType) {
    if (strncmp(ifType, "elif", 4) == 0) {
        strcat(printFile, "\t\t(else (if");
    } else if (strncmp(ifType, "then", 4) == 0) {
        strcat(printFile, "\n\t\t(then");
    } else {
        sprintf(copyWAT, "\t\t(%s", ifType);
        strcat(printFile, copyWAT);
    }
}

// Function module for exiting if statements
void generateIfExitWAT(char * printFile, char * ifType) {
    if (strncmp(ifType, "then", 4) == 0) {
        strcat(printFile, "\t\t)");
    } else if (strncmp(ifType, "elif", 4) == 0) {
        strcat(printFile, "\t\t))");
    }
    else {
        strcat(printFile, "\t\t)\n");
    }
}

// Standard function to generate the main section and text section
// Required before generating any WAT statements
void generateText(char * IRcode) {
    // Create a copy of IRCode, called IRText so that memory doesn't get overwritten
    char * IRText = malloc(1000000 * sizeof(char));
    strncpy(IRText, IRcode, 1000000);

    // Get the first line, and delimit by newline characters, along with the line number
    char * line;
    line = strtok(IRText, "\n");
    int lineNumber = 1;

    // Current operation variable
    currOp = calloc(100, sizeof(char));

    // Global return type variable for a given function
    returnType = calloc(100, sizeof(char));

    // Finish indexes array
    finishIndexes = malloc(MAX_ARRAY_LENGTH * sizeof(int*));
    for (int i = 0; i < MAX_ARRAY_LENGTH; i++) { prevScopes[i] = 0; }

    // Print file variable
    char * printFile = MAINcode;

    // Current scope and previous scope variables
    currScope = calloc(1000, sizeof(char));
    strcpy(currScope, "global");
    prevScopes = malloc(MAX_ARRAY_LENGTH * sizeof(char*));
    for (int i = 0; i < MAX_ARRAY_LENGTH; i++) { prevScopes[i] = malloc(100 * sizeof(char)); }
    strcpy(prevScopes[0], "global");

    // Loop through each line in the code and generate WAT for each valid statement
    while (line != NULL) {
        // Ensure the line isn't beyond the max
        // Avoids buffer overflow attacks
        if (strlen(line) == MAX_LINE_LENGTH - 1 && line[MAX_LINE_LENGTH - 2] != '\n') 	  {
            printf("Semantic Error: Line %d is too long.\n", lineNumber);
            exit(0);
        }
        printf("Line: %s\n", line);
        lineNumber++;

        // Break apart the line into an iterable array of strings
        // Set the program delimiter for separating terms by word
        char ** strArr;
        strArr = malloc(MAX_ARRAY_LENGTH * sizeof(char *));
        for (int i = 0; i < MAX_ARRAY_LENGTH; i++) { strArr[i] = malloc(100 * sizeof(char)); }
        char progDelimiter[] = " ";
        char * lineCopy = malloc(MAX_LINE_LENGTH * sizeof(char));
        strcpy(lineCopy, line);
        char * token = strtok(lineCopy, progDelimiter);

        // Add all tokens to a string array
        int lenIndex = 0;
        while (token != NULL) {
            // Assign to string array
            strcpy(strArr[lenIndex], token);
            lenIndex++;
            token = strtok(NULL, progDelimiter);
        }

        // Remove "\n" character from the last string
        // lenIndex--;
        // strArr[lenIndex][strlen(strArr[lenIndex])-1] = '\0';

        // Get the current print file for the loop iteration
        printFile = inMain ? MAINcode : LOCALcode;

        // Case for ignoring file comments
        if (strncmp(strArr[0], "\n", 1) == 0 || strncmp(strArr[0], "#", 1) == 0) { }

        // Case for entering function declarations
        else if (strncmp(strArr[0], "entry", 5) == 0) {
            // Set all types after this to local declarations
            inMain = 0;
            inParams = 1;

            // Get WAT Type for Action
            char * newType = getWATType(strArr[1]);

            // Set global return type variable, which is specific to this function
            returnType = newType;

            // Set new scope and retain history using previous scope
            currScope = malloc(1000 * sizeof(char));
            strcpy(currScope, strArr[2]);
            strcpy(prevScopes[totalWebScopes], currScope);
            totalWebScopes++;

            // Start function declaration
            sprintf(copyWAT, "\t(func $%s ", strArr[2]);
            strcat(WATcode, copyWAT);
        }

        // Case for handling suboperation states
        else if (strncmp(strArr[0], "subop", 5) == 0) {
            // Switch param context
            checkParamContext();

            // Set current operation
            strcpy(currOp, strArr[1]);
        }

        // Case for end of strings
        else if (strncmp(strArr[0], "endstring", 9) == 0) {
            // Get the variable scopeType, scopeName, arrIndex and WATType
            char * scopeType = getScopeType(strArr[1]);
            char * arrScope = findVarScope(strArr[1], prevScopes, totalWebScopes);
            int arrIndex = atoi(strArr[2]) + get(&stringAddresses, strArr[1], arrScope);
            char * WATType = getWATType(getItemType(strArr[1], arrScope, 1));

            // Assign the NULL character to the last value
            setStringElementWAT(printFile, WATType, scopeType, strArr[1], arrIndex, 0);

            // Update string array size
            set(&stringSizes, strArr[1], arrScope, arrIndex);
        }

        // Case for addline statement
        else if (strncmp(strArr[0], "addline", 7) == 0) {
            // Call print line module
            generateAddLineWAT(printFile);
        }

        // Case for parameter declarations in functions
        else if (strncmp(strArr[0], "param", 5) == 0) {
            // Call parameter module
            generateParameterWAT(WATcode, strArr[1], strArr[2]);
        }

        // Case for exiting functions
        else if (strncmp(strArr[0], "exit", 4) == 0) { 
            inMain = 1; // Set flag back to global
            generateLocalOperations(); // Load in everything from the local file
            generateActionEndWAT(currScope); // Generate WAT function ending code
            
            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalWebScopes-2]);
            totalWebScopes--;
        }

        // Case for return statements with a return type in actions
        else if (strncmp(strArr[0], "return", 6) == 0) {
            // Switch param context
            checkParamContext();

            // Get variable scopeType
            char * scopeType = getScopeType(strArr[1]);
            
            // Print action ending code
            generateReturnWAT(strArr[1], scopeType);
        }

        // Case for void return statements in actions
        else if (strncmp(strArr[0], "voidreturn", 10) == 0) {
            // Switch param context
            checkParamContext();
            
            // Print void action ending code
            generateVoidReturnWAT();
        }

        // Case for finish keywords in logical statements
        else if (strncmp(strArr[0], "finish", 6) == 0) {
            // Print finish code
            generateFinishWAT(printFile);
        }

        // Case for output statements
        else if (strncmp(strArr[0], "output", 6) == 0) {
            // Switch param context
            checkParamContext();

            // Concatenate the rest of the tokens into one variable to get the whole string
            char * strVar = calloc(MAX_ARRAY_LENGTH, sizeof(char));
            for (int i = 1; i < lenIndex + 1; i++) {
                strcat(strVar, strArr[i]);
                if (i != lenIndex) {
                    strcat(strVar, " ");
                }
            }

            // Generate the output statement
            generateOutputStatementWAT(printFile, strVar);
        }

        // Case for type declarations
        else if (strncmp(strArr[0], "type", 4) == 0) {
            // Switch param context
            checkParamContext();

            // Generate Declare Statement
            generateVarDeclareStatementWAT(printFile, strArr[1], strArr[2]);

            // If the variable is an array, generate the $create_array statement
            if (strncmp(strArr[3], "array", 5) == 0) {
                generateArrayDeclareStatementWAT(printFile, strArr[1], strArr[2], strArr[4]);
            }
        }

        // Case for While Statements
        else if (strncmp(strArr[0], "while", 5) == 0) {
            // Generate starting statement
            generateWhileStartWAT(printFile);

            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000 * sizeof(char));
            snprintf(newScope, 1000, "while %s %d", currScope, getItemBlockNumber("while", findVarScope("while", prevScopes, totalWebScopes), 1));
            currScope = malloc(1000 * sizeof(char));
            strcpy(currScope, newScope);
            strcpy(prevScopes[totalWebScopes], currScope);
            totalWebScopes++;

            // Set finish index
            stackNumber++;
            finishIndexes[stackNumber]++;

            // Determine if uses a single statement
            if (lenIndex == 3) {
                generateComparisonWAT(printFile, strArr[1], strArr[2], strArr[3], "while");
            }

            // End While Condition Line
            strcat(printFile, ")\n");

            // Set scope to be in a logical statement
            inLogic = 1;
        }

        // Case for If Statements
        else if (strncmp(strArr[0], "if", 2) == 0) {
            generateIfStartWAT(printFile, "if");

            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000 * sizeof(char));
            snprintf(newScope, 1000, "if %s %d", currScope, getItemBlockNumber("if", findVarScope("if", prevScopes, totalWebScopes), 1));
            currScope = malloc(1000 * sizeof(char));
            strcpy(currScope, newScope);
            strcpy(prevScopes[totalWebScopes], currScope);
            totalWebScopes++;

            // Set finish index
            stackNumber++;
            finishIndexes[stackNumber]++;

            // Determine if uses a single statement
            if (lenIndex == 3) {
                generateComparisonWAT(printFile, strArr[1], strArr[2], strArr[3], "if");
            }

            // Generate Then Line
            generateIfStartWAT(printFile, "then");

            // Set scope to be in a logical statement
            inLogic = 1;
        }

        // Case for Elif Statements
        else if (strncmp(strArr[0], "elif", 4) == 0) {
            generateIfStartWAT(printFile, "elif");

            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "elif %s %d", currScope, getItemBlockNumber("elif", findVarScope("elif", prevScopes, totalWebScopes), 1));
            currScope = malloc(1000 * sizeof(char));
            strcpy(currScope, newScope);
            strcpy(prevScopes[totalWebScopes], currScope);
            totalWebScopes++;

            // Set finish index
            finishIndexes[stackNumber]++;

            // Determine if uses a single statement
            if (lenIndex == 3) {
                generateComparisonWAT(printFile, strArr[1], strArr[2], strArr[3], "elif");
            }

            // Generate Then Line
            generateIfStartWAT(printFile, "then");

            // Set scope to be in a logical statement
            inLogic = 1;
        }

        // Case for Else Statements
        else if (strncmp(strArr[0], "else", 4) == 0) {
            generateIfStartWAT(printFile, "else");

            // Set new scope and retain history using previous scope
            char * newScope = malloc(1000*sizeof(char));
            snprintf(newScope, 1000, "else %s %d", currScope, getItemBlockNumber("else", findVarScope("else", prevScopes, totalWebScopes), 1));
            currScope = malloc(1000 * sizeof(char));
            strcpy(currScope, newScope);
            strcpy(prevScopes[totalWebScopes], currScope);
            totalWebScopes++;

            // Set finish index
            finishIndexes[stackNumber]++;

            // Set scope to be in a logical statement
            inLogic = 1;
        }

        // Case for Ending While Statements
        else if (strncmp(strArr[0], "endwhile", 8) == 0) {
            generateWhileExitWAT(printFile);

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalWebScopes-2]);
            totalWebScopes--;

            // Reduce stack number
            finishIndexes[stackNumber] = 0;
            stackNumber--;

            // If the previous scope is not another logic statement, change logical scope boolean
            if (strncmp(currScope, "while", 5) != 0 && strncmp(currScope, "if", 2) != 0 && strncmp(currScope, "elif", 4) != 0 && strncmp(currScope, "else", 4) != 0) {
                inLogic = 0;
            }
        }

        // Case for Ending If Statements
        else if (strncmp(strArr[0], "endif", 5) == 0) {
            generateIfExitWAT(printFile, "then");

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalWebScopes-2]);
            totalWebScopes--;

            // If the previous scope is not another logic statement, change logical scope boolean
            if (strncmp(currScope, "while", 5) != 0 && strncmp(currScope, "if", 2) != 0 && strncmp(currScope, "elif", 4) != 0 && strncmp(currScope, "else", 4) != 0) {
                inLogic = 0;
            }
        }

        // Case for Ending Elif Statements
        else if (strncmp(strArr[0], "endelif", 7) == 0) {
            generateIfExitWAT(printFile, "then");

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalWebScopes-2]);
            totalWebScopes--;

            // If the previous scope is not another logic statement, change logical scope boolean
            if (strncmp(currScope, "while", 5) != 0 && strncmp(currScope, "if", 2) != 0 && strncmp(currScope, "elif", 4) != 0 && strncmp(currScope, "else", 4) != 0) {
                inLogic = 0;
            }
        }

        // Case for Ending Else Statements
        else if (strncmp(strArr[0], "endelse", 7) == 0) {
            // Print all elif endings and reset iterator variable
            for (int i = 0; i < finishIndexes[stackNumber] - 1; i++) {
                if (i == 0) {
                    generateIfExitWAT(printFile, "then"); 
                } else {
                    generateIfExitWAT(printFile, "elif"); 
                }
            }

            // Set current scope as previous scope
            strcpy(currScope, prevScopes[totalWebScopes-2]);
            totalWebScopes--;

            // If the previous scope is not another logic statement, change logical scope boolean
            if (strncmp(currScope, "while", 5) != 0 && strncmp(currScope, "if", 2) != 0 && strncmp(currScope, "elif", 4) != 0 && strncmp(currScope, "else", 4) != 0) {
                inLogic = 0;
            }
        }

        // Case for Ending Flow Statements
        else if (strncmp(strArr[0], "endlogic", 8) == 0) {
            generateIfExitWAT(printFile, "logic");

            // Reduce stack number
            finishIndexes[stackNumber] = 0;
            stackNumber--;
        }

        // Case for Void Action Calls
        else if (strncmp(strArr[0], "call", 4) == 0) {
            // Switch param context
            checkParamContext();

            // Print starting call line
            sprintf(copyWAT, "\t\t(call $%s)", strArr[1]);
            strcat(printFile, copyWAT);

            // If there are any arguments after the "args" token, generate the call with the list of parameters
            if (strArr[3] != NULL && strncmp(strArr[3], "\0", 1) != 0) {
                // Loop through and add all available parameters as lines under the function call
                // Determine if each one is a variable or not, and assign accordingly
                int index = 3;
                while (strArr[index] != NULL && strncmp(strArr[index], "\0", 1) != 0) {
                    generateGetStatementWAT(printFile, strArr[index]);
                    index++;
                }

                // End the call statement with parameters
                strcat(printFile, ")\n");
            }
        }

        // Case for Assignment Statements
        else if (strArr[3] == NULL || strncmp(strArr[3], "\0", 1) == 0 || strncmp(strArr[3], "\"", 1) == 0) {
            // Switch param context
            checkParamContext();

            // Generate the assignment statement
            generateAssignmentWAT(printFile, strArr[0], strArr[2]);
        }

        // Case for Standard Action Calls
        else if (strncmp(strArr[2], "call", 4) == 0) {
            // Switch param context
            checkParamContext();

            // Generate the starting action call
            generateActionCallStartWAT(printFile, strArr[0], strArr[3]);

            // If there are any arguments after the "args" token, generate the call with the list of parameters
            if (strArr[5] != NULL && strncmp(strArr[5], "\0", 1) != 0) {
                // Loop through and add all available parameters as lines under the function call
                // Determine if each one is a variable or not, and assign accordingly
                int index = 5;
                while (strArr[index] != NULL && strncmp(strArr[index], "\0", 1) != 0) {
                    generateGetStatementWAT(printFile, strArr[index]);
                    index++;
                }
                // End the call statement with parameters
                strcat(printFile, ")");
            } else {
                // End the call statement with parameters
                strcat(printFile, ")");
            }
            // End the assignment statement
            strcat(printFile, ")\n");
        }

        // Case for Basic Operations
        else if (strncmp(strArr[3], "+", 1) == 0 || strncmp(strArr[3], "-", 1) == 0 || strncmp(strArr[3], "*", 1) == 0 || strncmp(strArr[3], "/", 1) == 0) {
            generateOperationWAT(printFile, strArr[0], strArr[2], strArr[3], strArr[4]);
        }

        // Move to next line
        line = strtok(NULL, "\n");
    }
}

void generateLocalOperations() {
    strncat(WATcode, LOCALcode, 1000000);
    strcat(WATcode, "\t\t\n");

    // Clear LOCALcode
    strcpy(LOCALcode, "");
}

void generateMainVars() {
    strncat(WATcode, VARScode, 1000000);
    strcat(WATcode, "\n");
}

void generateMain() {
    // Generate the starting code for the main function
    strcat(WATcode, "\t;; Start Main Function\n");
    strcat(WATcode, "\t(func $main\n");

    // Generate main function vars
    generateMainVars();

    // Read all WAT code that was inserted into the main file
    strncat(WATcode, MAINcode, 1000000);

    // End the main function
    strcat(WATcode, "\t)\n");
    strcat(WATcode, "\t(start $main)\n");
}

// Function to end WAT generation once all commands are printed from the IRcodeOptimized
void completeFile() {
    // Print ending WAT commands to the file    
    strcat(WATcode, ")\n");
}


// Main driver function to generate all WAT code
char * generateWATcode(char * IRcode) {
    // Initialize String Hash Tables
    init(&stringAddresses);
    init(&stringSizes);

    // Generate the WATcode
    initAssemblyFile();
    generateStartModule();
    generateText(IRcode);
    generateMain();
    completeFile();

    // Return final WATcode
    return WATcode;
}