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
#line 5 "1705010.y"

#include<bits/stdc++.h>
#include <typeinfo>
#include <fstream>
using namespace std;

#include "SymbolTable.h"
#include "SymbolInfo.h"
#include "ScopeTable.h"
#include "Helper.h"

FILE* logout;
FILE* errout;
FILE* parseout;

extern int line_count;
int err_count = 0;

extern FILE *yyin;

void yyerror(string s){
    fprintf(errout, "Line# %d: %s\n", line_count, s.data());
    err_count++;
}

int yyparse(void);
int yylex(void);



//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// SYMBOL TABLE //////////////////////////////////////////////////

int hashF(string s)
{
    int h = 0;
    for(int i=0;i<(int)s.size();i++)
    {
        h = (h + s[i]);
    }
    return h;
}

int bucket_size = 30;
SymbolTable *sym_tab = new SymbolTable(bucket_size,hashF);

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// FUNCTION PARAMETER  ///////////////////////////////////////////

bool is_function_now = false;
vector<SymbolInfo>function_params;

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// ERROR HANDLING ///////////////////////////////////////////////

string do_implicit_typecast(string left_op,string right_op)
{
    if(left_op == "NULL" || right_op == "NULL") return "NULL"; // already reported , now supressing more errors

    if(left_op == "void" || right_op == "void") return "error";

    if((left_op == "float" || left_op == "float_array") && (right_op == "float" || right_op == "float_array")) return "float";
    if((left_op == "float" || left_op == "float_array") && (right_op == "int" || right_op == "int_array")) return "float";
    if((left_op == "int" || left_op=="int_array") && (right_op == "float" || right_op == "float_array")) return "float";
    if((left_op == "int" || left_op=="int_array") && (right_op == "int" || right_op == "int_array")) return "int";

    return "error";
}

bool is_param_typecast_ok(string og_p,string pass_p)
{
    if(pass_p == "NULL") return true; // already error reported and converted to NULL , this is made true to supress more error

    if(og_p == "void") return pass_p == "void";
    if(og_p == "int") return (pass_p == "int" || pass_p == "int_array");
    if(og_p == "float") return pass_p != "void";
}

bool check_assignop(string left_op,string right_op)
{
    if(left_op == "NULL" || right_op == "NULL") return true; // already error reported and converted to NULL , this is made true to supress more error

    if(left_op == "void" || right_op == "void") return false;
    if(left_op == "" || right_op == "") return false;

    if((left_op == "int" || left_op == "int_array") && (right_op == "int" || right_op == "int_array") ) return true;
    
    if((left_op == "float" || left_op == "float_array") && (right_op != "void") )return true;

    return false;
}

void print_grammar_rule(string left_part,string right_part)
{
    fprintf(logout, "%s : %s\n", left_part.data(), right_part.data());
}

void print_log_text(string log_text)
{
    fprintf(parseout, "%s\n", log_text.data());
}

void error_multiple_declaration(string error_symbol)
{
    fprintf(logout, "Line# %d: Multiple declaration of %s\n", line_count, error_symbol.data());
    fprintf(errout, "Line# %d: Multiple declaration of %s\n", line_count, error_symbol.data());
    err_count++;
}

void error_array_size_float()
{
    fprintf(logout, "Line# %d: Non-integer Array Size\n", line_count);
    fprintf(errout, "Line# %d: Non-integer Array Size\n", line_count);
    err_count++;
}

void error_array_index_invalid()
{
    fprintf(logout, "Line# %d: Expression inside third brackets not an integer\n", line_count);
    fprintf(errout, "Line# %d: Expression inside third brackets not an integer\n", line_count);
    err_count++;
}

void error_type_cast()
{
    fprintf(logout, "Line# %d: Incompatible Operand\n", line_count);
    fprintf(errout, "Line# %d: Incompatible Operand\n", line_count);
    err_count++;
}

void error_type_cast_void()
{
    fprintf(logout, "Line# %d: Void function used in expression\n", line_count);
    fprintf(errout, "Line# %d: Void function used in expression\n", line_count);
    err_count++;
}

void error_type_cast_mod()
{
    fprintf(logout, "Line# %d: Non-Integer operand on modulus operator\n", line_count);
    fprintf(errout, "Line# %d: Non-Integer operand on modulus operator\n", line_count);
    err_count++;
}

void error_type_cast_mod_zero()
{
    fprintf(logout, "Line# %d: Modulus by Zero\n", line_count);
    fprintf(errout, "Line# %d: Modulus by Zero\n", line_count);
    err_count++;
}

void error_undeclared_variable(string var_name)
{
    fprintf(logout, "Line# %d: Undeclared variable %s\n", line_count, var_name.data());
    fprintf(errout, "Line# %d: Undeclared variable %s\n", line_count, var_name.data());
    err_count++;
}

void error_undeclared_function(string var_name)
{
    fprintf(logout, "Line# %d: Undeclared function %s\n", line_count, var_name.data());
    fprintf(errout, "Line# %d: Undeclared function %s\n", line_count, var_name.data());
    err_count++;
}

void error_type_mismatch(string msg="")
{
    fprintf(logout, "Line# %d: Type mismatch %s\n", line_count, msg.data());
    fprintf(errout, "Line# %d: Type mismatch %s\n", line_count, msg.data());
    err_count++;
}

void error_function_parameter_type(int param_id,string extra_s="")
{
    fprintf(logout, "Line# %d: %d-th argument mismatch in function %s\n", line_count, param_id, extra_s.data());
    fprintf(errout, "Line# %d: %d-th argument mismatch in function %s\n", line_count, param_id, extra_s.data());
    err_count++;
}

void error_function_parameter_number(string name,bool declaration_definition_clash = false)
{
    if(declaration_definition_clash){
        fprintf(logout, "Line# %d: Total number of arguments mismatch with declaration in function %s\n", line_count, name.data());
        fprintf(errout, "Line# %d: Total number of arguments mismatch with declaration in function %s\n", line_count, name.data());
    }
    else{
        fprintf(logout, "Line# %d: Total number of arguments mismatch in function %s\n", line_count, name.data());
        fprintf(errout, "Line# %d: Total number of arguments mismatch in function %s\n", line_count, name.data());
    }

    err_count++;
}

void error_function_not_implemented()
{
    fprintf(logout, "Line# %d: Function not implemented\n", line_count);
    fprintf(errout, "Line# %d: Function not implemented\n", line_count);
    err_count++;
}

void error_function_return_condflict(string name)
{
    fprintf(logout, "Line# %d: Return type mismatch with function declaration in function %s\n", line_count, name.data());
    fprintf(errout, "Line# %d: Return type mismatch with function declaration in function %s\n", line_count, name.data());
    err_count++;
}

void error_not_function(string f_name)
{
    fprintf(logout, "Line# %d: %s not a function\n", line_count, f_name.data());
    fprintf(errout, "Line# %d: %s not a function\n", line_count, f_name.data());
    err_count++;
}

void error_nested_function()
{
    fprintf(logout, "Line# %d: A function is defined inside a function\n", line_count);
    fprintf(errout, "Line# %d: A function is defined inside a function\n", line_count);
    err_count++;
}

void error_var_type()
{
    fprintf(logout, "Line# %d: Variable type cannot be void\n", line_count);
    fprintf(errout, "Line# %d: Variable type cannot be void\n", line_count);
    err_count++;
}

void error_not_array(string name)
{
    fprintf(logout, "Line# %d: %s not an array\n", line_count, name.data());
    fprintf(errout, "Line# %d: %s not an array\n", line_count, name.data());
    err_count++;
}

void error_parameter_name_missing(int param_id,string func_name)
{
    fprintf(logout, "Line# %d: %d th parameter's name not given in function definition of \n", line_count, param_id, func_name.data());
    fprintf(errout, "Line# %d: %d th parameter's name not given in function definition of \n", line_count, param_id, func_name.data());
    err_count++;
}

///////////////////////////////////////////

