#include "symbolTable.h"

// Single Scope declaration
// This approach will change in the next iteration
struct Entry symTabItems[100];
int symTabIndex;

// Simple function to notify user that the table has been accessed successfully
void symTabAccess(void){
	printf("::::> Symbol table accessed.\n");
}

// Add a single entry struct to the symbol table for the appropriate scope
void addItem(char itemName[50], char itemKind[8], char itemType[8], int arrayLength, char scope[50]){
		// Set entry struct item data for each parameter in the function
		symTabItems[symTabIndex].itemID = symTabIndex;
		strcpy(symTabItems[symTabIndex].itemName, itemName);
		strcpy(symTabItems[symTabIndex].itemKind, itemKind);
		strcpy(symTabItems[symTabIndex].itemType, itemType);
		symTabItems[symTabIndex].arrayLength = arrayLength;
		strcpy(symTabItems[symTabIndex].scope, scope);

		// Increment the symbol table index for this scope
		symTabIndex++;
	
}

struct Entry* getParamList(char * id, struct AST* paramlist, char scopeStack[50][50], int stackPointer) {
	struct Entry * list = malloc(sizeof(struct Entry));	
	if(strcmp(paramlist->nodeType, "variable parm") == 0) {
		// Semantic check
		// If the parameter variable name has already been declared before this, throw a semantic error
		if (found(paramlist->RHS, scopeStack, stackPointer) == 1) {
			printf("SEMANTIC ERROR: Parameter %s already declared as a variable.\n", paramlist->RHS);
			exit(1);
		}

		list->itemID = symTabIndex;
		strcpy(list->itemName, paramlist->RHS);
		strcpy(list->itemKind, "Var");
		strcpy(list->itemType, paramlist->LHS);
		list->arrayLength = 0;
		strcpy(list->scope, id);
		list->paramlist = 0;

		return list;
	}
	else if(strcmp(paramlist->nodeType, "ParaDecl comma ParaDeclListTail") == 0) {
		list = getParamList(id, paramlist -> left, scopeStack, stackPointer);
		list->paramlist = getParamList(id, paramlist -> right, scopeStack, stackPointer);
	}

	return list;

}


void addFunction(char *type, char *id, struct AST* paramlist, char scopeStack[50][50], int stackPointer){
	if (paramlist == 0){
		symTabItems[symTabIndex].paramlist = 0;
	}
	else if (strcmp(paramlist->nodeType, "variable parm") == 0) {
		struct Entry* list = malloc(sizeof(struct Entry));

		// Semantic check
		// If the parameter variable name has already been declared before this, throw a semantic error
		if (found(paramlist->RHS, scopeStack, stackPointer) == 1) {
			printf("SEMANTIC ERROR: Parameter %s already declared as a variable.\n", paramlist->RHS);
			exit(1);
		}
		
		list->itemID = symTabIndex;
		strcpy(list->itemName, paramlist->RHS);
		strcpy(list->itemKind, "Var");
		strcpy(list->itemType, paramlist->LHS);
		list->arrayLength = 0;
		strcpy(list->scope, id);
		list->paramlist = 0;

		symTabItems[symTabIndex].paramlist = list;
	} else if(strcmp(paramlist->nodeType, "ParaDecl comma ParaDeclListTail") == 0) {		
		symTabItems[symTabIndex].paramlist = getParamList(id, paramlist, scopeStack, stackPointer);
	}

	// Set entry struct item data for each parameter in the function
		symTabItems[symTabIndex].itemID = symTabIndex;
		strcpy(symTabItems[symTabIndex].itemName, id);
		strcpy(symTabItems[symTabIndex].itemKind, "function");
		strcpy(symTabItems[symTabIndex].itemType, type);
		symTabItems[symTabIndex].arrayLength = 0;
		strcpy(symTabItems[symTabIndex].scope, "global");

		// Increment the symbol table index for this scope
		symTabIndex++;
}

void showSymTable(){
	// Show the format of the symboltable using the scope's current entries
	printf("itemID    itemName    itemKind    itemType     ArrayLength    itemScope\n");
	printf("-----------------------------------------------------------------------\n");
	for (int i=0; i<symTabIndex; i++){
		printf("%5d %15s  %7s  %7s %6d %15s \n",symTabItems[i].itemID, symTabItems[i].itemName, symTabItems[i].itemKind, symTabItems[i].itemType, symTabItems[i].arrayLength, symTabItems[i].scope);
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				printf("%5d %15s  %7s  %7s %6d %15s \n",tempList -> itemID, tempList -> itemName, tempList -> itemKind, tempList ->itemType, tempList  -> arrayLength, tempList ->scope);
				tempList = tempList -> paramlist;
			}
		}
	}
	

	printf("-----------------------------------------------------------------------\n");
}

