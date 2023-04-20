//Abstract Syntax Tree Implementation
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
#include "symbolTable.h"
#include "helper.h"

// Generate an AST Node with just a RHS
struct AST * AST_SingleChildNode(char nodeType[50], char RHS[50], struct AST * childNode) {
	// Declare size of AST with malloc
	struct AST* astSingleChildNode = malloc(sizeof(struct AST));

	// Declare the node type and RHS
	strcpy(astSingleChildNode->nodeType, nodeType);
	strcpy(astSingleChildNode->RHS, RHS);

	// Set a pointer to the RHS
	astSingleChildNode->right = childNode;

	// Return the node to the parser
	return astSingleChildNode;
}

struct AST * AST_DoublyChildNodes(char nodeType[50], char LHS[50], char RHS[50], struct AST * leftChildNode, struct AST * rightChildNode){
	// Declare size of AST with malloc
	struct AST* astDoubleChildNode = malloc(sizeof(struct AST));

	// Declare the node type, RHS, and LHS data
	strcpy(astDoubleChildNode->nodeType, nodeType);
	strcpy(astDoubleChildNode->LHS, LHS);
	astDoubleChildNode->left = leftChildNode;

	// Only utilize RHS if it isn't NULL
	if (RHS != NULL) {
		strcpy(astDoubleChildNode->RHS, RHS);
		astDoubleChildNode->right = rightChildNode;
	}


// AST Tree Model
/*
	 NODE
	/    \
  LHS    RHS

*/
	// Return the node to the parser
	return astDoubleChildNode;
	
}

char * getExprOp(struct AST * root) {
	char * exprOp;
	if (root == NULL) {
		exprOp = malloc(100 * sizeof(char));
		strcpy(exprOp, "none");
		return exprOp;
	}
	if (strncmp(root->nodeType, "int", 3) == 0
		|| strncmp(root->nodeType, "float", 5) == 0
		|| strncmp(root->nodeType, "string", 6) == 0
		|| strncmp(root->nodeType, "flag", 7) == 0) {

		// Assign number/variable type to operation type variable
		exprOp = malloc(strlen(root->nodeType) * sizeof(char) + 1);
		strcpy(exprOp, root->nodeType);

		// Return operation type
		return exprOp;

	} else if (strncmp(root->nodeType, "action call", 14) == 0) {
		char ** scopeStack = { "global" };
		// Find the type of the action in the action table
		char * funcType = getItemType(root->left->nodeType, "global", 0);

		// Assign action type to operation type variable
		exprOp = malloc(strlen(funcType) * sizeof(char) + 1);
		strcpy(exprOp, funcType);

		// Return operation type
		return exprOp;
	} else if (strncmp(root->nodeType, "Comparison", 10) == 0) {
		exprOp = malloc(100 * sizeof(char));
		strcpy(exprOp, "flag");
		return exprOp;
	} else if (strncmp(root->LHS, "", 1) != 0) {
		return getExprOp(root->left);
	} else if (strncmp(root->RHS, "", 1) != 0) {
		return getExprOp(root->right);
	} else {
		exprOp = malloc(100 * sizeof(char));
		strcpy(exprOp, "none");
		return exprOp;
	}
}

int evaluateIntExpr(struct AST *root) {
    if (root == NULL)
        return 0;
    int val = 0;
    if (isInt(root->nodeType) == 1) {
        val = atoi(root->nodeType);
    }
    if (root->left != NULL) {
        val += evaluateIntExpr(root->left);
    }
    if (root->right != NULL) {
        val += evaluateIntExpr(root->right);
    }
    return val;
}

float evaluateFloatExpr(struct AST * root) {
    if (root == NULL)
        return 0.0;
	float val = 0.0;
	if (isFloat(root->nodeType) == 1) {
		val = atof(root->nodeType);
	}
	if (root->left != NULL) {
        val += evaluateFloatExpr(root->left);
    }
    if (root->right != NULL) {
        val += evaluateFloatExpr(root->right);
    }
    return val;
}

int containsNonVars(struct AST * root) {
	if (root == NULL) {
		return 0;
	}
	if (isVar(root->nodeType) == 0) {
		return 1;
	}
	if (root->left != NULL && strncmp(root->LHS, "\0", 1) == 0 || strncmp(root->LHS, " ", 1) == 0) {
		return containsNonVars(root->left);
	}
	if (root->right != NULL && strncmp(root->RHS, "\0", 1) == 0 || strncmp(root->RHS, " ", 1) == 0) {
		return containsNonVars(root->right);
	}
	return 0;
}

