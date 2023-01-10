/* A Bison parser, made by GNU Bison 3.8.2.  */

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
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

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
#include <iostream>
#include<bits/stdc++.h>
#include <typeinfo>
#include <fstream>
#include <sstream>
#include <string.h>
using namespace std;

#include "SymbolTable.h"
#include "SymbolInfo.h"
#include "ScopeTable.h"

FILE* logout;
FILE* errout;
FILE* parseout;

extern int line_count;
int err_count = 0;

extern FILE *yyin;

void yyerror(string s){
    // fprintf(errout, "Line# %d: %s\n", line_count, s.data());
    // err_count++;
}

int yyparse(void);
int yylex(void);

SymbolTable *sym_tab = new SymbolTable(10);

bool curr_ID_func = false;
vector<SymbolInfo> temp_param_list;


string typecast(char* left, char* right)
{
    if(!strcmp(left,"NULL") || !strcmp(right,"NULL")) return "NULL";

    if(!strcmp(left,"void") || !strcmp(right,"void")) return "error";

    if((!strcmp(left,"float") || !strcmp(left,"float_array")) && (!strcmp(right,"float") || !strcmp(right,"float_array"))) return "float";
    if((!strcmp(left,"float") || !strcmp(left,"float_array")) && (!strcmp(right,"int") || !strcmp(right,"int_array"))) return "float";
    if((!strcmp(left,"int") || !strcmp(left,"int_array")) && (!strcmp(right,"float") || !strcmp(right,"float_array"))) return "float";
    if((!strcmp(left,"int") || !strcmp(left,"int_array")) && (!strcmp(right,"int") || !strcmp(right,"int_array"))) return "int";

    return "error";
}

bool is_typecast_valid(string required,string given)
{
    if(required == "void") return given == "void";
    if(required == "int") return (given == "int" || given == "int_array");
    if(required == "float") return given == "float";
}

bool is_assignment_valid(char* left, char* right)
{
    if(!strcmp(left,"NULL") || !strcmp(right,"NULL")) return true; 

    if(!strcmp(left,"void") || !strcmp(right,"void")) return false;
    if(!strcmp(left,"") || !strcmp(right,"")) return false;

    if((!strcmp(left,"int") || !strcmp(left,"int_array")) && (!strcmp(right,"int") || !strcmp(right,"int_array"))) return true;
    if((!strcmp(left,"float") || !strcmp(left,"float_array")) && strcmp(right,"void")) return true;

    return false;
}

void print_rule(string head,string body)
{
    fprintf(logout, "%s : %s\n", head.data(), body.data());
}

void mult_declaration_error(string error_symbol, bool _isFunc)
{
    if(_isFunc) fprintf(errout, "Line# %d: Redefinition of parameter \'%s\'\n", line_count, error_symbol.data());
    else    fprintf(errout, "Line# %d: \'%s\' redeclared as different kind of symbol\n", line_count, error_symbol.data());
    err_count++;
}

void typecast_error()
{
    fprintf(errout, "Line# %d: Incompatible Operand\n", line_count);
    err_count++;
}

void void_expression_error()
{
    fprintf(errout, "Line# %d: Void cannot be used in expression\n", line_count);
    err_count++;
}

void type_conflict_error(string msg)
{
    fprintf(errout, "Line# %d: Conflicting types for \'%s\'\n", line_count, msg.data());
    err_count++;
}

typedef struct ptnode {
    char value[100];
    char token[100];
    char resultType[100];
    vector<string> args_list;
    vector<SymbolInfo*> args_info_list;
	bool lastchild;
	struct ptnode *child;
    struct ptnode *leaf;
} ptnode;

ptnode *Root=NULL;
string output = "\n";
int numWS = 0;

void printtree(ptnode *node)
{
    ptnode *itr;

    for(int i=1;i<numWS;i++)   fprintf(parseout, " ");

    if(numWS)  fprintf(parseout, "\\");

    if(node->lastchild)
    {
        fprintf(parseout, "%s-> %s\n", node->token, node->value);
        output.append(node->value);
        output.append(" ");
    }
    else
    {
        fprintf(parseout, "%s\n", node->token);
        output.append(node->token);
        output.append(" ");
        numWS++;
    }

    for(itr = node->child; itr != NULL; itr = itr->leaf) printtree(itr);

    if(node->lastchild==0) numWS--;
}

ptnode* getNewNode()
{
    ptnode *t = new struct ptnode();
    t->leaf = NULL;
    t->child = NULL;
    t->lastchild = 0;

    strcpy(t->token, "");
    strcpy(t->value, "");
    strcpy(t->resultType, "");

    return(t);
}

ptnode* makeChildNode(ptnode* to_insert, ptnode* root)
{
	if(root->child  ==  NULL)
		root->child = to_insert;
	else
	{
		ptnode *itr;
		for(itr = root->child; itr->leaf != NULL; itr = itr->leaf);
		itr->leaf = to_insert;
	}
	return(root);
}

ptnode* makeChildNode(SymbolInfo* sym, ptnode* root)
{
	ptnode *to_insert = getNewNode();
	to_insert->lastchild = 1;
	strcpy(to_insert->value, sym->lexeme.c_str());
	strcpy(to_insert->token, sym->token.c_str());
	root = makeChildNode(to_insert, root);
	return (root);
}

ptnode* makeChildNode(string value, string token, ptnode* root)
{
	ptnode *to_insert = getNewNode();
	to_insert->lastchild = 1;
	strcpy(to_insert->value, value.c_str());
	strcpy(to_insert->token, token.c_str());
	root = makeChildNode(to_insert, root);
	return (root);
}

ptnode* makeChildNode(string token, ptnode* t)
{
	ptnode *to_insert = getNewNode();
	to_insert->lastchild = 1;
    if(token == "LPAREN")   strcpy(to_insert->value, "(");
    else if(token == "RPAREN")  strcpy(to_insert->value, ")");
    else if(token == "COMMA")  strcpy(to_insert->value, ",");
    else if(token == "SEMICOLON")  strcpy(to_insert->value, ";");
    else if(token == "LCURL")  strcpy(to_insert->value, "{");
    else if(token == "RCURL")  strcpy(to_insert->value, "}");
    else if(token == "LTHIRD")  strcpy(to_insert->value, "[");
    else if(token == "RTHIRD")  strcpy(to_insert->value, "]");
    else if(token == "NOT")  strcpy(to_insert->value, "!");
    else if(token == "ASSIGNOP")  strcpy(to_insert->value, "=");
    else if(token == "DECOP")  strcpy(to_insert->value, "--");
    else if(token == "INCOP")  strcpy(to_insert->value, "++");
    else if(token == "RETURN")  strcpy(to_insert->value, "return");
	strcpy(to_insert->token, token.c_str());
	t = makeChildNode(to_insert,t);
	return (t);
}

void insert_to_symtable(SymbolInfo* sym,char* data_type)
{
    bool isParamListError = false;

    for(int i=0;i<temp_param_list.size();i++)
    {
        if(temp_param_list[i].lexeme == "dummy_key"){
            fprintf(errout, "Line# %d: Syntax error at parameter list of function definition\n", line_count);
            err_count++;
            isParamListError = true;
            break;
        }
    }

    if(!isParamListError){
        
        sym->token = "FUNCTION";
        sym->data_type = data_type;
        sym->isFunc = true;

        for(auto param : temp_param_list)
        {
            sym->args_list.push_back(param.data_type);
        }

        if(!sym_tab->_insert(*sym))
        {
            SymbolInfo* ret_symbol = sym_tab->_search(sym->lexeme);

            if(ret_symbol->isFuncDecl == false){
                mult_declaration_error(sym->lexeme, ret_symbol->isFuncParam);
            }
            else{
                if(ret_symbol->data_type != sym->data_type)
                {
                    fprintf(errout, "Line# %d: Conflicting types for \'%s\'\n", line_count, (sym->lexeme).data());
                    err_count++;
                }

                if(ret_symbol->args_list.size() != sym->args_list.size())
                {
                    type_conflict_error(sym->lexeme);
                }
                else
                {
                    for(int i = 0; i < (ret_symbol->args_list.size()); i++)
                    {
                        if(ret_symbol->args_list[i] != sym->args_list[i]){
                            type_conflict_error(sym->lexeme);
                        }
                    }
                }
                ret_symbol->isFuncDecl = false;
            }
        }
        else{
            SymbolInfo* ret_symbol = sym_tab->_search(sym->lexeme);
            ret_symbol->isFuncDecl = false;
        }
    }
}


