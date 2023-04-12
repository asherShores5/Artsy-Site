#ifndef IRCODE_H 
#define IRCODE_H

extern struct AST* ast;

// Error logging
extern int lines;

// ---- Functions to handle IR code emissions ---- //

char* emitBinaryOperation(char op[1], const char* id1, const char* id2);

void emitAssignment(char * id1, char * id2);

void emitConstantIntAssignment(char id1[50], char id2[50]);

void emitWriteId(char * id);

void generateIRCode(); 

char * generateIRCodeOptimized();

#endif