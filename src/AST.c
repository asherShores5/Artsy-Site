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



//This action is to show the abstract symbol tree
//Will allow for an easier ability to debug future projects!
void showSingleAST(struct AST * ASTNode) {
	// Use recursion to traverse the AST
	// Base case
	if (ASTNode == NULL)
        return;

    showSingleAST(ASTNode->left); // Start by using recursion to traverse the LHS of the tree
    printf("%s\t%s\t%s", ASTNode->nodeType, ASTNode->LHS, ASTNode->RHS); // Print field in tree position
    showSingleAST(ASTNode->right); // Traverse RHS of tree
}

void showAST(struct AST * ASTNode) {
	// Use recursion to traverse the AST
	// Base case
	if (ASTNode == NULL)
        return;

    showSingleAST(ASTNode->left); // Start by using recursion to traverse the LHS of the tree
    printf("\nAST Node: %s----------------\n", ASTNode->nodeType); // Print field in tree position
    showSingleAST(ASTNode->right); // Traverse RHS of tree
}

char * getExprOp(struct AST * root) {
	if (root == NULL) {
		return "none";
	}
	if (strncmp(root->nodeType, "int", 3) == 0
		|| strncmp(root->nodeType, "float", 5) == 0
		|| strncmp(root->nodeType, "string", 6) == 0
		|| strncmp(root->nodeType, "flag", 7) == 0) {
		char * op = "";

		// Assign number/variable type to operation type variable
		op = malloc(strlen(root->nodeType)*sizeof(char));
		strcpy(op, root->nodeType);
		// printf("%s\n", op);
		fflush(stdout);

		// Return operation type
		return op;

	} else if (strncmp(root->nodeType, "action call", 14) == 0) {
		char * op = "";

		// Find the type of the action in the action table

		char ** scopeStack = { "global" };
		char * funcType = getItemType(root->left->nodeType, scopeStack, 0);

		// Assign action type to operation type variable
		op = malloc(strlen(funcType)*sizeof(char));
		strcpy(op, funcType);

		// Return operation type
		return op;
	} else if (strncmp(root->nodeType, "Comparison", 10) == 0) {
		return "flag";
	} else if (strncmp(root->LHS, "", 1) != 0) {
		return getExprOp(root->left);
	} else if (strncmp(root->RHS, "", 1) != 0) {
		return getExprOp(root->right);
	} else {
		return "none";
	}
}

int evaluateIntExpr(struct AST * root) {
    if (root == NULL)
        return 0;
	int val = 0;
	if (isInt(root->nodeType) == 1) {
		val = atoi(root->nodeType);
	}

    return (val + evaluateIntExpr(root->left) + evaluateIntExpr(root->right));
}

float evaluateFloatExpr(struct AST * root) {
    if (root == NULL)
        return 0.0;
	float val = 0.0;
	if (isFloat(root->nodeType) == 1) {
		val = atof(root->nodeType);
	}

    return (val + evaluateFloatExpr(root->left) + evaluateFloatExpr(root->right));
}

int containsNonVars(struct AST * root) {
	if (root == NULL) {
		return 0;
	}
	if (isVar(root->nodeType) == 0) {
		return 1;
	}

	if (strncmp(root->LHS, "", 1) != 0) {
		containsNonVars(root->left);
	}
	if (strncmp(root->RHS, "", 1) != 0) {
		containsNonVars(root->right);
	}
}

int getNumExprs(struct AST * root) {
	if (root == NULL) {
		return 0;
	}
	int count = 0;
	// printf("Node: %s, LHS: %s, RHS: %s\n", root->nodeType, root->LHS, root->RHS);
	if (strncmp(root->nodeType, "action call param list", 24) != 0 && strncmp(root->nodeType, "exprlist exprtail", 17) != 0 && strncmp(root->nodeType, "exprlist end", 12) != 0 && strncmp(root->nodeType, "exprlist exprtail", 17) != 0 && strncmp(root->nodeType, ")", 1) != 0 && strncmp(root->nodeType, ",", 1) != 0) {
		return 1;
	}
	if (strncmp(root->LHS, "", 1) != 0) {
		count += getNumExprs(root->left);
	}

	if (strncmp(root->RHS, "", 1) != 0) {
		count += getNumExprs(root->right);
	}

	return count;
}

