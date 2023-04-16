/* A Bison parser, made by GNU Bison 3.8.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30801

/* Bison version string.  */
#define YYBISON_VERSION "3.8.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"
#include "symbolTable.h"
#include "AST.h"
#include "ircode.h"
#include "semantic.h"
#include "webgen.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;

extern int lines;

void yyerror(const char* s);
char scopeStack[50][50];
char currentFunctionScope[50];
int stackPointer;
int blockNumber;
int inLoop;


#line 99 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TYPE = 3,                       /* TYPE  */
  YYSYMBOL_ID = 4,                         /* ID  */
  YYSYMBOL_INTEGER = 5,                    /* INTEGER  */
  YYSYMBOL_FLOAT = 6,                      /* FLOAT  */
  YYSYMBOL_STRING = 7,                     /* STRING  */
  YYSYMBOL_LOGICALOPERATOR = 8,            /* LOGICALOPERATOR  */
  YYSYMBOL_COMPARISONOPERATOR = 9,         /* COMPARISONOPERATOR  */
  YYSYMBOL_COMMA = 10,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 11,                 /* SEMICOLON  */
  YYSYMBOL_EQ = 12,                        /* EQ  */
  YYSYMBOL_LEFTBRACKET = 13,               /* LEFTBRACKET  */
  YYSYMBOL_RIGHTBRACKET = 14,              /* RIGHTBRACKET  */
  YYSYMBOL_LEFTPAREN = 15,                 /* LEFTPAREN  */
  YYSYMBOL_RIGHTPAREN = 16,                /* RIGHTPAREN  */
  YYSYMBOL_LEFTSQUARE = 17,                /* LEFTSQUARE  */
  YYSYMBOL_RIGHTSQUARE = 18,               /* RIGHTSQUARE  */
  YYSYMBOL_NUMBER = 19,                    /* NUMBER  */
  YYSYMBOL_LET = 20,                       /* LET  */
  YYSYMBOL_DECLARE = 21,                   /* DECLARE  */
  YYSYMBOL_AS = 22,                        /* AS  */
  YYSYMBOL_PRINT = 23,                     /* PRINT  */
  YYSYMBOL_ADDLINE = 24,                   /* ADDLINE  */
  YYSYMBOL_REPORT = 25,                    /* REPORT  */
  YYSYMBOL_IF = 26,                        /* IF  */
  YYSYMBOL_ELIF = 27,                      /* ELIF  */
  YYSYMBOL_ELSE = 28,                      /* ELSE  */
  YYSYMBOL_WHILE = 29,                     /* WHILE  */
  YYSYMBOL_FINISH = 30,                    /* FINISH  */
  YYSYMBOL_TRUE = 31,                      /* TRUE  */
  YYSYMBOL_FALSE = 32,                     /* FALSE  */
  YYSYMBOL_ACTION = 33,                    /* ACTION  */
  YYSYMBOL_PLUS = 34,                      /* PLUS  */
  YYSYMBOL_MINUS = 35,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 36,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 37,                    /* DIVIDE  */
  YYSYMBOL_MODULO = 38,                    /* MODULO  */
  YYSYMBOL_EXPONENT = 39,                  /* EXPONENT  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_ProgramStart = 41,              /* ProgramStart  */
  YYSYMBOL_Program = 42,                   /* Program  */
  YYSYMBOL_DeclList = 43,                  /* DeclList  */
  YYSYMBOL_Decl = 44,                      /* Decl  */
  YYSYMBOL_VarDecl = 45,                   /* VarDecl  */
  YYSYMBOL_ActionDeclList = 46,            /* ActionDeclList  */
  YYSYMBOL_ActionDeclListTail = 47,        /* ActionDeclListTail  */
  YYSYMBOL_ActionHeader = 48,              /* ActionHeader  */
  YYSYMBOL_ActionDecl = 49,                /* ActionDecl  */
  YYSYMBOL_ParamDeclList = 50,             /* ParamDeclList  */
  YYSYMBOL_ParamDeclListTail = 51,         /* ParamDeclListTail  */
  YYSYMBOL_ParamDecl = 52,                 /* ParamDecl  */
  YYSYMBOL_StmtList = 53,                  /* StmtList  */
  YYSYMBOL_Stmt = 54,                      /* Stmt  */
  YYSYMBOL_IfStmt = 55,                    /* IfStmt  */
  YYSYMBOL_IfStmtTail = 56,                /* IfStmtTail  */
  YYSYMBOL_Loop = 57,                      /* Loop  */
  YYSYMBOL_WhileL = 58,                    /* WhileL  */
  YYSYMBOL_WhileHead = 59,                 /* WhileHead  */
  YYSYMBOL_IfHead = 60,                    /* IfHead  */
  YYSYMBOL_If = 61,                        /* If  */
  YYSYMBOL_ElifHead = 62,                  /* ElifHead  */
  YYSYMBOL_Elif = 63,                      /* Elif  */
  YYSYMBOL_ElseHead = 64,                  /* ElseHead  */
  YYSYMBOL_Else = 65,                      /* Else  */
  YYSYMBOL_Primary = 66,                   /* Primary  */
  YYSYMBOL_ExprListTail = 67,              /* ExprListTail  */
  YYSYMBOL_ExprList = 68,                  /* ExprList  */
  YYSYMBOL_Block = 69,                     /* Block  */
  YYSYMBOL_Expr = 70,                      /* Expr  */
  YYSYMBOL_ActionCall = 71                 /* ActionCall  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   244

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  141

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    91,    91,    95,    98,   110,   112,   115,   117,   126,
     152,   175,   176,   177,   180,   181,   186,   210,   237,   246,
     249,   252,   253,   256,   259,   264,   267,   272,   277,   278,
     282,   295,   299,   310,   324,   335,   336,   337,   340,   346,
     347,   348,   350,   355,   364,   387,   407,   417,   437,   447,
     465,   476,   477,   478,   479,   485,   486,   495,   496,   499,
     510,   511,   516,   521,   529,   553,   579,   589,   598,   609,
     646,   655,   663,   668,   673,   674,   675,   676,   680
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TYPE", "ID",
  "INTEGER", "FLOAT", "STRING", "LOGICALOPERATOR", "COMPARISONOPERATOR",
  "COMMA", "SEMICOLON", "EQ", "LEFTBRACKET", "RIGHTBRACKET", "LEFTPAREN",
  "RIGHTPAREN", "LEFTSQUARE", "RIGHTSQUARE", "NUMBER", "LET", "DECLARE",
  "AS", "PRINT", "ADDLINE", "REPORT", "IF", "ELIF", "ELSE", "WHILE",
  "FINISH", "TRUE", "FALSE", "ACTION", "PLUS", "MINUS", "MULTIPLY",
  "DIVIDE", "MODULO", "EXPONENT", "$accept", "ProgramStart", "Program",
  "DeclList", "Decl", "VarDecl", "ActionDeclList", "ActionDeclListTail",
  "ActionHeader", "ActionDecl", "ParamDeclList", "ParamDeclListTail",
  "ParamDecl", "StmtList", "Stmt", "IfStmt", "IfStmtTail", "Loop",
  "WhileL", "WhileHead", "IfHead", "If", "ElifHead", "Elif", "ElseHead",
  "Else", "Primary", "ExprListTail", "ExprList", "Block", "Expr",
  "ActionCall", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-45)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-4)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -17,    12,    31,   -45,   156,   -45,   -17,    19,   -45,    40,
     -45,   -45,   -45,   -45,   -17,   212,   -45,    57,   212,    78,
     194,    73,    76,    79,   -45,   -45,    20,   -45,   -45,    86,
      59,    15,   156,   -45,   -45,   -45,    86,    86,    46,   -45,
     -45,    11,   -45,   -45,   114,    75,   113,   109,    -2,    89,
      17,   -45,   -45,    61,   212,   212,   -45,   121,   111,   -45,
     -45,    59,   -45,   -45,   -45,   -45,   116,   -45,   -45,    86,
      46,    86,   -45,   212,   212,   212,   -45,   212,   212,   212,
     212,   212,     0,   107,   117,   -45,   122,   115,   -45,   -45,
     212,   127,   -45,   -45,    68,    77,   125,   140,   212,   -45,
     -45,   -45,   120,   158,   100,   105,   105,    28,    28,   135,
     -45,   145,    75,   -45,   -45,   100,   134,   -45,   -45,   140,
     149,   142,   -45,   154,   112,   147,   -45,   160,   150,   153,
     -45,   140,   -45,   162,   212,   -45,   159,   -45,   -45,   100,
     -45
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       7,     0,     0,     2,    11,     6,     7,     0,     1,    53,
      51,    55,    54,    28,     7,     0,    52,     0,     0,     0,
       0,     0,     0,     0,    76,    77,     0,    27,    13,     0,
      12,     7,    11,    37,    36,    42,     0,     0,    39,    63,
      35,     0,    75,     5,     0,    57,     0,     0,     0,     0,
       0,    31,    34,     0,     0,     0,    32,     0,     0,    18,
      15,    14,     4,    26,    43,    46,     0,    49,    38,     0,
      39,     0,    40,     0,     0,     0,    29,     0,     0,     0,
       0,     0,     0,    53,    58,    61,     0,     0,    62,    74,
       0,     0,    30,    33,     0,     0,     0,    19,     0,    48,
      41,    50,    73,    72,    71,    66,    67,    68,    69,    70,
       9,     0,    57,    78,    56,    64,     0,    45,    44,    19,
       0,     0,    20,    21,     0,     0,    59,     0,     0,    23,
      17,     0,    47,     0,     0,    16,     0,    22,    10,    65,
      24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -45,   -45,    -1,   168,   -45,   -45,   -45,   -18,   -45,   -12,
      64,    47,   -45,   152,   -45,   -45,   126,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -44,    90,   -45,   -27,
     -15,   -45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     4,     5,     6,    27,    28,    29,    30,
     121,   122,   123,    31,    32,    33,    68,    34,    35,    36,
      37,    38,    69,    70,    71,    72,    39,    85,    86,    40,
      41,    42
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,    84,    59,    50,     1,    53,    73,    74,    75,    64,
      65,   110,    60,    47,    89,    -3,     7,   111,    61,    73,
      74,    75,    76,    57,    58,    73,    74,    75,    92,    -3,
      62,     8,    77,    78,    79,    80,     1,    81,    75,    94,
      95,    44,    99,    60,   101,    77,    78,    79,    80,    61,
      81,    77,    78,    79,    80,    45,    81,    46,   102,   103,
     104,    49,   105,   106,   107,   108,   109,    81,    84,    73,
      74,    75,    93,    66,    67,   115,    73,    74,    75,    83,
      10,    11,    12,   124,   117,    73,    74,    75,    54,    51,
      56,    55,    26,   118,    16,    77,    78,    79,    80,    14,
      81,    90,    77,    78,    79,    80,    91,    81,    73,    74,
      75,    77,    78,    79,    80,    75,    81,    82,    87,   139,
      73,    74,    75,    88,    46,    96,    97,   112,   132,    74,
      75,    98,   116,   114,    77,    78,    79,    80,   113,    81,
     119,    79,    80,   120,    81,    75,    77,    78,    79,    80,
     125,    81,   127,   129,    77,    78,    79,    80,   130,    81,
       9,    10,    11,    12,   131,   133,   135,    13,    75,    14,
     136,    15,   134,   138,    43,    16,    17,   140,   137,    18,
      19,    20,    21,   128,    63,    22,    23,    24,    25,    26,
       0,     0,    77,    78,    79,    80,   100,    81,     9,    10,
      11,    12,   126,     0,     0,    52,     0,     0,     0,    15,
       0,     0,     0,    16,    17,     0,     9,    10,    11,    12,
       0,     0,     0,     0,     0,    24,    25,    15,     0,     0,
       0,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,    25
};

static const yytype_int16 yycheck[] =
{
      15,    45,    29,    18,    21,    20,     8,     9,    10,    36,
      37,    11,    30,    14,    16,     0,     4,    17,    30,     8,
       9,    10,    11,     3,     4,     8,     9,    10,    11,    14,
      31,     0,    34,    35,    36,    37,    21,    39,    10,    54,
      55,    22,    69,    61,    71,    34,    35,    36,    37,    61,
      39,    34,    35,    36,    37,    15,    39,    17,    73,    74,
      75,     4,    77,    78,    79,    80,    81,    39,   112,     8,
       9,    10,    11,    27,    28,    90,     8,     9,    10,     4,
       5,     6,     7,    98,    16,     8,     9,    10,    15,    11,
      11,    15,    33,    16,    19,    34,    35,    36,    37,    13,
      39,    12,    34,    35,    36,    37,    17,    39,     8,     9,
      10,    34,    35,    36,    37,    10,    39,     3,     5,   134,
       8,     9,    10,    14,    17,     4,    15,    10,    16,     9,
      10,    15,     5,    18,    34,    35,    36,    37,    16,    39,
      15,    36,    37,     3,    39,    10,    34,    35,    36,    37,
       5,    39,    18,     4,    34,    35,    36,    37,    16,    39,
       4,     5,     6,     7,    10,    18,    16,    11,    10,    13,
      17,    15,    12,    11,     6,    19,    20,    18,   131,    23,
      24,    25,    26,   119,    32,    29,    30,    31,    32,    33,
      -1,    -1,    34,    35,    36,    37,    70,    39,     4,     5,
       6,     7,   112,    -1,    -1,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    19,    20,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    31,    32,    15,    -1,    -1,
      -1,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    21,    41,    42,    43,    44,    45,     4,     0,     4,
       5,     6,     7,    11,    13,    15,    19,    20,    23,    24,
      25,    26,    29,    30,    31,    32,    33,    46,    47,    48,
      49,    53,    54,    55,    57,    58,    59,    60,    61,    66,
      69,    70,    71,    43,    22,    15,    17,    42,    70,     4,
      70,    11,    11,    70,    15,    15,    11,     3,     4,    69,
      47,    49,    42,    53,    69,    69,    27,    28,    56,    62,
      63,    64,    65,     8,     9,    10,    11,    34,    35,    36,
      37,    39,     3,     4,    66,    67,    68,     5,    14,    16,
      12,    17,    11,    11,    70,    70,     4,    15,    15,    69,
      56,    69,    70,    70,    70,    70,    70,    70,    70,    70,
      11,    17,    10,    16,    18,    70,     5,    16,    16,    15,
       3,    50,    51,    52,    70,     5,    67,    18,    50,     4,
      16,    10,    16,    18,    12,    16,    17,    51,    11,    70,
      18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    44,    44,    45,
      45,    46,    46,    46,    47,    47,    48,    48,    49,    50,
      50,    51,    51,    52,    52,    53,    53,    53,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    55,    56,
      56,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    66,    66,    66,    66,    66,    67,    67,    67,
      68,    68,    69,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    71
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     3,     2,     1,     0,     1,     5,
       8,     0,     1,     1,     1,     2,     6,     5,     2,     0,
       1,     1,     3,     2,     4,     1,     2,     1,     1,     2,
       3,     2,     2,     3,     2,     1,     1,     1,     2,     0,
       1,     2,     1,     2,     4,     4,     2,     4,     2,     1,
       2,     1,     1,     1,     1,     1,     4,     0,     1,     3,
       0,     1,     3,     1,     4,     7,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* ProgramStart: Program  */
