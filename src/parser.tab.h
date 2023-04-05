/* A Bison parser, made by GNU Bison 3.8.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TYPE = 258,                    /* TYPE  */
    ID = 259,                      /* ID  */
    INTEGER = 260,                 /* INTEGER  */
    FLOAT = 261,                   /* FLOAT  */
    STRING = 262,                  /* STRING  */
    LOGICALOPERATOR = 263,         /* LOGICALOPERATOR  */
    COMPARISONOPERATOR = 264,      /* COMPARISONOPERATOR  */
    COMMA = 265,                   /* COMMA  */
    SEMICOLON = 266,               /* SEMICOLON  */
    EQ = 267,                      /* EQ  */
    LEFTBRACKET = 268,             /* LEFTBRACKET  */
    RIGHTBRACKET = 269,            /* RIGHTBRACKET  */
    LEFTPAREN = 270,               /* LEFTPAREN  */
    RIGHTPAREN = 271,              /* RIGHTPAREN  */
    LEFTSQUARE = 272,              /* LEFTSQUARE  */
    RIGHTSQUARE = 273,             /* RIGHTSQUARE  */
    NUMBER = 274,                  /* NUMBER  */
    LET = 275,                     /* LET  */
    DECLARE = 276,                 /* DECLARE  */
    AS = 277,                      /* AS  */
    PRINT = 278,                   /* PRINT  */
    ADDLINE = 279,                 /* ADDLINE  */
    REPORT = 280,                  /* REPORT  */
    IF = 281,                      /* IF  */
    ELIF = 282,                    /* ELIF  */
    ELSE = 283,                    /* ELSE  */
    WHILE = 284,                   /* WHILE  */
    FINISH = 285,                  /* FINISH  */
    TRUE = 286,                    /* TRUE  */
    FALSE = 287,                   /* FALSE  */
    ACTION = 288,                  /* ACTION  */
    PLUS = 289,                    /* PLUS  */
    MINUS = 290,                   /* MINUS  */
    MULTIPLY = 291,                /* MULTIPLY  */
    DIVIDE = 292,                  /* DIVIDE  */
    MODULO = 293,                  /* MODULO  */
    EXPONENT = 294                 /* EXPONENT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 26 "parser.y"

	char* number;
	char* decimal; 
	char character;
	char* string;
	struct AST* ast;

#line 111 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