int found(char itemName[50], char scopeStack[50][50], int stackPointer){
	// Lookup an identifier in the symbol table
	// return TRUE or FALSE
	// Later on, this may return additional information for an item being found
	for(int i=0; i<symTabIndex; i++){
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				int str1 = strcmp(tempList -> itemName, itemName);
				// If these strings are the same, return true
				if( str1 == 0){
					for(int j = stackPointer; j >= 0; j--) {
						int str2 = strcmp(tempList -> scope, scopeStack[j]);
						if(str2 == 0) {
							//printf("Found: %s\n-----------------------", itemName);
							return 1; // found the ID in the table
						}
					}
				}
				tempList = tempList -> paramlist;
			}
		}
		
		int str1 = strcmp(symTabItems[i].itemName, itemName);
		// If these strings are the same, return true
		if( str1 == 0){
			for(int j = stackPointer; j >= 0; j--) {
				int str2 = strcmp(symTabItems[i].scope, scopeStack[j]);
				
				if(str2 == 0) {
				//printf("Found: %s\n-----------------------", itemName);
				return 1; // found the ID in the table
				}
			}
		}
	}
	// Else, return false
	return 0;
}

void updateItemArrayLength(char itemName[50], char scopeStack[50][50], int stackPointer, int newLen) {
	// Lookup an identifier in the symbol table
	// return TRUE or FALSE
	// Later on, this may return additional information for an item being found
	for(int i=0; i<symTabIndex; i++){
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				int str1 = strcmp(tempList -> itemName, itemName);

				// If these strings are the same, update length
				if(str1 == 0){
					for(int j = stackPointer; j >= 0; j--) {
						int str2 = strcmp(tempList -> scope, scopeStack[j]);
						if(str2 == 0) {
							symTabItems[i].arrayLength = newLen;
							return;
						}
					}
				}
				tempList = tempList -> paramlist;
			}
		}
		
		int str1 = strcmp(symTabItems[i].itemName, itemName);

		// If these strings are the same, update length
		if( str1 == 0){
			for(int j = stackPointer; j >= 0; j--) {
				int str2 = strcmp(symTabItems[i].scope, scopeStack[j]);
				if(str2 == 0) {
					symTabItems[i].arrayLength = newLen;
					return;
				}
			}
		}
	}
	// Else, return false
	printf("SEMANTIC ERROR: Variable %s d1 does not exist.\n", itemName);
	exit(1);
}

struct Entry* getItem(char itemName[50], char scopeStack[50][50], int stackPointer) {
	// Lookup an identifier in the symbol table
	// return TRUE or FALSE
	// Later on, this may return additional information for an item being found
	for(int i=0; i<symTabIndex; i++){
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				int str1 = strcmp(tempList -> itemName, itemName);
				// If these strings are the same, return true
				if( str1 == 0){
					for(int j = stackPointer; j >= 0; j--) {
						int str2 = strcmp(tempList -> scope, scopeStack[j]);
						if(str2 == 0) {
							//printf("Found: %s\n-----------------------", itemName);
							return 1; // found the ID in the table
						}
					}
				}
				tempList = tempList -> paramlist;
			}
		}
		
		int str1 = strcmp(symTabItems[i].itemName, itemName);
		// If these strings are the same, return true
		if( str1 == 0 ){
			for(int j = stackPointer; j >= 0; j--) {
				int str2 = strcmp(symTabItems[i].scope, scopeStack[j]);
				if(str2 == 0) {
					//printf("Found: %s\n-----------------------", itemName);
					return &symTabItems[i]; // found the ID in the table
				}
			}
		}
	}
	// Else, return false
	return NULL;
}

int getItemID(char itemName[50], char scopeStack[50][50], int stackPointer) {
	// Lookup an identifier in the symbol table
	// return TRUE or FALSE
	// Later on, this may return additional information for an item being found
	for(int i=0; i<symTabIndex; i++){
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				int str1 = strcmp(tempList -> itemName, itemName);

				// If these strings are the same, return true
				if( str1 == 0){
					for(int j = stackPointer; j >= 0; j--) {
						int str2 = strcmp(tempList -> scope, scopeStack[j]);
						if (str2 == 0) {
							return symTabItems[i].itemID;
						}
					}
				}
				tempList = tempList -> paramlist;
			}
		}
		
		int str1 = strcmp(symTabItems[i].itemName, itemName);

		// If these strings are the same, return true
		if( str1 == 0){
			for(int j = stackPointer; j >= 0; j--) {
				int str2 = strcmp(symTabItems[i].scope, scopeStack[j]);
				if(str2 == 0) {
					//printf("Found: %s\n-----------------------", itemName);
					return symTabItems[i].itemID;
				}
			}
		}
	}
	// Else, return false
	printf("SEMANTIC ERROR: Variable %s d2 does not exist.\n", itemName);
	exit(1);
}