#line 347 "y.tab.c"

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

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    LOWER_THAN_ELSE = 260,         /* LOWER_THAN_ELSE  */
    FOR = 261,                     /* FOR  */
    WHILE = 262,                   /* WHILE  */
    DO = 263,                      /* DO  */
    BREAK = 264,                   /* BREAK  */
    CHAR = 265,                    /* CHAR  */
    DOUBLE = 266,                  /* DOUBLE  */
    RETURN = 267,                  /* RETURN  */
    SWITCH = 268,                  /* SWITCH  */
    CASE = 269,                    /* CASE  */
    DEFAULT = 270,                 /* DEFAULT  */
    CONTINUE = 271,                /* CONTINUE  */
    PRINTLN = 272,                 /* PRINTLN  */
    INCOP = 273,                   /* INCOP  */
    DECOP = 274,                   /* DECOP  */
    ASSIGNOP = 275,                /* ASSIGNOP  */
    NOT = 276,                     /* NOT  */
    LPAREN = 277,                  /* LPAREN  */
    RPAREN = 278,                  /* RPAREN  */
    LCURL = 279,                   /* LCURL  */
    RCURL = 280,                   /* RCURL  */
    LTHIRD = 281,                  /* LTHIRD  */
    RTHIRD = 282,                  /* RTHIRD  */
    COMMA = 283,                   /* COMMA  */
    SEMICOLON = 284,               /* SEMICOLON  */
    ID = 285,                      /* ID  */
    INT = 286,                     /* INT  */
    FLOAT = 287,                   /* FLOAT  */
    VOID = 288,                    /* VOID  */
    ADDOP = 289,                   /* ADDOP  */
    MULOP = 290,                   /* MULOP  */
    RELOP = 291,                   /* RELOP  */
    LOGICOP = 292,                 /* LOGICOP  */
    CONST_CHAR = 293,              /* CONST_CHAR  */
    CONST_INT = 294,               /* CONST_INT  */
    CONST_FLOAT = 295,             /* CONST_FLOAT  */
    STRING = 296,                  /* STRING  */
    ERROR_CHAR = 297,              /* ERROR_CHAR  */
    ERROR_FLOAT = 298              /* ERROR_FLOAT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define LOWER_THAN_ELSE 260
#define FOR 261
#define WHILE 262
#define DO 263
#define BREAK 264
#define CHAR 265
#define DOUBLE 266
#define RETURN 267
#define SWITCH 268
#define CASE 269
#define DEFAULT 270
#define CONTINUE 271
#define PRINTLN 272
#define INCOP 273
#define DECOP 274
#define ASSIGNOP 275
#define NOT 276
#define LPAREN 277
#define RPAREN 278
#define LCURL 279
#define RCURL 280
#define LTHIRD 281
#define RTHIRD 282
#define COMMA 283
#define SEMICOLON 284
#define ID 285
#define INT 286
#define FLOAT 287
#define VOID 288
#define ADDOP 289
#define MULOP 290
#define RELOP 291
#define LOGICOP 292
#define CONST_CHAR 293
#define CONST_INT 294
#define CONST_FLOAT 295
#define STRING 296
#define ERROR_CHAR 297
#define ERROR_FLOAT 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 278 "parser.y"

    SymbolInfo* symInfo;
    char* string;
    struct ptnode *node;

#line 492 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_LOWER_THAN_ELSE = 5,            /* LOWER_THAN_ELSE  */
  YYSYMBOL_FOR = 6,                        /* FOR  */
  YYSYMBOL_WHILE = 7,                      /* WHILE  */
  YYSYMBOL_DO = 8,                         /* DO  */
  YYSYMBOL_BREAK = 9,                      /* BREAK  */
  YYSYMBOL_CHAR = 10,                      /* CHAR  */
  YYSYMBOL_DOUBLE = 11,                    /* DOUBLE  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_SWITCH = 13,                    /* SWITCH  */
  YYSYMBOL_CASE = 14,                      /* CASE  */
  YYSYMBOL_DEFAULT = 15,                   /* DEFAULT  */
  YYSYMBOL_CONTINUE = 16,                  /* CONTINUE  */
  YYSYMBOL_PRINTLN = 17,                   /* PRINTLN  */
  YYSYMBOL_INCOP = 18,                     /* INCOP  */
  YYSYMBOL_DECOP = 19,                     /* DECOP  */
  YYSYMBOL_ASSIGNOP = 20,                  /* ASSIGNOP  */
  YYSYMBOL_NOT = 21,                       /* NOT  */
  YYSYMBOL_LPAREN = 22,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 23,                    /* RPAREN  */
  YYSYMBOL_LCURL = 24,                     /* LCURL  */
  YYSYMBOL_RCURL = 25,                     /* RCURL  */
  YYSYMBOL_LTHIRD = 26,                    /* LTHIRD  */
  YYSYMBOL_RTHIRD = 27,                    /* RTHIRD  */
  YYSYMBOL_COMMA = 28,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 29,                 /* SEMICOLON  */
  YYSYMBOL_ID = 30,                        /* ID  */
  YYSYMBOL_INT = 31,                       /* INT  */
  YYSYMBOL_FLOAT = 32,                     /* FLOAT  */
  YYSYMBOL_VOID = 33,                      /* VOID  */
  YYSYMBOL_ADDOP = 34,                     /* ADDOP  */
  YYSYMBOL_MULOP = 35,                     /* MULOP  */
  YYSYMBOL_RELOP = 36,                     /* RELOP  */
  YYSYMBOL_LOGICOP = 37,                   /* LOGICOP  */
  YYSYMBOL_CONST_CHAR = 38,                /* CONST_CHAR  */
  YYSYMBOL_CONST_INT = 39,                 /* CONST_INT  */
  YYSYMBOL_CONST_FLOAT = 40,               /* CONST_FLOAT  */
  YYSYMBOL_STRING = 41,                    /* STRING  */
  YYSYMBOL_ERROR_CHAR = 42,                /* ERROR_CHAR  */
  YYSYMBOL_ERROR_FLOAT = 43,               /* ERROR_FLOAT  */
  YYSYMBOL_YYACCEPT = 44,                  /* $accept  */
  YYSYMBOL_start = 45,                     /* start  */
  YYSYMBOL_program = 46,                   /* program  */
  YYSYMBOL_unit = 47,                      /* unit  */
  YYSYMBOL_func_declaration = 48,          /* func_declaration  */
  YYSYMBOL_func_definition = 49,           /* func_definition  */
  YYSYMBOL_50_1 = 50,                      /* $@1  */
  YYSYMBOL_51_2 = 51,                      /* $@2  */
  YYSYMBOL_52_3 = 52,                      /* $@3  */
  YYSYMBOL_53_4 = 53,                      /* $@4  */
  YYSYMBOL_parameter_list = 54,            /* parameter_list  */
  YYSYMBOL_compound_statement = 55,        /* compound_statement  */
  YYSYMBOL_dummy_scope_function = 56,      /* dummy_scope_function  */
  YYSYMBOL_var_declaration = 57,           /* var_declaration  */
  YYSYMBOL_type_specifier = 58,            /* type_specifier  */
  YYSYMBOL_declaration_list = 59,          /* declaration_list  */
  YYSYMBOL_statements = 60,                /* statements  */
  YYSYMBOL_statement = 61,                 /* statement  */
  YYSYMBOL_expression_statement = 62,      /* expression_statement  */
  YYSYMBOL_variable = 63,                  /* variable  */
  YYSYMBOL_expression = 64,                /* expression  */
  YYSYMBOL_logic_expression = 65,          /* logic_expression  */
  YYSYMBOL_rel_expression = 66,            /* rel_expression  */
  YYSYMBOL_simple_expression = 67,         /* simple_expression  */
  YYSYMBOL_term = 68,                      /* term  */
  YYSYMBOL_unary_expression = 69,          /* unary_expression  */
  YYSYMBOL_factor = 70,                    /* factor  */
  YYSYMBOL_argument_list = 71,             /* argument_list  */
  YYSYMBOL_arguments = 72                  /* arguments  */
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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   280

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  160

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   298


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
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   297,   297,   309,   317,   326,   333,   340,   349,   386,
     424,   462,   500,   531,   561,   592,   624,   624,   640,   640,
     657,   657,   673,   673,   694,   711,   728,   742,   757,   771,
     785,   799,   813,   827,   841,   856,   875,   906,   931,   941,
     951,   962,   979,   998,  1020,  1042,  1065,  1088,  1099,  1116,
    1135,  1143,  1152,  1164,  1172,  1183,  1193,  1200,  1207,  1220,
    1231,  1245,  1252,  1275,  1286,  1293,  1303,  1333,  1374,  1384,
    1412,  1422,  1459,  1469,  1506,  1517,  1552,  1563,  1624,  1634,
    1644,  1656,  1667,  1732,  1744,  1753,  1762,  1771,  1779,  1789,
    1797,  1805,  1819
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
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE",
  "LOWER_THAN_ELSE", "FOR", "WHILE", "DO", "BREAK", "CHAR", "DOUBLE",
  "RETURN", "SWITCH", "CASE", "DEFAULT", "CONTINUE", "PRINTLN", "INCOP",
  "DECOP", "ASSIGNOP", "NOT", "LPAREN", "RPAREN", "LCURL", "RCURL",
  "LTHIRD", "RTHIRD", "COMMA", "SEMICOLON", "ID", "INT", "FLOAT", "VOID",
  "ADDOP", "MULOP", "RELOP", "LOGICOP", "CONST_CHAR", "CONST_INT",
  "CONST_FLOAT", "STRING", "ERROR_CHAR", "ERROR_FLOAT", "$accept", "start",
  "program", "unit", "func_declaration", "func_definition", "$@1", "$@2",
  "$@3", "$@4", "parameter_list", "compound_statement",
  "dummy_scope_function", "var_declaration", "type_specifier",
  "declaration_list", "statements", "statement", "expression_statement",
  "variable", "expression", "logic_expression", "rel_expression",
  "simple_expression", "term", "unary_expression", "factor",
  "argument_list", "arguments", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-105)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-23)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     114,  -105,  -105,  -105,    10,   114,  -105,  -105,  -105,  -105,
     -17,  -105,  -105,    33,     8,    21,    50,    78,   -13,  -105,
       4,     2,    11,    31,     5,    37,    40,  -105,    41,    42,
    -105,  -105,    54,    -9,    84,   114,  -105,  -105,  -105,    56,
      85,  -105,  -105,    54,  -105,  -105,    87,   114,  -105,  -105,
      54,    67,   103,    53,    72,  -105,    62,  -105,  -105,    54,
      80,  -105,  -105,    88,    90,  -105,  -105,   167,    98,   110,
     126,   182,   145,   182,   182,  -105,  -105,    55,   182,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,    97,  -105,  -105,   140,
      83,  -105,   118,    22,   143,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,   132,   182,   147,   182,   151,   152,   133,  -105,
     160,   182,   182,  -105,   202,  -105,  -105,  -105,  -105,   182,
    -105,   182,   182,   182,   182,   237,  -105,   162,   147,   170,
    -105,   171,  -105,  -105,   172,   174,   184,  -105,  -105,  -105,
    -105,   143,   179,  -105,   237,   182,   237,   186,  -105,   182,
    -105,   213,   195,  -105,  -105,  -105,   237,   237,  -105,  -105
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    38,    39,    40,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    47,     0,     0,     0,     0,     0,    36,
       0,     0,     0,    29,     0,     0,     0,    37,    41,     0,
      13,    12,     0,     0,     0,     0,    28,    48,    49,    42,
       0,    15,    14,     0,    35,    21,     0,     0,     9,     8,
       0,    26,     0,     0,     0,    23,     0,    11,    10,     0,
      27,    17,    24,     0,     0,    43,    45,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    64,    66,     0,    84,
      85,    86,    55,    54,    57,    53,     0,    50,    56,    81,
       0,    68,    70,    72,    74,    76,    80,    19,    25,    44,
      46,    34,     0,     0,     0,     0,     0,     0,    81,    79,
       0,    90,     0,    78,     0,    30,    51,    87,    88,     0,
      65,     0,     0,     0,     0,     0,    33,     0,     0,     0,
      63,     0,    83,    92,     0,    89,     0,    32,    52,    69,
      71,    75,    73,    77,     0,     0,     0,     0,    82,     0,
      67,    59,     0,    61,    62,    91,     0,     0,    60,    58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -105,  -105,  -105,   215,    16,    20,  -105,  -105,  -105,  -105,
    -105,    91,  -105,    24,    -7,  -105,   161,   -84,   -81,   -73,
     -70,  -104,   108,   107,   115,   -67,  -105,  -105,  -105
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,     5,     6,    82,    83,    50,    59,    32,    43,
      22,    84,    56,    85,    10,    14,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,   134,   135
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   106,   116,    30,   110,   108,   109,   133,    23,    17,
      11,   113,    33,    13,    46,   139,     7,    28,   116,    47,
       8,     7,    20,   128,     9,     8,   -20,    29,    51,     9,
     138,    31,    37,   127,    34,   129,    18,    19,   108,    35,
      60,   138,   136,    41,    21,   155,   108,   145,   108,   108,
     108,   108,     1,     2,     3,    15,   122,   143,   123,    16,
     151,    36,   153,    67,    38,    68,   -22,    40,    69,    70,
      39,    42,   158,   159,    71,   152,   108,   111,    44,    72,
      65,   112,    52,    73,    74,    48,    44,    75,    57,    24,
      25,    76,    77,     1,     2,     3,    78,    62,   114,    66,
      68,    79,    80,    69,    70,    81,    26,    27,   -16,    71,
      98,   -18,   120,    49,    72,    99,    58,   100,    73,    74,
     103,    44,   115,    45,    53,    54,    76,    77,     1,     2,
       3,    78,   104,   125,    55,    68,    79,    80,    69,    70,
      81,    61,    63,    64,    71,     1,     2,     3,   105,    72,
      97,   117,   118,    73,    74,   121,    44,   126,   117,   118,
     119,    76,    77,     1,     2,     3,    78,   107,    73,    74,
      68,    79,    80,    69,    70,    81,    76,    77,   124,    71,
     130,    78,   131,   132,    72,   144,    79,    80,    73,    74,
      81,    44,   101,   146,   147,   148,    76,    77,     1,     2,
       3,    78,   149,    73,    74,    68,    79,    80,    69,    70,
      81,   150,    77,   122,    71,   154,    78,   156,   157,    72,
      12,    79,    80,    73,    74,    81,    44,   137,   102,   140,
     142,    76,    77,     1,     2,     3,    78,   141,     0,     0,
      68,    79,    80,    69,    70,    81,     0,     0,     0,    71,
       0,     0,     0,     0,    72,     0,     0,     0,    73,    74,
       0,    44,     0,     0,     0,     0,    76,    77,     1,     2,
       3,    78,     0,     0,     0,     0,    79,    80,     0,     0,
      81
};