void insert_function_to_global(SymbolInfo* temp_s,string var_type)
{
    // insert function ID to SymbolTable with VAR_TYPE
    temp_s->setVarType(var_type);
    temp_s->isFunction = true;

    // update parameter type
    for(auto temp_p : function_params)
    {
        temp_s->param_v.push_back(temp_p.var_type);
    }

    if(!sym_tab->insert_symbol(*temp_s))
    {
        SymbolInfo* ret_symbol = sym_tab->lookup(temp_s->key);

        if(ret_symbol->isFunctionDeclaration == false){
            error_multiple_declaration(temp_s->key);
        }
        else{

            // declared before , now definition happening

            // check if any clash between declaration and definition

            if(ret_symbol->var_type != temp_s->var_type)
            {
                error_function_return_condflict(temp_s->key);
            }

            if(ret_symbol->param_v.size() != temp_s->param_v.size())
            {
                error_function_parameter_number(temp_s->key,true);
            }
            else
            {
                for(int i=0;i<ret_symbol->param_v.size();i++)
                {
                    if(ret_symbol->param_v[i] != temp_s->param_v[i]){
                        error_function_parameter_type(i+1,temp_s->key);
                        break;
                    }
                }
            }

            // the following line is commented out because in case of clash , use the declaration info 
            // ret_symbol->param_v = $2->param_v;
            ret_symbol->isFunctionDeclaration = false; // declaration + 
        }

        // cout<<"Dec -> "<<ret_symbol->key<<" :: "<<ret_symbol->isFunctionDeclaration<<endl;
    }
    else{

        // Finalizing Definition

        SymbolInfo* ret_symbol = sym_tab->lookup(temp_s->key);
        ret_symbol->isFunctionDeclaration = false;
        // cout<<"Dec ->> "<<ret_symbol->key<<" :: "<<ret_symbol->isFunctionDeclaration<<endl;

        for(int i=0;i<function_params.size();i++)
        {
            if(function_params[i].key == "dummy_key"){
                error_parameter_name_missing(i+1,ret_symbol->key);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  FREE   MEMORY /////////////////////////////////////////

void erm_s(SymbolInfo* s) // erase memory of SymbolInfo pointer
{
    if(s!=NULL) free(s);
}

void erm_h(Helper* h) // erase memory of Helper pointer
{
    if(h!=NULL) free(h);
}



#line 400 "y.tab.c"

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
/* "%code requires" blocks.  */
#line 1 "1705010.y"

    #include <bits/stdc++.h>

#line 439 "y.tab.c"

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
#line 336 "1705010.y"

    SymbolInfo* symbol_info;
    // SymbolInfo* symbol_info_vec[100];
    string* symbol_info_str;
    string* temp_str;
    Helper* helper;
    // int ival;
    // double dval;

#line 555 "y.tab.c"

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

#if 1

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
#endif /* 1 */

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
#define YYLAST   281

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  161

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
       0,   368,   368,   383,   395,   407,   416,   425,   436,   479,
     521,   569,   616,   650,   688,   727,   768,   768,   791,   791,
     820,   820,   847,   847,   881,   903,   930,   949,   974,   992,
    1011,  1030,  1050,  1069,  1088,  1103,  1123,  1157,  1195,  1205,
    1215,  1227,  1250,  1279,  1307,  1341,  1377,  1417,  1432,  1455,
    1483,  1493,  1505,  1520,  1530,  1541,  1552,  1562,  1572,  1589,
    1604,  1623,  1638,  1661,  1674,  1690,  1698,  1713,  1745,  1789,
    1802,  1834,  1847,  1890,  1903,  1945,  1959,  2001,  2015,  2080,
    2095,  2109,  2124,  2138,  2212,  2229,  2243,  2256,  2269,  2280,
    2293,  2306,  2312,  2329
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
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

#define YYPACT_NINF (-106)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-23)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      58,  -106,  -106,  -106,    10,    58,  -106,  -106,  -106,  -106,
     -17,  -106,  -106,    -3,     8,    23,   103,   118,   -10,  -106,
       3,    24,    37,    -1,     5,    32,    38,  -106,    56,    33,
    -106,  -106,    47,   -11,    43,    58,  -106,  -106,  -106,    72,
     109,  -106,  -106,    47,  -106,  -106,    85,    58,  -106,  -106,
      47,    70,   112,    81,    84,  -106,    63,  -106,  -106,    47,
      86,  -106,  -106,    94,   108,  -106,  -106,   168,    96,   111,
     122,   148,   131,   218,   218,  -106,  -106,    91,   218,  -106,
    -106,  -106,  -106,  -106,  -106,  -106,    98,  -106,  -106,   106,
     127,  -106,   124,    45,   144,  -106,  -106,  -106,  -106,  -106,
    -106,  -106,   133,   218,   183,   218,  -106,   139,   151,   141,
    -106,   160,   218,   218,  -106,   203,  -106,  -106,  -106,  -106,
     218,  -106,   218,   218,   218,   218,   238,  -106,   161,   183,
     163,  -106,   171,  -106,  -106,   172,   175,   169,  -106,  -106,
    -106,  -106,   144,   180,  -106,   238,   218,   238,   187,  -106,
     218,  -106,   214,   196,  -106,  -106,  -106,   238,   238,  -106,
    -106
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
       0,     0,     0,     0,     0,    31,    65,    67,     0,    85,
      86,    87,    55,    54,    57,    53,     0,    50,    56,    82,
       0,    69,    71,    73,    75,    77,    81,    19,    25,    44,
      46,    34,     0,     0,     0,     0,    64,     0,     0,    82,
      80,     0,    91,     0,    79,     0,    30,    51,    88,    89,
       0,    66,     0,     0,     0,     0,     0,    33,     0,     0,
       0,    63,     0,    84,    93,     0,    90,     0,    32,    52,
      70,    72,    76,    74,    78,     0,     0,     0,     0,    83,
       0,    68,    59,     0,    61,    62,    92,     0,     0,    60,
      58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -106,  -106,  -106,   216,    22,    78,  -106,  -106,  -106,  -106,
    -106,   -29,  -106,   107,    -7,  -106,   162,   -84,   -88,   -73,
     -70,  -105,   116,   123,   126,   -67,  -106,  -106,  -106
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,     5,     6,    82,    83,    50,    59,    32,    43,
      22,    84,    56,    85,    10,    14,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,   135,   136
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     109,   107,   117,    45,   111,   109,   110,   134,    23,    17,
      11,   114,    46,    13,    55,   140,   129,    47,   117,    15,
      28,    61,     7,    16,    20,    30,    29,     7,    51,    36,
      97,   139,    37,   128,    41,   130,    18,    19,    33,   109,
      60,   146,   139,   137,    48,   156,    21,   109,   -20,   109,
     109,   109,   109,    31,     1,     2,     3,   -22,   144,    38,
      34,   152,    42,   154,    67,    35,    68,   -16,    39,    69,
      70,    44,    49,   159,   160,    71,   153,   109,     8,   123,
      72,   124,    40,     8,    73,    74,    57,    44,    75,     1,
       2,     3,    76,    77,     1,     2,     3,    78,    52,   115,
      62,    68,    79,    80,    69,    70,    81,     9,    65,   -18,
      71,    66,     9,   112,    58,    72,    98,   113,   103,    73,
      74,    99,    44,   116,   118,   119,   120,    76,    77,     1,
       2,     3,    78,   104,   126,   100,    68,    79,    80,    69,
      70,    81,    24,    25,   105,    71,    26,    27,    53,    54,
      72,    63,    64,   108,    73,    74,   121,    44,   127,   118,
     119,   122,    76,    77,     1,     2,     3,    78,   131,    73,
      74,    68,    79,    80,    69,    70,    81,   106,    77,   125,
      71,   132,    78,   133,   145,    72,   147,    79,    80,    73,
      74,    81,    44,   101,   148,   149,   151,    76,    77,     1,
       2,     3,    78,   150,    73,    74,    68,    79,    80,    69,
      70,    81,    76,    77,   123,    71,   155,    78,   157,   158,
      72,    12,    79,    80,    73,    74,    81,    44,   138,   102,
       0,     0,    76,    77,     1,     2,     3,    78,   141,    73,
      74,    68,    79,    80,    69,    70,    81,   143,    77,   142,
      71,     0,    78,     0,     0,    72,     0,    79,    80,    73,
      74,    81,    44,     0,     0,     0,     0,    76,    77,     1,
       2,     3,    78,     0,     0,     0,     0,    79,    80,     0,
       0,    81
};

static const yytype_int16 yycheck[] =
{
      73,    71,    86,    32,    74,    78,    73,   112,    15,     1,
       0,    78,    23,    30,    43,   120,   104,    28,   102,    22,
      30,    50,     0,    26,     1,     1,    23,     5,    35,    30,
      59,   115,    27,   103,     1,   105,    28,    29,     1,   112,
      47,   129,   126,   113,     1,   150,    23,   120,    24,   122,
     123,   124,   125,    29,    31,    32,    33,    24,   125,    27,
      23,   145,    29,   147,     1,    28,     3,    24,    30,     6,
       7,    24,    29,   157,   158,    12,   146,   150,     0,    34,
      17,    36,    26,     5,    21,    22,     1,    24,    25,    31,
      32,    33,    29,    30,    31,    32,    33,    34,    26,     1,
      30,     3,    39,    40,     6,     7,    43,     0,    27,    24,
      12,    27,     5,    22,    29,    17,    30,    26,    22,    21,
      22,    27,    24,    25,    18,    19,    20,    29,    30,    31,
      32,    33,    34,    22,     1,    27,     3,    39,    40,     6,
       7,    43,    39,    40,    22,    12,    28,    29,    39,    40,
      17,    39,    40,    22,    21,    22,    29,    24,    25,    18,
      19,    37,    29,    30,    31,    32,    33,    34,    29,    21,
      22,     3,    39,    40,     6,     7,    43,    29,    30,    35,
      12,    30,    34,    23,    23,    17,    23,    39,    40,    21,
      22,    43,    24,    25,    23,    23,    27,    29,    30,    31,
      32,    33,    34,    28,    21,    22,     3,    39,    40,     6,
       7,    43,    29,    30,    34,    12,    29,    34,     4,    23,
      17,     5,    39,    40,    21,    22,    43,    24,    25,    67,
      -1,    -1,    29,    30,    31,    32,    33,    34,   122,    21,
      22,     3,    39,    40,     6,     7,    43,   124,    30,   123,
      12,    -1,    34,    -1,    -1,    17,    -1,    39,    40,    21,
      22,    43,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    39,    40,    -1,
      -1,    43
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
      27,    25,    60,    22,    22,    22,    29,    64,    22,    63,
      69,    64,    22,    26,    69,     1,    25,    61,    18,    19,
      20,    29,    37,    34,    36,    35,     1,    25,    64,    62,
      64,    29,    30,    23,    65,    71,    72,    64,    25,    61,
      65,    66,    68,    67,    69,    23,    62,    23,    23,    23,
      28,    27,    61,    64,    61,    29,    65,     4,    23,    61,
      61
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
      61,    61,    61,    61,    61,    62,    62,    63,    63,    64,
      64,    65,    65,    66,    66,    67,    67,    68,    68,    69,
      69,    69,    70,    70,    70,    70,    70,    70,    70,    70,
      71,    71,    72,    72
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
       7,     5,     5,     3,     2,     1,     2,     1,     4,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     2,
       2,     1,     1,     4,     3,     1,     1,     1,     2,     2,
       1,     0,     3,     1
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


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
  switch (yykind)
    {
    case YYSYMBOL_ID: /* ID  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1750 "y.tab.c"
        break;

    case YYSYMBOL_INT: /* INT  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1756 "y.tab.c"
        break;

    case YYSYMBOL_FLOAT: /* FLOAT  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1762 "y.tab.c"
        break;

    case YYSYMBOL_VOID: /* VOID  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1768 "y.tab.c"
        break;

    case YYSYMBOL_ADDOP: /* ADDOP  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1774 "y.tab.c"
        break;

    case YYSYMBOL_MULOP: /* MULOP  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1780 "y.tab.c"
        break;

    case YYSYMBOL_RELOP: /* RELOP  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1786 "y.tab.c"
        break;

    case YYSYMBOL_LOGICOP: /* LOGICOP  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1792 "y.tab.c"
        break;

    case YYSYMBOL_CONST_CHAR: /* CONST_CHAR  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1798 "y.tab.c"
        break;

    case YYSYMBOL_CONST_INT: /* CONST_INT  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1804 "y.tab.c"
        break;

    case YYSYMBOL_CONST_FLOAT: /* CONST_FLOAT  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1810 "y.tab.c"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1816 "y.tab.c"
        break;

    case YYSYMBOL_ERROR_CHAR: /* ERROR_CHAR  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1822 "y.tab.c"
        break;

    case YYSYMBOL_ERROR_FLOAT: /* ERROR_FLOAT  */
#line 359 "1705010.y"
            { erm_s(((*yyvaluep).symbol_info));  }
#line 1828 "y.tab.c"
        break;

    case YYSYMBOL_start: /* start  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1834 "y.tab.c"
        break;

    case YYSYMBOL_program: /* program  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1840 "y.tab.c"
        break;

    case YYSYMBOL_unit: /* unit  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1846 "y.tab.c"
        break;

    case YYSYMBOL_func_declaration: /* func_declaration  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1852 "y.tab.c"
        break;

    case YYSYMBOL_func_definition: /* func_definition  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1858 "y.tab.c"
        break;

    case YYSYMBOL_parameter_list: /* parameter_list  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1864 "y.tab.c"
        break;

    case YYSYMBOL_compound_statement: /* compound_statement  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1870 "y.tab.c"
        break;

    case YYSYMBOL_var_declaration: /* var_declaration  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1876 "y.tab.c"
        break;

    case YYSYMBOL_type_specifier: /* type_specifier  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1882 "y.tab.c"
        break;

    case YYSYMBOL_declaration_list: /* declaration_list  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1888 "y.tab.c"
        break;

    case YYSYMBOL_statements: /* statements  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1894 "y.tab.c"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1900 "y.tab.c"
        break;

    case YYSYMBOL_expression_statement: /* expression_statement  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1906 "y.tab.c"
        break;

    case YYSYMBOL_variable: /* variable  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1912 "y.tab.c"
        break;

    case YYSYMBOL_expression: /* expression  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1918 "y.tab.c"
        break;

    case YYSYMBOL_logic_expression: /* logic_expression  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1924 "y.tab.c"
        break;

    case YYSYMBOL_rel_expression: /* rel_expression  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1930 "y.tab.c"
        break;

    case YYSYMBOL_simple_expression: /* simple_expression  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1936 "y.tab.c"
        break;

    case YYSYMBOL_term: /* term  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1942 "y.tab.c"
        break;

    case YYSYMBOL_unary_expression: /* unary_expression  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1948 "y.tab.c"
        break;

    case YYSYMBOL_factor: /* factor  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1954 "y.tab.c"
        break;

    case YYSYMBOL_argument_list: /* argument_list  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1960 "y.tab.c"
        break;

    case YYSYMBOL_arguments: /* arguments  */
#line 358 "1705010.y"
            { erm_h(((*yyvaluep).helper));  }
#line 1966 "y.tab.c"
        break;

      default:
        break;
    }
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

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

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
#line 369 "1705010.y"
        {
		//write your code in this block in all the similar blocks below

        print_grammar_rule("start","program");

        (yyval.helper) = new Helper();
        (yyval.helper)->text = (yyvsp[0].helper)->text;

        print_log_text("start : program");
        
        erm_h((yyvsp[0].helper));
	}
#line 2250 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 383 "1705010.y"
                       {
            print_grammar_rule("program","program unit");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[-1].helper)->text;
            (yyval.helper)->text += "\n";
            (yyval.helper)->text += (yyvsp[0].helper)->text;

            print_log_text("program : program unit");

            erm_h((yyvsp[-1].helper)); erm_h((yyvsp[0].helper));
        }
#line 2267 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 395 "1705010.y"
               { 
            print_grammar_rule("program","unit");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("program : unit"); 

            erm_h((yyvsp[0].helper));
        }
#line 2282 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 407 "1705010.y"
                      { 
            print_grammar_rule("unit","var_declaration"); 

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("unit : var_declaration");  
            erm_h((yyvsp[0].helper)); 
        }
#line 2296 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 416 "1705010.y"
                        { 
            print_grammar_rule("unit","func_declaration"); 

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("unit : func_declaration");  
            erm_h((yyvsp[0].helper)); 
        }
#line 2310 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 425 "1705010.y"
                       { 
            print_grammar_rule("unit","func_definition");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("unit : func_definition");
            erm_h((yyvsp[0].helper)); 
        }
#line 2324 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON  */
#line 436 "1705010.y"
                                                                           { 
                
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-5].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-4].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += (yyvsp[-2].helper)->text;
                (yyval.helper)->text += ")";
                (yyval.helper)->text += ";";

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-4].symbol_info)->setVarType((yyvsp[-5].helper)->text);
                (yyvsp[-4].symbol_info)->isFunction = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    (yyvsp[-4].symbol_info)->param_v.push_back(temp_s.var_type);
                }

                if(sym_tab->insert_symbol(*(yyvsp[-4].symbol_info)))
                {
                    SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-4].symbol_info)->key);
                    ret_symbol->isFunctionDeclaration = true; // mark as function declaration
                }
                else
                {
                    error_multiple_declaration((yyvsp[-4].symbol_info)->key);
                }

                print_log_text("func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");

                // clear param_info
                function_params.clear();

                erm_h((yyvsp[-5].helper)); erm_s((yyvsp[-4].symbol_info)) ; erm_h((yyvsp[-2].helper));
    
        }
