// Set of functions to emit WAT code
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "webgen.h"
#include "semantic.h"
#include "symbolTable.h"

// Initialize WATcode file, IRcode file, MAINcode file, and other required variables
FILE * WATcode;
FILE * IRcode;
FILE * MAINcode;
FILE * VARScode;
FILE * LOCALcode;
char code[10000];

// String hash table functions
// Used for updating array index calls by dedicated memory index
#define TABLE_SIZE 256
#define MAX_KEY_LENGTH 32
int currMaxStringIndex = 0;

struct StringEntry {
    char key[MAX_KEY_LENGTH];
    int value;
};

struct StringHashTable {
    struct StringEntry table[TABLE_SIZE];
};

unsigned int hash(char* key) {
    unsigned int value = 0;
    for (int i = 0; i < strlen(key); i++) {
        value = value * 31 + key[i];
    }
    return value % TABLE_SIZE;
}

void init(struct StringHashTable* dict) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        dict->table[i].key[0] = '\0';
        dict->table[i].value = 0;
    }
}

void set(struct StringHashTable* dict, char* key, int value) {
    unsigned int index = hash(key);
    strcpy(dict->table[index].key, key);
    dict->table[index].value = value;
}

int get(struct StringHashTable* dict, char* key) {
    unsigned int index = hash(key);
    if (strcmp(dict->table[index].key, key) == 0) {
        return dict->table[index].value;
    }
    return -1;
}

// Create String Hash Tables
struct StringHashTable stringAddresses;
struct StringHashTable stringSizes;

// Function to open the files for IRcodeOptimized.ir and WATcode.asm
// Required before generating any WAT code
void initAssemblyFile() {
    printf("\nOpening WAT Code file\n\n"); // Creates a WAT file with a generic header that needs to be in every file
    IRcode = fopen("IRcodeOptimized.ir", "r");
    WATcode = fopen("WATcode.wat", "w");
    MAINcode = fopen("MAINcode.wat", "w");
    VARScode = fopen("VARScode.wat", "w");
    LOCALcode = fopen("LOCALcode.wat", "w");
}

// Function to generate the initial set of lines required for proper WebAssembly output
void generateModule() {
    fprintf(WATcode, "(module\n");
    fprintf(WATcode, "\t;; WAT Setup Declarations\n");
    fprintf(WATcode, "\t(import \"env\" \"jsprint\" (func $jsprint (param i32)))\n");
    fprintf(WATcode, "\t(import \"env\" \"newline\" (func $newline))\n");
    fprintf(WATcode, "\t(import \"env\" \"writeconsoleInt\" (func $writeconsoleInt (param i32)))\n");
    fprintf(WATcode, "\t(import \"env\" \"writeconsoleFloat\" (func $writeconsoleFloat (param f32)))\n");
	fprintf(WATcode, "\t(import \"env\" \"writeconsoleString\" (func $writeconsoleString (param i32)))\n");
    fprintf(WATcode, "\t(memory $0 100)\n");
    fprintf(WATcode, "\t(export \"pagememory\" (memory $0))\n");
    fprintf(WATcode, "\t(func $create_array (param $size i32) (result i32)\n");
    fprintf(WATcode, "\t\t(local $ptr i32)\n");
    fprintf(WATcode, "\t\t(set_local $ptr (i32.const 0))\n");
    fprintf(WATcode, "\t\t(block\n");
    fprintf(WATcode, "\t\t\t(loop $loop\n");
    fprintf(WATcode, "\t\t\t\t(br_if $loop (i32.eq (get_local $size) (i32.const 0)))\n");
    fprintf(WATcode, "\t\t\t\t(set_local $ptr (i32.add (get_local $ptr) (i32.const 4)))\n");
    fprintf(WATcode, "\t\t\t\t(set_local $size (i32.sub (get_local $size) (i32.const 1)))\n");
    fprintf(WATcode, "\t\t\t)\n");
    fprintf(WATcode, "\t\t)\n");
    fprintf(WATcode, "\t\t(get_local $ptr)\n");
    fprintf(WATcode, "\t)\n");
    fprintf(WATcode, "\t(export \"create_array\" (func $get_element))\n");
    fprintf(WATcode, "\t(func $get_element (param $ptr i32) (param $index i32) (result i32)\n");
    fprintf(WATcode, "\t\t(i32.load (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))))\n");
    fprintf(WATcode, "\t)\n");
    fprintf(WATcode, "\t(export \"get_element\" (func $get_element))\n");
    fprintf(WATcode, "\t(func $set_element (param $ptr i32) (param $index i32) (param $value i32)\n");
    fprintf(WATcode, "\t\t(i32.store (i32.add (get_local $ptr) (i32.mul (get_local $index) (i32.const 4))) (get_local $value))\n");
    fprintf(WATcode, "\t)\n");
    fprintf(WATcode, "\t(export \"set_element\" (func $set_element))\n\n");
    fprintf(WATcode, "\t;; Artsy Program in WAT\n");
}

// Helper function that maps IRcode to the corresponding WebAssembly type
char * getWATType(char * phrase) {
    char * watType = "";

    if (strncmp(phrase, "float", 5) == 0) {
        watType = "f32";
        return watType;
    } else if (strncmp(phrase, "int", 3) == 0 || strncmp(phrase, "string", 6) == 0) {
        watType = "i32";
        return watType;
    } else {
        // Uses a void or undefined keyword
        watType = "void";
        return watType;
    }
}

// Helper function that returns the number of characters to move after finding the corresponding WebAssembly type
int getMoveAmount(char * phrase) {
    if (strncmp(phrase, "float", 5) == 0) {
        return 6;
    } else if (strncmp(phrase, "int", 3) == 0) {
        return 4;
    } else if (strncmp(phrase, "string", 6) == 0) {
        return 7;
    }
}

// Helper function to convert string phrases to an ASCII character 
char * convertToASCII(char * phrase) {
    char * asciiVal;
    snprintf(asciiVal, 100, "%d", (int)phrase[1]);
    return asciiVal;
}

