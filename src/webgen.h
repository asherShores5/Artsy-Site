#ifndef WEBGEN_H 
#define WEBGEN_H

// Error log file
extern FILE* errorFile;
extern int lines;

// File Functions
void initAssemblyFile();
FILE * getFileType(char * fileType);

// WAT CODE PRINT MODULES
void generateStartModule();
void generateText();
void generateLocalOperations();
void generateMain();
void completeFile();

// Driver Function
void generateWATcode();

#endif