char* getItemKind(char itemName[50], char scopeStack[50][50], int stackPointer) {
	// Lookup an identifier in the symbol table
	// return TRUE or FALSE
	// Later on, this may return additional information for an item being found
	for(int i=0; i<symTabIndex; i++){
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				int str1 = strcmp(tempList -> itemName, itemName);

				// If these strings are the same, return true
				if( str1 == 0){
					for(int j = stackPointer; j >= 0; j--) {
						int str2 = strcmp(tempList -> scope, scopeStack[j]);
						if(str2 == 0) {
							return tempList->itemKind;
						}
					}
				}
				tempList = tempList -> paramlist;
			}
		}
		
		int str1 = strcmp(symTabItems[i].itemName, itemName);

		// If these strings are the same, return true
		if( str1 == 0){
			for(int j = stackPointer; j >= 0; j --) {
				int str2 = strcmp(symTabItems[i].scope, scopeStack[j]);
				if(str2 == 0) {
					//printf("Found: %s\n-----------------------", itemName);
					return symTabItems[i].itemKind;
				}
			}
		}
	}
	// Else, return false
	printf("SEMANTIC ERROR: Variable %s d3 does not exist.\n", itemName);
	exit(1);
}

char* getItemType(char itemName[50], char scopeStack[50][50], int stackPointer) {
	// Lookup an identifier in the symbol table
	// return TRUE or FALSE
	// Later on, this may return additional information for an item being found
	for(int i=0; i<symTabIndex; i++){
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				int str1 = strcmp(tempList -> itemName, itemName);

				// If these strings are the same, return true
				if( str1 == 0){
					for(int j = stackPointer; j >= 0; j--) {
						int str2 = strcmp(tempList -> scope, scopeStack[j]);
						if(str2 == 0) {
							return tempList->itemType;
						}
					}
				}
				tempList = tempList -> paramlist;
			}
		}
		
		int str1 = strcmp(symTabItems[i].itemName, itemName);

		// If these strings are the same, return true
		if( str1 == 0){
			for(int j = stackPointer; j >= 0; j--) {
				int str2 = strcmp(symTabItems[i].scope, scopeStack[j]);
				if(str2 == 0) {
					//printf("Found: %s\n-----------------------", itemName);
					return symTabItems[i].itemType;
				}
			}
		}
	}
	// Else, return false
	printf("SEMANTIC ERROR: Variable %s d4 does not exist.\n", itemName);
	exit(1);
}

int getArrayLength(char itemName[50], char scopeStack[50][50], int stackPointer) {
	// Lookup an identifier in the symbol table
	// return TRUE or FALSE
	// Later on, this may return additional information for an item being found
	for(int i=0; i<symTabIndex; i++){
		if(symTabItems[i].paramlist) {
			struct Entry* tempList = symTabItems[i].paramlist;
			while(tempList) {
				int str1 = strcmp(tempList -> itemName, itemName);

				// If these strings are the same, return true
				if( str1 == 0){
					for(int j = stackPointer; j >= 0; j--) {
						int str2 = strcmp(tempList -> scope, scopeStack[j]);
						if(str2 == 0) {
							return tempList->arrayLength;
						}
					}
				}
				tempList = tempList -> paramlist;
			}
		}
		
		int str1 = strcmp(symTabItems[i].itemName, itemName);

		// If these strings are the same, return true
		if( str1 == 0){
			for(int j = stackPointer; j >= 0; j--) {
				int str2 = strcmp(symTabItems[i].scope, scopeStack[j]);
				if(str2 == 0) {
					//printf("Found: %s\n-----------------------", itemName);
					return symTabItems[i].arrayLength;
				}
			}
		}
	}
	// Else, return false
	printf("SEMANTIC ERROR: Variable %s d5 does not exist.\n", itemName);
	exit(1);
}

int compareTypes(char item1[50], char item2[50], char scope[50]) {
	// Compare two item types for any symbol table entry
	const char* idType1 = item1;

	const char* idType2 = item2;

	printf("%s = %s\n", idType1, idType2);

	// Return true or false to state if these items are the same
	return strcmp(idType1, idType2);
}

int countParams(int itemID) {
	// Count variable
	int totalParams = 0;

	// Create the list of all parameters
	struct Entry* tempList = symTabItems[itemID].paramlist;

	// For each loop, add one parameter to the counter
	while(tempList) {
		totalParams++;
		tempList = tempList -> paramlist;
	}
	
	// Return total count
	return totalParams;
}

int getNumFuncParams(char funcName[50]) {
	char ** scopeStack = { "global" };
	return countParams(getItemID(funcName, scopeStack, 0));
}

char * getParamType(int itemID, int numParams, int searchIndex) {
	// Initial index variable
	int currIndex = 0;

	// Create the list of all parameters
	struct Entry* tempList = symTabItems[itemID].paramlist;

	// For each loop, add one parameter to the counter
	for (int i = 0; i < numParams; i++) {
		if (i == searchIndex) {
			char * paramType = malloc(100*sizeof(char));
			strcpy(paramType, tempList->itemType);
			return paramType;
		}
		tempList = tempList -> paramlist;
	}
	// If it is not found, return none
	return "none";
}

char * getFuncParamItemType(char funcName[50], int numParams, int index) {
	char ** scopeStack = { "global" };
	return getParamType(getItemID(funcName, scopeStack, 0), numParams, index);
}