static const yytype_int16 yycheck[] =
{
      73,    71,    86,     1,    74,    78,    73,   111,    15,     1,
       0,    78,     1,    30,    23,   119,     0,    30,   102,    28,
       0,     5,     1,   104,     0,     5,    24,    23,    35,     5,
     114,    29,    27,   103,    23,   105,    28,    29,   111,    28,
      47,   125,   112,     1,    23,   149,   119,   128,   121,   122,
     123,   124,    31,    32,    33,    22,    34,   124,    36,    26,
     144,    30,   146,     1,    27,     3,    24,    26,     6,     7,
      30,    29,   156,   157,    12,   145,   149,    22,    24,    17,
      27,    26,    26,    21,    22,     1,    24,    25,     1,    39,
      40,    29,    30,    31,    32,    33,    34,    30,     1,    27,
       3,    39,    40,     6,     7,    43,    28,    29,    24,    12,
      30,    24,    29,    29,    17,    27,    29,    27,    21,    22,
      22,    24,    25,    32,    39,    40,    29,    30,    31,    32,
      33,    34,    22,     1,    43,     3,    39,    40,     6,     7,
      43,    50,    39,    40,    12,    31,    32,    33,    22,    17,
      59,    18,    19,    21,    22,    37,    24,    25,    18,    19,
      20,    29,    30,    31,    32,    33,    34,    22,    21,    22,
       3,    39,    40,     6,     7,    43,    29,    30,    35,    12,
      29,    34,    30,    23,    17,    23,    39,    40,    21,    22,
      43,    24,    25,    23,    23,    23,    29,    30,    31,    32,
      33,    34,    28,    21,    22,     3,    39,    40,     6,     7,
      43,    27,    30,    34,    12,    29,    34,     4,    23,    17,
       5,    39,    40,    21,    22,    43,    24,    25,    67,   121,
     123,    29,    30,    31,    32,    33,    34,   122,    -1,    -1,
       3,    39,    40,     6,     7,    43,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    21,    22,
      -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    39,    40,    -1,    -1,
      43
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    31,    32,    33,    45,    46,    47,    48,    49,    57,
      58,     0,    47,    30,    59,    22,    26,     1,    28,    29,
       1,    23,    54,    58,    39,    40,    28,    29,    30,    23,
       1,    29,    52,     1,    23,    28,    30,    27,    27,    30,
      26,     1,    29,    53,    24,    55,    23,    28,     1,    29,
      50,    58,    26,    39,    40,    55,    56,     1,    29,    51,
      58,    55,    30,    39,    40,    27,    27,     1,     3,     6,
       7,    12,    17,    21,    22,    25,    29,    30,    34,    39,
      40,    43,    48,    49,    55,    57,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    55,    30,    27,
      27,    25,    60,    22,    22,    22,    64,    22,    63,    69,
      64,    22,    26,    69,     1,    25,    61,    18,    19,    20,
      29,    37,    34,    36,    35,     1,    25,    64,    62,    64,
      29,    30,    23,    65,    71,    72,    64,    25,    61,    65,
      66,    68,    67,    69,    23,    62,    23,    23,    23,    28,
      27,    61,    64,    61,    29,    65,     4,    23,    61,    61
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    47,    48,    48,
      48,    48,    48,    48,    48,    48,    50,    49,    51,    49,
      52,    49,    53,    49,    54,    54,    54,    54,    54,    54,
      55,    55,    55,    55,    55,    56,    57,    57,    58,    58,
      58,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    60,    60,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    62,    62,    63,    63,    64,    64,
      65,    65,    66,    66,    67,    67,    68,    68,    69,    69,
      69,    70,    70,    70,    70,    70,    70,    70,    70,    71,
      71,    72,    72
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     6,
       7,     7,     5,     5,     6,     6,     0,     7,     0,     8,
       0,     6,     0,     7,     4,     5,     3,     4,     2,     1,
       4,     3,     5,     5,     4,     0,     3,     4,     1,     1,
       1,     3,     4,     6,     7,     6,     7,     1,     4,     4,
       1,     2,     3,     1,     1,     1,     1,     1,     7,     5,
       7,     5,     5,     3,     1,     2,     1,     4,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     2,     2,
       1,     1,     4,     3,     1,     1,     1,     2,     2,     1,
       0,     3,     1
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
  case 2: /* start: program  */
#line 298 "parser.y"
        {
		//write your code in this block in all the similar blocks below

        print_rule("start","program");

        Root = getNewNode();
        strcpy(Root->token,"start");
        Root = makeChildNode((yyvsp[0].node),Root);
	}
#line 1685 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 309 "parser.y"
                       {
            print_rule("program","program unit");
            ptnode *a = getNewNode();
            strcpy(a->token,"program");
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 1698 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 317 "parser.y"
               { 
            print_rule("program","unit");
            ptnode *a = getNewNode();
            strcpy(a->token,"program");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 1710 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 326 "parser.y"
                      { 
            print_rule("unit","var_declaration"); 
            ptnode *a = getNewNode();
            strcpy(a->token,"unit");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 1722 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 333 "parser.y"
                        { 
            print_rule("unit","func_declaration"); 
            ptnode *a = getNewNode();
            strcpy(a->token,"unit");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 1734 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 340 "parser.y"
                       { 
            print_rule("unit","func_definition");
            ptnode *a = getNewNode();
            strcpy(a->token,"unit");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 1746 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON  */
#line 349 "parser.y"
                                                                           {  
                print_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_declaration");
                a=makeChildNode((yyvsp[-5].node),a);
                a=makeChildNode((yyvsp[-4].symInfo),a);
                a=makeChildNode("LPAREN", a);
                a=makeChildNode((yyvsp[-2].node),a);
                a=makeChildNode("RPAREN", a);
                a=makeChildNode("SEMICOLON", a);
                (yyval.node) = a;        

                (yyvsp[-4].symInfo)->setDataType((yyvsp[-5].node)->token);
                (yyvsp[-4].symInfo)->token = "FUNCTION";
                (yyvsp[-4].symInfo)->isFunc = true;


                for(auto sym : temp_param_list)
                {
                    (yyvsp[-4].symInfo)->args_list.push_back(sym.data_type);
                }
                
                if(sym_tab->_insert(*(yyvsp[-4].symInfo)))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    ret_symbol->isFuncDecl = true;
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    mult_declaration_error((yyvsp[-4].symInfo)->lexeme, ret_symbol->isFuncParam);
                }


                temp_param_list.clear();
        }
#line 1788 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN error  */
#line 386 "parser.y"
                                                               { 
                
                print_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_declaration");
                a=makeChildNode((yyvsp[-5].node),a);
                a=makeChildNode((yyvsp[-4].symInfo),a);
                a=makeChildNode("LPAREN", a);
                a=makeChildNode((yyvsp[-2].node),a);
                a=makeChildNode("RPAREN", a);
                (yyval.node) = a;


                (yyvsp[-4].symInfo)->setDataType((yyvsp[-5].node)->token);
                (yyvsp[-4].symInfo)->token = "FUNCTION";
                (yyvsp[-4].symInfo)->isFunc = true;


                for(auto sym : temp_param_list)
                {
                    (yyvsp[-4].symInfo)->args_list.push_back(sym.data_type);
                }

                if(sym_tab->_insert(*(yyvsp[-4].symInfo)))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    ret_symbol->isFuncDecl = true;
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    mult_declaration_error((yyvsp[-4].symInfo)->lexeme, ret_symbol->isFuncParam);
                }


                temp_param_list.clear();    
        }
#line 1831 "y.tab.c"
    break;

  case 10: /* func_declaration: type_specifier ID LPAREN parameter_list error RPAREN SEMICOLON  */
#line 424 "parser.y"
                                                                         { 
                print_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_declaration");
                a=makeChildNode((yyvsp[-6].node),a);
                a=makeChildNode((yyvsp[-5].symInfo),a);
                a=makeChildNode("LPAREN", a);
                a=makeChildNode((yyvsp[-3].node),a);
                a=makeChildNode("RPAREN", a);
                a=makeChildNode("SEMICOLON", a);
                (yyval.node) = a;


                (yyvsp[-5].symInfo)->setDataType((yyvsp[-6].node)->token);
                (yyvsp[-5].symInfo)->token = "FUNCTION";
                (yyvsp[-5].symInfo)->isFunc = true;


                for(auto sym : temp_param_list)
                {
                    (yyvsp[-5].symInfo)->args_list.push_back(sym.data_type);
                }

                if(sym_tab->_insert(*(yyvsp[-5].symInfo)))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-5].symInfo)->lexeme);
                    ret_symbol->isFuncDecl = true;
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-5].symInfo)->lexeme);
                    mult_declaration_error((yyvsp[-5].symInfo)->lexeme, ret_symbol->isFuncParam);
                }


                temp_param_list.clear();
        }