int getNumExprs(struct AST * root) {
	if (root == NULL) {
		return 0;
	}
	printf("root: %s\n", root->nodeType);
	int count = 0;
	if (strncmp(root->nodeType, "action call param list", 24) != 0 && strncmp(root->nodeType, "exprlist exprtail", 17) != 0 && strncmp(root->nodeType, "exprlist end", 13) != 0 && strncmp(root->nodeType, ")", 1) != 0 && strncmp(root->nodeType, ",", 1) != 0) {
		return 1;
	}
	if (strncmp(root->LHS, "\0", 1) != 0) {
		printf("left:%s\n", root->LHS);
		count += getNumExprs(root->left);
	}

	if (strncmp(root->RHS, "\0", 1) != 0) {
		printf("right:%s\n", root->RHS);
		count += getNumExprs(root->right);
	}

	return count;
}

char * getCallListItemType(struct AST * root, int searchIndex, int currIndex, char * currentScope) {
	char * itemType;
	if (root == NULL) {
		itemType = malloc(100 * sizeof(char));
		strcpy(itemType, "none");
		return itemType;
	}

	// Logic
	// 1. If func call param list is encountered, traverse to the right
	// 2. If an exprlist tail is encountered:
	//		- If searchIndex != currIndex, add +1 to currIndex, then traverse to the right
	//		- Otherwise, return the left hand side case
	// 3. If an exprlist end is encountered:
	//		- If searchIndex != currIndex, then throw a Semantic Error
	//		- Otherwise, return the right hand side case
	// 4. If anything else is encounered, skip and traverse to the right

	if (strncmp(root->nodeType, "action call param list", 24) == 0) {
		getCallListItemType(root->right, searchIndex, currIndex, currentScope);
	} else if (strncmp(root->nodeType, "exprlist exprtail", 17) == 0) {
		if (searchIndex != currIndex) {
			currIndex++;
			getCallListItemType(root->right, searchIndex, currIndex, currentScope);
		} else {
			char * nodeType = malloc(100 * sizeof(char));
			strcpy(nodeType, root->LHS);

			if (strncmp(nodeType, "int", 3) == 0 || strncmp(nodeType, "float", 5) == 0 || strncmp(nodeType, "string", 6) == 0) {
				// Assign number/variable type to operation type variable
				itemType = malloc(strlen(nodeType)*sizeof(char) + 1);
				strcpy(itemType, nodeType);

				// Return operation type
				return itemType;
			} else if (strncmp(nodeType, "action call", 14) == 0) {
				// Find the type of the item using the symbol table
				char * funcType = malloc(100*sizeof(char));
				if (!found(root->right->left->nodeType, "global", 0)) {
					strcpy(funcType, getItemType(root->right->left->nodeType, currentScope, 0));
				} else {
					strcpy(funcType, getItemType(root->right->left->nodeType, "global", 0));
				}

				// Assign action type to operation type variable
				itemType = malloc(strlen(funcType)*sizeof(char) + 1);
				strcpy(itemType, funcType);

				// Return operation type
				return itemType;
			} else {
				// Find the type of the action in the action table
				char * varType = malloc(100*sizeof(char));
				if (!found(nodeType, "global", 0)) {
					strcpy(varType, getItemType(nodeType, currentScope, 0));
				} else {
					strcpy(varType, getItemType(nodeType, "global", 0));
				}
				
				// Assign variable type to operation type variable
				itemType = malloc(strlen(varType)*sizeof(char) + 1);
				strcpy(itemType, varType);

				// Return operation type
				return itemType;
			}
		}
	} else if (strncmp(root->nodeType, "exprlist end", 12) == 0) {
		if (searchIndex != currIndex) {
			exit(1);
		} else {
			char * nodeType = malloc(100 * sizeof(char));
			strcpy(nodeType, root->RHS);

			if (strncmp(nodeType, "int", 3) == 0 || strncmp(nodeType, "float", 5) == 0 || strncmp(nodeType, "string", 6) == 0) {
				// Assign number/variable type to operation type variable
				itemType = malloc(strlen(nodeType)*sizeof(char) + 1);
				strcpy(itemType, nodeType);

				// Return operation type
				return itemType;
			} else if (strncmp(nodeType, "action call", 14) == 0) {
				// Find the type of the item using the symbol table
				char * funcType = malloc(100*sizeof(char));
				if (!found(root->right->left->nodeType, "global", 0)) {
					strcpy(funcType, getItemType(root->right->left->nodeType, currentScope, 0));
				} else {
					strcpy(funcType, getItemType(root->right->left->nodeType, "global", 0));
				}

				// Assign action type to operation type variable
				itemType = malloc(strlen(funcType)*sizeof(char) + 1);
				strcpy(itemType, funcType);

				// Return operation type
				return itemType;
			} else {
				// Find the type of the action in the action table
				char * varType = malloc(100*sizeof(char));
				if (!found(nodeType, "global", 0)) {
					strcpy(varType, getItemType(nodeType, currentScope, 0));
				} else {
					strcpy(varType, getItemType(nodeType, "global", 0));
				}

				// Assign variable type to operation type variable
				itemType = malloc(strlen(varType)*sizeof(char) + 1);
				strcpy(itemType, varType);

				// Return operation type
				return itemType;
			}
		}
	} else {
		getCallListItemType(root->right, searchIndex, currIndex, currentScope);
	}
}