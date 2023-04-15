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
    // Create an input path
    char * inputPath = malloc(1000 * sizeof(char));
    strcpy(inputPath, "./in/");
    strcat(inputPath, argv[1]);
    strcat(inputPath, ".txt");

    // Open the input file name
    FILE * inputFile = fopen(inputPath, "r");

    // Check if the input file actually exists
    if (inputFile == NULL) {
        printf("Error: Failed to open input file. Try running again.");
        return 1;
    }

    // Next, create an error log file
    char * errorPath = malloc(1000 * sizeof(char));
    strcpy(errorPath, "./in/");
    strcat(errorPath, argv[1]);
    strcat(errorPath, "_error.txt");

    // Open the new error log file
    errorFile = fopen(errorPath, "w");

    // Run the parser
    // If it returns an error, exit the program with a warning number
    if (parser_main(inputFile) != 0) {
      printf("Parser Failed, Exiting Program.\n\n");
      exit(1);
    }

    // Next, create the unoptimized IRcode filename
    char * unoptIrCodePath = malloc(1000 * sizeof(char));
    strcpy(unoptIrCodePath, "./in/");
    strcat(unoptIrCodePath, argv[1]);
    strcat(unoptIrCodePath, "_IRcode.ir");

    // Call the ircode generator
    generateIRCode(unoptIrCodePath);

    // Next, create the optimized IRcode filename
    char * optIrCodePath = malloc(1000 * sizeof(char));
    strcpy(optIrCodePath, "./in/");
    strcat(optIrCodePath, argv[1]);
    strcat(optIrCodePath, "_IRcodeOptimized.ir");

    // Call the optimizer to generate optimized IRcode
    // Use this function to generate IRcode that the WAT code generator will actually use
    // Moreover, use this function to compare unoptimized IRcode to optimized IRcode
    generateIRCodeOptimized(optIrCodePath); 

    // Generate optimized WAT code as a .wat file
    // This function is in webgen.c and webgen.h
    // Once this process completes, the next command in the makefile compiles the .wat to a .wasm file
    // After the .wasm file is generated, the changes will be immediately live
    // on testProg.html using the LiveServer extension or on artsy-project.com
    generateWATcode(argv[1]);

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