#line 1874 "y.tab.c"
    break;

  case 11: /* func_declaration: type_specifier ID LPAREN parameter_list error RPAREN error  */
#line 462 "parser.y"
                                                                     { 
                
                print_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_declaration");
                a=makeChildNode((yyvsp[-6].node),a);
                a=makeChildNode((yyvsp[-5].symInfo),a);
                a=makeChildNode("LPAREN", a);
                a=makeChildNode((yyvsp[-3].node),a);
                a=makeChildNode("RPAREN", a);
                (yyval.node) = a;


                (yyvsp[-5].symInfo)->setDataType((yyvsp[-6].node)->token);
                (yyvsp[-5].symInfo)->token = "FUNCTION";
                (yyvsp[-5].symInfo)->isFunc = true;


                for(auto sym : temp_param_list)
                {
                    (yyvsp[-5].symInfo)->args_list.push_back(sym.data_type);
                }

                if(sym_tab->_insert(*(yyvsp[-5].symInfo)))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-5].symInfo)->lexeme);
                    ret_symbol->isFuncDecl = true;
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-5].symInfo)->lexeme);
                    mult_declaration_error((yyvsp[-5].symInfo)->lexeme, ret_symbol->isFuncParam);
                }


                temp_param_list.clear();
        }
#line 1917 "y.tab.c"
    break;

  case 12: /* func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON  */
#line 500 "parser.y"
                                                            { 

                print_rule("func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_declaration");
                a=makeChildNode((yyvsp[-4].node),a);
                a=makeChildNode((yyvsp[-3].symInfo),a);
                a=makeChildNode("LPAREN", a);
                a=makeChildNode("RPAREN", a);
                a=makeChildNode("SEMICOLON", a);
                (yyval.node) = a;


                (yyvsp[-3].symInfo)->setDataType((yyvsp[-4].node)->token);
                (yyvsp[-3].symInfo)->token = "FUNCTION";
                (yyvsp[-3].symInfo)->isFunc = true;
                
                if(sym_tab->_insert(*(yyvsp[-3].symInfo)))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-3].symInfo)->lexeme);
                    ret_symbol->isFuncDecl = true;
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-3].symInfo)->lexeme);
                    mult_declaration_error((yyvsp[-3].symInfo)->lexeme, ret_symbol->isFuncParam);
                }

                temp_param_list.clear();
            }
#line 1953 "y.tab.c"
    break;

  case 13: /* func_declaration: type_specifier ID LPAREN RPAREN error  */
#line 531 "parser.y"
                                                    { 

            print_rule("func_declaration","type_specifier ID LPAREN RPAREN");

            ptnode *a = getNewNode();
            strcpy(a->token,"func_declaration");
            a=makeChildNode((yyvsp[-4].node),a);
            a=makeChildNode((yyvsp[-3].symInfo),a);
            a=makeChildNode("LPAREN", a);
            a=makeChildNode("RPAREN", a);
            (yyval.node) = a;

            // insert function ID to SymbolTable with data_type
            (yyvsp[-3].symInfo)->setDataType((yyvsp[-4].node)->token);
            (yyvsp[-3].symInfo)->token = "FUNCTION";
            (yyvsp[-3].symInfo)->isFunc = true;
            
            if(sym_tab->_insert(*(yyvsp[-3].symInfo)))
            {
                SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-3].symInfo)->lexeme);
                ret_symbol->isFuncDecl = true;
            }
            else
            {
                SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-3].symInfo)->lexeme);
                mult_declaration_error((yyvsp[-3].symInfo)->lexeme, ret_symbol->isFuncParam);
            }

            temp_param_list.clear();
        }
#line 1988 "y.tab.c"
    break;

  case 14: /* func_declaration: type_specifier ID LPAREN error RPAREN SEMICOLON  */
#line 561 "parser.y"
                                                          { 

                print_rule("func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_declaration");
                a=makeChildNode((yyvsp[-5].node),a);
                a=makeChildNode((yyvsp[-4].symInfo),a);
                a=makeChildNode("LPAREN", a);
                a=makeChildNode("RPAREN", a);
                a=makeChildNode("SEMICOLON", a);
                (yyval.node) = a;


                (yyvsp[-4].symInfo)->setDataType((yyvsp[-5].node)->token);
                (yyvsp[-4].symInfo)->token = "FUNCTION";
                (yyvsp[-4].symInfo)->isFunc = true;
                
                if(sym_tab->_insert(*(yyvsp[-4].symInfo)))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    ret_symbol->isFuncDecl = true;
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    mult_declaration_error((yyvsp[-4].symInfo)->lexeme, ret_symbol->isFuncParam);
                }

                temp_param_list.clear();
            }
#line 2024 "y.tab.c"
    break;

  case 15: /* func_declaration: type_specifier ID LPAREN error RPAREN error  */
#line 592 "parser.y"
                                                      { 
                print_rule("func_declaration","type_specifier ID LPAREN RPAREN");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_declaration");
                a=makeChildNode((yyvsp[-5].node),a);
                a=makeChildNode((yyvsp[-4].symInfo),a);
                a=makeChildNode("LPAREN", a);
                a=makeChildNode("RPAREN", a);
                (yyval.node) = a;


                (yyvsp[-4].symInfo)->setDataType((yyvsp[-5].node)->token);
                (yyvsp[-4].symInfo)->token = "FUNCTION";
                (yyvsp[-4].symInfo)->isFunc = true;
                
                if(sym_tab->_insert(*(yyvsp[-4].symInfo)))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    ret_symbol->isFuncDecl = true;
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-4].symInfo)->lexeme);
                    mult_declaration_error((yyvsp[-4].symInfo)->lexeme, ret_symbol->isFuncParam);
                }

                temp_param_list.clear();
            }
#line 2058 "y.tab.c"
    break;

  case 16: /* $@1: %empty  */
#line 624 "parser.y"
                                                                { curr_ID_func = true;insert_to_symtable((yyvsp[-3].symInfo),(yyvsp[-4].node)->token); }
#line 2064 "y.tab.c"
    break;

  case 17: /* func_definition: type_specifier ID LPAREN parameter_list RPAREN $@1 compound_statement  */