char * getCallListItemType(struct AST * root, int searchIndex, int currIndex, char * currentScope) {
	if (root == NULL) {
		return "none";
	}

	// printf("Node: %s, LHS: %s, RHS: %s\n", root->nodeType, root->LHS, root->RHS);

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
			char * nodeType = root->LHS;

			if (strncmp(nodeType, "int", 3) == 0 || strncmp(nodeType, "float", 5) == 0 || strncmp(nodeType, "string", 6) == 0) {
				// Get the node type
				char * type = nodeType;

				// Assign number/variable type to operation type variable
				char * op = malloc(strlen(nodeType)*sizeof(char));
				strcpy(op, type);

				// Return operation type
				return op;
			} else if (strncmp(nodeType, "action call", 14) == 0) {
				// Find the type of the item using the symbol table
				char * funcType = malloc(100*sizeof(char));
				if (!found(root->right->left->nodeType, "global", 0)) {
					funcType = getItemType(root->right->left->nodeType, currentScope, 0);
				} else {
					funcType = getItemType(root->right->left->nodeType, "global", 0);
				}

				// Assign action type to operation type variable
				char * op = malloc(strlen(funcType)*sizeof(char));
				strcpy(op, funcType);

				// Return operation type
				return op;
			} else {
				// Find the type of the action in the action table
				char * varType = malloc(100*sizeof(char));
				if (!found(nodeType, "global", 0)) {
					varType = getItemType(nodeType, currentScope, 0);
				} else {
					varType = getItemType(nodeType, "global", 0);
				}
				
				// Assign variable type to operation type variable
				char * op = malloc(strlen(varType)*sizeof(char) + 1);
				strcpy(op, varType);

				// Return operation type
				return op;
			}
		}
	} else if (strncmp(root->nodeType, "exprlist end", 12) == 0) {
		if (searchIndex != currIndex) {
			fprintf(errorFile, "Semantic Error, line %d: Search index (%d) does not match the total number of call parameters (%d).\n", lines, searchIndex, currIndex);
			exit(1);
		} else {
			char * nodeType = root->RHS;

			if (strncmp(nodeType, "int", 3) == 0 || strncmp(nodeType, "float", 5) == 0 || strncmp(nodeType, "string", 6) == 0) {
				// Get the node type
				char * type = nodeType;

				// Assign number/variable type to operation type variable
				char * op = malloc(strlen(nodeType)*sizeof(char));
				strcpy(op, type);

				// Return operation type
				return op;
			} else if (strncmp(nodeType, "action call", 14) == 0) {
				// Find the type of the item using the symbol table
				char * funcType = malloc(100*sizeof(char));
				if (!found(root->right->left->nodeType, "global", 0)) {
					funcType = getItemType(root->right->left->nodeType, currentScope, 0);
				} else {
					funcType = getItemType(root->right->left->nodeType, "global", 0);
				}

				// Assign action type to operation type variable
				char * op = malloc(strlen(funcType)*sizeof(char));
				strcpy(op, funcType);

				// Return operation type
				return op;
			} else {
				// Find the type of the action in the action table
				char * varType = malloc(100*sizeof(char));
				if (!found(nodeType, "global", 0)) {
					varType = getItemType(nodeType, currentScope, 0);
				} else {
					varType = getItemType(nodeType, "global", 0);
				}

				// Assign variable type to operation type variable
				char * op = malloc(strlen(varType)*sizeof(char) + 1);
				strcpy(op, varType);

				// Return operation type
				return op;
			}
		}
	} else {
		getCallListItemType(root->right, searchIndex, currIndex, currentScope);
	}
}