// Standard function to generate the main section and text section
// Required before generating any WAT statements
void generateText() {
    // Variables to detect if declared types and params are within a function
    int isGlobal = 1;
    int inParams = 0;

    // Current operation variable
    char * currOp= "";

    // Global return type variable for a given function
    char * returnType = malloc(100*sizeof(char));

    // Current scope variable
    char * currScope = "global";

    // Loop through each line in the code and generate WAT for each valid statement
    while (fgets(code, 10000, IRcode) != NULL) {
        // Case for type declarations
        printf("%s", code);
        // Case for parameter declarations in functions
        if (strncmp(code, "param ", 6) == 0) {
            char * nextPart = code + 6;
            nextPart[strlen(nextPart) - 1] = 0;

            // Get the rest of the string
            char * newType = getWATType(nextPart);
            int moveAmount = getMoveAmount(nextPart);

            char * variable = nextPart + moveAmount;

            fprintf(WATcode, "(param $%s %s) ", variable, newType);
        }
        // Case for exiting functions
        else if (strncmp(code, "exit", 4) == 0) {
            // Set all types after this to global declarations
            isGlobal = 1;

            // Load in everything from the local file
            generateLocalOperations();

            // End function declaration
            fprintf(WATcode, "\t)\n");
            fprintf(WATcode, "\t(export \"%s\" (func $%s))\n", currScope, currScope);

            // Set scope back to global
            currScope = "global";
        }
        // Case for return statements in functions
        else if (strncmp(code, "return ", 7) == 0) {
            // Set function return type first if this is the first call in a function
            if (inParams) {
                inParams = 0;
                if (strncmp(returnType, "void", 4) != 0) {
                    fprintf(WATcode, "(result %s)\n", returnType);
                } else {
                    fprintf(WATcode, "\n");
                }
            }

            // Get variable name
            char * variable = code + 7;
            variable[strlen(variable) - 1] = 0;

            // Get array token for array index callbacks
            char * token = malloc(100*sizeof(char));
            char * index = malloc(100*sizeof(char));
            char delimiter[] = "[], ";
            strcpy(token, variable);
            token = strtok(token, delimiter);

            // Determine if the variable is global or variable
            char * scopeType = "global";
            char ** scopeStack = { "global" , currScope };
            int scopePointer = 0;

            if (!found(token, scopeStack, scopePointer)) {
                scopePointer = 1;
                scopeType = "local";
            }
            // Print function ending code
            
            // If it's not a variable, return the value as it's WAT type
            if (strncmp(getPrimaryType(variable), "var", 3) != 0) {
                // Get WAT Type for printing to the console
                char * WATType = getWATType(getPrimaryType(variable));
                fprintf(LOCALcode, "\t\t(return (%s.const %s))", WATType, variable);
            } else { // Else, it is a variable or array index
                // If the variable is an array index, call a get_element statement
                int len = strlen(variable);
                if (variable[len - 1] == ']') {
                    // Get the array variable and the index number
                    char * arrayName = malloc(100*sizeof(char));
                    char * arrIndex = malloc(100*sizeof(char));

                    // Assign variable name
                    strcpy(arrayName, token);

                    // Assign array index variable
                    token = strtok(NULL, delimiter);
                    sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(token));

                    // Print array index return code
                    fprintf(LOCALcode, "\t\t(return\n");
                    fprintf(LOCALcode, "\t\t\t(call $get_element\n");
                    fprintf(LOCALcode, "\t\t\t\t(%s.get $%s)\n", scopeType, arrayName);
                    fprintf(LOCALcode, "\t\t\t\t(i32.const %s)\n", arrIndex);
                    fprintf(LOCALcode, "\t\t\t)\n");
                    fprintf(LOCALcode, "\t\t)\n");
                } else {
                    // Print standard variable return code
                    fprintf(LOCALcode, "\t\t(return (local.get $%s))", variable); 
                }     
            }
        }
        // If the code contains a writeln statement, print a new line
        else if (strncmp(code, "addline", 7) == 0) {
            // Call print line function
            if (isGlobal) {
                fprintf(MAINcode, "\t\t(call $newline)\n");
            } else {
                fprintf(LOCALcode, "\t\t(call $newline)\n");
            }
        }
        // Case for handling suboperation states
        else if (strncmp(code, "subop ", 6) == 0) {
            // Set function return type first if this is the first call in a function
            if (inParams) {
                inParams = 0;
                if (strncmp(returnType, "void", 4) != 0) {
                    fprintf(WATcode, "(result %s)\n", returnType);
                } else {
                    fprintf(WATcode, "\n");
                }
            }

            // Get variable name
            char * variable = code + 6;
            variable[strlen(variable) - 1] = 0;

            // Set current operation
            currOp = malloc(strlen(variable)*sizeof(char));
            strcpy(currOp, variable);
        }
        // If the file contains a newline or a comment, ignore the statement
        else if (strncmp(code, "\n", 1) == 0 || strncmp(code, "#", 1) == 0){
        }
        // If the IRcode declares an assignment, operation, or call statement
        else {
            // Algorithm
            // Step 1: Break apart the line into an iterable array of strings
            // Set the delimiter for separating terms by word
            char ** strArr;
            int maxArrSize = 1000;
            strArr = malloc(maxArrSize * sizeof(char *));
            char delimiter[] = " ";
            char * token = strtok(code, delimiter);

            // Free specific string index for loop reuse
            strArr[3] = "\0";
            strArr[7] = "\0";

            // Add all tokens to a string array
            int lenIndex = 0;
            while(token != NULL) {
                // Assign to string array
                strArr[lenIndex] = token;
                lenIndex++;
                token = strtok(NULL, delimiter);
            }

            // Remove "\n" character from the last string
            lenIndex--;
            strArr[lenIndex][strlen(strArr[lenIndex])-1] = '\0';

            // Case for entering function declarations
            if (strncmp(strArr[0], "entry", 5) == 0) {
                // Set all types after this to local declarations
                isGlobal = 0;
                inParams = 1;

                char * nextPart = strArr[1];
                char * variable = strArr[2];

                // Get the rest of the string
                char * newType = getWATType(nextPart);

                // Set global return type variable, which is specific to this function
                returnType = newType;

                // Set new scope
                currScope = malloc(strlen(variable)*sizeof(char));
                strcpy(currScope, variable);

                // Start function declaration
                fprintf(WATcode, "\t(func $%s ", variable);
            }

            // Case for end of strings
            else if (strncmp(strArr[0], "endstring", 9) == 0) {
                // Determine if the variable is global or variable
                char * scopeType = "global";
                char ** scopeStack = { "global", currScope };
                int scopePointer = 0;

                if (!found(strArr[1], scopeStack, scopePointer)) {
                    scopePointer = 1;
                    scopeType = "local";
                }

                char * arrayName = strArr[1];

                // Assign NULL characters to all other values ahead of the call index
                for (int arrIndex = atoi(strArr[2]) + get(&stringAddresses, arrayName); arrIndex < get(&stringAddresses, arrayName) + get(&stringSizes, arrayName); arrIndex++) {
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t(call $set_element (%s.get $%s) (i32.const %d) (i32.const 0))\n", scopeType, arrayName, arrIndex);
                    } else {
                        fprintf(LOCALcode, "\t\t(call $set_element (%s.get $%s) (i32.const %d) (i32.const 0))\n", scopeType, arrayName, arrIndex);
                    }
                }
            }

            // Case for output statements
            else if (strncmp(strArr[0], "output", 6) == 0) {
                // Set function return type first if this is the first call in a function
                if (inParams) {
                    inParams = 0;
                    if (strncmp(returnType, "void", 4) != 0) {
                        fprintf(WATcode, "(result %s)\n", returnType);
                    } else {
                        fprintf(WATcode, "\n");
                    }
                }

                char * variable = strArr[1];

                // If the write statement does not use a variable
                if (strncmp(getPrimaryType(variable), "var", 3) != 0) {
                    if (!isGlobal) { // If its within a function, write to WATcode
                        if (strncmp(variable, "int", 3) == 0) {
                            fprintf(LOCALcode, "\t\t(call $writeconsoleInt\n");
                        } else if (strncmp(variable, "float", 3) == 0) {
                            fprintf(LOCALcode, "\t\t(call $writeconsoleFloat\n");
                        } else if (strncmp(variable, "string", 3) == 0) {
                            fprintf(LOCALcode, "\t\t(call $writeconsoleString\n");
                        }
                        fprintf(LOCALcode, "\t\t\t(%s.const %s)\n", getWATType(variable), variable);
                        fprintf(LOCALcode, "\t\t)\n");
                    }
                    // Else, write to the MAINcode file
                    else {
                        if (strncmp(variable, "int", 3) == 0) {
                            fprintf(MAINcode, "\t\t(call $writeconsoleInt\n");
                        } else if (strncmp(variable, "float", 3) == 0) {
                            fprintf(MAINcode, "\t\t(call $writeconsoleFloat\n");
                        } else if (strncmp(variable, "string", 3) == 0) {
                            fprintf(MAINcode, "\t\t(call $writeconsoleString\n");
                        }
                        fprintf(MAINcode, "\t\t\t(%s.const %s)\n", getWATType(variable), variable);
                        fprintf(MAINcode, "\t\t)\n");
                    }

                } else { // Else, the variable uses a variable
                    // Determine if the variable is global or variable
                    char * scopeType = "global";
                    char ** scopeStack = { "global", currScope };
                    int scopePointer = 0;

                    if (!found(variable, scopeStack, scopePointer)) {
                        scopePointer = 1;
                        scopeType = "local";
                    }

                    // Get primary type of the output
                    char * token;
                    char delimiter[] = "[], ";
                    token = strtok(variable, delimiter);

                    // If scopeType is global, use "global"
                    // Else, use currScope
                    char * primaryType;
                    char * itemKind;
                    if (strncmp(scopeType, "global", 6) == 0) {
                        primaryType = getItemType(token, "global", scopePointer);
                        itemKind = getItemKind(token, "global", scopePointer);
                    } else {
                        primaryType = getItemType(token, currScope, scopePointer);
                        itemKind = getItemKind(token, currScope, scopePointer);
                    }

                    // Declare a write string variable
                    char * writeStr = malloc(200*sizeof(char));

                    // Start call statement with primary type
                    if (strncmp(primaryType, "int", 3) == 0) {
                        strcpy(writeStr, "\t\t(call $writeconsoleInt");
                    } else if (strncmp(primaryType, "float", 3) == 0) {
                        strcpy(writeStr, "\t\t(call $writeconsoleFloat");
                    } else if (strncmp(primaryType, "string", 6) == 0) {
                        strcpy(writeStr, "\t\t(call $writeconsoleString");
                    }

                    if (!isGlobal) { // If its within a function, write to WATcode
                        // If the type is an array index, get the array index val and write to the console
                        int len = strlen(variable);
                        if (variable[len - 1] == ']' && strncmp(itemKind, "Array", 5) == 0) {
                            // Get the array variable and the index number
                            char * arrayName = malloc(100*sizeof(char));
                            char * arrIndex = malloc(100*sizeof(char));

                            // Assign variable name
                            strcpy(arrayName, token);

                            // Assign array index variable
                            token = strtok(NULL, delimiter);
                            sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(token));

                            // Print get_element call
                            fprintf(LOCALcode, "%s (call $get_element (%s.get $%s) (i32.const %s)))\n", writeStr, scopeType, arrayName, arrIndex);

                        } else if (strncmp(itemKind, "Array", 5) == 0 || strncmp(primaryType, "string", 6) == 0) {
                            // Else if the type is a full array, end the current statement and write out all available indexes

                            // Get the array variable and the index number
                            int arrIndex = 0;

                            // Assign array index variable
                            arrIndex = get(&stringAddresses, variable);
                            
                            // Print out all available indexes using a for-loop
                            for (int newIndex = 0; newIndex < get(&stringSizes, variable); newIndex++) {
                                fprintf(LOCALcode, "%s (call $get_element (%s.get $%s) (i32.const %d)))\n", writeStr, scopeType, variable, arrIndex + newIndex);
                            }
                        } else {
                            // Else, print out the standard variable call
                            fprintf(LOCALcode, "%s (%s.get $%s))\n", writeStr, scopeType, variable);
                        }
                    }
                    // Else, write to the MAINcode file
                    else {
                        // If the type is an array index, get the array index val and write to the console
                        int len = strlen(variable);
                        if (variable[len - 1] == ']' && strncmp(itemKind, "Array", 5) == 0) {
                            // Get the array variable and the index number
                            char * arrayName = malloc(100*sizeof(char));
                            char * arrIndex = malloc(100*sizeof(char));

                            // Assign variable name
                            strcpy(arrayName, token);

                            // Assign array index variable
                            token = strtok(NULL, delimiter);
                            sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(token));

                            fprintf(MAINcode, "%s (call $get_element (%s.get $%s) (i32.const %s)))\n", writeStr, scopeType, arrayName, arrIndex);
                        } else if (strncmp(itemKind, "Array", 5) == 0 || strncmp(primaryType, "string", 6) == 0) {
                            // Else if the type is a full array, end the current statement and write out all available indexes

                            // Get the array variable and the index number
                            int arrIndex = 0;

                            // Assign array index variable
                            arrIndex = get(&stringAddresses, variable);
                            
                            // Print out all available indexes using a for-loop
                            for (int newIndex = 0; newIndex < get(&stringSizes, variable); newIndex++) {
                                fprintf(MAINcode, "%s (call $get_element (%s.get $%s) (i32.const %d)))\n", writeStr, scopeType, variable, arrIndex + newIndex);
                            }
                        } else {
                            // Else, print out the standard variable call
                            fprintf(MAINcode, "%s (%s.get $%s))\n", writeStr, scopeType, variable);
                        }
                    }
                }
            }

            // Case for type declarations
            else if (strncmp(strArr[0], "type", 4) == 0) {
                // Set function return type first if this is the first call in a function
                if (inParams) {
                    inParams = 0;
                    if (strncmp(returnType, "void", 4) != 0) {
                        fprintf(WATcode, "(result %s)\n", returnType);
                    } else {
                        fprintf(WATcode, "\n");
                    }
                }

                // Get the type and variable
                char * newType = getWATType(strArr[1]);
                char * variable = strArr[2];

                // Print declaration to WATcode
                if (isGlobal) {
                    char * placeholder = "0";
                    if (strncmp(newType, "f32", 3) == 0) {
                        placeholder = "0.0";
                    }

                    // Output the type to the global section
                    fprintf(WATcode, "\t(global $%s (mut %s) (%s.const %s))\n", variable, newType, newType, placeholder);
                } else {
                    // If the code is in the $main function
                    if (strncmp(currScope, "global", 6) == 0) {
                        fprintf(VARScode, "\t\t(local $%s %s)\n", variable, newType);
                    } else {
                        // Else, its within a function scope
                        fprintf(WATcode, "\t\t(local $%s %s)\n", variable, newType);
                    }
                }
                
                // If the type is an array, instantiate the array with the corresponding type
                if (strncmp(strArr[3], "array", 5) == 0 && strncmp(currScope, "global", 6) == 0) {
                    // Create string variable in hash table
                    int indexEntry = currMaxStringIndex;
                    set(&stringAddresses, variable, indexEntry);
                    set(&stringSizes, variable, atoi(strArr[4]));

                    // Update max string index for the next array entry
                    currMaxStringIndex += atoi(strArr[4]);

                    // Generate WAT code for creating the new array
                    fprintf(MAINcode, "\t\t(global.set $%s\n", variable);
                    fprintf(MAINcode, "\t\t\t(call $create_array\n");
                    fprintf(MAINcode, "\t\t\t\t(i32.const %s)\n", strArr[4]);
                    fprintf(MAINcode, "\t\t\t)\n");
                    fprintf(MAINcode, "\t\t)\n");

                } else if (strncmp(strArr[3], "array", 5) == 0) {
                    // Create string variable in hash table
                    int indexEntry = currMaxStringIndex;
                    set(&stringAddresses, variable, indexEntry);
                    set(&stringSizes, variable, atoi(strArr[4]));

                    // Update max string index for the next array entry
                    currMaxStringIndex += atoi(strArr[4]);

                    // Generate WAT code for creating the new array
                    fprintf(LOCALcode, "\t\t(local.set $%s\n", variable);
                    fprintf(LOCALcode, "\t\t\t(call $create_array\n");
                    fprintf(LOCALcode, "\t\t\t\t(i32.const %s)\n", strArr[4]);
                    fprintf(LOCALcode, "\t\t\t)\n");
                    fprintf(LOCALcode, "\t\t)\n");
                }
            }

            // Case for Void Function Calls
            else if (strncmp(strArr[0], "call", 4) == 0) {
                // Declare all three variables
                char * funcVar = strArr[1];

                // Set function return type first if this is the first call in a function
                if (inParams) {
                    inParams = 0;
                    if (strncmp(returnType, "void", 4) != 0) {
                        fprintf(WATcode, "(result %s)\n", returnType);
                    } else {
                        fprintf(WATcode, "\n");
                    }
                }

                // Output function call lines
                // If there are no arguments after the "args" token, generate the call on a single line
                if (strArr[3] == NULL || strncmp(strArr[3], "", 1) == 0) {
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t(call $%s)\n", funcVar);
                    } else {
                        fprintf(LOCALcode, "\t\t(call $%s)\n", funcVar);
                    }
                }
                // Otherwise, generate the call with the list of parameters
                else {
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t(call $%s\n", funcVar);
                    } else {
                        fprintf(LOCALcode, "\t\t(call $%s\n", funcVar);
                    }

                    // Loop through and add all available parameters as lines under the function call
                    // Determine if each one is a variable or not, and assign accordingly
                    int index = 3;
                    while (strArr[index] != NULL) {
                        // Double check and break if it's the end of sequence
                        if (strncmp(strArr[index], "", 1) == 0) {
                            break;
                        }

                        char * callVar = strArr[index];
                        
                        // If var references an actual variable, add a dollar sign in front and build the line accordingly
                        if (strncmp(getPrimaryType(strArr[index]), "var", 3) == 0) {
                            // Determine if the variable is global or variable
                            char * varScopeType = "global";
                            char ** scopeStack = { "global", currScope };
                            int scopePointer = 0;

                            if (!found(strArr[index], scopeStack, scopePointer)) {
                                scopePointer = 1;
                                varScopeType = "local";
                            }

                            if (isGlobal) {
                                fprintf(MAINcode, "\t\t\t(%s.get $%s)\n", varScopeType, callVar);
                            } else {
                                fprintf(LOCALcode, "\t\t\t(%s.get $%s)\n", varScopeType, callVar);
                            }
                            
                        } else {
                            char * opType = getWATType(getPrimaryType(strArr[index]));

                            if (isGlobal) {
                                fprintf(MAINcode, "\t\t\t(%s.const %s)\n", opType, callVar);
                            } else {
                                fprintf(LOCALcode, "\t\t\t(%s.const %s)\n", opType, callVar);
                            }
                        }
                        index++;
                    }

                    // End the call statement with parameters
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t)\n");
                    } else {
                        fprintf(LOCALcode, "\t\t\t)\n");
                    }
                }
            }

            // Assignment Statements
            // Total Strings = 3
            // STR1 = Var, STR2 = "=", STR3 = primary/variable

            // Assignment Operation
            else if (strArr[3] == NULL || strncmp(strArr[3], "", 1) == 0 || strncmp(strArr[3], "\"", 1) == 0) {
                // Set function return type first if this is the first call in a function
                if (inParams) {
                    inParams = 0;
                    if (strncmp(returnType, "void", 4) != 0) {
                        fprintf(WATcode, "(result %s)\n", returnType);
                    } else {
                        fprintf(WATcode, "\n");
                    }
                }

                // Declare all three variables
                char * assignVar = strArr[0];
                char * var2 = strArr[2];
                
                // Declare operation type variable
                char * opType;

                // Determine if the variable is global or variable
                char * scopeType = "global";
                char ** scopeStack = { "global", currScope };
                int scopePointer = 0;

                if (!found(strArr[0], scopeStack, scopePointer)) {
                    scopePointer = 1;
                    scopeType = "local";
                }

                // If a current operation has not been assigned, get the current operation from the assignment statement
                if (strncmp(currOp, "", 1) == 0 || currOp == NULL) {
                    currOp = getPrimaryType(var2);
                }

                // Determine the operation type
                opType = getWATType(currOp);

                // Output the assignVar call line
                // If the assigned variable is an array index
                int lenArr0 = strlen(assignVar);
                if (assignVar[lenArr0 - 1] == ']' && isGlobal) {
                    // Declare a size variable
                    char * size = strtok(strArr[0], "[], ");

                    // Redetermine if the scope is global or local
                    scopePointer = 0;
                    if (!found(size, scopeStack, scopePointer)) {
                        scopePointer = 1;
                        scopeType = "local";
                    } else {
                        scopeType = "global";
                    }

                    // Get the size number from the assignVar
                    while (isInt(size) != 1) {
                        size = strtok(NULL, "[], ");
                    }

                    // Declare a new array index variable
                    char * arrIndex = malloc(50*sizeof(char));
                    sprintf(arrIndex, "%d", get(&stringAddresses, assignVar) + atoi(size));

                    // Start the declaration statement
                    fprintf(MAINcode, "\t\t(call $set_element\n");
                    fprintf(MAINcode, "\t\t\t(%s.get $%s)\n", scopeType, assignVar);
                    fprintf(MAINcode, "\t\t\t(i32.const %s)\n", arrIndex);
                } else if (assignVar[lenArr0 - 1] == ']') {
                    // Declare a size variable
                    char * size = strtok(strArr[0], "[], ");

                    // Redetermine if the scope is global or local
                    scopePointer = 0;
                    if (!found(size, scopeStack, scopePointer)) {
                        scopePointer = 1;
                        scopeType = "local";
                    } else {
                        scopeType = "global";
                    }

                    // Get the size number from the assignVar
                    while (isInt(size) != 1) {
                        size = strtok(NULL, "[], ");
                    }

                    // Declare a new array index variable
                    char * arrIndex = malloc(50*sizeof(char));
                    sprintf(arrIndex, "%d", get(&stringAddresses, assignVar) + atoi(size));

                    // Start the declaration statement
                    fprintf(LOCALcode, "\t\t(call $set_element\n");
                    fprintf(LOCALcode, "\t\t\t(%s.get $%s)\n", scopeType, assignVar);
                    fprintf(LOCALcode, "\t\t\t(i32.const %s)\n", arrIndex);

                } else if (isGlobal) { // If it's a temp variable in global, print to MAINcode
                    fprintf(MAINcode, "\t\t(%s.set $%s\n", scopeType, assignVar);
                }
                // Else, print to WATcode
                else {
                    fprintf(LOCALcode, "\t\t(%s.set $%s\n", scopeType, assignVar);
                }

                char * tokenVar2 = malloc(100*sizeof(char));
                char delimiter[] = "[], ";
                strcpy(tokenVar2, var2);
                tokenVar2 = strtok(tokenVar2, delimiter);
                int lenArr2 = strlen(var2);

                // If var2 references an actual variable, add a dollar sign in front and build the line accordingly
                if (strncmp(getPrimaryType(tokenVar2), "var", 3) == 0) {
                    // Determine if the variable is global or variable
                    char * varScopeType = "global";
                    scopePointer = 0;

                    if (!found(tokenVar2, scopeStack, scopePointer)) {
                        scopePointer = 1;
                        varScopeType = "local";
                    }

                    // If var is an array index callout, generate a call to the appropriate index
                    if (var2[lenArr2 - 1] == ']' && isGlobal) {
                        // If it's array call in global, print to MAIN code
                        // Get the array variable and the index number
                        char * arrayName = malloc(100*sizeof(char));
                        char * arrIndex = malloc(100*sizeof(char));

                        // Assign variable name
                        strcpy(arrayName, tokenVar2);

                        // Assign array index variable
                        tokenVar2 = strtok(NULL, delimiter);
                        sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(tokenVar2));

                        // Print array index code
                        fprintf(MAINcode, "\t\t\t(call $get_element\n");
                        fprintf(MAINcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, arrayName);
                        fprintf(MAINcode, "\t\t\t\t(i32.const %s)\n", arrIndex);
                        fprintf(MAINcode, "\t\t\t)\n");
                        
                    } else if (var2[lenArr2 - 1] == ']') {
                        // If it's array call in a function, print to LOCAL code
                        // Get the array variable and the index number
                        char * arrayName = malloc(100*sizeof(char));
                        char * arrIndex = malloc(100*sizeof(char));

                        // Assign variable name
                        strcpy(arrayName, tokenVar2);

                        // Assign array index variable
                        tokenVar2 = strtok(NULL, delimiter);
                        sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(tokenVar2));

                        // Print array index code
                        fprintf(LOCALcode, "\t\t\t(call $get_element\n");
                        fprintf(LOCALcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, arrayName);
                        fprintf(LOCALcode, "\t\t\t\t(i32.const %s)\n", arrIndex);
                        fprintf(LOCALcode, "\t\t\t)\n");

                    } else if (isGlobal) { // If it's a temp variable in global, print to MAINcode
                        fprintf(MAINcode, "\t\t\t(%s.get $%s)\n", varScopeType, var2);
                    }
                    // Else, print to WATcode
                    else {
                        fprintf(LOCALcode, "\t\t\t(%s.get $%s)\n", varScopeType, var2);
                    }
                } else {
                    // Check if var2 references a char
                    // If it does, convert it to an ASCII value
                    if (strncmp(getPrimaryType(var2), "string", 6) == 0) {
                        var2 = convertToASCII(var2);
                    }

                    // Check if var2 equals zero (unidentified value)
                    // If so, convert it to a space in ASCII
                    if (strncmp(var2, "0", 1) == 0) {
                        var2 = "32";
                    }
                    
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t(%s.const %s)\n", opType, var2);
                    } else {
                        fprintf(LOCALcode, "\t\t\t(%s.const %s)\n", opType, var2);
                    }
                }

                // End assignment statement
                if (isGlobal) { // If it's a temp variable in global, print to MAINcode
                    fprintf(MAINcode, "\t\t)\n");
                }
                // Else, print to WATcode
                else {
                    fprintf(LOCALcode, "\t\t)\n");
                }
                
                // Reset currOp variable
                currOp = "";
            }

            // Case for Standard Function Calls
            // Contains a "call" token at index 3
            // Always acts as an assignment statement, but calls a function register with a set of parameters
            else if (strncmp(strArr[2], "call", 4) == 0) {
                // Declare all three variables
                char * assignVar = strArr[0];
                char * funcVar = strArr[3];
                
                // Declare operation type variable
                char * opType;

                // Determine if the variable is global or variable
                char * scopeType = "global";
                char ** scopeStack = { "global", currScope };
                int scopePointer = 0;

                if (!found(strArr[0], scopeStack, scopePointer)) {
                    scopePointer = 1;
                    scopeType = "local";
                }

                // Determine the operation type
                opType = getWATType(currOp);

                // Set function return type first if this is the first call in a function
                if (inParams) {
                    inParams = 0;
                    if (strncmp(returnType, "void", 4) != 0) {
                        fprintf(WATcode, "(result %s)\n", returnType);
                    } else {
                        fprintf(WATcode, "\n");
                    }
                }
                // Else, print a temporary variable declaration line with the WAT type
                else {
                    if (isGlobal) {
                        fprintf(VARScode, "\t\t(%s $%s %s)\n", scopeType, assignVar, opType);
                    } else {
                        fprintf(WATcode, "\t\t(%s $%s %s)\n", scopeType, assignVar, opType);
                    }
                }

                // Output the assignVar call line
                if (isGlobal) {
                    fprintf(MAINcode, "\t\t(%s.set $%s\n", scopeType, assignVar);
                } else {
                    fprintf(LOCALcode, "\t\t(%s.set $%s\n", scopeType, assignVar);
                }

                // Output function call lines
                // If there are no arguments after the "args" token, generate the call on a single line
                if (strArr[5] == NULL) {
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t(call $%s)\n", funcVar);
                    } else {
                        fprintf(LOCALcode, "\t\t\t(call $%s)\n", funcVar);
                    }
                }
                // Otherwise, generate the call with the list of parameters
                else {
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t(call $%s\n", funcVar);
                    } else {
                        fprintf(LOCALcode, "\t\t\t(call $%s\n", funcVar);
                    }


                    // Loop through and add all available parameters as lines under the function call
                    // Determine if each one is a variable or not, and assign accordingly
                    int index = 5;
                    while (strArr[index] != NULL) {
                        // Double check and break if it's the end of sequence
                        if (strncmp(strArr[index], "", 1) == 0) {
                            break;
                        }

                        char * callVar = strArr[index];
                        
                        // If var references an actual variable, add a dollar sign in front and build the line accordingly
                        if (strncmp(getPrimaryType(strArr[index]), "var", 3) == 0) {
                            // Determine if the variable is global or variable
                            char * varScopeType = "global";
                            scopePointer = 0;

                            if (!found(strArr[index], scopeStack, scopePointer)) {
                                scopePointer = 1;
                                varScopeType = "local";
                            }

                            if (isGlobal) {
                                fprintf(MAINcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, callVar);
                            } else {
                                fprintf(LOCALcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, callVar);
                            }
                            
                        } else {
                            opType = getWATType(getPrimaryType(strArr[index]));

                            if (isGlobal) {
                                fprintf(MAINcode, "\t\t\t\t(%s.const %s)\n", opType, callVar);
                            } else {
                                fprintf(LOCALcode, "\t\t\t\t(%s.const %s)\n", opType, callVar);
                            }
                        }
                        index++;
                    }

                    // End the call statement with parameters
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t)\n");
                    } else {
                        fprintf(LOCALcode, "\t\t\t)\n");
                    }
                }

                // End assignment statement
                if (isGlobal) {
                    fprintf(MAINcode, "\t\t)\n");
                } else {
                    fprintf(LOCALcode, "\t\t)\n");
                }
                
                // Reset currOp variable
                // currOp = "";
            }

            // c. Basic Operations
            // - Total Strings = 5
            // - STR1 = Var, STR2 = "=", STR3 = primary/variable,
            // - STR4 = Operand, STR5 = primary/variable

            else if (strncmp(strArr[3], "+", 1) == 0 || strncmp(strArr[3], "-", 1) == 0 || strncmp(strArr[3], "*", 1) == 0 || strncmp(strArr[3], "/", 1) == 0) {
                // Declare all three variables
                char * assignVar = strArr[0];
                char * var1 = strArr[2];
                char * var2 = strArr[4];
                
                // Declare operation type variable
                char * opType;

                // Determine if the variable is global or variable
                char * scopeType = "global";
                char ** scopeStack = { "global", currScope };
                int scopePointer = 0;

                if (!found(assignVar, scopeStack, scopePointer)) {
                    scopePointer = 1;
                    scopeType = "local";
                }
                // Determine the operation type
                opType = getWATType(currOp);

                // If not in parameters, print a temporary variable declaration line with the WAT type
                if (!inParams) {
                    if (isGlobal) {
                        fprintf(VARScode, "\t\t(%s $%s %s)\n", scopeType, assignVar, opType);
                    } else {
                        fprintf(WATcode, "\t\t(%s $%s %s)\n", scopeType, assignVar, opType);
                    }
                    
                }

                // Output the variable set line
                if (isGlobal) {
                    fprintf(MAINcode, "\t\t(%s.set $%s\n", scopeType, assignVar);
                } else {
                    fprintf(LOCALcode, "\t\t(%s.set $%s\n", scopeType, assignVar);
                }

                // Determine operation WAT call
                // "+" = "add", "-" = sub, "*" = mul, and "/" = "div" or "div_s" 
                char * opCall = "";
                char * specialOp = "";

                if (strncmp(strArr[3], "+", 1) == 0) {
                    opCall = "add";
                } else if (strncmp(strArr[3], "-", 1) == 0) {
                    opCall = "sub";
                } else if (strncmp(strArr[3], "*", 1) == 0) {
                    opCall = "mul";
                } else if (strncmp(strArr[3], "/", 1) == 0) {
                    opCall = "div";
                }

                // Special case for division
                // If the operation is an integer, specify "div_s"
                if (strncmp(opCall, "div", 3) == 0 && strncmp(opType, "i32", 3) == 0) {
                    specialOp = "_s";
                }

                // Start the operation call
                if (isGlobal) {
                    fprintf(MAINcode, "\t\t\t(%s.%s%s\n", opType, opCall, specialOp);
                } else {
                    fprintf(LOCALcode, "\t\t\t(%s.%s%s\n", opType, opCall, specialOp);
                }

                // Declare variables for the operation
                char * tokenVar1 = malloc(100*sizeof(char));
                char delimiter[] = "[], ";
                strcpy(tokenVar1, var1);
                tokenVar1 = strtok(tokenVar1, delimiter);
                int lenVar1 = strlen(var1);

                // If var1 references an actual variable, add a dollar sign in front and build the line accordingly
                if (strncmp(getPrimaryType(tokenVar1), "var", 3) == 0) {
                    // Determine if the variable is global or variable
                    char * varScopeType = "global";
                    char ** scopeStack = { "global", currScope };
                    int scopePointer = 0;

                    if (!found(tokenVar1, scopeStack, scopePointer)) {
                        scopePointer = 1;
                        varScopeType = "local";
                    }

                    // If var1 is an array index callout, generate a call to the appropriate index
                    if (var1[lenVar1 - 1] == ']' && isGlobal) {
                        // If it's in global, print to MAIN code
                        // Get the array variable and the index number
                        char * arrayName = malloc(100*sizeof(char));
                        char * arrIndex = malloc(100*sizeof(char));

                        // Assign variable name
                        strcpy(arrayName, tokenVar1);

                        // Assign array index variable
                        tokenVar1 = strtok(NULL, delimiter);
                        sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(tokenVar1));

                        // Print array index code
                        fprintf(MAINcode, "\t\t\t\t(call $get_element\n");
                        fprintf(MAINcode, "\t\t\t\t\t(%s.get $%s)\n", varScopeType, arrayName);
                        fprintf(MAINcode, "\t\t\t\t\t(i32.const %s)\n", arrIndex);
                        fprintf(MAINcode, "\t\t\t\t)\n");

                    } else if (var1[lenVar1 - 1] == ']') {
                        // If it's in a function, print to LOCAL code
                        // Get the array variable and the index number
                        char * arrayName = malloc(100*sizeof(char));
                        char * arrIndex = malloc(100*sizeof(char));

                        // Assign variable name
                        strcpy(arrayName, tokenVar1);

                        // Assign array index variable
                        tokenVar1 = strtok(NULL, delimiter);
                        sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(tokenVar1));

                        // Print array index code
                        fprintf(LOCALcode, "\t\t\t\t(call $get_element\n");
                        fprintf(LOCALcode, "\t\t\t\t\t(%s.get $%s)\n", varScopeType, arrayName);
                        fprintf(LOCALcode, "\t\t\t\t\t(i32.const %s)\n", arrIndex);
                        fprintf(LOCALcode, "\t\t\t\t)\n");

                    } else if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, var1);
                    } else {
                        fprintf(LOCALcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, var1);
                    }
                } else {
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t\t(%s.const %s)\n", opType, var1);   
                    } else {
                        fprintf(LOCALcode, "\t\t\t\t(%s.const %s)\n", opType, var1);
                    }
                }

                // Declare variables for the operation
                char * tokenVar2 = malloc(100*sizeof(char));
                strcpy(tokenVar2, var2);
                tokenVar2 = strtok(tokenVar2, delimiter);
                int lenVar2 = strlen(var2);

                // If var2 references an actual variable, add a dollar sign in front and build the line accordingly
                if (strncmp(getPrimaryType(tokenVar2), "var", 3) == 0) {
                    // Determine if the variable is global or variable
                    char * varScopeType = "global";
                    char ** scopeStack = { "global", currScope };
                    int scopePointer = 0;

                    if (!found(tokenVar2, scopeStack, scopePointer)) {
                        scopePointer = 1;
                        varScopeType = "local";
                    }

                    // If var2 is an array index callout, generate a call to the appropriate index
                    if (var2[lenVar2 - 1] == ']' && isGlobal) {
                        // If it's in global, print to MAIN code
                        // Get the array variable and the index number
                        char * arrayName = malloc(100*sizeof(char));
                        char * arrIndex = malloc(100*sizeof(char));

                        // Assign variable name
                        strcpy(arrayName, tokenVar2);

                        // Assign array index variable
                        tokenVar2 = strtok(NULL, delimiter);
                        sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(tokenVar2));

                        // Print array index code
                        fprintf(MAINcode, "\t\t\t\t(call $get_element\n");
                        fprintf(MAINcode, "\t\t\t\t\t(%s.get $%s)\n", varScopeType, arrayName);
                        fprintf(MAINcode, "\t\t\t\t\t(i32.const %s)\n", arrIndex);
                        fprintf(MAINcode, "\t\t\t\t)\n");

                    } else if (var2[lenVar2 - 1] == ']') {
                        // If it's in a function, print to LOCAL code
                        // Get the array variable and the index number
                        char * arrayName = malloc(100*sizeof(char));
                        char * arrIndex = malloc(100*sizeof(char));

                        // Assign variable name
                        strcpy(arrayName, tokenVar2);

                        // Assign array index variable
                        tokenVar2 = strtok(NULL, delimiter);
                        sprintf(arrIndex, "%d", get(&stringAddresses, arrayName) + atoi(tokenVar2));

                        // Print array index code
                        fprintf(LOCALcode, "\t\t\t\t(call $get_element\n");
                        fprintf(LOCALcode, "\t\t\t\t\t(%s.get $%s)\n", varScopeType, arrayName);
                        fprintf(LOCALcode, "\t\t\t\t\t(i32.const %s)\n", arrIndex);
                        fprintf(LOCALcode, "\t\t\t\t)\n");

                    } else if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, var2);
                    } else {
                        fprintf(LOCALcode, "\t\t\t\t(%s.get $%s)\n", varScopeType, var2);
                    }
                } else {
                    if (isGlobal) {
                        fprintf(MAINcode, "\t\t\t\t(%s.const %s)\n", opType, var2);
                    } else {
                        fprintf(LOCALcode, "\t\t\t\t(%s.const %s)\n", opType, var2);
                    }
                }

                // End the operation call
                if (isGlobal) {
                    fprintf(MAINcode, "\t\t\t)\n");
                    fprintf(MAINcode, "\t\t)\n");
                } else {
                    fprintf(LOCALcode, "\t\t\t)\n");
                    fprintf(LOCALcode, "\t\t)\n");
                }
            }
            
        }
    }

}