#line 624 "parser.y"
                                                                                                                                             { 
                print_rule("func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement");
                ptnode *a = getNewNode();
                strcpy(a->token,"func_definition");
                a=makeChildNode((yyvsp[-6].node),a);
                a=makeChildNode((yyvsp[-5].symInfo),a);
                a=makeChildNode("LPAREN",a);
                a=makeChildNode((yyvsp[-3].node),a);
                a=makeChildNode("RPAREN",a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                curr_ID_func = false;
                temp_param_list.clear();
            }
#line 2085 "y.tab.c"
    break;

  case 18: /* $@2: %empty  */
#line 640 "parser.y"
                                                               { curr_ID_func = true;insert_to_symtable((yyvsp[-4].symInfo),(yyvsp[-5].node)->token); }
#line 2091 "y.tab.c"
    break;

  case 19: /* func_definition: type_specifier ID LPAREN parameter_list error RPAREN $@2 compound_statement  */
#line 640 "parser.y"
                                                                                                                                            { 
                print_rule("func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_definition");
                a=makeChildNode((yyvsp[-7].node),a);
                a=makeChildNode((yyvsp[-6].symInfo),a);
                a=makeChildNode("LPAREN",a);
                a=makeChildNode((yyvsp[-4].node),a);
                a=makeChildNode("RPAREN",a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                curr_ID_func = false;
                temp_param_list.clear();
        }
#line 2113 "y.tab.c"
    break;

  case 20: /* $@3: %empty  */
#line 657 "parser.y"
                                                    {curr_ID_func = true;insert_to_symtable((yyvsp[-2].symInfo),(yyvsp[-3].node)->token);}
#line 2119 "y.tab.c"
    break;

  case 21: /* func_definition: type_specifier ID LPAREN RPAREN $@3 compound_statement  */
#line 657 "parser.y"
                                                                                                                               { 
                print_rule("func_definition","type_specifier ID LPAREN RPAREN compound_statement");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_definition");
                a=makeChildNode((yyvsp[-5].node),a);
                a=makeChildNode((yyvsp[-4].symInfo),a);
                a=makeChildNode("LPAREN",a);
                a=makeChildNode("RPAREN",a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                curr_ID_func = false;
                temp_param_list.clear();
            }
#line 2140 "y.tab.c"
    break;

  case 22: /* $@4: %empty  */
#line 673 "parser.y"
                                                 { curr_ID_func = true;insert_to_symtable((yyvsp[-3].symInfo),(yyvsp[-4].node)->token); }
#line 2146 "y.tab.c"
    break;

  case 23: /* func_definition: type_specifier ID LPAREN error RPAREN $@4 compound_statement  */
#line 673 "parser.y"
                                                                                                                              {
                print_rule("func_definition","type_specifier ID LPAREN error RPAREN compound_statement");

                ptnode *a = getNewNode();
                strcpy(a->token,"func_definition");
                a=makeChildNode((yyvsp[-6].node),a);
                a=makeChildNode((yyvsp[-5].symInfo),a);
                a=makeChildNode("LPAREN",a);
                a=makeChildNode("RPAREN",a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;

                curr_ID_func = false;
                temp_param_list.clear();

                yyclearin;
                yyerrok;
        }
#line 2169 "y.tab.c"
    break;

  case 24: /* parameter_list: parameter_list COMMA type_specifier ID  */
#line 694 "parser.y"
                                                       {
                print_rule("parameter_list","parameter_list COMMA type_specifier ID");

                ptnode *a = getNewNode();
                strcpy(a->token,"parameter_list");
                a=makeChildNode((yyvsp[-3].node),a);
                a=makeChildNode("COMMA",a);
                a=makeChildNode((yyvsp[-1].node),a);
                a=makeChildNode((yyvsp[0].symInfo),a);
                (yyval.node) = a;


                (yyvsp[0].symInfo)->setDataType((yyvsp[-1].node)->value);
                (yyvsp[0].symInfo)->token = (yyvsp[-1].node)->token;
                (yyvsp[0].symInfo)->isFuncParam = true;
                temp_param_list.push_back(*(yyvsp[0].symInfo));
            }
#line 2191 "y.tab.c"
    break;

  case 25: /* parameter_list: parameter_list error COMMA type_specifier ID  */
#line 711 "parser.y"
                                                       {
            print_rule("parameter_list","parameter_list COMMA type_specifier ID");
        
            ptnode *a = getNewNode();
            strcpy(a->token,"parameter_list");
            a=makeChildNode((yyvsp[-4].node),a);
            a=makeChildNode("COMMA",a);
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode((yyvsp[0].symInfo),a);
            (yyval.node) = a;


            (yyvsp[0].symInfo)->setDataType((yyvsp[-1].node)->value);
            (yyvsp[0].symInfo)->token = (yyvsp[-1].node)->token;
            (yyvsp[0].symInfo)->isFuncParam = true;
            temp_param_list.push_back(*(yyvsp[0].symInfo));
        }
#line 2213 "y.tab.c"
    break;

  case 26: /* parameter_list: parameter_list COMMA type_specifier  */
#line 728 "parser.y"
                                              {
            print_rule("parameter_list","parameter_list COMMA type_specifier");
            ptnode *a = getNewNode();
            strcpy(a->token,"parameter_list");
            a=makeChildNode((yyvsp[-2].node),a);
            a=makeChildNode("COMMA",a);
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;

            SymbolInfo sym = SymbolInfo("dummy_key","dummy_value");
            sym.data_type = (yyvsp[0].node)->value;
            sym.isFuncParam = true;
            temp_param_list.push_back(sym);
        }
#line 2232 "y.tab.c"
    break;

  case 27: /* parameter_list: parameter_list error COMMA type_specifier  */
#line 742 "parser.y"
                                                    {
            print_rule("parameter_list","parameter_list COMMA type_specifier");

            ptnode *a = getNewNode();
            strcpy(a->token,"parameter_list");
            a=makeChildNode((yyvsp[-3].node),a);
            a=makeChildNode("COMMA",a);
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;

            SymbolInfo sym = SymbolInfo("dummy_key","dummy_value");
            sym.data_type = (yyvsp[0].node)->value;
            sym.isFuncParam = true;
            temp_param_list.push_back(sym);
        }
#line 2252 "y.tab.c"
    break;

  case 28: /* parameter_list: type_specifier ID  */
#line 757 "parser.y"
                                     { 
                print_rule("parameter_list","type_specifier ID");
                ptnode *a = getNewNode();
                strcpy(a->token,"parameter_list");
                a=makeChildNode((yyvsp[-1].node),a);
                a=makeChildNode((yyvsp[0].symInfo),a);
                (yyval.node) = a;


                (yyvsp[0].symInfo)->setDataType((yyvsp[-1].node)->value);
                (yyvsp[0].symInfo)->token = (yyvsp[-1].node)->token;
                (yyvsp[0].symInfo)->isFuncParam = true;
                temp_param_list.push_back(*(yyvsp[0].symInfo));
        }
#line 2271 "y.tab.c"
    break;

  case 29: /* parameter_list: type_specifier  */
#line 771 "parser.y"
                                 {
            print_rule("parameter_list","type_specifier");
            ptnode *a = getNewNode();
            strcpy(a->token,"parameter_list");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;

            SymbolInfo sym = SymbolInfo("dummy_key","dummy_value");
            sym.data_type = (yyvsp[0].node)->value;
            sym.isFuncParam = true;
            temp_param_list.push_back(sym);
        }
#line 2288 "y.tab.c"
    break;

  case 30: /* compound_statement: LCURL dummy_scope_function statements RCURL  */
#line 785 "parser.y"
                                                                {
                print_rule("compound_statement","LCURL statements RCURL");

                ptnode *a = getNewNode();
                strcpy(a->token,"compound_statement");
                a=makeChildNode("LCURL", a);
                a=makeChildNode((yyvsp[-1].node), a);
                a=makeChildNode("RCURL", a);
                (yyval.node) = a;


                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
            }
#line 2307 "y.tab.c"
    break;

  case 31: /* compound_statement: LCURL dummy_scope_function RCURL  */
#line 799 "parser.y"
                                               {

                print_rule("compound_statement","LCURL RCURL");

                ptnode *a = getNewNode();
                strcpy(a->token,"compound_statement");
                a=makeChildNode("LCURL", a);
                a=makeChildNode("RCURL", a);
                (yyval.node) = a;


                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
             }
#line 2326 "y.tab.c"
    break;

  case 32: /* compound_statement: LCURL dummy_scope_function statements error RCURL  */
#line 813 "parser.y"
                                                                {
                print_rule("compound_statement","LCURL statements RCURL");

                ptnode *a = getNewNode();
                strcpy(a->token,"compound_statement");
                a=makeChildNode("LCURL", a);
                a=makeChildNode((yyvsp[-2].node), a);
                a=makeChildNode("RCURL", a);
                (yyval.node) = a;

 
                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
            }
#line 2345 "y.tab.c"
    break;

  case 33: /* compound_statement: LCURL dummy_scope_function error statements RCURL  */
#line 827 "parser.y"
                                                                {
                print_rule("compound_statement","LCURL statements RCURL");

                ptnode *a = getNewNode();
                strcpy(a->token,"compound_statement");
                a=makeChildNode("LCURL", a);
                a=makeChildNode((yyvsp[-1].node), a);
                a=makeChildNode("RCURL", a);
                (yyval.node) = a;


                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
            }
#line 2364 "y.tab.c"
    break;

  case 34: /* compound_statement: LCURL dummy_scope_function error RCURL  */
#line 841 "parser.y"
                                                      {
                
                print_rule("compound_statement","LCURL error RCURL");

                ptnode *a = getNewNode();
                strcpy(a->token,"compound_statement");
                a=makeChildNode("LCURL", a);
                a=makeChildNode("RCURL", a);
                (yyval.node) = a;


                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
             }
#line 2383 "y.tab.c"
    break;

  case 35: /* dummy_scope_function: %empty  */
#line 856 "parser.y"
                       {

                    sym_tab->enterScope(); 

                    if(curr_ID_func)
                    {
                        for(auto el:temp_param_list)
                        {

                            if(el.lexeme == "dummy_key") continue;
                            if(!sym_tab->_insert(el))
                            {
                                mult_declaration_error(el.lexeme,el.isFuncParam);
                            }
                        }
                    }
                }
#line 2405 "y.tab.c"
    break;

  case 36: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 875 "parser.y"
                                                           { 

            print_rule("var_declaration","type_specifier declaration_list SEMICOLON");
            ptnode *a = getNewNode();
            strcpy(a->token,"var_declaration");
            a=makeChildNode((yyvsp[-2].node),a);
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode("SEMICOLON",a);
            (yyval.node) = a;

            if(!strcmp((yyvsp[-2].node)->value,"void")){
                fprintf(errout, "Line# %d: Variable or field \'%s\' declared void\n", line_count, (((yyvsp[-1].node)->args_info_list[0])->lexeme).data());
                err_count++;
            }
            else{
                for(auto el:(yyvsp[-1].node)->args_info_list)
                {
                    if(el->data_type == "array") { el->setDataType(strcat((yyvsp[-2].node)->value, "_array")) ; el->token = "ARRAY";}
                    else { el->setDataType((yyvsp[-2].node)->value); el->token = (yyvsp[-2].node)->token; }
                    
                    if(el->token == "INT")  el->setDataType("int");
                    else if(el->token == "FLOAT")  el->setDataType("float");

                    if(!sym_tab->_insert(*el))
                    {
                        if(!strcmp(el->data_type.c_str(),(yyvsp[-2].node)->value)) mult_declaration_error(el->lexeme, el->isFuncParam);
                        else type_conflict_error(el->lexeme);
                    }
                }
            }
        }
#line 2441 "y.tab.c"
    break;

  case 37: /* var_declaration: type_specifier declaration_list error SEMICOLON  */
#line 906 "parser.y"
                                                          {       
            print_rule("var_declaration","type_specifier declaration_list SEMICOLON");

            ptnode *a = getNewNode();
            strcpy(a->token,"var_declaration");
            a=makeChildNode((yyvsp[-3].node),a);
            a=makeChildNode((yyvsp[-2].node),a);
            a=makeChildNode("SEMICOLON",a);
            (yyval.node) = a;
            
            for(auto el:(yyvsp[-2].node)->args_info_list)
            {
                if(el->data_type == "array") {el->setDataType(strcat((yyvsp[-3].node)->value,"_array")) ; el->token = "ARRAY";}
                else {el->setDataType((yyvsp[-3].node)->value); el->token = (yyvsp[-3].node)->value;}
                
                if(!sym_tab->_insert(*el))
                {
                    if(!strcmp(el->data_type.c_str(),(yyvsp[-3].node)->value)) mult_declaration_error(el->lexeme, el->isFuncParam);
                    else type_conflict_error(el->lexeme);
                }

            }
        }
#line 2469 "y.tab.c"
    break;

  case 38: /* type_specifier: INT  */
#line 931 "parser.y"
                     { 
                    print_rule("type_specifier","INT"); 

                    ptnode *a = getNewNode();
                    strcpy(a->token,"type_specifier");
                    a=makeChildNode((yyvsp[0].symInfo),a);
                    (yyval.node) = a;
                    strcpy((yyval.node)->value,"int");
                    strcpy((yyval.node)->token,"INT");
                }
#line 2484 "y.tab.c"
    break;

  case 39: /* type_specifier: FLOAT  */
#line 941 "parser.y"
                        { 
                    print_rule("type_specifier","FLOAT"); 

                    ptnode *a = getNewNode();
                    strcpy(a->token,"type_specifier");
                    a=makeChildNode((yyvsp[0].symInfo),a);
                    (yyval.node) = a;
                    strcpy((yyval.node)->value,"float");
                    strcpy((yyval.node)->token,"FLOAT");
                }
#line 2499 "y.tab.c"
    break;

  case 40: /* type_specifier: VOID  */
#line 951 "parser.y"
                       { 
                    print_rule("type_specifier","VOID"); 
                    ptnode *a = getNewNode();
                    strcpy(a->token,"type_specifier");
                    a=makeChildNode((yyvsp[0].symInfo),a);
                    (yyval.node) = a;
                    strcpy((yyval.node)->value,"void");
                    strcpy((yyval.node)->token,"VOID");
                }
#line 2513 "y.tab.c"
    break;

  case 41: /* declaration_list: declaration_list COMMA ID  */
#line 962 "parser.y"
                                            { 
                    print_rule("declaration_list","declaration_list COMMA ID");
                    
                    ptnode *a = getNewNode();
                    strcpy(a->token,"declaration_list");
                    a=makeChildNode((yyvsp[-2].node),a);
                    a=makeChildNode("COMMA",a);
                    a=makeChildNode((yyvsp[0].symInfo),a);
                    (yyval.node) = a;

                    
                    strcpy((yyval.node)->resultType, (yyvsp[-2].node)->resultType);

                    
                    (yyval.node)->args_info_list= (yyvsp[-2].node)->args_info_list;
                    (yyval.node)->args_info_list.push_back((yyvsp[0].symInfo));
            }
#line 2535 "y.tab.c"
    break;

  case 42: /* declaration_list: declaration_list error COMMA ID  */
#line 979 "parser.y"
                                              {
                print_rule("declaration_list","declaration_list COMMA ID");

                ptnode *a = getNewNode();
                strcpy(a->token,"declaration_list");
                a=makeChildNode((yyvsp[-3].node),a);
                a=makeChildNode("COMMA",a);
                a=makeChildNode((yyvsp[0].symInfo),a);
                (yyval.node) = a;

                
                strcpy((yyval.node)->resultType, (yyvsp[-3].node)->resultType);

                
                (yyval.node)->args_info_list= (yyvsp[-3].node)->args_info_list;
                (yyval.node)->args_info_list.push_back((yyvsp[0].symInfo));
                fprintf(errout, "Line# %d: Syntax error at declaration list of variable declaration\n", line_count);
                err_count++;
            }
#line 2559 "y.tab.c"
    break;

  case 43: /* declaration_list: declaration_list COMMA ID LTHIRD CONST_INT RTHIRD  */
#line 998 "parser.y"
                                                                        {
                print_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");

                ptnode *a = getNewNode();
                strcpy(a->token,"declaration_list");
                a=makeChildNode((yyvsp[-5].node),a);
                a=makeChildNode("COMMA",a);
                a=makeChildNode((yyvsp[-3].symInfo),a);
                a=makeChildNode("LTHIRD",a);
                a=makeChildNode((yyvsp[-1].symInfo),a);
                a=makeChildNode("RTHIRD",a);
                (yyval.node) = a;

                
                strcpy((yyval.node)->resultType, (yyvsp[-5].node)->resultType);

                
                (yyval.node)->args_info_list= (yyvsp[-5].node)->args_info_list;
                (yyvsp[-3].symInfo)->token ="ARRAY";
                (yyvsp[-3].symInfo)->data_type ="array";
                (yyval.node)->args_info_list.push_back((yyvsp[-3].symInfo));
           }
#line 2586 "y.tab.c"
    break;

  case 44: /* declaration_list: declaration_list error COMMA ID LTHIRD CONST_INT RTHIRD  */
#line 1020 "parser.y"
                                                                     {
                print_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");

                ptnode *a = getNewNode();
                strcpy(a->token,"declaration_list");
                a=makeChildNode((yyvsp[-6].node),a);
                a=makeChildNode("COMMA",a);
                a=makeChildNode((yyvsp[-3].symInfo),a);
                a=makeChildNode("LTHIRD",a);
                a=makeChildNode((yyvsp[-1].symInfo),a);
                a=makeChildNode("RTHIRD",a);
                (yyval.node) = a;

                
                strcpy((yyval.node)->resultType, (yyvsp[-6].node)->resultType);

                
                (yyval.node)->args_info_list= (yyvsp[-6].node)->args_info_list;
                (yyvsp[-3].symInfo)->token ="ARRAY";
                (yyvsp[-3].symInfo)->data_type ="array";
                (yyval.node)->args_info_list.push_back((yyvsp[-3].symInfo));
           }
#line 2613 "y.tab.c"
    break;

  case 45: /* declaration_list: declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD  */
#line 1042 "parser.y"
                                                                 {
                print_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD");

                ptnode *a = getNewNode();
                strcpy(a->token,"declaration_list");
                a=makeChildNode((yyvsp[-5].node),a);
                a=makeChildNode("COMMA",a);
                a=makeChildNode((yyvsp[-3].symInfo),a);
                a=makeChildNode("LTHIRD",a);
                a=makeChildNode((yyvsp[-1].symInfo),a);
                a=makeChildNode("RTHIRD",a);
                (yyval.node) = a;

                
                strcpy((yyval.node)->resultType, (yyvsp[-5].node)->resultType);

                
                (yyval.node)->args_info_list= (yyvsp[-5].node)->args_info_list;
                (yyval.node)->args_info_list.push_back((yyvsp[-3].symInfo));

                fprintf(errout, "Line# %d: Non-integer Array Size\n", line_count);
                err_count++;
            }
#line 2641 "y.tab.c"
    break;

  case 46: /* declaration_list: declaration_list error COMMA ID LTHIRD CONST_FLOAT RTHIRD  */
#line 1065 "parser.y"
                                                                        {
                print_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD");

                ptnode *a = getNewNode();
                strcpy(a->token,"declaration_list");
                a=makeChildNode((yyvsp[-6].node),a);
                a=makeChildNode("COMMA",a);
                a=makeChildNode((yyvsp[-3].symInfo),a);
                a=makeChildNode("LTHIRD",a);
                a=makeChildNode((yyvsp[-1].symInfo),a);
                a=makeChildNode("RTHIRD",a);
                (yyval.node) = a;

                
                strcpy((yyval.node)->resultType, (yyvsp[-6].node)->resultType);

                
                (yyval.node)->args_info_list= (yyvsp[-6].node)->args_info_list;
                (yyval.node)->args_info_list.push_back((yyvsp[-3].symInfo));

                fprintf(errout, "Line# %d: Non-integer Array Size\n", line_count);
                err_count++;
            }
#line 2669 "y.tab.c"
    break;

  case 47: /* declaration_list: ID  */
#line 1088 "parser.y"
                         {     
                    print_rule("declaration_list","ID");

                    ptnode *a = getNewNode();
                    strcpy(a->token,"declaration_list");
                    a=makeChildNode((yyvsp[0].symInfo),a);
                    (yyval.node) = a;

                    
                    (yyval.node)->args_info_list.push_back((yyvsp[0].symInfo));
            }
#line 2685 "y.tab.c"
    break;

  case 48: /* declaration_list: ID LTHIRD CONST_INT RTHIRD  */
#line 1099 "parser.y"
                                                 {

                    print_rule("declaration_list","ID LTHIRD CONST_INT RTHIRD");

                    ptnode *a = getNewNode();
                    strcpy(a->token,"declaration_list");
                    a=makeChildNode((yyvsp[-3].symInfo),a);
                    a=makeChildNode("LTHIRD",a);
                    a=makeChildNode((yyvsp[-1].symInfo),a);
                    a=makeChildNode("RTHIRD",a);
                    (yyval.node) = a;

                    
                    (yyvsp[-3].symInfo)->token = "ARRAY";
                    (yyvsp[-3].symInfo)->data_type ="array";
                    (yyval.node)->args_info_list.push_back((yyvsp[-3].symInfo));
            }
#line 2707 "y.tab.c"
    break;

  case 49: /* declaration_list: ID LTHIRD CONST_FLOAT RTHIRD  */
#line 1116 "parser.y"
                                           {
                    print_rule("declaration_list","ID LTHIRD CONST_FLOAT RTHIRD");

                    ptnode *a = getNewNode();
                    strcpy(a->token,"declaration_list");
                    a=makeChildNode((yyvsp[-3].symInfo),a);
                    a=makeChildNode("LTHIRD",a);
                    a=makeChildNode((yyvsp[-1].symInfo),a);
                    a=makeChildNode("RTHIRD",a);
                    (yyval.node) = a;

                    
                    (yyval.node)->args_info_list.push_back((yyvsp[-3].symInfo));

                    fprintf(errout, "Line# %d: Non-integer Array Size\n", line_count);
                    err_count++;
           }
#line 2729 "y.tab.c"
    break;

  case 50: /* statements: statement  */
#line 1135 "parser.y"
                      {
            print_rule("statements","statement");
            ptnode *a = getNewNode();
            strcpy(a->token,"statements");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
            
        }
#line 2742 "y.tab.c"
    break;

  case 51: /* statements: statements statement  */
#line 1143 "parser.y"
                                  {
            print_rule("statements","statements statement");
        
            ptnode *a = getNewNode();
            strcpy(a->token,"statements");
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 2756 "y.tab.c"
    break;

  case 52: /* statements: statements error statement  */
#line 1152 "parser.y"
                                     {
            print_rule("statements","statements statement");
        
            ptnode *a = getNewNode();
            strcpy(a->token,"statements");
            a=makeChildNode((yyvsp[-2].node),a);
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 2770 "y.tab.c"
    break;

  case 53: /* statement: var_declaration  */
#line 1164 "parser.y"
                           {
            print_rule("statement","var_declaration");

            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 2783 "y.tab.c"
    break;

  case 54: /* statement: func_definition  */
#line 1172 "parser.y"
                        {
            print_rule("statement","func_definition");

            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;

            fprintf(errout, "Line# %d: A function cannot be defined inside another function\n", line_count);
            err_count++;
      }
#line 2799 "y.tab.c"
    break;

  case 55: /* statement: func_declaration  */
#line 1183 "parser.y"
                         {
            print_rule("statement","func_declaration");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;

            fprintf(errout, "Line# %d: A function cannot be defined inside another function\n", line_count);
            err_count++;
      }
#line 2814 "y.tab.c"
    break;

  case 56: /* statement: expression_statement  */
#line 1193 "parser.y"
                                 {
            print_rule("statement","expression_statement");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 2826 "y.tab.c"
    break;

  case 57: /* statement: compound_statement  */
#line 1200 "parser.y"
                               {
            print_rule("statement","compound_statement");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 2838 "y.tab.c"
    break;

  case 58: /* statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement  */
#line 1207 "parser.y"
                                                                                             {
            print_rule("statement","FOR LPAREN expression_statement expression_statement expression RPAREN statement");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[-6].string), "FOR", a);
            a=makeChildNode("LPAREN", a);
            a=makeChildNode((yyvsp[-4].node), a);
            a=makeChildNode((yyvsp[-3].node), a);
            a=makeChildNode((yyvsp[-2].node), a);
            a=makeChildNode("RPAREN", a);
            a=makeChildNode((yyvsp[0].node), a);
            (yyval.node) = a;
        }
#line 2856 "y.tab.c"
    break;

  case 59: /* statement: IF LPAREN expression RPAREN statement  */
#line 1220 "parser.y"
                                                                        { 
            print_rule("statement","IF LPAREN expression RPAREN statement");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[-4].string), "IF",a);
            a=makeChildNode("LPAREN",a);
            a=makeChildNode((yyvsp[-2].node),a);
            a=makeChildNode("RPAREN",a);
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;
        }
#line 2872 "y.tab.c"
    break;

  case 60: /* statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 1231 "parser.y"
                                                                 {

            print_rule("statement","IF LPAREN expression RPAREN statement ELSE statement");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[-6].string), "IF",a);
            a=makeChildNode("LPAREN",a);
            a=makeChildNode((yyvsp[-4].node),a);
            a=makeChildNode("RPAREN",a);
            a=makeChildNode((yyvsp[-6].string), "ELSE",a);
            a=makeChildNode((yyvsp[-2].node),a);
            (yyval.node) = a;
        
        }
#line 2891 "y.tab.c"
    break;

  case 61: /* statement: WHILE LPAREN expression RPAREN statement  */
#line 1245 "parser.y"
                                                     {
            print_rule("statement","WHILE LPAREN expression RPAREN statement");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[-4].string),"WHILE",a);
            (yyval.node) = a;
        }
#line 2903 "y.tab.c"
    break;

  case 62: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 1252 "parser.y"
                                               {
            print_rule("statement","PRINTLN LPAREN ID RPAREN SEMICOLON");

            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode((yyvsp[-4].string),"PRINTLN",a);
            a=makeChildNode("LPAREN",a);
            a=makeChildNode((yyvsp[-2].symInfo),a);
            a=makeChildNode("RPAREN",a);
            a=makeChildNode("SEMICOLON",a);
            (yyval.node) = a;


            SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-2].symInfo)->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared variable \'%s\'\n", line_count, ((yyvsp[-2].symInfo)->lexeme).data());
                err_count++;
                strcpy((yyval.node)->resultType,"NULL");
            }
            
        }
