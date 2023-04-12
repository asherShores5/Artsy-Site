#ifndef WEBGEN_H 
#define WEBGEN_H

// Error logging
extern int lines;

// File Functions
char * getFileType(char * fileType);

// WAT CODE PRINT MODULES
void generateStartModule();
void generateText(char * IRcode);
void generateLocalOperations();
void generateMain();
void completeFile();

// Driver Function
char * generateWATcode(char * IRcode);

#endif