#line 2372 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN error  */
#line 479 "1705010.y"
                                                               { 
                
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-5].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-4].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += (yyvsp[-2].helper)->text;
                (yyval.helper)->text += ")";

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-4].symbol_info)->setVarType((yyvsp[-5].helper)->text);
                (yyvsp[-4].symbol_info)->isFunction = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    (yyvsp[-4].symbol_info)->param_v.push_back(temp_s.var_type);
                }

                if(sym_tab->insert_symbol(*(yyvsp[-4].symbol_info)))
                {
                    SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-4].symbol_info)->key);
                    ret_symbol->isFunctionDeclaration = true; // mark as function declaration
                }
                else
                {
                    error_multiple_declaration((yyvsp[-4].symbol_info)->key);
                }

                print_log_text("func_declaration: type_specifier ID LPAREN parameter_list RPAREN error");

                // clear param_info
                function_params.clear();

                erm_h((yyvsp[-5].helper)); erm_s((yyvsp[-4].symbol_info)) ; erm_h((yyvsp[-2].helper));
    
        }
#line 2419 "y.tab.c"
    break;

  case 10: /* func_declaration: type_specifier ID LPAREN parameter_list error RPAREN SEMICOLON  */
#line 521 "1705010.y"
                                                                         { 

                /**
                    To handle errors like: 
                        void foo(int x-y);
                **/
                
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-6].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-5].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += (yyvsp[-3].helper)->text;
                (yyval.helper)->text += ")";
                (yyval.helper)->text += ";";

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-5].symbol_info)->setVarType((yyvsp[-6].helper)->text);
                (yyvsp[-5].symbol_info)->isFunction = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    (yyvsp[-5].symbol_info)->param_v.push_back(temp_s.var_type);
                }

                if(sym_tab->insert_symbol(*(yyvsp[-5].symbol_info)))
                {
                    SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-5].symbol_info)->key);
                    ret_symbol->isFunctionDeclaration = true; // mark as function declaration
                }
                else
                {
                    error_multiple_declaration((yyvsp[-5].symbol_info)->key);
                }

                print_log_text("func_declaration : type_specifier ID LPAREN parameter_list error RPAREN SEMICOLON");

                // clear param_info
                function_params.clear();

                erm_h((yyvsp[-6].helper)); erm_s((yyvsp[-5].symbol_info)) ; erm_h((yyvsp[-3].helper));
    
        }
#line 2472 "y.tab.c"
    break;

  case 11: /* func_declaration: type_specifier ID LPAREN parameter_list error RPAREN error  */
#line 569 "1705010.y"
                                                                     { 

                /**
                    To handle errors like: 
                        void foo(int x-y)
                **/
                
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-6].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-5].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += (yyvsp[-3].helper)->text;
                (yyval.helper)->text += ")";

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-5].symbol_info)->setVarType((yyvsp[-6].helper)->text);
                (yyvsp[-5].symbol_info)->isFunction = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    (yyvsp[-5].symbol_info)->param_v.push_back(temp_s.var_type);
                }

                if(sym_tab->insert_symbol(*(yyvsp[-5].symbol_info)))
                {
                    SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-5].symbol_info)->key);
                    ret_symbol->isFunctionDeclaration = true; // mark as function declaration
                }
                else
                {
                    error_multiple_declaration((yyvsp[-5].symbol_info)->key);
                }

                print_log_text("func_declaration : type_specifier ID LPAREN parameter_list error RPAREN error");

                // clear param_info
                function_params.clear();

                erm_h((yyvsp[-6].helper)); erm_s((yyvsp[-5].symbol_info)) ; erm_h((yyvsp[-3].helper));
    
        }
#line 2524 "y.tab.c"
    break;

  case 12: /* func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON  */
#line 616 "1705010.y"
                                                            { 

                print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-4].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-3].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += ")";
                (yyval.helper)->text += ";";

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-3].symbol_info)->setVarType((yyvsp[-4].helper)->text);
                (yyvsp[-3].symbol_info)->isFunction = true;
                
                if(sym_tab->insert_symbol(*(yyvsp[-3].symbol_info)))
                {
                    SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-3].symbol_info)->key);
                    ret_symbol->isFunctionDeclaration = true; // mark as function declaration
                }
                else
                {
                    error_multiple_declaration((yyvsp[-3].symbol_info)->key);
                }

                print_log_text("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");

                function_params.clear();

                erm_h((yyvsp[-4].helper)); erm_s((yyvsp[-3].symbol_info)) ; 
            }
#line 2563 "y.tab.c"
    break;

  case 13: /* func_declaration: type_specifier ID LPAREN RPAREN error  */
#line 650 "1705010.y"
                                                    { 

                /**
                    To handle errors like: 
                        void foo()
                **/

            print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN");
            
            (yyval.helper) = new Helper();

            // update text
            (yyval.helper)->text = (yyvsp[-4].helper)->text;
            (yyval.helper)->text += " ";
            (yyval.helper)->text += (yyvsp[-3].symbol_info)->key;
            (yyval.helper)->text += "(";
            (yyval.helper)->text += ")";

            // insert function ID to SymbolTable with VAR_TYPE
            (yyvsp[-3].symbol_info)->setVarType((yyvsp[-4].helper)->text);
            (yyvsp[-3].symbol_info)->isFunction = true;
            
            if(sym_tab->insert_symbol(*(yyvsp[-3].symbol_info)))
            {
                SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-3].symbol_info)->key);
                ret_symbol->isFunctionDeclaration = true; // mark as function declaration
            }
            else
            {
                error_multiple_declaration((yyvsp[-3].symbol_info)->key);
            }

            print_log_text("func_declaration : type_specifier ID LPAREN RPAREN error");

            function_params.clear();

            erm_h((yyvsp[-4].helper)); erm_s((yyvsp[-3].symbol_info)) ; 
        }
#line 2606 "y.tab.c"
    break;

  case 14: /* func_declaration: type_specifier ID LPAREN error RPAREN SEMICOLON  */
#line 688 "1705010.y"
                                                          { 

                /**
                    To handle errors like: 
                        void foo(-);
                **/

                print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-5].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-4].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += ")";
                (yyval.helper)->text += ";";

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-4].symbol_info)->setVarType((yyvsp[-5].helper)->text);
                (yyvsp[-4].symbol_info)->isFunction = true;
                
                if(sym_tab->insert_symbol(*(yyvsp[-4].symbol_info)))
                {
                    SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-4].symbol_info)->key);
                    ret_symbol->isFunctionDeclaration = true; // mark as function declaration
                }
                else
                {
                    error_multiple_declaration((yyvsp[-4].symbol_info)->key);
                }

                print_log_text("func_declaration : type_specifier ID LPAREN error RPAREN SEMICOLON");

                function_params.clear();

                erm_h((yyvsp[-5].helper)); erm_s((yyvsp[-4].symbol_info)) ; 
            }