#line 91 "parser.y"
                      {
	ast = (yyval.ast);
}
#line 1277 "parser.tab.c"
    break;

  case 3: /* Program: DeclList StmtList  */
#line 95 "parser.y"
                           {
		(yyval.ast) = AST_DoublyChildNodes("program", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
	}
#line 1285 "parser.tab.c"
    break;

  case 4: /* Program: DeclList StmtList Program  */
#line 98 "parser.y"
                                  {
		// printf("\nProgram Version: DeclList StmtList Program\n");
		struct AST * funcChildNode = AST_DoublyChildNodes("program", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
		(yyval.ast) = AST_DoublyChildNodes("program",(yyvsp[-2].ast), funcChildNode, (yyvsp[-2].ast), funcChildNode);
	}
#line 1295 "parser.tab.c"
    break;

  case 5: /* DeclList: VarDecl DeclList  */
#line 110 "parser.y"
                                { (yyval.ast) = AST_DoublyChildNodes("vardec", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
							}
#line 1302 "parser.tab.c"
    break;

  case 6: /* DeclList: Decl  */
#line 112 "parser.y"
                { (yyval.ast) = (yyvsp[0].ast); }
#line 1308 "parser.tab.c"
    break;

  case 7: /* Decl: %empty  */
#line 115 "parser.y"
      {
	(yyval.ast) = AST_SingleChildNode("empty", "empty", "empty");}
#line 1315 "parser.tab.c"
    break;

  case 8: /* Decl: VarDecl  */
#line 117 "parser.y"
                  {
	// Basic Var Declaration Rule, generates AST for all variable declarations
	(yyval.ast) = (yyvsp[0].ast);
	}
#line 1324 "parser.tab.c"
    break;

  case 9: /* VarDecl: DECLARE ID AS TYPE SEMICOLON  */
#line 126 "parser.y"
                                        {
		// printf("\n RECOGNIZED RULE: Variable declaration %s\n", $2);
		// Variable declaration rule
		// Symbol Table
		
		int inSymTab = found((yyvsp[-3].string), scopeStack, stackPointer);
		//// printf("looking for %s in symtab - found: %d \n", $2, inSymTab);
		
		// Check if the variable has been declared
		// If it has, throw an error
		if (inSymTab == 0) 
			addItem((yyvsp[-3].string), "Var", (yyvsp[-1].string), 0, scopeStack[stackPointer], stackPointer, blockNumber);
		else {
			// New semantic system test
			fprintf(errorFile, "Semantic Error, line %d: Variable %s has already been declared.\n", lines, (yyvsp[-3].string));
			exit(1);
		}
		// If the variable has not been declared 
		
		

		// Generate AST node as a doubly node
		(yyval.ast) = AST_DoublyChildNodes("type", (yyvsp[-1].string), (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-3].string));

	}
#line 1354 "parser.tab.c"
    break;

  case 10: /* VarDecl: DECLARE ID AS TYPE LEFTSQUARE INTEGER RIGHTSQUARE SEMICOLON  */
#line 152 "parser.y"
                                                                      {
		// printf("RECOGNIZED RULE: Array declaration"); 
		 
		int inSymTab = found((yyvsp[-6].string), scopeStack, stackPointer);
		//// printf("looking for %s in symtab - found: %d \n", $2, inSymTab);
							
		// Check if the variable has been declared
		// If it has, throw an error
		if (inSymTab == 0) 
			addItem((yyvsp[-6].string), "Array", (yyvsp[-4].string), atoi((yyvsp[-2].string)), scopeStack[stackPointer], stackPointer, blockNumber);
		else {
			fprintf(errorFile, "Semantic Error, line %d: Variable %s has already been declared.\n", lines, (yyvsp[-6].string));
			exit(1);
		}
		// If the variable has not been declared 
		

		struct AST* arraySize = AST_SingleChildNode("size", (yyvsp[-2].string), (yyvsp[-2].string)); 
		struct AST* array = AST_DoublyChildNodes((yyvsp[-6].string), "array", arraySize, "array", arraySize);
		(yyval.ast) = AST_DoublyChildNodes("type", (yyvsp[-4].string), array, (yyvsp[-4].string), array);
	}
#line 1380 "parser.tab.c"
    break;

  case 11: /* ActionDeclList: %empty  */
#line 175 "parser.y"
                { 	(yyval.ast) = AST_SingleChildNode("empty", "empty", "empty");}
#line 1386 "parser.tab.c"
    break;

  case 12: /* ActionDeclList: ActionDecl  */
#line 176 "parser.y"
                     {(yyval.ast) = (yyvsp[0].ast);}
#line 1392 "parser.tab.c"
    break;

  case 13: /* ActionDeclList: ActionDeclListTail  */
#line 177 "parser.y"
                             {(yyval.ast) = (yyvsp[0].ast);}
#line 1398 "parser.tab.c"
    break;

  case 14: /* ActionDeclListTail: ActionDecl  */
#line 180 "parser.y"
                               {(yyval.ast) = (yyvsp[0].ast);}
#line 1404 "parser.tab.c"
    break;

  case 15: /* ActionDeclListTail: ActionDecl ActionDeclListTail  */
#line 181 "parser.y"
                                        {
		(yyval.ast) = AST_DoublyChildNodes("ActionDecl ActionDeclListTail", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
	}
#line 1412 "parser.tab.c"
    break;

  case 16: /* ActionHeader: ACTION TYPE ID LEFTPAREN ParamDeclList RIGHTPAREN  */
#line 186 "parser.y"
                                                                {
		// printf("RECOGNIZED RULE: Standard Action\n");

		// Check if the function variable has already been declared
		// If it has, throw an error
		
		int inSymTab = found((yyvsp[-3].string), scopeStack, stackPointer);
		if (inSymTab == 0){
			addAction((yyvsp[-4].string), (yyvsp[-3].string), (yyvsp[-1].ast), scopeStack, stackPointer, blockNumber); //id
		}
		else {
			fprintf(errorFile, "Semantic Error, line %d: Action %s has already been declared.\n", lines, (yyvsp[-3].string));
			exit(1);
		}

		
		(yyval.ast) = AST_DoublyChildNodes("action context", (yyvsp[-3].string), (yyvsp[-1].ast), (yyvsp[-3].string), (yyvsp[-1].ast));

		stackPointer++;
		memset(scopeStack[stackPointer], 0, 50 * sizeof(char));
		strcpy(scopeStack[stackPointer], (yyvsp[-3].string));
		memset(currentFunctionScope, 0, 50 * sizeof(char));
		strcpy(currentFunctionScope, (yyvsp[-3].string));
	}
#line 1441 "parser.tab.c"
    break;

  case 17: /* ActionHeader: ACTION ID LEFTPAREN ParamDeclList RIGHTPAREN  */
#line 210 "parser.y"
                                                       {
		// printf("RECOGNIZED RULE: Void Action\n");
		
		
		int inSymTab = found((yyvsp[-3].string), scopeStack, stackPointer);

		// Check if the function variable has already been declared
		// If it has, throw an error
		if (inSymTab == 0){
			addAction("void", (yyvsp[-3].string), (yyvsp[-1].ast), scopeStack, stackPointer, blockNumber); //id
		}
		else {
			fprintf(errorFile, "Semantic Error, line %d: Action %s has already been declared.\n", lines, (yyvsp[-3].string));
			exit(1);
		}

		
		(yyval.ast) = AST_DoublyChildNodes("action context", (yyvsp[-3].string), (yyvsp[-1].ast), (yyvsp[-3].string), (yyvsp[-1].ast));

		stackPointer++;
		memset(scopeStack[stackPointer], 0, 50 * sizeof(char));
		strcpy(scopeStack[stackPointer], (yyvsp[-3].string));
		memset(currentFunctionScope, 0, 50 * sizeof(char));
		strcpy(currentFunctionScope, (yyvsp[-3].string));
	}
#line 1471 "parser.tab.c"
    break;

  case 18: /* ActionDecl: ActionHeader Block  */
#line 237 "parser.y"
                               {
	// Generate AST node as a doubly node
	(yyval.ast) = AST_DoublyChildNodes("action", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
	stackPointer--;
	memset(currentFunctionScope, 0, 50 * sizeof(char));
	strcpy(currentFunctionScope, scopeStack[stackPointer]);
}
#line 1483 "parser.tab.c"
    break;

  case 19: /* ParamDeclList: %empty  */
#line 246 "parser.y"
               {
		(yyval.ast) = AST_SingleChildNode("empty", "empty", "empty");
}
#line 1491 "parser.tab.c"
    break;

  case 20: /* ParamDeclList: ParamDeclListTail  */
#line 249 "parser.y"
                            {(yyval.ast) = (yyvsp[0].ast);}
#line 1497 "parser.tab.c"
    break;

  case 21: /* ParamDeclListTail: ParamDecl  */
#line 252 "parser.y"
                             {(yyval.ast) = (yyvsp[0].ast);}
#line 1503 "parser.tab.c"
    break;

  case 22: /* ParamDeclListTail: ParamDecl COMMA ParamDeclListTail  */
#line 253 "parser.y"
                                            {(yyval.ast) = AST_DoublyChildNodes("ParaDecl comma ParaDeclListTail", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));}
#line 1509 "parser.tab.c"
    break;

  case 23: /* ParamDecl: TYPE ID  */
#line 256 "parser.y"
                   { // printf("RECOGNIZED RULE: Variable Parameter\n");
		(yyval.ast) = AST_DoublyChildNodes("variable parm", (yyvsp[-1].string), (yyvsp[0].string), (yyvsp[-1].string), (yyvsp[0].string));
	}
#line 1517 "parser.tab.c"
    break;

  case 24: /* ParamDecl: TYPE ID LEFTSQUARE RIGHTSQUARE  */
#line 259 "parser.y"
                                         { // printf("RECOGNIZED RULE: Array Parameter\n");
		(yyval.ast) = AST_DoublyChildNodes("array parm", (yyvsp[-3].string), (yyvsp[-2].string), (yyvsp[-3].string), (yyvsp[-2].string));
	}
#line 1525 "parser.tab.c"
    break;

  case 25: /* StmtList: Stmt  */
#line 264 "parser.y"
               {
		(yyval.ast) = (yyvsp[0].ast);
	}
#line 1533 "parser.tab.c"
    break;

  case 26: /* StmtList: Stmt StmtList  */
#line 267 "parser.y"
                        {
		// printf("\n RECOGNIZED RULE: StmtList statement\n");
		// Generate a list of all statement declarations below vardecl
		(yyval.ast) = AST_DoublyChildNodes("statements", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
		}
#line 1543 "parser.tab.c"
    break;

  case 27: /* StmtList: ActionDeclList  */
#line 272 "parser.y"
                         {
		(yyval.ast) = (yyvsp[0].ast);
	}
#line 1551 "parser.tab.c"
    break;

  case 28: /* Stmt: SEMICOLON  */
#line 277 "parser.y"
                        { 	(yyval.ast) = AST_SingleChildNode("empty", "empty", "empty");}
#line 1557 "parser.tab.c"
    break;

  case 29: /* Stmt: Expr SEMICOLON  */
#line 278 "parser.y"
                                {
		// Simplest expr statement in grammar
		(yyval.ast) = (yyvsp[-1].ast);
	}
#line 1566 "parser.tab.c"
    break;

  case 30: /* Stmt: PRINT Expr SEMICOLON  */
#line 282 "parser.y"
                                { // printf("\n RECOGNIZED RULE: PRINT statement\n");
		// Generate write declarations as a statement in the parser
		(yyval.ast) = AST_SingleChildNode("write", (yyvsp[-1].ast), (yyvsp[-1].ast));

		// printf("Write: %s\n", $2->nodeType);

		// If the primary type is a variable, check if the variable is in the symbol table
		if (!strcmp((yyvsp[-1].ast)->nodeType, "int") && !strcmp((yyvsp[-1].ast)->nodeType, "float") && !strcmp((yyvsp[-1].ast)->nodeType, "string") && strncmp(getPrimaryType((yyvsp[-1].ast)), "var", 3) == 0 && !found((yyvsp[-1].ast), scopeStack, stackPointer)) {
			fprintf(errorFile, "Semantic Error, line %d: Variable %s does not exist.\n", lines, (yyvsp[-1].ast));
			exit(1);
		}

	}
#line 1584 "parser.tab.c"
    break;

  case 31: /* Stmt: ADDLINE SEMICOLON  */
#line 295 "parser.y"
                            {
		// printf("\n RECOGNIZED RULE: ADDLINE statement\n");
		(yyval.ast) = AST_SingleChildNode("addline", "addline", "addline");
	}
#line 1593 "parser.tab.c"
    break;

  case 32: /* Stmt: FINISH SEMICOLON  */
#line 299 "parser.y"
                           {
		// printf("\n RECOGNIZED RULE: FINISH statement\n");
		(yyval.ast) = AST_SingleChildNode("finish", "finish", "finish");

		// Semantic check
		// If the finish statement is not within a loop, throw a Semantic Error
		if (!inLoop) {
			fprintf(errorFile, "Semantic Error, line %d: The \"finish\" keyword was specified outside of a loop.\n", lines);
			exit(1);
		}
	}
#line 1609 "parser.tab.c"
    break;

  case 33: /* Stmt: REPORT Expr SEMICOLON  */
#line 310 "parser.y"
                                {
		// printf("\n RECOGNIZED RULE: REPORT statement\n");
		(yyval.ast) = AST_SingleChildNode("report", (yyvsp[-1].ast), (yyvsp[-1].ast)); 

		// Semantic check for void functions
		// If the function is a void function and states a return, throw a Semantic Error
		if (strncmp(getItemType(currentFunctionScope, scopeStack, 1), "void", 4) == 0) {
			fprintf(errorFile, "Semantic Error, line %d: Cannot specify \"report\" with a report type for void actions.\n", lines);
			exit(1);
		}

		// Check if the return type matches the function type
		CheckAssignmentType(currentFunctionScope, getExprOp((yyvsp[-1].ast)), scopeStack, stackPointer);
	}
#line 1628 "parser.tab.c"
    break;

  case 34: /* Stmt: REPORT SEMICOLON  */
#line 324 "parser.y"
                           {
		// printf("\n RECOGNIZED RULE: VOID REPORT statement\n");
		(yyval.ast) = AST_SingleChildNode("voidreport", "voidreport", "voidreport"); 

		// Semantic check for non-void functions
		// If the function is a non-void function and states a blank return, throw a Semantic Error
		if (strncmp(getItemType(currentFunctionScope, scopeStack, 1), "void", 4) != 0) {
			fprintf(errorFile, "Semantic Error, line %d: Cannot specify a \"report\" without a report type for non-void actions.\n", lines);
			exit(1);
		}
	}
#line 1644 "parser.tab.c"
    break;

  case 35: /* Stmt: Block  */
#line 335 "parser.y"
                {(yyval.ast) = (yyvsp[0].ast);}
#line 1650 "parser.tab.c"
    break;

  case 36: /* Stmt: Loop  */
#line 336 "parser.y"
               {(yyval.ast) = (yyvsp[0].ast);}
#line 1656 "parser.tab.c"
    break;

  case 37: /* Stmt: IfStmt  */
#line 337 "parser.y"
                 {(yyval.ast) = (yyvsp[0].ast);}
#line 1662 "parser.tab.c"
    break;

  case 38: /* IfStmt: If IfStmtTail  */
#line 340 "parser.y"
                      {
	// printf("\n RECOGNIZED RULE: if statement block\n");
	(yyval.ast) = AST_DoublyChildNodes("IfStmt", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
}
#line 1671 "parser.tab.c"
    break;

  case 39: /* IfStmtTail: %empty  */
#line 346 "parser.y"
            { (yyval.ast) = AST_SingleChildNode("IfStmtTail else end", "", "");}
#line 1677 "parser.tab.c"
    break;

  case 40: /* IfStmtTail: Else  */
#line 347 "parser.y"
               { (yyval.ast) = AST_SingleChildNode("IfStmtTail else end", (yyvsp[0].ast), (yyvsp[0].ast));}
#line 1683 "parser.tab.c"
    break;

  case 41: /* IfStmtTail: Elif IfStmtTail  */
#line 348 "parser.y"
                          { (yyval.ast) = AST_DoublyChildNodes("IfStmtTail continue", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast)); }
#line 1689 "parser.tab.c"
    break;

  case 42: /* Loop: WhileL  */
#line 350 "parser.y"
             {
		(yyval.ast) = (yyvsp[0].ast);
	}
#line 1697 "parser.tab.c"
    break;

  case 43: /* WhileL: WhileHead Block  */
#line 355 "parser.y"
                        {
	(yyval.ast) = AST_DoublyChildNodes("WhileL", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
	stackPointer--;
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, scopeStack[stackPointer]);
	inLoop = 0;
}
#line 1709 "parser.tab.c"
    break;

  case 44: /* WhileHead: WHILE LEFTPAREN Expr RIGHTPAREN  */
#line 364 "parser.y"
                                           {
	(yyval.ast) = AST_SingleChildNode((yyvsp[-1].ast), (yyvsp[-1].ast), (yyvsp[-1].ast));

	// Add the while loop to the symbol table
	
	addLogic("while", "While", scopeStack[stackPointer], stackPointer, blockNumber);
	

	// Indicate that it's within a loop
	inLoop = 1;

	// Create tempScopeName
	char tempScopeName[50];
	sprintf(tempScopeName, "%s %s %d", "while", scopeStack[stackPointer], blockNumber);

	stackPointer++;
	blockNumber++;
	memset(scopeStack[stackPointer], 0, 50 * sizeof(char));
	strcpy(scopeStack[stackPointer], tempScopeName);
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, tempScopeName);
}
#line 1736 "parser.tab.c"
    break;

  case 45: /* IfHead: IF LEFTPAREN Expr RIGHTPAREN  */
#line 387 "parser.y"
                                     {
	(yyval.ast) = AST_SingleChildNode((yyvsp[-1].ast), (yyvsp[-1].ast), (yyvsp[-1].ast));

	// Add the if-statement to the symbol table
	
	addLogic("if", "If", scopeStack[stackPointer], stackPointer, blockNumber);
	

	// Create tempScopeName
	char tempScopeName[50];
	sprintf(tempScopeName, "%s %s %d", "if", scopeStack[stackPointer], blockNumber);
	stackPointer++;
	blockNumber++;
	memset(scopeStack[stackPointer], 0, 50 * sizeof(char));
	strcpy(scopeStack[stackPointer], tempScopeName);
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, tempScopeName);
}
#line 1759 "parser.tab.c"
    break;

  case 46: /* If: IfHead Block  */
#line 407 "parser.y"
                 {
	// printf("\n RECOGNIZED RULE: if statement\n");

	(yyval.ast) = AST_DoublyChildNodes("If", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
	stackPointer--;
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, scopeStack[stackPointer]);
}
#line 1772 "parser.tab.c"
    break;

  case 47: /* ElifHead: ELIF LEFTPAREN Expr RIGHTPAREN  */
#line 417 "parser.y"
                                         {
	(yyval.ast) = AST_SingleChildNode((yyvsp[-1].ast), (yyvsp[-1].ast), (yyvsp[-1].ast));

	// Add the elif-statement to the symbol table
	
	addLogic("elif", "Elif", scopeStack[stackPointer], stackPointer, blockNumber);
	

	// Create tempScopeName
	char tempScopeName[50];
	sprintf(tempScopeName, "%s %s %d", "elif", scopeStack[stackPointer], blockNumber);
	stackPointer++;
	blockNumber++;
	memset(scopeStack[stackPointer], 0, 50 * sizeof(char));
	strcpy(scopeStack[stackPointer], tempScopeName);
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, tempScopeName);
}
#line 1795 "parser.tab.c"
    break;

  case 48: /* Elif: ElifHead Block  */
#line 437 "parser.y"
                      {
	// printf("\n RECOGNIZED RULE: elif statement\n");

	(yyval.ast) = AST_DoublyChildNodes("Elif", (yyvsp[-1].ast), (yyvsp[0].ast), (yyvsp[-1].ast), (yyvsp[0].ast));
	stackPointer--;
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, scopeStack[stackPointer]);
}
#line 1808 "parser.tab.c"
    break;

  case 49: /* ElseHead: ELSE  */