#line 2931 "y.tab.c"
    break;

  case 63: /* statement: RETURN expression SEMICOLON  */
#line 1275 "parser.y"
                                        {
            print_rule("statement","RETURN expression SEMICOLON");
            ptnode *a = getNewNode();
            strcpy(a->token,"statement");
            a=makeChildNode("RETURN",a);
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode("SEMICOLON",a);
            (yyval.node) = a;
        }
#line 2945 "y.tab.c"
    break;

  case 64: /* expression_statement: SEMICOLON  */
#line 1286 "parser.y"
                                {
                    print_rule("expression_statement","SEMICOLON");
                    ptnode *a = getNewNode();
                    strcpy(a->token,"expression_statement");
                    a=makeChildNode("SEMICOLON",a);
                    (yyval.node) = a;
                }
#line 2957 "y.tab.c"
    break;

  case 65: /* expression_statement: expression SEMICOLON  */
#line 1293 "parser.y"
                                               {
                    print_rule("expression_statement","expression SEMICOLON");
                    ptnode *a = getNewNode();
                    strcpy(a->token,"expression_statement");
                    a=makeChildNode((yyvsp[-1].node),a);
                    a=makeChildNode("SEMICOLON",a);
                    (yyval.node) = a;
                }
#line 2970 "y.tab.c"
    break;

  case 66: /* variable: ID  */