#line 2650 "y.tab.c"
    break;

  case 15: /* func_declaration: type_specifier ID LPAREN error RPAREN error  */
#line 727 "1705010.y"
                                                      { 

                /**
                    To handle errors like: 
                        void foo(-)
                **/

                print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-5].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-4].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += ")";

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-4].symbol_info)->setVarType((yyvsp[-5].helper)->text);
                (yyvsp[-4].symbol_info)->isFunction = true;
                
                if(sym_tab->insert_symbol(*(yyvsp[-4].symbol_info)))
                {
                    SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-4].symbol_info)->key);
                    ret_symbol->isFunctionDeclaration = true; // mark as function declaration
                }
                else
                {
                    error_multiple_declaration((yyvsp[-4].symbol_info)->key);
                }

                print_log_text("func_declaration : type_specifier ID LPAREN error RPAREN error");

                function_params.clear();

                erm_h((yyvsp[-5].helper)); erm_s((yyvsp[-4].symbol_info)) ; 
            }
#line 2693 "y.tab.c"
    break;

  case 16: /* $@1: %empty  */
#line 768 "1705010.y"
                                                                { is_function_now = true;insert_function_to_global((yyvsp[-3].symbol_info),(yyvsp[-4].helper)->text);}
#line 2699 "y.tab.c"
    break;

  case 17: /* func_definition: type_specifier ID LPAREN parameter_list RPAREN $@1 compound_statement  */
#line 768 "1705010.y"
                                                                                                                                                     { 
                print_grammar_rule("func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-6].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-5].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += (yyvsp[-3].helper)->text;
                (yyval.helper)->text += ")";
                (yyval.helper)->text += (yyvsp[0].helper)->text; 

                print_log_text("func_definition : type_specifier ID LPAREN parameter_list RPAREN");

                // clear temp function params
                is_function_now = false;
                function_params.clear();

                erm_h((yyvsp[-6].helper)); erm_h((yyvsp[-3].helper)); erm_h((yyvsp[0].helper));
                erm_s((yyvsp[-5].symbol_info));
            }
#line 2727 "y.tab.c"
    break;

  case 18: /* $@2: %empty  */
#line 791 "1705010.y"
                                                               { is_function_now = true;insert_function_to_global((yyvsp[-4].symbol_info),(yyvsp[-5].helper)->text);}
#line 2733 "y.tab.c"
    break;

  case 19: /* func_definition: type_specifier ID LPAREN parameter_list error RPAREN $@2 compound_statement  */
#line 791 "1705010.y"
                                                                                                                                                    { 
                
                /**
                    To handle cases like :
                        void foo(int x-y){}
                **/
                
                print_grammar_rule("func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-7].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-6].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += (yyvsp[-4].helper)->text;
                (yyval.helper)->text += ")";
                (yyval.helper)->text += (yyvsp[0].helper)->text; 

                print_log_text("func_definition : type_specifier ID LPAREN parameter_list error RPAREN");

                // clear temp function params
                is_function_now = false;
                function_params.clear();

                erm_h((yyvsp[-7].helper)); erm_h((yyvsp[-4].helper)); erm_h((yyvsp[0].helper));
                erm_s((yyvsp[-6].symbol_info));
        }
#line 2767 "y.tab.c"
    break;

  case 20: /* $@3: %empty  */
#line 820 "1705010.y"
                                                    {is_function_now = true;insert_function_to_global((yyvsp[-2].symbol_info),(yyvsp[-3].helper)->text);}
#line 2773 "y.tab.c"
    break;

  case 21: /* func_definition: type_specifier ID LPAREN RPAREN $@3 compound_statement  */
#line 820 "1705010.y"
                                                                                                                                        { 
                print_grammar_rule("func_definition","type_specifier ID LPAREN RPAREN compound_statement");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-5].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-4].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += ")";
                (yyval.helper)->text += (yyvsp[0].helper)->text;

                // insert function ID to SymbolTable with VAR_TYPE
                (yyvsp[-4].symbol_info)->setVarType((yyvsp[-5].helper)->text);
                (yyvsp[-4].symbol_info)->isFunction = true;
                sym_tab->insert_symbol(*(yyvsp[-4].symbol_info));

                print_log_text("func_definition : type_specifier ID LPAREN RPAREN");
            
                // clear temp function params
                is_function_now = false;
                function_params.clear();

                erm_h((yyvsp[-5].helper)); erm_h((yyvsp[0].helper));
                erm_s((yyvsp[-4].symbol_info));
            }
#line 2805 "y.tab.c"
    break;

  case 22: /* $@4: %empty  */
#line 847 "1705010.y"
                                                 { is_function_now = true;insert_function_to_global((yyvsp[-3].symbol_info),(yyvsp[-4].helper)->text);}
#line 2811 "y.tab.c"
    break;

  case 23: /* func_definition: type_specifier ID LPAREN error RPAREN $@4 compound_statement  */
#line 847 "1705010.y"
                                                                                                                                      {
                
                /**
                    To handle cases like :
                        void foo(-){}
                **/
                
                // cout<<"inside func_definition syntax_error 1"<<endl;

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-6].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[-5].symbol_info)->key;
                (yyval.helper)->text += "(";
                (yyval.helper)->text += ")";
                (yyval.helper)->text += (yyvsp[0].helper)->text; 

                print_log_text("func_definition : type_specifier ID LPAREN error RPAREN");

                // clear temp function params
                is_function_now = false;
                function_params.clear();

                yyclearin;
                yyerrok;

                erm_h((yyvsp[-6].helper)); erm_h((yyvsp[0].helper));
                erm_s((yyvsp[-5].symbol_info));
        }
#line 2847 "y.tab.c"
    break;

  case 24: /* parameter_list: parameter_list COMMA type_specifier ID  */
#line 881 "1705010.y"
                                                       {

               print_grammar_rule("parameter_list","parameter_list COMMA type_specifier ID");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-3].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[-1].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[0].symbol_info)->key;

                // insert parameter ID to SymbolTable with VAR_TYPE
                (yyvsp[0].symbol_info)->setVarType((yyvsp[-1].helper)->text);
                function_params.push_back(*(yyvsp[0].symbol_info));

                print_log_text("parameter_list : parameter_list COMMA type_specifier ID");

                erm_h((yyvsp[-3].helper)); erm_h((yyvsp[-1].helper));
                erm_s((yyvsp[0].symbol_info));
            }
#line 2874 "y.tab.c"
    break;

  case 25: /* parameter_list: parameter_list error COMMA type_specifier ID  */
#line 903 "1705010.y"
                                                       {

                /**
                    To handle errors like:
                    void foo(int x-y,int z){}
                **/

               print_grammar_rule("parameter_list","parameter_list COMMA type_specifier ID");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-4].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[-1].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[0].symbol_info)->key;

                // insert parameter ID to SymbolTable with VAR_TYPE
                (yyvsp[0].symbol_info)->setVarType((yyvsp[-1].helper)->text);
                function_params.push_back(*(yyvsp[0].symbol_info));

                print_log_text("parameter_list : parameter_list error COMMA type_specifier ID");

                erm_h((yyvsp[-4].helper)); erm_h((yyvsp[-1].helper));
                erm_s((yyvsp[0].symbol_info));
        }
#line 2906 "y.tab.c"
    break;

  case 26: /* parameter_list: parameter_list COMMA type_specifier  */
#line 930 "1705010.y"
                                              {
             print_grammar_rule("parameter_list","parameter_list COMMA type_specifier");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-2].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[0].helper)->text;

                SymbolInfo temp_s = SymbolInfo("dummy_key","dummy_value");
                temp_s.var_type = (yyvsp[0].helper)->text;

                function_params.push_back(temp_s);

                print_log_text("parameter_list : parameter_list COMMA type_specifier");

                erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
        }
#line 2930 "y.tab.c"
    break;

  case 27: /* parameter_list: parameter_list error COMMA type_specifier  */
#line 949 "1705010.y"
                                                    {

            /**
                To handle cases like:
                    void foo(int x-y,int);
            **/

             print_grammar_rule("parameter_list","parameter_list COMMA type_specifier");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-3].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[0].helper)->text;

                SymbolInfo temp_s = SymbolInfo("dummy_key","dummy_value");
                temp_s.var_type = (yyvsp[0].helper)->text;

                function_params.push_back(temp_s);

                print_log_text("parameter_list : parameter_list error COMMA type_specifier");

                erm_h((yyvsp[-3].helper)); erm_h((yyvsp[0].helper));
        }
#line 2960 "y.tab.c"
    break;

  case 28: /* parameter_list: type_specifier ID  */
#line 974 "1705010.y"
                                     { 
                print_grammar_rule("parameter_list","type_specifier ID");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-1].helper)->text;
                (yyval.helper)->text += " ";
                (yyval.helper)->text += (yyvsp[0].symbol_info)->key;

                // insert parameter ID to Parameter SymbolTable with VAR_TYPE
                (yyvsp[0].symbol_info)->setVarType((yyvsp[-1].helper)->text);
                function_params.push_back(*(yyvsp[0].symbol_info));

                print_log_text("parameter_list : type_specifier ID");

                erm_h((yyvsp[-1].helper)); erm_s((yyvsp[0].symbol_info));
        }
#line 2983 "y.tab.c"
    break;

  case 29: /* parameter_list: type_specifier  */
#line 992 "1705010.y"
                                 {
            print_grammar_rule("parameter_list","type_specifier");

            (yyval.helper) = new Helper();

            // update text
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            SymbolInfo temp_s = SymbolInfo("dummy_key","dummy_value");
            temp_s.var_type = (yyvsp[0].helper)->text;

            function_params.push_back(temp_s);

            print_log_text("parameter_list : type_specifier");

            erm_h((yyvsp[0].helper));
        }
#line 3005 "y.tab.c"
    break;

  case 30: /* compound_statement: LCURL dummy_scope_function statements RCURL  */
#line 1011 "1705010.y"
                                                                {
                print_grammar_rule("compound_statement","LCURL statements RCURL");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = "{\n"; 
                (yyval.helper)->text += (yyvsp[-1].helper)->text; 
                (yyval.helper)->text += "\n}"; 

                print_log_text("compound_statement: LCURL dummy_scope_function statements RCURL");

                // EXIT
                sym_tab->print_all_scope(logout);
                sym_tab->exit_scope();

                erm_h((yyvsp[-1].helper));

            }
#line 3029 "y.tab.c"
    break;

  case 31: /* compound_statement: LCURL dummy_scope_function RCURL  */