#line 447 "parser.y"
               {
	// Add the else-statement to the symbol table
	
	addLogic("else", "Else", scopeStack[stackPointer], stackPointer, blockNumber);
	

	// Create tempScopeName
	char tempScopeName[50];
	sprintf(tempScopeName, "%s %s %d", "else", scopeStack[stackPointer], blockNumber);
	stackPointer++;
	blockNumber++;
	memset(scopeStack[stackPointer], 0, 50 * sizeof(char));
	strcpy(scopeStack[stackPointer], tempScopeName);
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, tempScopeName);
}
#line 1829 "parser.tab.c"
    break;

  case 50: /* Else: ElseHead Block  */
#line 465 "parser.y"
                      {
	// printf("\n RECOGNIZED RULE: else statement\n");

	(yyval.ast) = AST_SingleChildNode("Else", (yyvsp[0].ast), (yyvsp[0].ast));
	stackPointer--;
	// memset(currentFunctionScope, 0, 50 * sizeof(char));
	// strcpy(currentFunctionScope, scopeStack[stackPointer]);
}
#line 1842 "parser.tab.c"
    break;

  case 51: /* Primary: INTEGER  */
#line 476 "parser.y"
                                {(yyval.ast) = AST_SingleChildNode("int", (yyvsp[0].string), (yyvsp[0].string)); }
