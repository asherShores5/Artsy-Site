#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h> 
#include "helper.h"

// Helper function to determine the type of a token
// within a given assignment or operation statement
char * getPrimaryType(char * phrase) {
    // If the phrase is a type of string or char
    if (phrase[0] == '\"' || phrase[0] == '\'') {
        // If it has quotation marks, it is a string
        return "string";
    }
    // Check if the phrase is an float
    else if (isFloat(phrase)) {
        return "float";
    }
    // Check if the phrase is an int
    else if (isInt(phrase)) {
        return "int";
    } else if (strncmp(phrase, "+", 1) == 0
        || strncmp(phrase, "-", 1) == 0
        || strncmp(phrase, "*", 1) == 0
        || strncmp(phrase, "/", 1) == 0
        || strncmp(phrase, ">", 1) == 0
        || strncmp(phrase, ">=", 1) == 0
        || strncmp(phrase, "<", 1) == 0
        || strncmp(phrase, "<=", 1) == 0
        || strncmp(phrase, "==", 1) == 0
        || strncmp(phrase, "!=", 1) == 0) {
            return "op";
    }
    // If all cases fail, the type must be a variable
    else {
        return "var";
    }
}

// Helper function to check if a string is alphanumeric
int isAlpha(char * phrase) {
    // Get length of string
    int len;
    for (len = 0; phrase[len] != '\0'; ++len);

    // Loop through each character
    // If there is an alphabetical character, return true
    for (int i = 0; i < len; i++) {
        if (isalpha(phrase[i])) {
            return 1;
        }
    }
    // If nothing is caught, return false
    return 0;
}

// Helper function to determine if the string is an integer
int isInt(char * phrase) {
    // Get length of string
    int len;
    for (len = 0; phrase[len] != '\0'; ++len);

    // If the length of the string is zero or contains only spaces, it cannot be an integer
    int contains_only_spaces = strspn(phrase, " \t\n\r") == len;
    if (len == 0 || contains_only_spaces) {
        return 0;
    }

    // Loop through each character
    // If there is a non-numerical character, return false
    for (int i = 0; i < len; i++) {
        if (i == 0 && phrase[i] == '-') {
            // Ignore case for negative integers
        } else if (!isdigit(phrase[i])) {
            return 0;
        }
    }

    // If nothing is caught, return true
    return 1;
}

// Helper function to determine if the string is a float
int isFloat(char * phrase) {
    // Get length of string
    int len = 0;
    for (len = 0; phrase[len] != '\0'; ++len);

    // Set a var for float condition (must require one and only one "." symbol)
    int condition = 0;

    // Loop through each character
    for (int i = 0; i < len; i++) {
        if (i == 0 && phrase[i] == '-') {
            // Ignore case for negative floats
        }
        else if (!isdigit(phrase[i]) && phrase[i] != '.') {
            // If there is a non-numerical character, return false
            return 0;
        } else if (phrase[i] == '.' && condition == 0) {
            // Set float condition to true, requirement for string to be a float
            condition = 1;
        } else if (phrase[i] == '.' && condition == 1) {
            // Return false if string has two "." symbols
            return 0;
        }
    }

    // If condition is true and nothing is caught, return true
    if (condition == 1) {
        return 1;
    }
    // Else return false
    return 0;
}

// Helper function that returns a boolean based on whether a phrase is a variable or not
int isVar(char * phrase) {
    if (phrase[0] == '\"' || phrase[0] == '\'' || isFloat(phrase) == 1 || isInt(phrase) == 1) {
        return 0;
    }
    return 1;
}

// Helper function that determines the escape character type; used alongside another function to parse if this is a escapeChar or not.
char * escapeCharType(char c) {
    if (c == '\"') {
        return "ESC_DOUBLE";  
    }
    if (c == '\'') {
        return "ESC_SINGLE";  
    }
    if (c == '\\') {
        return "ESC_BACK";  
    }
    if (c == 'n') {
        return "ESC_NEWLINE";  
    }
    if (c == 't') {
        return "ESC_TAB";  
    }
    if (c == ' ') {
        return "SPACE";  
    }
    return "NONE";
}

// Helper function that checks each char and determines if the whole string is a digit or not
int isNumeric(char * var) {
    for(int i = 0; var[i]; i++) {
        // If any char in the string is not a digit, return false
        if(!isdigit((int)var[i])) {
            return 0;
        }
    }
    // Else, return true, it is a digit
    return 1;
}