#line 1030 "1705010.y"
                                               {

                print_grammar_rule("compound_statement","LCURL RCURL");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = "{";  
                (yyval.helper)->text += "}"; 

                print_log_text("compound_statement: LCURL dummy_scope_function RCURL");

                // EXIT
                sym_tab->print_all_scope(logout);
                sym_tab->exit_scope();

                // // clear temp function params
                // is_function_now = false;
                // function_params.clear();
             }
#line 3054 "y.tab.c"
    break;

  case 32: /* compound_statement: LCURL dummy_scope_function statements error RCURL  */
#line 1050 "1705010.y"
                                                                {
                print_grammar_rule("compound_statement","LCURL statements RCURL");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = "{\n"; 
                (yyval.helper)->text += (yyvsp[-2].helper)->text; 
                (yyval.helper)->text += "\n}"; 

                print_log_text("compound_statement: LCURL dummy_scope_function statements error RCURL");

                // EXIT
                sym_tab->print_all_scope(logout);
                sym_tab->exit_scope();

                erm_h((yyvsp[-2].helper));

            }
#line 3078 "y.tab.c"
    break;

  case 33: /* compound_statement: LCURL dummy_scope_function error statements RCURL  */
#line 1069 "1705010.y"
                                                                {
                print_grammar_rule("compound_statement","LCURL statements RCURL");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = "{\n"; 
                (yyval.helper)->text += (yyvsp[-1].helper)->text; 
                (yyval.helper)->text += "\n}"; 

                print_log_text("compound_statement: LCURL dummy_scope_function error statements RCURL");

                // EXIT
                sym_tab->print_all_scope(logout);
                sym_tab->exit_scope();

                erm_h((yyvsp[-1].helper));

            }
#line 3102 "y.tab.c"
    break;

  case 34: /* compound_statement: LCURL dummy_scope_function error RCURL  */
#line 1088 "1705010.y"
                                                      {
                
                print_grammar_rule("compound_statement","LCURL error RCURL");

                (yyval.helper) = new Helper();

                 // update text
                (yyval.helper)->text = "{";  
                (yyval.helper)->text += "}";

                print_log_text("compound_statement: LCURL dummy_scope_function error RCURL");

             }
#line 3120 "y.tab.c"
    break;

  case 35: /* dummy_scope_function: %empty  */
#line 1103 "1705010.y"
                       {
                    sym_tab->enter_scope(); 

                    if(is_function_now)
                    {
                        for(auto el:function_params)
                        {

                            if(el.key == "dummy_key") continue;
                            // insert ID
                            // cout<<"INSIDE FUNCTIONNN"<<endl;
                            if(!sym_tab->insert_symbol(el)) // already present in current scope
                            {
                                error_multiple_declaration(el.key + " in parameter");
                            }
                        }
                    }
                }
#line 3143 "y.tab.c"
    break;

  case 36: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 1123 "1705010.y"
                                                           { 

            print_grammar_rule("var_declaration","type_specifier declaration_list SEMICOLON");
            
            (yyval.helper) = new Helper();

            // update text
            (yyval.helper)->text = (yyvsp[-2].helper)->text;
            (yyval.helper)->text += " ";
            (yyval.helper)->text += (yyvsp[-1].helper)->text;
            (yyval.helper)->text += ";";

            if((yyvsp[-2].helper)->text == "void"){
                error_var_type();
            }
            else{
                // insert all declaration_list ID to SymbolTable with VAR_TYPE
                for(auto el:(yyvsp[-1].helper)->v)
                {
                    if(el->var_type == "array") el->setVarType((yyvsp[-2].helper)->text + "_array") ; 
                    else el->setVarType((yyvsp[-2].helper)->text); 
                    
                    if(!sym_tab->insert_symbol(*el)) // already present in current scope
                    {
                        error_multiple_declaration(el->key);
                    }

                }
            }

            print_log_text("var_declaration: type_specifier declaration_list SEMICOLON");

            erm_h((yyvsp[-2].helper)); erm_h((yyvsp[-1].helper));
        }
#line 3182 "y.tab.c"
    break;

  case 37: /* var_declaration: type_specifier declaration_list error SEMICOLON  */
#line 1157 "1705010.y"
                                                          { 

            /**
                To handle errors like :
                    int x-y;
                    int x[10]-y;
                    int x[10.5]-y;
            **/            

            print_grammar_rule("var_declaration","type_specifier declaration_list SEMICOLON");
            
            (yyval.helper) = new Helper();

            // update text
            (yyval.helper)->text = (yyvsp[-3].helper)->text;
            (yyval.helper)->text += " ";
            (yyval.helper)->text += (yyvsp[-2].helper)->text;
            (yyval.helper)->text += ";";

            // insert all declaration_list ID to SymbolTable with VAR_TYPE
            for(auto el:(yyvsp[-2].helper)->v)
            {
                if(el->var_type == "array") el->setVarType((yyvsp[-3].helper)->text + "_array") ; 
                else el->setVarType((yyvsp[-3].helper)->text); 
                
                if(!sym_tab->insert_symbol(*el)) // already present in current scope
                {
                    error_multiple_declaration(el->key);
                }

            }

            print_log_text("var_declaration: type_specifier declaration_list error SEMICOLON");

            erm_h((yyvsp[-3].helper)); erm_h((yyvsp[-2].helper));
        }
#line 3223 "y.tab.c"
    break;

  case 38: /* type_specifier: INT  */
#line 1195 "1705010.y"
                     { 
                    print_grammar_rule("type_specifier","INT"); 

                    (yyval.helper) = new Helper();
                    (yyval.helper)->text = (yyvsp[0].symbol_info)->key; 

                    print_log_text("type_specifier: INT");

                    erm_s((yyvsp[0].symbol_info));
                }
#line 3238 "y.tab.c"
    break;

  case 39: /* type_specifier: FLOAT  */
#line 1205 "1705010.y"
                        { 
                    print_grammar_rule("type_specifier","FLOAT"); 

                    (yyval.helper) = new Helper();
                    (yyval.helper)->text = (yyvsp[0].symbol_info)->key; 

                    print_log_text("type_specifier: FLOAT");

                    erm_s((yyvsp[0].symbol_info));
                }
#line 3253 "y.tab.c"
    break;

  case 40: /* type_specifier: VOID  */
#line 1215 "1705010.y"
                       { 
                    print_grammar_rule("type_specifier","VOID"); 

                    (yyval.helper) = new Helper();
                    (yyval.helper)->text = (yyvsp[0].symbol_info)->key;

                    print_log_text("type_specifier: VOID");

                    erm_s((yyvsp[0].symbol_info));
                }
#line 3268 "y.tab.c"
    break;

  case 41: /* declaration_list: declaration_list COMMA ID  */
#line 1227 "1705010.y"
                                            { 
                    print_grammar_rule("declaration_list","declaration_list COMMA ID");
                    
                    (yyval.helper) = new Helper();

                    // update text
                    (yyval.helper)->text = (yyvsp[-2].helper)->text;
                    (yyval.helper)->text += ",";
                    (yyval.helper)->text += (yyvsp[0].symbol_info)->key;

                    // update type
                    (yyval.helper)->HelperType = (yyvsp[-2].helper)->HelperType;

                    // init update vector
                    (yyval.helper)->v = (yyvsp[-2].helper)->v;
                    (yyval.helper)->v.push_back((yyvsp[0].symbol_info));
                    // $$->print();

                    print_log_text("declaration_list: declaration_list COMMA ID");

                    erm_h((yyvsp[-2].helper)); 
                    // erm_s($3); // can't delete this as we will need it later to insert
            }
#line 3296 "y.tab.c"
    break;

  case 42: /* declaration_list: declaration_list error COMMA ID  */
#line 1250 "1705010.y"
                                              {

                /**
                To handle errors like :
                    int x-y,z;
                **/    

                print_grammar_rule("declaration_list","declaration_list COMMA ID");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-3].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[0].symbol_info)->key;

                // update type
                (yyval.helper)->HelperType = (yyvsp[-3].helper)->HelperType;

                // init update vector
                (yyval.helper)->v = (yyvsp[-3].helper)->v;
                (yyval.helper)->v.push_back((yyvsp[0].symbol_info));
                // $$->print();

                print_log_text("declaration_list: declaration_list error COMMA ID");

                erm_h((yyvsp[-3].helper)); 
                // erm_s($4);
            }
#line 3330 "y.tab.c"
    break;

  case 43: /* declaration_list: declaration_list COMMA ID LTHIRD CONST_INT RTHIRD  */
#line 1279 "1705010.y"
                                                                        {
               print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
           
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-5].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[-3].symbol_info)->key;
                (yyval.helper)->text += "[";
                (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                (yyval.helper)->text += "]";

                // update type
                (yyval.helper)->HelperType = (yyvsp[-5].helper)->HelperType;

                // init & update vector
                (yyval.helper)->v = (yyvsp[-5].helper)->v;
                (yyvsp[-3].symbol_info)->setVarType("array");
                (yyval.helper)->v.push_back((yyvsp[-3].symbol_info));
                // $$->print();

                print_log_text("declaration_list: declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");

                erm_h((yyvsp[-5].helper)); 
                // erm_s($3); 
                erm_s((yyvsp[-1].symbol_info));
           }
#line 3363 "y.tab.c"
    break;

  case 44: /* declaration_list: declaration_list error COMMA ID LTHIRD CONST_INT RTHIRD  */
#line 1307 "1705010.y"
                                                                     {

               /**
                To handle errors like :
                    int x-y,z[10];
                **/  

               print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
           
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-6].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[-3].symbol_info)->key;
                (yyval.helper)->text += "[";
                (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                (yyval.helper)->text += "]";

                // update type
                (yyval.helper)->HelperType = (yyvsp[-6].helper)->HelperType;

                // init & update vector
                (yyval.helper)->v = (yyvsp[-6].helper)->v;
                (yyvsp[-3].symbol_info)->setVarType("array");
                (yyval.helper)->v.push_back((yyvsp[-3].symbol_info));
                // $$->print();

                print_log_text("declaration_list: declaration_list error COMMA ID LTHIRD CONST_INT RTHIRD");

                erm_h((yyvsp[-6].helper)); 
                // erm_s($4); 
                erm_s((yyvsp[-1].symbol_info));
           }
#line 3402 "y.tab.c"
    break;

  case 45: /* declaration_list: declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD  */
#line 1341 "1705010.y"
                                                                 {

                /***
                    THIS IS AS EXTRA RULE ADDED TO CATCH ERROR
                ***/

               print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD");
           
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-5].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[-3].symbol_info)->key;
                (yyval.helper)->text += "[";
                (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                (yyval.helper)->text += "]";

                // update type
                (yyval.helper)->HelperType = (yyvsp[-5].helper)->HelperType;

                // int & update vector
                (yyval.helper)->v = (yyvsp[-5].helper)->v;
                (yyvsp[-3].symbol_info)->setVarType("array");
                (yyval.helper)->v.push_back((yyvsp[-3].symbol_info));
                // $$->print();

                error_array_size_float();

                print_log_text("declaration_list: declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD");

                erm_h((yyvsp[-5].helper)); 
                // erm_s($3); 
                erm_s((yyvsp[-1].symbol_info));
           
            }