#line 1848 "parser.tab.c"
    break;

  case 52: /* Primary: NUMBER  */
#line 477 "parser.y"
                        {(yyval.ast) = AST_SingleChildNode("float", (yyvsp[0].number), (yyvsp[0].number)); }
#line 1854 "parser.tab.c"
    break;

  case 53: /* Primary: ID  */
#line 478 "parser.y"
              {(yyval.ast) = AST_SingleChildNode((yyvsp[0].string), (yyvsp[0].string), (yyvsp[0].string));}
#line 1860 "parser.tab.c"
    break;

  case 54: /* Primary: STRING  */
#line 479 "parser.y"
                  {
		// SEMANTIC CHECK: See if the string contains any invalid escape character combinations
		checkEscapeChars((yyvsp[0].string));
		
		(yyval.ast) = AST_SingleChildNode("string", (yyvsp[0].string), (yyvsp[0].string));
	}
#line 1871 "parser.tab.c"
    break;

  case 55: /* Primary: FLOAT  */
#line 485 "parser.y"
                {(yyval.ast) = AST_SingleChildNode("float", (yyvsp[0].string), (yyvsp[0].string));}
#line 1877 "parser.tab.c"
    break;

  case 56: /* Primary: ID LEFTSQUARE INTEGER RIGHTSQUARE  */
