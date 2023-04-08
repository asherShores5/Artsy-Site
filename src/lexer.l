%option noyywrap
%{
#include <stdio.h>
#include <string.h>

#define YY_DECL int yylex()
#include "parser.tab.h"

int words = 0;
int chars = 0;
int lines = 1;

%}

letter      [a-zA-Z]
digit       [0-9]
ID          {letter}({letter}|{digit})*
delim       [ \t\n]
INTEGER		[-]?{digit}+
FLOAT		[-]?{digit}+(\.{digit}+)?(E[+\-]?(digit)+)?f
NUMBER      [-]?{digit}+(\.{digit}+)?(E[+\-]?(digit)+)?
LINEBREAK	(\n\r)|(\r\n)|[\n\r]
ws          {delim}+
CHAR		'([^'\n]|\\n|\\\\)'
STRING		\"([^\\\"]|\\.)*\"
SINGLE_LINE_COMMENT	\#.*
MULTI_LINE_COMMENT 	\/\#([^\#]|\#+[^\#\/])*\#\/

%%


"TRUE"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : TRUE\n", yytext);
	yylval.string = strdup(yytext); 
	return TRUE;
}

"FALSE"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : FALSE\n", yytext);
	yylval.string = strdup(yytext); 
	return FALSE;
}

"int"	{
			// Recognize integers as a TYPE, return as a token
			words++; chars += strlen(yytext);
			printf("%s : INT\n", yytext);
			yylval.string = strdup(yytext); 
			return TYPE;
		}

"string" {
			// Recognize chars as a TYPE, return as a token
			words++; chars += strlen(yytext);
			printf("%s : STRING\n", yytext);
			yylval.string = strdup(yytext); 
			return TYPE;
		}


"float"	{
			// Recognize strings as a TYPE, return as a token
			words++; chars += strlen(yytext);
			printf("%s : FLOAT\n", yytext);
			yylval.string = strdup(yytext); 
			return TYPE;
		}

"flag"	{
			// Recognize strings as a TYPE, return as a token
			words++; chars += strlen(yytext);
			printf("%s : FLAG\n", yytext);
			yylval.string = strdup(yytext); 
			return TYPE;
		}


"let"	{
			// Recognize strings as a TYPE, return as a token
			words++; chars += strlen(yytext);
			printf("%s : LET\n", yytext);
			yylval.string = strdup(yytext); 
			return LET;
		}

"declare"	{
			// Recognize strings as a TYPE, return as a token
			words++; chars += strlen(yytext);
			printf("%s : DECLARE\n", yytext);
			yylval.string = strdup(yytext); 
			return DECLARE;
		}

"as"	{
			// Recognize strings as a TYPE, return as a token
			words++; chars += strlen(yytext);
			printf("%s : AS\n", yytext);
			yylval.string = strdup(yytext); 
			return AS;
		}

"print"	{
			// The write operation is special, return as a KEYWORD token
			words++; chars += strlen(yytext);
			printf("%s : PRINT\n", yytext);
			yylval.string = strdup(yytext); 
			return PRINT;
		}

"addline"	{
			// The write operation is special, return as a KEYWORD token
			words++; chars += strlen(yytext);
			printf("%s : ADDLINE\n", yytext);
			yylval.string = strdup(yytext); 
			return ADDLINE;
		}

"report"	{
			// The report operation is special, return as a KEYWORD token
			words++; chars += strlen(yytext);
			printf("%s : KEYWORD\n", yytext);
			yylval.string = strdup(yytext); 
			return REPORT;
		}

"action"	{
			// The function operation is special, return as a ACTION token
			words++; chars += strlen(yytext);
			printf("%s : ACTION\n", yytext);
			yylval.string = strdup(yytext); 
			return ACTION;
		}

"while"	{
			// The write operation is special, return as a KEYWORD token
			words++; chars += strlen(yytext);
			printf("%s : KEYWORD\n", yytext);
			yylval.string = strdup(yytext); 
			return WHILE;
		}

"finish"	{
			// The write operation is special, return as a KEYWORD token
			// TO-DO: Implement in parser
			words++; chars += strlen(yytext);
			printf("%s : FINISH\n", yytext);
			yylval.string = strdup(yytext); 
			return FINISH;
		}

"if"	{
			// The "IF" operation is special, return as a KEYWORD token
			words++; chars += strlen(yytext);
			printf("%s : KEYWORD\n", yytext);
			yylval.string = strdup(yytext); 
			return IF;
		}

"elif"	{
			// The "IF" operation is special, return as a KEYWORD token
			words++; chars += strlen(yytext);
			printf("%s : KEYWORD\n", yytext);
			yylval.string = strdup(yytext); 
			return ELIF;
		}

"else"	{
			// The else operation is special, return as a KEYWORD token
			words++; chars += strlen(yytext);
		 	printf("%s : KEYWORD\n", yytext);
			yylval.string = strdup(yytext); 
			return ELSE;
		}


","		{
		  // Comma Recognition
		  chars++;
		  printf("%s : COMMA\n", yytext);
		  yylval.string = strdup(yytext); 
		  return COMMA;
		}

";"		{
		  // End of statement recognition
		  chars++;
		  printf("%s : SEMICOLON\n", yytext);
		  yylval.string = strdup(yytext); 
		  return SEMICOLON;
		}
		