#line 3443 "y.tab.c"
    break;

  case 46: /* declaration_list: declaration_list error COMMA ID LTHIRD CONST_FLOAT RTHIRD  */
#line 1377 "1705010.y"
                                                                        {

                /***
                    THIS IS AS EXTRA RULE ADDED TO CATCH ERROR
                    
                    Also,
                    To handle errors like :
                    int x-y,z[10.5];
                ***/

               print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD");
           
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-6].helper)->text;
                (yyval.helper)->text += ",";
                (yyval.helper)->text += (yyvsp[-3].symbol_info)->key;
                (yyval.helper)->text += "[";
                (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                (yyval.helper)->text += "]";

                // update type
                (yyval.helper)->HelperType = (yyvsp[-6].helper)->HelperType;

                // int & update vector
                (yyval.helper)->v = (yyvsp[-6].helper)->v;
                (yyvsp[-3].symbol_info)->setVarType("array");
                (yyval.helper)->v.push_back((yyvsp[-3].symbol_info));
                // $$->print();

                error_array_size_float();

                print_log_text("declaration_list: declaration_list error COMMA ID LTHIRD CONST_FLOAT RTHIRD");

                erm_h((yyvsp[-6].helper)); 
                // erm_s($4); 
                erm_s((yyvsp[-1].symbol_info));
           
            }
#line 3488 "y.tab.c"
    break;

  case 47: /* declaration_list: ID  */
#line 1417 "1705010.y"
                         {     
                    print_grammar_rule("declaration_list","ID");

                    (yyval.helper) = new Helper();

                    // update text
                    (yyval.helper)->text = (yyvsp[0].symbol_info)->key;

                    // init vector
                    (yyval.helper)->v.push_back((yyvsp[0].symbol_info));

                    print_log_text("declaration_list: ID");

                    // erm_s($1);
            }
#line 3508 "y.tab.c"
    break;

  case 48: /* declaration_list: ID LTHIRD CONST_INT RTHIRD  */
#line 1432 "1705010.y"
                                                 {

                    print_grammar_rule("declaration_list","ID LTHIRD CONST_INT RTHIRD");

                    (yyval.helper) = new Helper();

                    // update text
                    (yyval.helper)->text = (yyvsp[-3].symbol_info)->key;
                    (yyval.helper)->text += "[";
                    (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                    (yyval.helper)->text += "]";

                    // init vector
                    (yyvsp[-3].symbol_info)->setVarType("array");
                    (yyval.helper)->v.push_back((yyvsp[-3].symbol_info));
                    // cout<<"PRINT"<<endl;
                    // $$->print();

                    print_log_text("declaration_list: ID LTHIRD CONST_INT RTHIRD");

                    // erm_s($1); 
                    erm_s((yyvsp[-1].symbol_info));
            }
#line 3536 "y.tab.c"
    break;

  case 49: /* declaration_list: ID LTHIRD CONST_FLOAT RTHIRD  */
#line 1455 "1705010.y"
                                           {

                    /***
                        THIS IS AS EXTRA RULE ADDED TO CATCH ERROR
                    ***/

                    print_grammar_rule("declaration_list","ID LTHIRD CONST_FLOAT RTHIRD");

                    (yyval.helper) = new Helper();

                    // update text
                    (yyval.helper)->text = (yyvsp[-3].symbol_info)->key;
                    (yyval.helper)->text += "[";
                    (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                    (yyval.helper)->text += "]";

                    // init vector
                    (yyval.helper)->v.push_back((yyvsp[-3].symbol_info));

                    error_array_size_float();

                    print_log_text("declaration_list: ID LTHIRD CONST_FLOAT RTHIRD");

                    // erm_s($1); 
                    erm_s((yyvsp[-1].symbol_info));
           }
#line 3567 "y.tab.c"
    break;

  case 50: /* statements: statement  */
#line 1483 "1705010.y"
                      {
            print_grammar_rule("statements","statement");
            
            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("statements: statement");

            erm_h((yyvsp[0].helper));  
        }
#line 3582 "y.tab.c"
    break;

  case 51: /* statements: statements statement  */
#line 1493 "1705010.y"
                                  {
            print_grammar_rule("statements","statements statement");
        
            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[-1].helper)->text;
            (yyval.helper)->text += "\n";
            (yyval.helper)->text += (yyvsp[0].helper)->text;

            print_log_text("statements: statements statement");

            erm_h((yyvsp[-1].helper));  erm_h((yyvsp[0].helper));   
        }
#line 3599 "y.tab.c"
    break;

  case 52: /* statements: statements error statement  */
#line 1505 "1705010.y"
                                     {
            print_grammar_rule("statements","statements statement");
        
            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[-2].helper)->text;
            (yyval.helper)->text += "\n";
            (yyval.helper)->text += (yyvsp[0].helper)->text;

            print_log_text("statements: statements error statement");

            erm_h((yyvsp[-2].helper));  erm_h((yyvsp[0].helper));   
        }
#line 3616 "y.tab.c"
    break;

  case 53: /* statement: var_declaration  */
#line 1520 "1705010.y"
                           {
            print_grammar_rule("statement","var_declaration");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("statement: var_declaration");

            erm_h((yyvsp[0].helper));
        }
#line 3631 "y.tab.c"
    break;

  case 54: /* statement: func_definition  */
#line 1530 "1705010.y"
                        {
          print_grammar_rule("statement","func_definition");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("statement: func_definition");
            error_nested_function();

            erm_h((yyvsp[0].helper));
      }
#line 3647 "y.tab.c"
    break;

  case 55: /* statement: func_declaration  */
#line 1541 "1705010.y"
                         {
          print_grammar_rule("statement","func_declaration");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("statement: func_declaration");
            error_nested_function();

            erm_h((yyvsp[0].helper));
      }
#line 3663 "y.tab.c"
    break;

  case 56: /* statement: expression_statement  */
#line 1552 "1705010.y"
                                 {
            print_grammar_rule("statement","expression_statement");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("statement: expression_statement");

            erm_h((yyvsp[0].helper));
        }
#line 3678 "y.tab.c"
    break;

  case 57: /* statement: compound_statement  */
#line 1562 "1705010.y"
                               {
            print_grammar_rule("statement","compound_statement");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].helper)->text;

            print_log_text("statement: compound_statement");

            erm_h((yyvsp[0].helper));
        }
#line 3693 "y.tab.c"
    break;

  case 58: /* statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement  */
#line 1572 "1705010.y"
                                                                                             {
            print_grammar_rule("statement","FOR LPAREN expression_statement expression_statement expression RPAREN statement");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = "for";
            (yyval.helper)->text += "(";
            (yyval.helper)->text += (yyvsp[-4].helper)->text;
            (yyval.helper)->text += (yyvsp[-3].helper)->text;
            (yyval.helper)->text += (yyvsp[-2].helper)->text;
            (yyval.helper)->text += ")";
            (yyval.helper)->text += (yyvsp[0].helper)->text;
            
            print_log_text("statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement");

            erm_h((yyvsp[-4].helper)); erm_h((yyvsp[-3].helper)); erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
        }
#line 3715 "y.tab.c"
    break;

  case 59: /* statement: IF LPAREN expression RPAREN statement  */
#line 1589 "1705010.y"
                                                                        { 
            print_grammar_rule("statement","IF LPAREN expression RPAREN statement");
            
            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = "if";
            (yyval.helper)->text += "(";
            (yyval.helper)->text += (yyvsp[-2].helper)->text;
            (yyval.helper)->text += ")";
            (yyval.helper)->text += (yyvsp[0].helper)->text;

            print_log_text("statement: IF LPAREN expression RPAREN statement");

            erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper)); 
        }
#line 3735 "y.tab.c"
    break;

  case 60: /* statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 1604 "1705010.y"
                                                                 {

            print_grammar_rule("statement","IF LPAREN expression RPAREN statement ELSE statement");
        
            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = "if";
            (yyval.helper)->text += "(";
            (yyval.helper)->text += (yyvsp[-4].helper)->text;
            (yyval.helper)->text += ")";
            (yyval.helper)->text += (yyvsp[-2].helper)->text;
            (yyval.helper)->text += "\nelse ";
            (yyval.helper)->text += (yyvsp[0].helper)->text;

            print_log_text("statement:IF LPAREN expression RPAREN statement ELSE statement");

            erm_h((yyvsp[-4].helper)); erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
        
        }
#line 3759 "y.tab.c"
    break;

  case 61: /* statement: WHILE LPAREN expression RPAREN statement  */
#line 1623 "1705010.y"
                                                     {
            print_grammar_rule("statement","WHILE LPAREN expression RPAREN statement");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = "while";
            (yyval.helper)->text += "(";
            (yyval.helper)->text += (yyvsp[-2].helper)->text;
            (yyval.helper)->text += ")";
            (yyval.helper)->text += (yyvsp[0].helper)->text;

            print_log_text("statement: WHILE LPAREN expression RPAREN statement");

            erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper)); 
        }
#line 3779 "y.tab.c"
    break;

  case 62: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 1638 "1705010.y"
                                               {
            print_grammar_rule("statement","PRINTLN LPAREN ID RPAREN SEMICOLON");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = "printf";
            (yyval.helper)->text += "(";
            (yyval.helper)->text += (yyvsp[-2].symbol_info)->key;
            (yyval.helper)->text += ")";
            (yyval.helper)->text += ";";

            print_log_text("statement: PRINTLN LPAREN ID RPAREN SEMICOLON");

            // check error
            SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-2].symbol_info)->key);

            if(ret_symbol == NULL)
            {
                error_undeclared_variable((yyvsp[-2].symbol_info)->key);
                (yyval.helper)->setHelperType("NULL");
            }
            
            erm_s((yyvsp[-2].symbol_info));
        }
#line 3807 "y.tab.c"
    break;

  case 63: /* statement: RETURN expression SEMICOLON  */
#line 1661 "1705010.y"
                                        {
            print_grammar_rule("statement","RETURN expression SEMICOLON");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = "return";
            (yyval.helper)->text += " ";
            (yyval.helper)->text += (yyvsp[-1].helper)->text;
            (yyval.helper)->text += ";";

            print_log_text("statement: RETURN expression SEMICOLON");

            erm_h((yyvsp[-1].helper)); 
        }
#line 3825 "y.tab.c"
    break;

  case 64: /* statement: RETURN SEMICOLON  */