#line 486 "parser.y"
                                            {
		char * arrayPrefix = malloc(100*sizeof(char));
		strcat(arrayPrefix, "inarray_");
		strcat(arrayPrefix, getItemType((yyvsp[-3].string), scopeStack, stackPointer));
		// printf("Prefix: %s\n", arrayPrefix);
		(yyval.ast) = AST_DoublyChildNodes(arrayPrefix, (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[-3].string), (yyvsp[-1].string));
	}
#line 1889 "parser.tab.c"
    break;

  case 57: /* ExprListTail: %empty  */
#line 495 "parser.y"
              {(yyval.ast) = AST_SingleChildNode("exprlist end", "\0", "\0");}
#line 1895 "parser.tab.c"
    break;

  case 58: /* ExprListTail: Primary  */
#line 496 "parser.y"
                        { 
			(yyval.ast) = AST_SingleChildNode("exprlist end", (yyvsp[0].ast), (yyvsp[0].ast)); 
	}
#line 1903 "parser.tab.c"
    break;

  case 59: /* ExprListTail: Primary COMMA ExprListTail  */
#line 499 "parser.y"
                                        {
			(yyval.ast) = AST_DoublyChildNodes("exprlist exprtail", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
	}
#line 1911 "parser.tab.c"
    break;

  case 60: /* ExprList: %empty  */
#line 510 "parser.y"
          {}
#line 1917 "parser.tab.c"
    break;

  case 61: /* ExprList: ExprListTail  */
#line 511 "parser.y"
                       {
		(yyval.ast) = (yyvsp[0].ast);
	}
#line 1925 "parser.tab.c"
    break;

  case 62: /* Block: LEFTBRACKET Program RIGHTBRACKET  */
#line 516 "parser.y"
                                         { // printf("\n RECOGNIZED RULE: Block statement\n");
		(yyval.ast) = AST_SingleChildNode("block", (yyvsp[-1].ast), (yyvsp[-1].ast));
		}
#line 1933 "parser.tab.c"
    break;

  case 63: /* Expr: Primary  */
#line 521 "parser.y"
                { // printf("\n RECOGNIZED RULE: Simplest expression\n");
		(yyval.ast) = (yyvsp[0].ast);
		
		char tempNodeType[50];
		strcpy(tempNodeType, CheckPrimaryType((yyvsp[0].ast), scopeStack, stackPointer));
		memset((yyval.ast)->nodeType, 0, 50*sizeof(char));
		strcpy((yyval.ast)->nodeType, tempNodeType);
	}
#line 1946 "parser.tab.c"
    break;

  case 64: /* Expr: LET ID EQ Expr  */
#line 529 "parser.y"
                                { // printf("\n RECOGNIZED RULE: Let Assignment statement \n");
		// --- SEMANTIC CHECKS --- //
		/*
			int semanticCorrectness = FALSE;
			1. Has ID been declared? If yes, semanticCorrectness = 1

			2. Does RHS.type = LHS.type? if yes, semanticCorrectness = 1

			If all tests == 1, then
				perform SEMANTIC ACTIONS
			
		*/
		
		// Check to see if the ID exists in the symbol table
		checkID((yyvsp[-2].string), scopeStack, stackPointer);

		// Check to see if the LHS matches the RHS
		CheckAssignmentType((yyvsp[-2].string), getExprOp((yyvsp[0].ast)), scopeStack, stackPointer);

		// Generate AST tree nodes
		// printf("DEBUG -- GENERATE AST\n");
		(yyval.ast) = AST_DoublyChildNodes("=", (yyvsp[-2].string), (yyvsp[0].ast), (yyvsp[-2].string), (yyvsp[0].ast));

	}
#line 1975 "parser.tab.c"
    break;

  case 65: /* Expr: LET ID LEFTSQUARE INTEGER RIGHTSQUARE EQ Expr  */
#line 553 "parser.y"
                                                                { // printf("\n RECOGNIZED RULE: Let Assignment element statement\n");
		// --- SEMANTIC CHECKS --- //
		/*
			int semanticCorrectness = FALSE;
			1. Has ID been declared? If yes, semanticCorrectness = 1

			2. Does RHS.type = LHS.type? if yes, semanticCorrectness = 1

			If all tests == 1, then
				perform SEMANTIC ACTIONS
			
		*/
		
		// Check to see if the ID exists in the symbol table
		checkID((yyvsp[-5].string), scopeStack, stackPointer);

		// Check to see if the LHS matches the RHS
		CheckAssignmentType((yyvsp[-5].string), (yyvsp[0].ast), scopeStack, stackPointer);

		// Generate AST tree nodes
		// printf("DEBUG -- GENERATE AST\n");
		struct AST* arrayElement = AST_DoublyChildNodes("element assignment", (yyvsp[-3].string), (yyvsp[0].ast), (yyvsp[-3].string), (yyvsp[0].ast)); 

		(yyval.ast) = AST_DoublyChildNodes("=", (yyvsp[-5].string), arrayElement, (yyvsp[-5].string), arrayElement);

	}
#line 2006 "parser.tab.c"
    break;

  case 66: /* Expr: Expr PLUS Expr  */
#line 579 "parser.y"
                         { // printf("\n RECOGNIZED RULE: PLUS statement\n");
		// Semantic checks

		// Check to see if the LHS matches the RHS
		CheckOperationType(getExprOp((yyvsp[-2].ast)), getExprOp((yyvsp[0].ast)));
		
		// Generate AST Nodes (doubly linked)
		(yyval.ast) = AST_DoublyChildNodes("+", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
		// printf("EXPR PLUS EXPR: %s \n------------------------------------------------------------------\n", $3->nodeType);
	}
#line 2021 "parser.tab.c"
    break;

  case 67: /* Expr: Expr MINUS Expr  */
#line 589 "parser.y"
                          { // printf("\n RECOGNIZED RULE: MINUS statement\n");
		// Semantic checks
		
		// Check to see if the LHS matches the RHS
		CheckOperationType(getExprOp((yyvsp[-2].ast)), getExprOp((yyvsp[0].ast)));
		
		// Generate AST Nodes (doubly linked)
		(yyval.ast) = AST_DoublyChildNodes("-", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
	}
#line 2035 "parser.tab.c"
    break;

  case 68: /* Expr: Expr MULTIPLY Expr  */
#line 598 "parser.y"
                             { // printf("\n RECOGNIZED RULE: MULTIPLY statement\n");
		// Semantic checks
		
		// Check to see if the LHS matches the RHS
		CheckOperationType(getExprOp((yyvsp[-2].ast)), getExprOp((yyvsp[0].ast)));

		// printf("Node: *, Term 1: %s, Term 2: %s\n", $1->RHS, $3->RHS);
		
		// Generate AST Nodes (doubly linked)
		(yyval.ast) = AST_DoublyChildNodes("*", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
	}
#line 2051 "parser.tab.c"
    break;

  case 69: /* Expr: Expr DIVIDE Expr  */
#line 609 "parser.y"
                           { // printf("\n RECOGNIZED RULE: DIVIDE statement\n");
		// Semantic checks
		
		// Check to see if the LHS matches the RHS
		CheckOperationType(getExprOp((yyvsp[-2].ast)), getExprOp((yyvsp[0].ast)));
		
		// Generate AST Nodes (doubly linked)
		(yyval.ast) = AST_DoublyChildNodes("/", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));

		// If the RHS is an int, check for integer division error
		if (strncmp((yyvsp[0].ast), "int", 3) == 0) {
			int numeratorInt = 1;
			int denominatorInt = 1;

			// Assign expression values if it's not just a sequence of vars
			if (containsNonVars((yyvsp[-2].ast))) {
				numeratorInt = evaluateIntExpr((yyvsp[-2].ast));
			}
			if (containsNonVars((yyvsp[0].ast))) {
				denominatorInt = evaluateIntExpr((yyvsp[0].ast));
			}
			checkIntDivisionError(numeratorInt, denominatorInt);
		} else if (strncmp((yyvsp[0].ast), "float", 5) == 0) {
			float numeratorFloat = 1.0;
			float denominatorFloat = 1.0;

			// Assign expression values if it's not just a sequence of vars
			if (containsNonVars((yyvsp[-2].ast))) {
				numeratorFloat = evaluateFloatExpr((yyvsp[-2].ast));
			}
			if (containsNonVars((yyvsp[0].ast))) {
				denominatorFloat = evaluateFloatExpr((yyvsp[0].ast));
			}
			checkFloatDivisionError(numeratorFloat, denominatorFloat);
		}

	}
#line 2093 "parser.tab.c"
    break;

  case 70: /* Expr: Expr EXPONENT Expr  */
#line 646 "parser.y"
                             { // printf("\n RECOGNIZED RULE: BinOp statement\n");
		// Semantic checks
		
		// Check to see if the LHS matches the RHS
		CheckOperationType(getExprOp((yyvsp[-2].ast)), getExprOp((yyvsp[0].ast)));
		
		// Generate AST Nodes (doubly linked)
		(yyval.ast) = AST_DoublyChildNodes("^", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
	}
#line 2107 "parser.tab.c"
    break;

  case 71: /* Expr: Expr COMMA Expr  */
#line 655 "parser.y"
                          { // printf("\n RECOGNIZED RULE: BinOp statement\n");
		// Semantic checks
		
		// Check if both exprs exist
		
		// Generate AST Nodes (doubly linked)
		(yyval.ast) = AST_DoublyChildNodes("EXP ", (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
	}
#line 2120 "parser.tab.c"
    break;

  case 72: /* Expr: Expr COMPARISONOPERATOR Expr  */
#line 663 "parser.y"
                                       {
		// printf("\n RECOGNIZED RULE: Comparison statement\n");
		struct AST * tempNode = AST_DoublyChildNodes((yyvsp[-1].string), (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
		(yyval.ast) = AST_SingleChildNode("Comparison", tempNode, tempNode);
	}
#line 2130 "parser.tab.c"
    break;

  case 73: /* Expr: Expr LOGICALOPERATOR Expr  */
#line 668 "parser.y"
                                    {
		// printf("\n RECOGNIZED RULE: Logical statement\n");
		struct AST * tempNode = AST_DoublyChildNodes((yyvsp[-1].string), (yyvsp[-2].ast), (yyvsp[0].ast), (yyvsp[-2].ast), (yyvsp[0].ast));
		(yyval.ast) = AST_SingleChildNode("Logical", tempNode, tempNode);
	}
#line 2140 "parser.tab.c"
    break;

  case 74: /* Expr: LEFTPAREN Expr RIGHTPAREN  */
#line 673 "parser.y"
                                    {(yyval.ast) = (yyvsp[-1].ast);}
#line 2146 "parser.tab.c"
    break;

  case 75: /* Expr: ActionCall  */
#line 674 "parser.y"
                     {(yyval.ast) = (yyvsp[0].ast);}
#line 2152 "parser.tab.c"
    break;

  case 76: /* Expr: TRUE  */
#line 675 "parser.y"
               {(yyval.ast) = AST_SingleChildNode("flag", (yyvsp[0].string), (yyvsp[0].string));}
#line 2158 "parser.tab.c"
    break;

  case 77: /* Expr: FALSE  */
#line 676 "parser.y"
                {(yyval.ast) = AST_SingleChildNode("flag",(yyvsp[0].string), (yyvsp[0].string));}
#line 2164 "parser.tab.c"
    break;

  case 78: /* ActionCall: ID LEFTPAREN ExprList RIGHTPAREN  */
#line 680 "parser.y"
                                             {
	// printf("\nRECOGNIZED RULE: ActionCall\n");
	struct AST* funcCallParamList = AST_SingleChildNode("action call param list", (yyvsp[-1].ast), (yyvsp[-1].ast));
	(yyval.ast) = AST_DoublyChildNodes("action call", (yyvsp[-3].string), funcCallParamList, (yyvsp[-3].string), funcCallParamList);

	// Check if the number of call parameters matches the number of function parameters
	CheckParamLength((yyvsp[-3].string), funcCallParamList);
	
	// Find the number of parameters
    int numParams = getNumExprs(funcCallParamList);

    // Compare parameters if there's at least one parameter
    if (numParams != 0) {
        // Iterate through for each parameter in the list
        for (int i = 0; i < numParams; i++) {
            // Get the function parameter type at this index
            char * funcParamType = getFuncParamItemType((yyvsp[-3].string), numParams, i);

            // Get the expression parameter type at this index
			char * callParamType = getCallListItemType(funcCallParamList, i, 0, scopeStack[stackPointer]);

            // Check to see if the two types do not match
            // If they don't, return a Semantic Error
            if (strncmp(funcParamType, callParamType, strlen(callParamType)) != 0) {
                fprintf(errorFile, "Semantic Error, line %d: The call for parameter #%d (%s) does not match the type for parameter #%d (%s) in the function declaration for \"%s\".\n", lines, i, callParamType, i, funcParamType, (yyvsp[-3].string));
                exit(1);
            }
        }
    }
}
#line 2199 "parser.tab.c"
    break;


#line 2203 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 712 "parser.y"


int parser_main(FILE * inputfile)
{
	// printf("\n----Starting Lexer and Parser----\n\n");
	stackPointer = 0;
	blockNumber = 0;
	inLoop = 0;
	memset(scopeStack[stackPointer], 0, 50 * sizeof(char));
	strcpy(scopeStack[stackPointer], "global");
	
	yyin = inputfile;
	
	return yyparse();
}

void yyerror(const char* s) {
	fprintf(errorFile, "Syntax error, line %d", lines);
	exit(1);
}