void generateLocalOperations() {
    // Temp variable
    char tempLocal[10000];

    // Char pointer
    char ch = '0';

    // Open LOCALcode file in read mode
    fclose(LOCALcode);
    LOCALcode = fopen("LOCALcode.wat", "r");

    // Read all WAT code that was inserted into the local file
    while (ch != EOF) {
        ch = fgetc(LOCALcode);
        if (ch == EOF) {
            break;
        }
        fprintf(WATcode, "%c", ch);
    }

    // End the local function
    fprintf(WATcode, "\t\t\n");

    // Close file and reopen in write mode
    fclose(LOCALcode);
    LOCALcode = fopen("LOCALcode.wat", "w");
}

void generateMainVars() {
    // Temp variable
    char tempVars[10000];

    // Char pointer
    char ch = '0';

    // Open LOCALcode file in read mode
    fclose(VARScode);
    VARScode = fopen("VARScode.wat", "r");

    // Read all WAT code that was inserted into the vars file
    // while (fgets(tempVars, 10000, VARScode) != NULL) {
    //     fprintf(WATcode, "%s", tempVars);
    // }

    while (ch != EOF) {
        ch = fgetc(VARScode);
        if (ch == EOF) {
            break;
        }
        fprintf(WATcode, "%c", ch);
    }

    fprintf(WATcode, "\n");

    // Close file
    fclose(VARScode);
}