#line 1674 "1705010.y"
                           {

            /***
                EXTRA RULE ADDED 
            ***/

            print_grammar_rule("statement","RETURN SEMICOLON");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = "return";
            (yyval.helper)->text += ";";

            print_log_text("statement: RETURN SEMICOLON");
        }
#line 3844 "y.tab.c"
    break;

  case 65: /* expression_statement: SEMICOLON  */
#line 1690 "1705010.y"
                                {
                    print_grammar_rule("expression_statement","SEMICOLON");

                    (yyval.helper) = new Helper();
                    (yyval.helper)->text = ";";

                    print_log_text("expression_statement: SEMICOLON");
                }
#line 3857 "y.tab.c"
    break;

  case 66: /* expression_statement: expression SEMICOLON  */
#line 1698 "1705010.y"
                                               {
                    print_grammar_rule("expression_statement","expression SEMICOLON");
                    
                    (yyval.helper) = new Helper();

                    // update text
                    (yyval.helper)->text = (yyvsp[-1].helper)->text;
                    (yyval.helper)->text += ";";

                    print_log_text("expression_statement: expression SEMICOLON");

                    erm_h((yyvsp[-1].helper));
                }
#line 3875 "y.tab.c"
    break;

  case 67: /* variable: ID  */
#line 1713 "1705010.y"
             { 
            print_grammar_rule("variable","ID");
            (yyval.helper) = new Helper();

            // update text
            (yyval.helper)->text = (yyvsp[0].symbol_info)->key;

            // check error
            SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[0].symbol_info)->key);

            if(ret_symbol == NULL)
            {
                error_undeclared_variable((yyvsp[0].symbol_info)->key);
                (yyval.helper)->setHelperType("NULL");
            }
            else
            {
                if(ret_symbol->var_type == "int_array" || ret_symbol->var_type == "float_array")
                {
                    error_type_mismatch(ret_symbol->key + " is an array"); // should i change this to indexing
                    (yyval.helper)->setHelperType("NULL");
                }
                else{
                    (yyval.helper)->setHelperType(ret_symbol->var_type);
                }
                //  cout<<"Helper : "<<$$->HelperType<<endl;
            }

            print_log_text("variable: ID");

            erm_s((yyvsp[0].symbol_info));
        }
#line 3912 "y.tab.c"
    break;

  case 68: /* variable: ID LTHIRD expression RTHIRD  */
#line 1745 "1705010.y"
                                       {
            print_grammar_rule("variable","ID LTHIRD expression RTHIRD");
            
            (yyval.helper) = new Helper();

            // update text
            (yyval.helper)->text = (yyvsp[-3].symbol_info)->key;
            (yyval.helper)->text += "[";
            (yyval.helper)->text += (yyvsp[-1].helper)->text;
            (yyval.helper)->text += "]";

            // check error
            SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-3].symbol_info)->key);

            if(ret_symbol == NULL)
            {
                error_undeclared_variable((yyvsp[-3].symbol_info)->key);
                (yyval.helper)->setHelperType("NULL");
            }
            else
            {
                if(ret_symbol->var_type == "int" || ret_symbol->var_type == "float")
                {
                    error_not_array(ret_symbol->key);
                    (yyval.helper)->setHelperType("NULL");
                }
                else{
                    (yyval.helper)->setHelperType(ret_symbol->var_type);
                }
                // cout<<"HelperType : "<<$$->HelperType<<endl;
            }

            if((yyvsp[-1].helper)->HelperType != "int")
            {
                error_array_index_invalid();
            }

            print_log_text("variable: ID LTHIRD expression RTHIRD");

            erm_h((yyvsp[-1].helper));
            erm_s((yyvsp[-3].symbol_info));
         }
#line 3959 "y.tab.c"
    break;

  case 69: /* expression: logic_expression  */
#line 1789 "1705010.y"
                                {
                print_grammar_rule("expression","logic_expression");

                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = (yyvsp[0].helper)->text;
                // update vector : push up
                (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

                print_log_text("expression: logic_expression");

                erm_h((yyvsp[0].helper));
            }
#line 3977 "y.tab.c"
    break;

  case 70: /* expression: variable ASSIGNOP logic_expression  */
#line 1802 "1705010.y"
                                                {
                print_grammar_rule("expression","variable ASSIGNOP logic_expression");
                
                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[-2].helper)->text;
                (yyval.helper)->text += "=";
                (yyval.helper)->text += (yyvsp[0].helper)->text;

                //check error
                // cout<<$1->HelperType<<" ---- "<<$3->HelperType<<endl;
                if(!check_assignop((yyvsp[-2].helper)->HelperType,(yyvsp[0].helper)->HelperType))
                {
                    if((yyvsp[-2].helper)->HelperType=="void" || (yyvsp[0].helper)->HelperType=="void")
                    {
                        error_type_cast_void();
                    }
                    else
                    {
                        error_type_mismatch();
                    }
                }

                print_log_text("expression: variable ASSIGNOP logic_expression");

                erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
            }
#line 4010 "y.tab.c"
    break;

  case 71: /* logic_expression: rel_expression  */
