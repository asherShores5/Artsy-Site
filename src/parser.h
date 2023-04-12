#include "AST.h"

// Main AST struct type recognition
struct AST * ast;

// Error logging
extern int lines;

// Main function to run parser
int parser_main(FILE * inputStream);