#line 1303 "parser.y"
             { 
            print_rule("variable","ID");

            ptnode *a = getNewNode();
            strcpy(a->token,"variable");
            a=makeChildNode((yyvsp[0].symInfo),a);
            (yyval.node) = a;


            SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[0].symInfo)->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared variable \'%s\'\n", line_count, ((yyvsp[0].symInfo)->lexeme).data());
                err_count++;
                strcpy((yyval.node)->resultType, "NULL");
            }
            else
            {
                if(ret_symbol->data_type == "int_array" || ret_symbol->data_type == "float_array")
                {
                    type_conflict_error(ret_symbol->lexeme);
                    strcpy((yyval.node)->resultType, "NULL");
                }
                else{
                    strcpy((yyval.node)->resultType, ret_symbol->data_type.c_str());
                }
            }

        }
#line 3005 "y.tab.c"
    break;

  case 67: /* variable: ID LTHIRD expression RTHIRD  */
#line 1333 "parser.y"
                                       {
            print_rule("variable","ID LTHIRD expression RTHIRD");

            ptnode *a = getNewNode();
            strcpy(a->token,"variable");
            a=makeChildNode((yyvsp[-3].symInfo),a);
            a=makeChildNode("LTHIRD",a);
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode("RTHIRD",a);
            (yyval.node) = a;


            SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-3].symInfo)->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared variable \'%s\'\n", line_count, ((yyvsp[-3].symInfo)->lexeme).data());
                err_count++;
                strcpy((yyval.node)->resultType, "NULL");
            }
            else
            {
                if(ret_symbol->data_type == "int" || ret_symbol->data_type == "float")
                {
                    fprintf(errout, "Line# %d: \'%s\' is not an array\n", line_count, (ret_symbol->lexeme).data());
                    err_count++;
                    strcpy((yyval.node)->resultType, "NULL");
                }
                else{
                    strcpy((yyval.node)->resultType, ret_symbol->data_type.c_str());
                }
            }

            if(strcmp((yyvsp[-1].node)->resultType, "int"))
            {
                fprintf(errout, "Line# %d: Array subscript is not an integer\n", line_count);
                err_count++;
            }
         }
#line 3049 "y.tab.c"
    break;

  case 68: /* expression: logic_expression  */
#line 1374 "parser.y"
                                {
                print_rule("expression","logic_expression");
                ptnode *a = getNewNode();
                strcpy(a->token,"expression");
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
            }
#line 3064 "y.tab.c"
    break;

  case 69: /* expression: variable ASSIGNOP logic_expression  */
#line 1384 "parser.y"
                                                {
                print_rule("expression","variable ASSIGNOP logic_expression");

                ptnode *a = getNewNode();
                strcpy(a->token,"expression");
                a=makeChildNode((yyvsp[-2].node),a);
                a=makeChildNode("ASSIGNOP",a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;

                if(!is_assignment_valid((yyvsp[-2].node)->resultType,(yyvsp[0].node)->resultType))
                {
                    if(strcmp((yyvsp[-2].node)->resultType,"void")==0 || strcmp((yyvsp[0].node)->resultType,"void")==0)
                    {
                        void_expression_error();
                    }
                    else if((!strcmp((yyvsp[-2].node)->resultType,"int") || !strcmp((yyvsp[-2].node)->resultType,"int_array")) && 
                            (!strcmp((yyvsp[0].node)->resultType,"float") || !strcmp((yyvsp[0].node)->resultType,"float_array")))
                    {
                        fprintf(errout, "Line# %d: Warning: possible loss of data in assignment of FLOAT to INT\n", line_count);
                        err_count++;
                    }
                }
            }
#line 3093 "y.tab.c"
    break;

  case 70: /* logic_expression: rel_expression  */
#line 1412 "parser.y"
                                 {
                print_rule("logic_expression","rel_expression"); 
                ptnode *a = getNewNode();
                strcpy(a->token,"logic_expression");
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
            }
#line 3108 "y.tab.c"
    break;

  case 71: /* logic_expression: rel_expression LOGICOP rel_expression  */
#line 1422 "parser.y"
                                                         {
                print_rule("logic_expression","rel_expression LOGICOP rel_expression");

                ptnode *a = getNewNode();
                strcpy(a->token,"logic_expression");
                a=makeChildNode((yyvsp[-2].node),a);
                a=makeChildNode((yyvsp[-1].symInfo),a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                string typecast_ret = typecast((yyvsp[-2].node)->resultType,(yyvsp[0].node)->resultType);

                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") strcpy((yyval.node)->resultType,"int");
                    else {

                        if(!strcmp((yyvsp[-2].node)->resultType, "void") || !strcmp((yyvsp[0].node)->resultType, "void"))
                        {
                            void_expression_error();
                        }
                        else
                        {
                            typecast_error();
                        }

                        strcpy((yyval.node)->resultType, "NULL");
                    }
                }
                else
                {
                    strcpy((yyval.node)->resultType, "NULL");
                }
            }
#line 3148 "y.tab.c"
    break;

  case 72: /* rel_expression: simple_expression  */
#line 1459 "parser.y"
                                  {
                print_rule("rel_expression","simple_expression");
                ptnode *a = getNewNode();
                strcpy(a->token,"rel_expression");
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
            }
#line 3163 "y.tab.c"
    break;

  case 73: /* rel_expression: simple_expression RELOP simple_expression  */
#line 1469 "parser.y"
                                                                {
                print_rule("rel_expression","simple_expression RELOP simple_expression");

                ptnode *a = getNewNode();
                strcpy(a->token,"rel_expression");
                a=makeChildNode((yyvsp[-2].node),a);
                a=makeChildNode((yyvsp[-1].symInfo),a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                string typecast_ret = typecast((yyvsp[-2].node)->resultType,(yyvsp[0].node)->resultType);

                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") strcpy((yyval.node)->resultType, "int");
                    else {

                        if(!strcmp((yyvsp[-2].node)->resultType, "void") || !strcmp((yyvsp[0].node)->resultType, "void"))
                        {
                            void_expression_error();
                        }
                        else
                        {
                            typecast_error();
                        }

                        strcpy((yyval.node)->resultType, "NULL");
                    }
                }
                else
                {
                    strcpy((yyval.node)->resultType, "NULL");
                }
            }
#line 3203 "y.tab.c"
    break;

  case 74: /* simple_expression: term  */
#line 1506 "parser.y"
                        {

                    print_rule("simple_expression","term");
                    ptnode *a = getNewNode();
                    strcpy(a->token,"simple_expression");
                    a=makeChildNode((yyvsp[0].node),a);
                    (yyval.node) = a;

    
                    strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
            }
#line 3219 "y.tab.c"
    break;

  case 75: /* simple_expression: simple_expression ADDOP term  */
#line 1517 "parser.y"
                                                     {
                    print_rule("simple_expression","simple_expression ADDOP term");

                    ptnode *a = getNewNode();
                    strcpy(a->token,"simple_expression");
                    a=makeChildNode((yyvsp[-2].node),a);
                    a=makeChildNode((yyvsp[-1].symInfo),a);
                    a=makeChildNode((yyvsp[0].node),a);
                    (yyval.node) = a;

                    string typecast_ret = typecast((yyvsp[-2].node)->resultType,(yyvsp[0].node)->resultType);

                    if(typecast_ret != "NULL")
                    {
                        if(typecast_ret != "error")     strcpy((yyval.node)->resultType, typecast_ret.c_str());
                        else {
                            if(!strcmp((yyvsp[-2].node)->resultType, "void") || !strcmp((yyvsp[0].node)->resultType, "void"))
                            {
                                void_expression_error();
                            }
                            else
                            {
                                typecast_error();
                            }

                            strcpy((yyval.node)->resultType, "NULL");
                        }
                    }
                    else
                    {
                        strcpy((yyval.node)->resultType, "NULL");
                    }
            }
#line 3257 "y.tab.c"
    break;

  case 76: /* term: unary_expression  */
#line 1552 "parser.y"
                         {

            print_rule("term","unary_expression");
            ptnode *a = getNewNode();
            strcpy(a->token,"term");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;


            strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
    }
#line 3273 "y.tab.c"
    break;

  case 77: /* term: term MULOP unary_expression  */
#line 1563 "parser.y"
                                   {

            print_rule("term","term MULOP unary_expression");

            ptnode *a = getNewNode();
            strcpy(a->token,"term");
            a=makeChildNode((yyvsp[-2].node),a);
            a=makeChildNode((yyvsp[-1].symInfo),a);
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;


            string typecast_ret = typecast((yyvsp[-2].node)->resultType,(yyvsp[0].node)->resultType);

            if((yyvsp[-1].symInfo)->lexeme == "%")
            {
                if(!strcmp((yyvsp[0].node)->value,"0"))
                {
                    fprintf(errout, "Line# %d: Warning: modulus by zero\n", line_count);
                    err_count++;
                    strcpy((yyval.node)->resultType, "NULL");
                }
                else
                {
                    if(typecast_ret != "int")
                    {
                        fprintf(errout, "Line# %d: Operands of modulus must be integers\n", line_count);
                        err_count++;
                        strcpy((yyval.node)->resultType, "NULL");
                    }
                    else{
                        strcpy((yyval.node)->resultType, "int");
                    }
                }
            }
            else
            {
                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") strcpy((yyval.node)->resultType, typecast_ret.c_str());
                    else {
                        if(strcmp((yyvsp[-2].node)->resultType, "void")==0 || strcmp((yyvsp[0].node)->resultType, "void")==0)
                        {
                            void_expression_error();
                        }
                        else
                        {
                            typecast_error();
                        }

                        strcpy((yyval.node)->resultType, "NULL");
                    }
                }
                else
                {
                    strcpy((yyval.node)->resultType, "NULL");
                }
            }
    }