"="		{
		  // Assignment recognition token
		  chars++;
		  printf("%s : EQ\n", yytext);
		  yylval.string = strdup(yytext); 
		  return EQ;
		}

"+"		{
	// Addition operator recognition, return as "PLUS" token
	chars++;
	printf("%s : PLUS\n", yytext);
	yylval.string = strdup(yytext); 
	return PLUS;
}

"-"		{
	// Minus operator recognition, return as "MINUS" token
	chars++;
	printf("%s : MINUS\n", yytext);
	yylval.string = strdup(yytext); 
	return MINUS;
}

"*"		{
	// Multiplication operator recognition, return as "PLUS" token
	chars++;
	printf("%s : MULTIPLY\n", yytext);
	yylval.string = strdup(yytext); 
	return MULTIPLY;
}

"/"		{
	// Division operator recognition, return as "DIVIDE" token
	chars++;
	printf("%s : DIVIDE\n", yytext);
	yylval.string = strdup(yytext); 
	return DIVIDE;
}

"^" 	{
	// EXPONENT operator recognition, return as "EXP" token
	chars++;
	printf("%s : EXPONENT\n", yytext);
	yylval.string = strdup(yytext); 
	return EXPONENT;
}

"%" 	{
	// EXPONENT operator recognition, return as "EXP" token
	chars++;
	printf("%s : MODULO\n", yytext);
	yylval.string = strdup(yytext); 
	return MODULO;
}

"{"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : LEFTBRACKET\n", yytext);
	yylval.string = strdup(yytext); 
	return LEFTBRACKET;
}

"}"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : RIGHTBRACKET\n", yytext);
	yylval.string = strdup(yytext); 
	return RIGHTBRACKET;
}


"("		{
	// Paren recognition, will be used later as a token for scopes
	chars++;
	printf("%s : LEFTPAREN\n", yytext);
	yylval.string = strdup(yytext); 
	return LEFTPAREN;
}

")"		{
	// Paren recognition, will be used later as a token for scopes
	chars++;
	printf("%s : RIGHTPAREN\n", yytext);
	yylval.string = strdup(yytext); 
	return RIGHTPAREN;
}

"["		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : LEFTSQUARE\n", yytext);
	yylval.string = strdup(yytext); 
	return LEFTSQUARE;
}

"]"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : RIGHTSQUARE\n", yytext);
	yylval.string = strdup(yytext); 
	return RIGHTSQUARE;
}

"<"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : LESSTHAN\n", yytext);
	yylval.string = strdup(yytext); 
	return COMPARISONOPERATOR;
}

">"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : GREATERTHAN\n", yytext);
	yylval.string = strdup(yytext); 
	return COMPARISONOPERATOR;
}

"=="		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : DOUBLEEQUAL\n", yytext);
	yylval.string = strdup(yytext); 
	return COMPARISONOPERATOR;
}

"!="		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : NOTEQUAL\n", yytext);
	yylval.string = strdup(yytext); 
	return COMPARISONOPERATOR;
}
	
">="		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : GREATERTHAN OR EQUAL\n", yytext);
	yylval.string = strdup(yytext); 
	return COMPARISONOPERATOR;
}


"<="		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : LESSTHAN OR EQUAL\n", yytext);
	yylval.string = strdup(yytext); 
	return COMPARISONOPERATOR;
}
"and"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : AND\n", yytext);
	yylval.string = strdup(yytext); 
	return LOGICALOPERATOR;
}

"or"		{
	// Bracket recognition, will be used later as a token for scopes
	chars++;
	printf("%s : OR\n", yytext);
	yylval.string = strdup(yytext); 
	return LOGICALOPERATOR;
}

{ID}	{
			  // IDs such as "x" or "y" are utilized with a TYPE or KEYWORD, return as an identifier token
			  words++; chars += strlen(yytext);
			  printf("%s : IDENTIFIER\n",yytext);
			  yylval.string = strdup(yytext); 
			  return ID;
			}

{INTEGER}	{
			  words++; chars += strlen(yytext);
			  printf("%s : INTEGER\n",yytext);
			  yylval.string = strdup(yytext); 
			  return INTEGER;
			}

{FLOAT}		{
			  words++; chars += strlen(yytext);
			  printf("%s : FLOAT\n",yytext);
			  yylval.string = strdup(yytext); 
			  return FLOAT;
			}
			
{NUMBER}	{
		      // Any integer needs to be assigned to a digit or number, so return any digit as a number token
			  words++; chars += strlen(yytext);
			  printf("%s : NUMBER\n",yytext);
			  yylval.number = strdup(yytext); 
			  //yylval.number = atoi(yytext); 
			  return NUMBER;
			}

{STRING}	{
  			  words++; chars += strlen(yytext);
			  printf("%s : STRING\n",yytext);
			  yylval.string = strdup(yytext); 
			  return STRING;
			}
{SINGLE_LINE_COMMENT}		{ /* Ignored (comment) */ }
{MULTI_LINE_COMMENT}	{ /* Ignored (comment) */}

{LINEBREAK}	{lines++; chars=0;}
[ \t]	{chars++;}
.		{chars++;
		 // Line break and tab recognition for processing files in Windows
         printf("%s : Unrecognized symbol at line %d char %d\n", yytext, lines, chars);
		}
		

%%