void generateMain() {
    // Char pointer
    char ch = '0';

    // Open MAINcode file in read mode
    fclose(MAINcode);
    MAINcode = fopen("MAINcode.wat", "r");

    // Generate the starting code for the main function
    fprintf(WATcode, "\t;; Start Main Function\n");
    fprintf(WATcode, "\t(func $main\n");

    // Generate main function vars
    generateMainVars();

    // Read all WAT code that was inserted into the main file
    // while (fgets(tempGlobal, 10000, MAINcode) != NULL) {
    //     fprintf(WATcode, "%s", tempGlobal);
    // }

    while (ch != EOF) {
        ch = fgetc(MAINcode);
        if (ch == EOF) {
            break;
        }
        fprintf(WATcode, "%c", ch);
    }

    // End the main function
    fprintf(WATcode, "\t)\n");
    fprintf(WATcode, "\t(start $main)\n");
}

// Function to end WAT generation once all commands are printed from the IRcodeOptimized.ir file
void completeFile() {
    // Print ending WAT commands to the file    
    fprintf(WATcode, ")\n");

    // Closes all files once complete
    fclose(IRcode);
    fclose(WATcode);
    fclose(MAINcode);
    fclose(LOCALcode);
}


// Main driver function to generate all WAT code
void generateWATcode() {
    // Initialize String Hash Tables
    init(&stringAddresses);
    init(&stringSizes);

    initAssemblyFile();
    generateModule();
    generateText();
    generateMain();
    completeFile();
}