#line 3337 "y.tab.c"
    break;

  case 78: /* unary_expression: ADDOP unary_expression  */
#line 1624 "parser.y"
                                          {
                print_rule("unary_expression","ADDOP unary_expression");
                ptnode *a = getNewNode();
                strcpy(a->token,"unary_expression");
                a=makeChildNode((yyvsp[-1].symInfo),a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;
    
                strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
            }
#line 3352 "y.tab.c"
    break;

  case 79: /* unary_expression: NOT unary_expression  */
#line 1634 "parser.y"
                                           {
                print_rule("unary_expression","NOT unary_expression");
                ptnode *a = getNewNode();
                strcpy(a->token,"unary_expression");
                a=makeChildNode("NOT",a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;
    
                strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
            }
#line 3367 "y.tab.c"
    break;

  case 80: /* unary_expression: factor  */
#line 1644 "parser.y"
                              { 
                print_rule("unary_expression","factor");
                ptnode *a = getNewNode();
                strcpy(a->token,"unary_expression");
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;
                strcpy((yyval.node)->value, (yyvsp[0].node)->value);
    
                strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
            }
#line 3382 "y.tab.c"
    break;

  case 81: /* factor: variable  */
#line 1656 "parser.y"
                 {

            print_rule("factor","variable");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode((yyvsp[0].node),a);
            (yyval.node) = a;


            strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
        }
#line 3398 "y.tab.c"
    break;

  case 82: /* factor: ID LPAREN argument_list RPAREN  */
#line 1667 "parser.y"
                                         {

            print_rule("factor","ID LPAREN argument_list RPAREN");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode((yyvsp[-3].symInfo),a);
            a=makeChildNode("LPAREN",a);
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode("RPAREN",a);
            (yyval.node) = a;


            SymbolInfo* ret_symbol = sym_tab->_search((yyvsp[-3].symInfo)->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared function \'%s\'\n", line_count, ((yyvsp[-3].symInfo)->lexeme).data());
                err_count++;
                strcpy((yyval.node)->resultType,"NULL");
            }
            else
            {
                if(ret_symbol->isFunc == false)
                {
                    strcpy((yyval.node)->resultType,"NULL");
                    fprintf(errout, "Line# %d: %s not a function\n", line_count, ((yyvsp[-3].symInfo)->lexeme).data());
                    err_count++;
                }

                //convert data_type to lowercase and pass it up as resultType of 'factor'
                char *temp = new char(100);
                strcpy(temp, ret_symbol->data_type.c_str());
                for(int i = 0; i < 100; i++)    temp[i] = tolower(temp[i]);
                strcpy((yyval.node)->resultType, temp);
                free(temp);
                //

                if(ret_symbol->isFuncDecl)
                {
                    fprintf(errout, "Line# %d: Function declared, but not defined\n", line_count);
                    err_count++;
                }
                else
                {
                    if(((yyvsp[-1].node)->args_list.size() - ret_symbol->args_list.size()) > 0){
                        fprintf(errout, "Line# %d: Too many arguments to function \'%s\'\n", line_count, (ret_symbol->lexeme).data());
                        err_count++;
                    }
                    else if(((yyvsp[-1].node)->args_list.size() - ret_symbol->args_list.size()) < 0){
                        fprintf(errout, "Line# %d: Too few arguments to function \'%s\'\n", line_count, (ret_symbol->lexeme).data());
                        err_count++;
                    }
                    else
                    {
                        for(int i=0;i<ret_symbol->args_list.size();i++)
                        {
                            if(!is_typecast_valid(ret_symbol->args_list[i], (yyvsp[-1].node)->args_list[i])){
                                fprintf(errout, "Line# %d: Type mismatch for argument %d of \'%s\'\n", line_count, (i+1), (ret_symbol->lexeme).data());
                                err_count++;
                            }
                        }
                    }
                }
            }
        }
#line 3468 "y.tab.c"
    break;

  case 83: /* factor: LPAREN expression RPAREN  */
#line 1732 "parser.y"
                                   {

            print_rule("factor","LPAREN expression RPAREN");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode("LPAREN",a);
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode("RPAREN",a);
            (yyval.node) = a;
        
            strcpy((yyval.node)->resultType, (yyvsp[-1].node)->resultType);
        }
#line 3485 "y.tab.c"
    break;

  case 84: /* factor: CONST_INT  */
#line 1744 "parser.y"
                     { 
            print_rule("factor","CONST_INT");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode((yyvsp[0].symInfo),a);
            (yyval.node) = a;
            strcpy((yyval.node)->value, (yyvsp[0].symInfo)->lexeme.c_str());
            strcpy((yyval.node)->resultType, "int");
        }
#line 3499 "y.tab.c"
    break;

  case 85: /* factor: CONST_FLOAT  */
#line 1753 "parser.y"
                       { 
            print_rule("factor","CONST_FLOAT");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode((yyvsp[0].symInfo),a);
            (yyval.node) = a;
            strcpy((yyval.node)->value, (yyvsp[0].symInfo)->lexeme.c_str());
            strcpy((yyval.node)->resultType, "float");
        }
#line 3513 "y.tab.c"
    break;

  case 86: /* factor: ERROR_FLOAT  */
#line 1762 "parser.y"
                   { 
            print_rule("factor","ERROR_FLOAT");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode((yyvsp[0].symInfo),a);
            (yyval.node) = a;
            strcpy((yyval.node)->value, (yyvsp[0].symInfo)->lexeme.c_str());
            strcpy((yyval.node)->resultType, "NULL");
        }
#line 3527 "y.tab.c"
    break;

  case 87: /* factor: variable INCOP  */
#line 1771 "parser.y"
                         {
            print_rule("factor","variable INCOP");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode("INCOP",a);
            (yyval.node) = a;
        }
#line 3540 "y.tab.c"
    break;

  case 88: /* factor: variable DECOP  */
#line 1779 "parser.y"
                         {
            print_rule("factor","variable DECOP");
            ptnode *a = getNewNode();
            strcpy(a->token,"factor");
            a=makeChildNode((yyvsp[-1].node),a);
            a=makeChildNode("DECOP",a);
            (yyval.node) = a;
        }
#line 3553 "y.tab.c"
    break;

  case 89: /* argument_list: arguments  */
#line 1789 "parser.y"
                         {
                    print_rule("argument_list","arguments");
                    ptnode *a = getNewNode();
                    strcpy(a->token,"argument_list");
                    a=makeChildNode((yyvsp[0].node),a);
                    (yyval.node) = a;
                    (yyval.node)->args_list = (yyvsp[0].node)->args_list; 
                }
#line 3566 "y.tab.c"
    break;

  case 90: /* argument_list: %empty  */
#line 1797 "parser.y"
                          {
                print_rule("argument_list","");
                ptnode *a = getNewNode();
                strcpy(a->token,"argument_list");
                (yyval.node) = a;
            }
#line 3577 "y.tab.c"
    break;

  case 91: /* arguments: arguments COMMA logic_expression  */
#line 1805 "parser.y"
                                            {

                print_rule("arguments","arguments COMMA logic_expression");
                ptnode *a = getNewNode();
                strcpy(a->token,"arguments");
                a=makeChildNode((yyvsp[-2].node),a);
                a=makeChildNode("COMMA",a);
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                (yyval.node)->args_list = (yyvsp[-2].node)->args_list; 
                (yyval.node)->args_list.push_back((yyvsp[0].node)->resultType);
            }
#line 3596 "y.tab.c"
    break;

  case 92: /* arguments: logic_expression  */
#line 1819 "parser.y"
                               {

                print_rule("arguments","logic_expression");
                ptnode *a = getNewNode();
                strcpy(a->token,"arguments");
                a=makeChildNode((yyvsp[0].node),a);
                (yyval.node) = a;


                strcpy((yyval.node)->resultType, (yyvsp[0].node)->resultType);
                (yyval.node)->args_list.push_back((yyvsp[0].node)->resultType);
            }
#line 3613 "y.tab.c"
    break;


#line 3617 "y.tab.c"

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

#line 1833 "parser.y"


main(int argc,char *argv[])
{
    if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

    logout = fopen("log.txt", "w");
	errout = fopen("error.txt", "w");
    parseout = fopen("parse.txt", "w");

    yyin=fin;
	yyparse();
    if(Root != NULL) printtree(Root);

    fprintf(logout, "Total lines: %d\n", line_count);
    fprintf(logout, "Total errors: %d\n", err_count);

    fclose(yyin);
    fclose(parseout);
    fclose(logout);
	fclose(errout);

    exit(0);
}
