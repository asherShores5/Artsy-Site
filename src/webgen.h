#ifndef WEBGEN_H 
#define WEBGEN_H

void initAssemblyFile();

void generateModule();

char * getWATType(char * phrase);

int getMoveAmount(char * phrase);

void generateText();

void generateLocalOperations();

void generateMain();

void completeFile();

void generateWATcode();

#endif