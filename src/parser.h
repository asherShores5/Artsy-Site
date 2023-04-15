#include "AST.h"

// Main AST struct type recognition
struct AST * ast;

// Error log file
extern FILE* errorFile;
extern int lines;

// Main function to run parser
int parser_main(FILE* inputfile);