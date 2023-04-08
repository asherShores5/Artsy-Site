#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include "parser.h"
#include "ircode.h"
#include "webgen.h"

// Global error log file
FILE* errorFile;

// Starts the whole program
int main(int argc, char ** argv) {
  // Track the total compilation time
  double total_time;
  clock_t start, end;

  // Start the clock
	start = clock();

  // If the program initialization is valid, allow the program to run
  if (argc > 1) {
    // Open the input file name
    FILE * inputFile = fopen(argv[1], "r");

    // Check if the input file actually exists
    if (inputFile == NULL) {
        printf("Error: Failed to open input file. Try running again.");
        return 1;
    }

    // Next, create an error log file
    char * path = malloc(1000 * sizeof(char));
    char * errorPath = malloc(1000 * sizeof(char));
    strncpy(path, argv[1], 100);

    // Find the last occurrence of '/' or '\' in the path
    char * filename = strrchr(path, '/');
    if (filename == NULL) {
        filename = strrchr(path, '\\');
    }
    if (filename == NULL) {
        // No directory separator found, use the whole path as the file name
        filename = path;
    } else {
        // Move the pointer to the character immediately after the directory separator
        filename++;
    }
    
    // Find the last occurrence of '.' in the filename
    char * extension = strrchr(filename, '.');
    if (extension != NULL) {
        // If an extension was found, truncate the string at the extension
        *extension = '\0';
    }

    // Create the error log path name
    strncpy(errorPath, "./in/", 5);
    strncat(errorPath, filename, 100);
    strncat(errorPath, "_error.txt", 15);

    // Open the new error log file
    errorFile = fopen(errorPath, "w");

    // Check if the error log file actually exists
    if (errorFile == NULL) {
        printf("Error: Failed to open error log file. Try running again.");
        return 1;
    }

    // Run the parser
    // If it returns an error, exit the program with a warning number
    if (parser_main(inputFile) != 0) {
      printf("Parser Failed, Exiting Program.\n\n");
      exit(1);
    }

    // Call the ircode generator
    generateIRCode();

    // Call the optimizer to generate optimized IRcode
    // Use this function to generate IRcode that the WAT code generator will actually use
    // Moreover, use this function to compare unoptimized IRcode to optimized IRcode
    generateIRCodeOptimized(); 

    // Generate optimized WAT code as a .wat file
    // This function is in webgen.c and webgen.h
    // Once this process completes, the next command in the makefile compiles the .wat to a .wasm file
    // After the .wasm file is generated, the changes will be immediately live
    // on testProg.html using the LiveServer extension or on artsy-project.com
    generateWATcode();

    // Print compilation confirmation message
    printf("\nArtsy Compilation complete! Displaying to website...\n");

    // Stop the time clock
    end = clock();

    // Calculate and print out the total time
    total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	  printf("\nArtsy Compilation Time: %f seonds\n\n", total_time);

    // End the program
    return 0;
  }
}