// Helper function that maps IRcode to the corresponding WebAssembly type
char * getWATType(char * phrase) {
    char * watType = calloc(10, sizeof(char));

    if (strncmp(phrase, "float", 5) == 0) {
        strcpy(watType, "f32");
    } else if (strncmp(phrase, "int", 3) == 0 || strncmp(phrase, "string", 6) == 0) {
        strcpy(watType, "i32");
    } else {
        // Uses a void or undefined keyword
        strcpy(watType, "void");
    }
    return watType;
}

char * getCompareWATType(char * symbol, char * WATType, char * logicType) {
    char * compareType = calloc(10, sizeof(char));

    // Get initial WAT Type
    if (strncmp(symbol, "<=", 2) == 0 && strncmp(WATType, "i32", 3) == 0) {
        strcpy(compareType, "le_s");
    } else if (strncmp(symbol, "<=", 2) == 0 && strncmp(WATType, "f32", 3) == 0) {
        strcpy(compareType, "le");
    } else if (strncmp(symbol, ">=", 2) == 0 && strncmp(WATType, "i32", 3) == 0) {
        strcpy(compareType, "ge_s");
    } else if (strncmp(symbol, ">=", 2) == 0 && strncmp(WATType, "f32", 3) == 0) {
        strcpy(compareType, "ge");
    } else if (strncmp(symbol, "<", 1) == 0 && strncmp(WATType, "i32", 3) == 0) {
        strcpy(compareType, "lt_s");
    } else if (strncmp(symbol, "<", 1) == 0 && strncmp(WATType, "f32", 3) == 0) {
        strcpy(compareType, "lt");
    } else if (strncmp(symbol, ">", 1) == 0 && strncmp(WATType, "i32", 3) == 0) {
        strcpy(compareType, "gt_s");
    } else if (strncmp(symbol, ">", 1) == 0 && strncmp(WATType, "f32", 3) == 0) {
        strcpy(compareType, "gt");
    } else if (strncmp(symbol, "==", 2) == 0) {
        strcpy(compareType, "eq");
    } else if (strncmp(symbol, "!=", 2) == 0) {
        strcpy(compareType, "ne");
    } else {
        // Uses a void or undefined keyword
        strcpy(compareType, "void");
    }

    // Switch sign direction if it's the condition for a while loop
    if (strncmp(logicType, "while", 5) == 0 && strncmp(compareType, "le", 2) == 0) {
        compareType[0] = 'g';
        compareType[1] = 't';
    } else if (strncmp(logicType, "while", 5) == 0 && strncmp(compareType, "lt", 2) == 0) {
        compareType[0] = 'g';
        compareType[1] = 'e';
    } else if (strncmp(logicType, "while", 5) == 0 && strncmp(compareType, "ge", 2) == 0) {
        compareType[0] = 'l';
        compareType[1] = 't';
    } else if (strncmp(logicType, "while", 5) == 0 && strncmp(compareType, "gt", 2) == 0) {
        compareType[0] = 'l';
        compareType[1] = 'e';
    } else if (strncmp(logicType, "while", 5) == 0 && compareType[0] == 'e') {
        strcpy(compareType, "ne");
    } else if (strncmp(logicType, "while", 5) == 0 && compareType[0] == 'n') {
        strcpy(compareType, "eq");
    }

    // Return final compare type symbol
    return compareType;
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
    // Define ASCII val variable as a string
    char * asciiVal = malloc(sizeof(char)*10);

    // If the index is a special escape character, return one of the following outputs
    // Else, return the associated char ASCII value
    if (strncmp(phrase, "\"ESC_DOUBLE\"", 11) == 0) {
        strncpy(asciiVal, "34", 10);
    } else if (strncmp(phrase, "\"ESC_SINGLE\"", 11) == 0) {
        strncpy(asciiVal, "39", 10);
    } else if (strncmp(phrase, "\"ESC_BACK\"", 11) == 0) {
        strncpy(asciiVal, "92", 10);
    } else if (strncmp(phrase, "\"ESC_NEWLINE\"", 11) == 0) {
        strncpy(asciiVal, "13", 10);
    } else if (strncmp(phrase, "\"ESC_TAB\"", 11) == 0) {
        strncpy(asciiVal, "9", 10);
    } else if (strncmp(phrase, "\"SPACE\"", 11) == 0) {
        strncpy(asciiVal, "32", 10);
    } else {
        snprintf(asciiVal, 10, "%d", (int)phrase[1]);
    }

    // Return ASCII value
    return asciiVal;
}

char * convertLogicalIR(char * phrase) {
    
}