#line 1834 "1705010.y"
                                 {
                print_grammar_rule("logic_expression","rel_expression");

                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = (yyvsp[0].helper)->text;
                // update vector : push up
                (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

                print_log_text("logic_expression: rel_expression");

                erm_h((yyvsp[0].helper)); 
            }
#line 4028 "y.tab.c"
    break;

  case 72: /* logic_expression: rel_expression LOGICOP rel_expression  */
#line 1847 "1705010.y"
                                                         {
                print_grammar_rule("logic_expression","rel_expression LOGICOP rel_expression");
                
                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = (yyvsp[-2].helper)->text;
                (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                (yyval.helper)->text += (yyvsp[0].helper)->text;

                // do implicit typecast
                string typecast_ret = do_implicit_typecast((yyvsp[-2].helper)->HelperType,(yyvsp[0].helper)->HelperType);

                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") (yyval.helper)->setHelperType("int"); // ALWAYS INT
                    else {

                        if((yyvsp[-2].helper)->HelperType=="void" || (yyvsp[0].helper)->HelperType=="void")
                        {
                            error_type_cast_void();
                        }
                        else
                        {
                            error_type_cast();
                        }

                         (yyval.helper)->setHelperType("NULL");
                    }
                    
                    // cout<<"Implicit Typecast : "<<$$->HelperType<<"\n"<<endl;
                }
                else
                {
                    (yyval.helper)->setHelperType("NULL");
                }

                print_log_text("logic_expression: rel_expression LOGICOP rel_expression");

                erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
                erm_s((yyvsp[-1].symbol_info));
            }
#line 4074 "y.tab.c"
    break;

  case 73: /* rel_expression: simple_expression  */
#line 1890 "1705010.y"
                                  {
                print_grammar_rule("rel_expression","simple_expression");

                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = (yyvsp[0].helper)->text;
                // update vector : push up
                (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

                print_log_text("rel_expression: simple_expression");

                erm_h((yyvsp[0].helper));
            }
#line 4092 "y.tab.c"
    break;

  case 74: /* rel_expression: simple_expression RELOP simple_expression  */
#line 1903 "1705010.y"
                                                                {
                print_grammar_rule("rel_expression","simple_expression RELOP simple_expression");
                
                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = (yyvsp[-2].helper)->text;
                (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                (yyval.helper)->text += (yyvsp[0].helper)->text;

                // do implicit typecast
                string typecast_ret = do_implicit_typecast((yyvsp[-2].helper)->HelperType,(yyvsp[0].helper)->HelperType);

                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") (yyval.helper)->setHelperType("int"); // ALWAYS INT
                    else {

                        if((yyvsp[-2].helper)->HelperType=="void" || (yyvsp[0].helper)->HelperType=="void")
                        {
                            error_type_cast_void();
                        }
                        else
                        {
                            error_type_cast();
                        }

                         (yyval.helper)->setHelperType("NULL");
                    }
                    // cout<<"Implicit Typecast : "<<$$->HelperType<<"\n"<<endl;
                }
                else
                {
                    (yyval.helper)->setHelperType("NULL");
                }

                print_log_text("rel_expression : simple_expression RELOP simple_expression");

                erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
                erm_s((yyvsp[-1].symbol_info));
            }
#line 4137 "y.tab.c"
    break;

  case 75: /* simple_expression: term  */
#line 1945 "1705010.y"
                        {

                    print_grammar_rule("simple_expression","term");

                    (yyval.helper) = new Helper();
                    // update text
                    (yyval.helper)->text = (yyvsp[0].helper)->text;
                    // update vector : push up
                    (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

                    print_log_text("simple_expression: term");

                    erm_h((yyvsp[0].helper));
            }
#line 4156 "y.tab.c"
    break;

  case 76: /* simple_expression: simple_expression ADDOP term  */
#line 1959 "1705010.y"
                                                     {
                    print_grammar_rule("simple_expression","simple_expression ADDOP term");

                    (yyval.helper) = new Helper();
                    // update text
                    (yyval.helper)->text = (yyvsp[-2].helper)->text;
                    (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
                    (yyval.helper)->text += (yyvsp[0].helper)->text;
                    // do implicit typecast
                    // cout<<$1->HelperType<<" --- "<<$3->HelperType<<endl;
                    string typecast_ret = do_implicit_typecast((yyvsp[-2].helper)->HelperType,(yyvsp[0].helper)->HelperType);

                    if(typecast_ret != "NULL")
                    {
                        if(typecast_ret != "error") (yyval.helper)->setHelperType(typecast_ret);
                        else {

                        if((yyvsp[-2].helper)->HelperType=="void" || (yyvsp[0].helper)->HelperType=="void")
                        {
                            error_type_cast_void();
                        }
                        else
                        {
                            error_type_cast();
                        }

                         (yyval.helper)->setHelperType("NULL");
                        }
                        // cout<<"Implicit Typecast : "<<$$->HelperType<<"\n"<<endl;
                    }
                    else
                    {
                        (yyval.helper)->setHelperType("NULL");
                    }

                    print_log_text("simple_expression: simple_expression ADDOP term");

                    erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
                    erm_s((yyvsp[-1].symbol_info));
            }
#line 4201 "y.tab.c"
    break;

  case 77: /* term: unary_expression  */
#line 2001 "1705010.y"
                         {

            print_grammar_rule("term","unary_expression");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = (yyvsp[0].helper)->text;
            // update vector : push up
            (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

            print_log_text((yyval.helper)->text);

            erm_h((yyvsp[0].helper));
    }
#line 4220 "y.tab.c"
    break;

  case 78: /* term: term MULOP unary_expression  */
#line 2015 "1705010.y"
                                   {

            print_grammar_rule("term","term MULOP unary_expression");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = (yyvsp[-2].helper)->text;
            (yyval.helper)->text += (yyvsp[-1].symbol_info)->key;
            (yyval.helper)->text += (yyvsp[0].helper)->text;
            // implicit typecast
            string typecast_ret = do_implicit_typecast((yyvsp[-2].helper)->HelperType,(yyvsp[0].helper)->HelperType);

            if((yyvsp[-1].symbol_info)->key == "%") // both operand should be integer
            {
                if((yyvsp[0].helper)->text == "0")
                {
                    error_type_cast_mod_zero();
                    (yyval.helper)->setHelperType("NULL");
                }
                else
                {
                    if(typecast_ret != "int")
                    {
                        error_type_cast_mod();
                        (yyval.helper)->setHelperType("NULL");
                    }
                    else{
                        (yyval.helper)->setHelperType("int");
                        // cout<<"HERERE"<<endl;
                    }
                }
            }
            else
            {
                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") (yyval.helper)->setHelperType(typecast_ret);
                    else {

                        if((yyvsp[-2].helper)->HelperType=="void" || (yyvsp[0].helper)->HelperType=="void")
                        {
                            error_type_cast_void();
                        }
                        else
                        {
                            error_type_cast();
                        }

                         (yyval.helper)->setHelperType("NULL");
                    }
                    // cout<<"Implicit Typecast : "<<$$->HelperType<<"\n"<<endl;
                }
                else
                {
                    (yyval.helper)->setHelperType("NULL");
                }
            }

            print_log_text((yyval.helper)->text);

            erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
            erm_s((yyvsp[-1].symbol_info));
    }
#line 4288 "y.tab.c"
    break;

  case 79: /* unary_expression: ADDOP unary_expression  */
#line 2080 "1705010.y"
                                          {
                print_grammar_rule("unary_expression","ADDOP unary_expression");
                
                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = (yyvsp[-1].symbol_info)->key;
                (yyval.helper)->text += (yyvsp[0].helper)->text;
                // implicit typecast
                (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

                print_log_text((yyval.helper)->text);

                erm_h((yyvsp[0].helper));
                erm_s((yyvsp[-1].symbol_info));
            }
#line 4308 "y.tab.c"
    break;

  case 80: /* unary_expression: NOT unary_expression  */
#line 2095 "1705010.y"
                                           {
                print_grammar_rule("unary_expression","NOT unary_expression");
                
                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = "!";
                (yyval.helper)->text += (yyvsp[0].helper)->text;
                // implicit typecast
                (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

                print_log_text((yyval.helper)->text);

                erm_h((yyvsp[0].helper));
            }
#line 4327 "y.tab.c"
    break;

  case 81: /* unary_expression: factor  */
#line 2109 "1705010.y"
                              { 
                print_grammar_rule("unary_expression","factor");
                
                (yyval.helper) = new Helper();
                // update text
                (yyval.helper)->text = (yyvsp[0].helper)->text;
                // implicit typecast
                (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

                print_log_text((yyval.helper)->text);

                erm_h((yyvsp[0].helper));
            }
#line 4345 "y.tab.c"
    break;

  case 82: /* factor: variable  */
#line 2124 "1705010.y"
                 {

            print_grammar_rule("factor","variable");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = (yyvsp[0].helper)->text;
            // implicit typecast
            (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;

            print_log_text((yyval.helper)->text);

            erm_h((yyvsp[0].helper));
        }
#line 4364 "y.tab.c"
    break;

  case 83: /* factor: ID LPAREN argument_list RPAREN  */
#line 2138 "1705010.y"
                                         {

            print_grammar_rule("factor","ID LPAREN argument_list RPAREN");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = (yyvsp[-3].symbol_info)->key;
            (yyval.helper)->text += "(";
            (yyval.helper)->text += (yyvsp[-1].helper)->text;
            (yyval.helper)->text += ")";

            // check error
            SymbolInfo* ret_symbol = sym_tab->lookup((yyvsp[-3].symbol_info)->key);

            if(ret_symbol == NULL)
            {
                error_undeclared_function((yyvsp[-3].symbol_info)->key);
                (yyval.helper)->setHelperType("NULL");
            }
            else
            {
                if(ret_symbol->isFunction == false)
                {
                    (yyval.helper)->setHelperType("NULL");
                    error_not_function((yyvsp[-3].symbol_info)->key);
                    break;
                }

                (yyval.helper)->setHelperType(ret_symbol->var_type);

                if(ret_symbol->isFunctionDeclaration) // only declared , no definition
                {
                    error_function_not_implemented();
                }
                else // other errors
                {
                    // printing function param_list
                    // cout<<"OG Param : ";
                    // for(auto s:ret_symbol->param_v)
                    // {
                    //     cout<<s<<" , ";
                    // }
                    // cout<<endl;

                    // // printing argument_list
                    // cout<<"Called Args : ";
                    // for(auto s:$3->param_v)
                    // {
                    //     cout<<s<<" , ";
                    // }
                    // cout<<endl;

                    if(ret_symbol->param_v.size() != (yyvsp[-1].helper)->param_v.size())
                    {
                        error_function_parameter_number(ret_symbol->key);
                    }
                    else
                    {
                        for(int i=0;i<ret_symbol->param_v.size();i++)
                        {
                            if(!is_param_typecast_ok(ret_symbol->param_v[i],(yyvsp[-1].helper)->param_v[i])){
                                error_function_parameter_type(i+1,ret_symbol->key);
                                break;
                            }
                        }
                    }
                }
            }

            print_log_text((yyval.helper)->text);

            erm_h((yyvsp[-1].helper));
            erm_s((yyvsp[-3].symbol_info));
        }
#line 4443 "y.tab.c"
    break;

  case 84: /* factor: LPAREN expression RPAREN  */
#line 2212 "1705010.y"
                                   {

            print_grammar_rule("factor","LPAREN expression RPAREN");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = "(";
            (yyval.helper)->text += (yyvsp[-1].helper)->text;
            (yyval.helper)->text += ")";

            (yyval.helper)->HelperType = (yyvsp[-1].helper)->HelperType;

            print_log_text((yyval.helper)->text);

            erm_h((yyvsp[-1].helper));
        
        }
#line 4465 "y.tab.c"
    break;

  case 85: /* factor: CONST_INT  */
#line 2229 "1705010.y"
                     { 
            print_grammar_rule("factor","CONST_INT");

            // update text
            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[0].symbol_info)->key;

            // pass up
            (yyval.helper)->setHelperType("int");

            print_log_text((yyval.helper)->text);

            erm_s((yyvsp[0].symbol_info));
        }
#line 4484 "y.tab.c"
    break;

  case 86: /* factor: CONST_FLOAT  */
#line 2243 "1705010.y"
                       { 
            print_grammar_rule("factor","CONST_FLOAT");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = (yyvsp[0].symbol_info)->key;
            // pass up
            (yyval.helper)->setHelperType("float");

            print_log_text((yyval.helper)->text);

            erm_s((yyvsp[0].symbol_info));
        }
#line 4502 "y.tab.c"
    break;

  case 87: /* factor: ERROR_FLOAT  */
#line 2256 "1705010.y"
                   { 
            print_grammar_rule("factor","ERROR_FLOAT");

            (yyval.helper) = new Helper();
            // update text
            (yyval.helper)->text = (yyvsp[0].symbol_info)->key;
            // pass up
            (yyval.helper)->setHelperType("NULL");

            print_log_text((yyval.helper)->text);

            erm_s((yyvsp[0].symbol_info));
        }
#line 4520 "y.tab.c"
    break;

  case 88: /* factor: variable INCOP  */
#line 2269 "1705010.y"
                         {
            print_grammar_rule("factor","variable INCOP");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[-1].helper)->text;
            (yyval.helper)->text += "++";

            print_log_text((yyval.helper)->text);

            erm_h((yyvsp[-1].helper));
        }
#line 4536 "y.tab.c"
    break;

  case 89: /* factor: variable DECOP  */
#line 2280 "1705010.y"
                         {
            print_grammar_rule("factor","variable DECOP");

            (yyval.helper) = new Helper();
            (yyval.helper)->text = (yyvsp[-1].helper)->text;
            (yyval.helper)->text += "--";

            print_log_text((yyval.helper)->text);

            erm_h((yyvsp[-1].helper));
        }
#line 4552 "y.tab.c"
    break;

  case 90: /* argument_list: arguments  */
#line 2293 "1705010.y"
                         {

                    print_grammar_rule("argument_list","arguments");

                    (yyval.helper) = new Helper();
                    (yyval.helper)->text = (yyvsp[0].helper)->text;

                    (yyval.helper)->param_v = (yyvsp[0].helper)->param_v; 

                    print_log_text((yyval.helper)->text);

                    erm_h((yyvsp[0].helper));
                }
#line 4570 "y.tab.c"
    break;

  case 91: /* argument_list: %empty  */
#line 2306 "1705010.y"
                          {
                print_grammar_rule("argument_list","");
                (yyval.helper) = new Helper();
            }
#line 4579 "y.tab.c"
    break;

  case 92: /* arguments: arguments COMMA logic_expression  */
#line 2312 "1705010.y"
                                            {

                print_grammar_rule("arguments","arguments COMMA logic_expression");
                
                (yyval.helper) = new Helper();
                (yyval.helper)->text = (yyvsp[-2].helper)->text; 
                (yyval.helper)->text += ","; 
                (yyval.helper)->text += (yyvsp[0].helper)->text;

                // update vector
                (yyval.helper)->param_v = (yyvsp[-2].helper)->param_v; 
                (yyval.helper)->param_v.push_back((yyvsp[0].helper)->HelperType);

                print_log_text((yyval.helper)->text);

                erm_h((yyvsp[-2].helper)); erm_h((yyvsp[0].helper));
            }
#line 4601 "y.tab.c"
    break;

  case 93: /* arguments: logic_expression  */
#line 2329 "1705010.y"
                               {

                print_grammar_rule("arguments","logic_expression");

                (yyval.helper) = new Helper();

                // update text
                (yyval.helper)->text = (yyvsp[0].helper)->text; 
                // update helper type
                (yyval.helper)->HelperType = (yyvsp[0].helper)->HelperType;
                // cout<<"Logic Helper : "<<$$->HelperType<<endl;
                // init vector
                (yyval.helper)->param_v.push_back((yyvsp[0].helper)->HelperType);

                print_log_text((yyval.helper)->text);

                erm_h((yyvsp[0].helper));
            }
#line 4624 "y.tab.c"
    break;


#line 4628 "y.tab.c"

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 2349 "1705010.y"


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

    sym_tab->print_all_scope(logout);

    fprintf(logout, "Total lines: %d\n", line_count);
    fprintf(logout, "Total errors: %d\n", err_count);

    fclose(yyin);
    fclose(parseout);
    fclose(logout);
	fclose(errout);

    exit(0);
}
