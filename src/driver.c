#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include "parser.h"
#include "ircode.h"
#include "webgen.h"

// Max program size in bytes
#define MAX_PROGRAM_SIZE 1000000

// Starts the whole program
int main(int argc, char *argv[]) {
    // If the program initialization is valid, allow the program to run
    if (argc > 1) {
      // Open the input file name
      char * inputStr = malloc(MAX_PROGRAM_SIZE * sizeof(char));
      strncpy(inputStr, argv[1], MAX_PROGRAM_SIZE);

      // Check if the input string is empty
      if (inputStr == NULL || strncmp(inputStr, "\0", 1) == 0) {
          printf("Error: Failed to open input stream. Try running again.");
          exit(0);
      }

      FILE* inputStream = fmemopen(inputStr, strlen(inputStr), "r");

      // Run the parser
      // If it returns an error, exit the program
      if (parser_main(inputStream) != 0) {
          printf("Parser Failed, Exiting Program.");
          exit(0);
      }

      // Call the ircode generator to run the first pass on the AST
      generateIRCode();

      // Call the optimizer to generate optimized IRcode string
      char * IRCodeOptimizedStr = malloc(MAX_PROGRAM_SIZE * sizeof(char));
      strncpy(IRCodeOptimizedStr, generateIRCodeOptimized(), MAX_PROGRAM_SIZE);

      // Generate optimized WAT code as a string
      // This string will be processed on artsy-project.tech using a JavaScript module
      char * WATcodeStr = malloc(MAX_PROGRAM_SIZE * sizeof(char));
      strncpy(WATcodeStr, generateWATcode(IRCodeOptimizedStr), MAX_PROGRAM_SIZE);

      // Print the WATcode to the console
      printf("%s", WATcodeStr);

      // End the program
      return 0;
      
    } else {
      // Error message for no arguments
      printf("Error: No arguments were called. Try compiling again with arguments.");
      return 0;
    }
}