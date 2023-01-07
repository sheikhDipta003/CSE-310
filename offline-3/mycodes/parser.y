%{
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

bool is_function_now = false;
vector<SymbolInfo>function_params;


string do_implicit_typecast(char* left_op, char* right_op)
{
    if(!strcmp(left_op,"NULL") || !strcmp(right_op,"NULL")) return "NULL";

    if(!strcmp(left_op,"void") || !strcmp(right_op,"void")) return "error";

    if((!strcmp(left_op,"float") || !strcmp(left_op,"float_array")) && (!strcmp(right_op,"float") || !strcmp(right_op,"float_array"))) return "float";
    if((!strcmp(left_op,"float") || !strcmp(left_op,"float_array")) && (!strcmp(right_op,"int") || !strcmp(right_op,"int_array"))) return "float";
    if((!strcmp(left_op,"int") || !strcmp(left_op,"int_array")) && (!strcmp(right_op,"float") || !strcmp(right_op,"float_array"))) return "float";
    if((!strcmp(left_op,"int") || !strcmp(left_op,"int_array")) && (!strcmp(right_op,"int") || !strcmp(right_op,"int_array"))) return "int";

    return "error";
}

bool is_param_typecast_ok(string og_p,string pass_p)
{
    if(og_p == "void") return pass_p == "void";
    if(og_p == "int") return (pass_p == "int" || pass_p == "int_array");
    if(og_p == "float") return pass_p == "float";
}

bool check_assignop(char* left_op, char* right_op)
{
    if(!strcmp(left_op,"NULL") || !strcmp(right_op,"NULL")) return true; 

    if(!strcmp(left_op,"void") || !strcmp(right_op,"void")) return false;
    if(!strcmp(left_op,"") || !strcmp(right_op,"")) return false;

    if((!strcmp(left_op,"int") || !strcmp(left_op,"int_array")) && (!strcmp(right_op,"int") || !strcmp(right_op,"int_array"))) return true;
    
    if((!strcmp(left_op,"float") || !strcmp(left_op,"float_array")) && strcmp(right_op,"void")) return true;

    return false;
}

void print_grammar_rule(string head,string body)
{
    fprintf(logout, "%s : %s\n", head.data(), body.data());
}

void error_multiple_declaration(string error_symbol, bool _isFunc)
{
    if(_isFunc) fprintf(errout, "Line# %d: Redefinition of parameter \'%s\'\n", line_count, error_symbol.data());
    else    fprintf(errout, "Line# %d: \'%s\' redeclared as different kind of symbol\n", line_count, error_symbol.data());
    err_count++;
}

void error_type_cast()
{
    fprintf(errout, "Line# %d: Incompatible Operand\n", line_count);
    err_count++;
}

void error_type_cast_void()
{
    fprintf(errout, "Line# %d: Void cannot be used in expression\n", line_count);
    err_count++;
}

void error_type_mismatch(string msg)
{
    fprintf(errout, "Line# %d: Conflicting types for \'%s\'\n", line_count, msg.data());
    err_count++;
}

typedef struct tnode {
    char valname[100];
    char token[100];
    char HelperType[100];
    vector<string> args_list;
    vector<SymbolInfo*> v;
	bool lastchild;
	struct tnode *child;
    struct tnode *ptr;
} tnode;

tnode *NTnode=NULL;
int numWS = 0;
string output = "\n";

void printtree(tnode *node)
{
    tnode *itr;

    for(int i=1;i<numWS;i++)   fprintf(parseout, " ");

    if(numWS)  fprintf(parseout, "\\");

    if(node->lastchild)
    {
        fprintf(parseout, "%s-> %s\n", node->token, node->valname);
        output.append(node->valname);
        output.append(" ");
    }
    else
    {
        fprintf(parseout, "%s\n", node->token);
        output.append(node->token);
        output.append(" ");
        numWS++;
    }

    for(itr = node->child; itr != NULL; itr = itr->ptr)
    printtree(itr);

    if(node->lastchild==0)
    numWS--;
}

tnode* CreateTnode()
{
    tnode *t = new struct tnode();
    t->ptr = NULL;
    t->child = NULL;
    t->lastchild = 0;

    strcpy(t->token, "");
    strcpy(t->valname, "");
    strcpy(t->HelperType, "");

    return(t);
}

tnode* ProgramNode(tnode* push, tnode* t)
{
	if(t->child  ==  NULL)
		t->child = push;
	else
	{
		tnode *itr;
		for(itr = t->child; itr->ptr != NULL; itr = itr->ptr);
		itr->ptr = push;
	}
	return(t);
}

tnode* ProgramNode(SymbolInfo* sym, tnode* t)
{
	tnode *push = CreateTnode();
	push->lastchild = 1;
	strcpy(push->valname, sym->lexeme.c_str());
	strcpy(push->token, sym->token.c_str());
	t = ProgramNode(push,t);
	return (t);
}

tnode* ProgramNode(string valname, string token, tnode* t)
{
	tnode *push = CreateTnode();
	push->lastchild = 1;
	strcpy(push->valname, valname.c_str());
	strcpy(push->token, token.c_str());
	t = ProgramNode(push,t);
	return (t);
}

tnode* ProgramNode(string token, tnode* t)
{
	tnode *push = CreateTnode();
	push->lastchild = 1;
    if(token == "LPAREN")   strcpy(push->valname, "(");
    else if(token == "RPAREN")  strcpy(push->valname, ")");
    else if(token == "COMMA")  strcpy(push->valname, ",");
    else if(token == "SEMICOLON")  strcpy(push->valname, ";");
    else if(token == "LCURL")  strcpy(push->valname, "{");
    else if(token == "RCURL")  strcpy(push->valname, "}");
    else if(token == "LTHIRD")  strcpy(push->valname, "[");
    else if(token == "RTHIRD")  strcpy(push->valname, "]");
    else if(token == "NOT")  strcpy(push->valname, "!");
    else if(token == "ASSIGNOP")  strcpy(push->valname, "=");
    else if(token == "DECOP")  strcpy(push->valname, "--");
    else if(token == "INCOP")  strcpy(push->valname, "++");
    else if(token == "RETURN")  strcpy(push->valname, "return");
	strcpy(push->token, token.c_str());
	t = ProgramNode(push,t);
	return (t);
}

void insert_function_to_global(SymbolInfo* temp_s,char* data_type)
{
    bool isParamListError = false;

    for(int i=0;i<function_params.size();i++)
    {
        if(function_params[i].lexeme == "dummy_key"){
            fprintf(errout, "Line# %d: Syntax error at parameter list of function definition\n", line_count);
            err_count++;
            isParamListError = true;
            break;
        }
    }

    if(!isParamListError){
        
        temp_s->token = "FUNCTION";
        temp_s->data_type = data_type;
        temp_s->isFunc = true;

        for(auto temp_p : function_params)
        {
            temp_s->args_list.push_back(temp_p.data_type);
        }

        if(!sym_tab->_insert(*temp_s))
        {
            SymbolInfo* ret_symbol = sym_tab->_search(temp_s->lexeme);

            if(ret_symbol->isFuncDecl == false){
                error_multiple_declaration(temp_s->lexeme, ret_symbol->isFuncParam);
            }
            else{
                if(ret_symbol->data_type != temp_s->data_type)
                {
                    fprintf(errout, "Line# %d: Conflicting types for \'%s\'\n", line_count, (temp_s->lexeme).data());
                    err_count++;
                }

                if(ret_symbol->args_list.size() != temp_s->args_list.size())
                {
                    error_type_mismatch(temp_s->lexeme);
                }
                else
                {
                    for(int i = 0; i < (ret_symbol->args_list.size()); i++)
                    {
                        if(ret_symbol->args_list[i] != temp_s->args_list[i]){
                            error_type_mismatch(temp_s->lexeme);
                        }
                    }
                }
                ret_symbol->isFuncDecl = false;
            }
        }
        else{
            SymbolInfo* ret_symbol = sym_tab->_search(temp_s->lexeme);
            ret_symbol->isFuncDecl = false;
        }
    }

    
}

%}


%union{
    SymbolInfo* symbol_info;
    char* string;
    struct tnode *node;
}


%token <string> IF ELSE LOWER_THAN_ELSE FOR WHILE DO BREAK CHAR DOUBLE RETURN SWITCH CASE DEFAULT CONTINUE PRINTLN INCOP DECOP ASSIGNOP NOT LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA SEMICOLON
%token <symbol_info> ID INT FLOAT VOID ADDOP MULOP RELOP LOGICOP CONST_CHAR CONST_INT CONST_FLOAT STRING ERROR_CHAR ERROR_FLOAT  

%type <node> start program unit variable var_declaration type_specifier func_declaration func_definition parameter_list
%type <node> expression factor unary_expression term simple_expression rel_expression statement statements compound_statement logic_expression expression_statement
%type <node> arguments argument_list
%type <node> declaration_list

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

start: program
	{
		//write your code in this block in all the similar blocks below

        print_grammar_rule("start","program");

        NTnode = CreateTnode();
        strcpy(NTnode->token,"start");
        NTnode = ProgramNode($1,NTnode);
	}
	;

program: program unit  {
            print_grammar_rule("program","program unit");
            tnode *a = CreateTnode();
            strcpy(a->token,"program");
            a=ProgramNode($1,a);
            a=ProgramNode($2,a);
            $$ = a;
        }
	| unit { 
            print_grammar_rule("program","unit");
            tnode *a = CreateTnode();
            strcpy(a->token,"program");
            a=ProgramNode($1,a);
            $$ = a;
        }
	;
	
unit: var_declaration { 
            print_grammar_rule("unit","var_declaration"); 
            tnode *a = CreateTnode();
            strcpy(a->token,"unit");
            a=ProgramNode($1,a);
            $$ = a;
        }
     | func_declaration { 
            print_grammar_rule("unit","func_declaration"); 
            tnode *a = CreateTnode();
            strcpy(a->token,"unit");
            a=ProgramNode($1,a);
            $$ = a;
        }
     | func_definition { 
            print_grammar_rule("unit","func_definition");
            tnode *a = CreateTnode();
            strcpy(a->token,"unit");
            a=ProgramNode($1,a);
            $$ = a;
        }
     ;
     
func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {  
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_declaration");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN", a);
                a=ProgramNode($4,a);
                a=ProgramNode("RPAREN", a);
                a=ProgramNode("SEMICOLON", a);
                $$ = a;        

                // insert function ID to SymbolTable with data_type
                $2->setDataType($1->token);
                $2->token = "FUNCTION";
                $2->isFunc = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    $2->args_list.push_back(temp_s.data_type);
                }
                
                if(sym_tab->_insert(*$2))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    ret_symbol->isFuncDecl = true; // mark as function declaration
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
                }

                // clear param_info
                function_params.clear();
        }
        | type_specifier ID LPAREN parameter_list RPAREN error { 
                
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_declaration");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN", a);
                a=ProgramNode($4,a);
                a=ProgramNode("RPAREN", a);
                $$ = a;

                // insert function ID to SymbolTable with data_type
                $2->setDataType($1->token);
                $2->token = "FUNCTION";
                $2->isFunc = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    $2->args_list.push_back(temp_s.data_type);
                }

                if(sym_tab->_insert(*$2))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    ret_symbol->isFuncDecl = true; // mark as function declaration
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
                }

                // clear param_info
                function_params.clear();    
        }
        | type_specifier ID LPAREN parameter_list error RPAREN SEMICOLON { 

                /**
                    To handle errors like: 
                        void foo(int x-y);
                **/
                
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_declaration");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN", a);
                a=ProgramNode($4,a);
                a=ProgramNode("RPAREN", a);
                a=ProgramNode("SEMICOLON", a);
                $$ = a;

                // insert function ID to SymbolTable with data_type
                $2->setDataType($1->token);
                $2->token = "FUNCTION";
                $2->isFunc = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    $2->args_list.push_back(temp_s.data_type);
                }

                if(sym_tab->_insert(*$2))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    ret_symbol->isFuncDecl = true; // mark as function declaration
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
                }

                // clear param_info
                function_params.clear();
        }
        | type_specifier ID LPAREN parameter_list error RPAREN error { 

                /**
                    To handle errors like: 
                        void foo(int x-y)
                **/
                
                print_grammar_rule("func_declaration","type_specifier ID LPAREN parameter_list RPAREN");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_declaration");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN", a);
                a=ProgramNode($4,a);
                a=ProgramNode("RPAREN", a);
                $$ = a;

                // insert function ID to SymbolTable with data_type
                $2->setDataType($1->token);
                $2->token = "FUNCTION";
                $2->isFunc = true;

                // update parameter type
                for(auto temp_s : function_params)
                {
                    $2->args_list.push_back(temp_s.data_type);
                }

                if(sym_tab->_insert(*$2))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    ret_symbol->isFuncDecl = true; // mark as function declaration
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
                }

                // clear param_info
                function_params.clear();
        }
		| type_specifier ID LPAREN RPAREN SEMICOLON { 

                print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_declaration");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN", a);
                a=ProgramNode("RPAREN", a);
                a=ProgramNode("SEMICOLON", a);
                $$ = a;

                // insert function ID to SymbolTable with data_type
                $2->setDataType($1->token);
                $2->token = "FUNCTION";
                $2->isFunc = true;
                
                if(sym_tab->_insert(*$2))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    ret_symbol->isFuncDecl = true; // mark as function declaration
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
                }

                function_params.clear();
            }
            | type_specifier ID LPAREN RPAREN error { 

                /**
                    To handle errors like: 
                        void foo()
                **/

            print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN");

            tnode *a = CreateTnode();
            strcpy(a->token,"func_declaration");
            a=ProgramNode($1,a);
            a=ProgramNode($2,a);
            a=ProgramNode("LPAREN", a);
            a=ProgramNode("RPAREN", a);
            $$ = a;

            // insert function ID to SymbolTable with data_type
            $2->setDataType($1->token);
            $2->token = "FUNCTION";
            $2->isFunc = true;
            
            if(sym_tab->_insert(*$2))
            {
                SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                ret_symbol->isFuncDecl = true; // mark as function declaration
            }
            else
            {
                SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
            }

            function_params.clear();
        }
        | type_specifier ID LPAREN error RPAREN SEMICOLON { 

                /**
                    To handle errors like: 
                        void foo(-);
                **/

                print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN SEMICOLON");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_declaration");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN", a);
                a=ProgramNode("RPAREN", a);
                a=ProgramNode("SEMICOLON", a);
                $$ = a;

                // insert function ID to SymbolTable with data_type
                $2->setDataType($1->token);
                $2->token = "FUNCTION";
                $2->isFunc = true;
                
                if(sym_tab->_insert(*$2))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    ret_symbol->isFuncDecl = true; // mark as function declaration
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
                }

                function_params.clear();
            }
        | type_specifier ID LPAREN error RPAREN error { 

                /**
                    To handle errors like: 
                        void foo(-)
                **/

                print_grammar_rule("func_declaration","type_specifier ID LPAREN RPAREN");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_declaration");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN", a);
                a=ProgramNode("RPAREN", a);
                $$ = a;

                // insert function ID to SymbolTable with data_type
                $2->setDataType($1->token);
                $2->token = "FUNCTION";
                $2->isFunc = true;
                
                if(sym_tab->_insert(*$2))
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    ret_symbol->isFuncDecl = true; // mark as function declaration
                }
                else
                {
                    SymbolInfo* ret_symbol = sym_tab->_search($2->lexeme);
                    error_multiple_declaration($2->lexeme, ret_symbol->isFuncParam);
                }

                function_params.clear();
            }
		;

		 
func_definition: type_specifier ID LPAREN parameter_list RPAREN { is_function_now = true;insert_function_to_global($2,$1->token); } compound_statement { 
                print_grammar_rule("func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement");
                tnode *a = CreateTnode();
                strcpy(a->token,"func_definition");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN",a);
                a=ProgramNode($4,a);
                a=ProgramNode("RPAREN",a);
                a=ProgramNode($7,a);
                $$ = a;

                // clear temp function params
                is_function_now = false;
                function_params.clear();
            }
        | type_specifier ID LPAREN parameter_list error RPAREN { is_function_now = true;insert_function_to_global($2,$1->token); } compound_statement { 
                
                /**
                    To handle cases like :
                        void foo(int x-y){}
                **/
                
                print_grammar_rule("func_definition","type_specifier ID LPAREN parameter_list RPAREN compound_statement");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_definition");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN",a);
                a=ProgramNode($4,a);
                a=ProgramNode("RPAREN",a);
                a=ProgramNode($8,a);
                $$ = a;

                // clear temp function params
                is_function_now = false;
                function_params.clear();
        }
		|   type_specifier ID LPAREN RPAREN {is_function_now = true;insert_function_to_global($2,$1->token);} compound_statement { 
                print_grammar_rule("func_definition","type_specifier ID LPAREN RPAREN compound_statement");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_definition");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN",a);
                a=ProgramNode("RPAREN",a);
                a=ProgramNode($6,a);
                $$ = a;

                // clear temp function params
                is_function_now = false;
                function_params.clear();
            }
        |  type_specifier ID LPAREN error RPAREN { is_function_now = true;insert_function_to_global($2,$1->token); } compound_statement {
                
                /**
                    To handle cases like :
                        void foo(-){}
                **/

                print_grammar_rule("func_definition","type_specifier ID LPAREN error RPAREN compound_statement");

                tnode *a = CreateTnode();
                strcpy(a->token,"func_definition");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode("LPAREN",a);
                a=ProgramNode("RPAREN",a);
                a=ProgramNode($7,a);
                $$ = a;
                
                // clear temp function params
                is_function_now = false;
                function_params.clear();

                yyclearin;
                yyerrok;
        }
 		;				


parameter_list: parameter_list COMMA type_specifier ID {
                print_grammar_rule("parameter_list","parameter_list COMMA type_specifier ID");

                tnode *a = CreateTnode();
                strcpy(a->token,"parameter_list");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($3,a);
                a=ProgramNode($4,a);
                $$ = a;

                // insert parameter ID to SymbolTable with data_type
                $4->setDataType($3->valname);
                $4->token = $3->token;
                $4->isFuncParam = true;
                function_params.push_back(*$4);
            }
        | parameter_list error COMMA type_specifier ID {

                /**
                    To handle errors like:
                    void foo(int x-y,int z){}
                **/

               print_grammar_rule("parameter_list","parameter_list COMMA type_specifier ID");
            
                tnode *a = CreateTnode();
                strcpy(a->token,"parameter_list");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($4,a);
                a=ProgramNode($5,a);
                $$ = a;

                // insert parameter ID to SymbolTable with data_type
                $5->setDataType($4->valname);
                $5->token = $4->token;
                $5->isFuncParam = true;
                function_params.push_back(*$5);
        }
        | parameter_list COMMA type_specifier {
            print_grammar_rule("parameter_list","parameter_list COMMA type_specifier");
            tnode *a = CreateTnode();
            strcpy(a->token,"parameter_list");
            a=ProgramNode($1,a);
            a=ProgramNode("COMMA",a);
            a=ProgramNode($3,a);
            $$ = a;

            SymbolInfo temp_s = SymbolInfo("dummy_key","dummy_value");
            temp_s.data_type = $3->valname;
            temp_s.isFuncParam = true;
            function_params.push_back(temp_s);
        }
        | parameter_list error COMMA type_specifier {

            /**
                To handle cases like:
                    void foo(int x-y,int);
            **/

             print_grammar_rule("parameter_list","parameter_list COMMA type_specifier");

            tnode *a = CreateTnode();
            strcpy(a->token,"parameter_list");
            a=ProgramNode($1,a);
            a=ProgramNode("COMMA",a);
            a=ProgramNode($4,a);
            $$ = a;

            SymbolInfo temp_s = SymbolInfo("dummy_key","dummy_value");
            temp_s.data_type = $4->valname;
            temp_s.isFuncParam = true;
            function_params.push_back(temp_s);
        }
 		| type_specifier ID  { 
                print_grammar_rule("parameter_list","type_specifier ID");
                tnode *a = CreateTnode();
                strcpy(a->token,"parameter_list");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                $$ = a;

                // insert parameter ID to Parameter SymbolTable with data_type
                $2->setDataType($1->valname);
                $2->token = $1->token;
                $2->isFuncParam = true;
                function_params.push_back(*$2);
        }
		| type_specifier {
            print_grammar_rule("parameter_list","type_specifier");
            tnode *a = CreateTnode();
            strcpy(a->token,"parameter_list");
            a=ProgramNode($1,a);
            $$ = a;

            SymbolInfo temp_s = SymbolInfo("dummy_key","dummy_value");
            temp_s.data_type = $1->valname;
            temp_s.isFuncParam = true;
            function_params.push_back(temp_s);
        }
 		;
 		
compound_statement: LCURL dummy_scope_function statements RCURL {
                print_grammar_rule("compound_statement","LCURL statements RCURL");

                tnode *a = CreateTnode();
                strcpy(a->token,"compound_statement");
                a=ProgramNode("LCURL", a);
                a=ProgramNode($3, a);
                a=ProgramNode("RCURL", a);
                $$ = a;

                // EXIT
                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
            }
            | LCURL dummy_scope_function RCURL {

                print_grammar_rule("compound_statement","LCURL RCURL");

                tnode *a = CreateTnode();
                strcpy(a->token,"compound_statement");
                a=ProgramNode("LCURL", a);
                a=ProgramNode("RCURL", a);
                $$ = a;

                // EXIT
                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
             }
            | LCURL dummy_scope_function statements error RCURL {
                print_grammar_rule("compound_statement","LCURL statements RCURL");

                tnode *a = CreateTnode();
                strcpy(a->token,"compound_statement");
                a=ProgramNode("LCURL", a);
                a=ProgramNode($3, a);
                a=ProgramNode("RCURL", a);
                $$ = a;

                 // EXIT
                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
            }
            | LCURL dummy_scope_function error statements RCURL {
                print_grammar_rule("compound_statement","LCURL statements RCURL");

                tnode *a = CreateTnode();
                strcpy(a->token,"compound_statement");
                a=ProgramNode("LCURL", a);
                a=ProgramNode($4, a);
                a=ProgramNode("RCURL", a);
                $$ = a;

                // EXIT
                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
            }
             | LCURL dummy_scope_function error RCURL {
                
                print_grammar_rule("compound_statement","LCURL error RCURL");

                tnode *a = CreateTnode();
                strcpy(a->token,"compound_statement");
                a=ProgramNode("LCURL", a);
                a=ProgramNode("RCURL", a);
                $$ = a;

                // EXIT
                sym_tab->_print(logout,'A');
                sym_tab->exitScope();
             }
 		    ;
dummy_scope_function:  {

                    sym_tab->enterScope(); 

                    if(is_function_now)
                    {
                        for(auto el:function_params)
                        {

                            if(el.lexeme == "dummy_key") continue;
                            // insert ID
                            if(!sym_tab->_insert(el)) // already present in current scope
                            {
                                error_multiple_declaration(el.lexeme,el.isFuncParam);
                            }
                        }
                    }
                }
                ;
 		    
var_declaration: type_specifier declaration_list SEMICOLON { 

            print_grammar_rule("var_declaration","type_specifier declaration_list SEMICOLON");
            tnode *a = CreateTnode();
            strcpy(a->token,"var_declaration");
            a=ProgramNode($1,a);
            a=ProgramNode($2,a);
            a=ProgramNode("SEMICOLON",a);
            $$ = a;

            if(!strcmp($1->valname,"void")){
                fprintf(errout, "Line# %d: Variable or field \'%s\' declared void\n", line_count, (($2->v[0])->lexeme).data());
                err_count++;
            }
            else{
                for(auto el:$2->v)
                {
                    if(el->data_type == "array") { el->setDataType(strcat($1->valname, "_array")) ; el->token = "ARRAY";}
                    else { el->setDataType($1->valname); el->token = $1->token; }
                    
                    if(el->token == "INT")  el->setDataType("int");
                    else if(el->token == "FLOAT")  el->setDataType("float");

                    if(!sym_tab->_insert(*el)) // already present in current scope
                    {
                        if(!strcmp(el->data_type.c_str(),$1->valname)) error_multiple_declaration(el->lexeme, el->isFuncParam);
                        else error_type_mismatch(el->lexeme);
                    }
                }
            }
        }
        | type_specifier declaration_list error SEMICOLON { 

            /**
                To handle errors like :
                    int x-y;
                    int x[10]-y;
                    int x[10.5]-y;
            **/            

            print_grammar_rule("var_declaration","type_specifier declaration_list SEMICOLON");

            tnode *a = CreateTnode();
            strcpy(a->token,"var_declaration");
            a=ProgramNode($1,a);
            a=ProgramNode($2,a);
            a=ProgramNode("SEMICOLON",a);
            $$ = a;
            
            // insert all declaration_list ID to SymbolTable with data_type
            for(auto el:$2->v)
            {
                if(el->data_type == "array") {el->setDataType(strcat($1->valname,"_array")) ; el->token = "ARRAY";}
                else {el->setDataType($1->valname); el->token = $1->valname;}
                
                if(!sym_tab->_insert(*el)) // already present in current scope
                {
                    if(!strcmp(el->data_type.c_str(),$1->valname)) error_multiple_declaration(el->lexeme, el->isFuncParam);
                    else error_type_mismatch(el->lexeme);
                }

            }
        }
 		;
 		 
type_specifier: INT  { 
                    print_grammar_rule("type_specifier","INT"); 

                    tnode *a = CreateTnode();
                    strcpy(a->token,"type_specifier");
                    a=ProgramNode($1,a);
                    $$ = a;
                    strcpy($$->valname,"int");
                    strcpy($$->token,"INT");
                }
 		| FLOAT { 
                    print_grammar_rule("type_specifier","FLOAT"); 

                    tnode *a = CreateTnode();
                    strcpy(a->token,"type_specifier");
                    a=ProgramNode($1,a);
                    $$ = a;
                    strcpy($$->valname,"float");
                    strcpy($$->token,"FLOAT");
                }
 		| VOID { 
                    print_grammar_rule("type_specifier","VOID"); 
                    tnode *a = CreateTnode();
                    strcpy(a->token,"type_specifier");
                    a=ProgramNode($1,a);
                    $$ = a;
                    strcpy($$->valname,"void");
                    strcpy($$->token,"VOID");
                }
 		;
 		
declaration_list: declaration_list COMMA ID { 
                    print_grammar_rule("declaration_list","declaration_list COMMA ID");
                    
                    tnode *a = CreateTnode();
                    strcpy(a->token,"declaration_list");
                    a=ProgramNode($1,a);
                    a=ProgramNode("COMMA",a);
                    a=ProgramNode($3,a);
                    $$ = a;

                    // update type
                    strcpy($$->HelperType, $1->HelperType);

                    // init update vector
                    $$->v = $1->v;
                    $$->v.push_back($3);
            }
            | declaration_list error COMMA ID {

                /**
                To handle errors like :
                    int x-y,z;
                **/    

                print_grammar_rule("declaration_list","declaration_list COMMA ID");

                tnode *a = CreateTnode();
                strcpy(a->token,"declaration_list");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($4,a);
                $$ = a;

                // update type
                strcpy($$->HelperType, $1->HelperType);

                // init update vector
                $$->v = $1->v;
                $$->v.push_back($4);
                fprintf(errout, "Line# %d: Syntax error at declaration list of variable declaration\n", line_count);
                err_count++;
            }
 		    | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD {
                print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");

                tnode *a = CreateTnode();
                strcpy(a->token,"declaration_list");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($3,a);
                a=ProgramNode("LTHIRD",a);
                a=ProgramNode($5,a);
                a=ProgramNode("RTHIRD",a);
                $$ = a;

                // update type
                strcpy($$->HelperType, $1->HelperType);

                // init & update vector
                $$->v = $1->v;
                $3->token ="ARRAY";
                $3->data_type ="array";
                $$->v.push_back($3);
           }
           | declaration_list error COMMA ID LTHIRD CONST_INT RTHIRD {

                /**
                    To handle errors like :
                        int x-y,z[10];
                    **/  

                print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");

                tnode *a = CreateTnode();
                strcpy(a->token,"declaration_list");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($4,a);
                a=ProgramNode("LTHIRD",a);
                a=ProgramNode($6,a);
                a=ProgramNode("RTHIRD",a);
                $$ = a;

                // update type
                strcpy($$->HelperType, $1->HelperType);

                // init & update vector
                $$->v = $1->v;
                $4->token ="ARRAY";
                $4->data_type ="array";
                $$->v.push_back($4);
           }
           | declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD {

                /***
                    THIS IS AS EXTRA RULE ADDED TO CATCH ERROR
                ***/

                print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD");

                tnode *a = CreateTnode();
                strcpy(a->token,"declaration_list");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($3,a);
                a=ProgramNode("LTHIRD",a);
                a=ProgramNode($5,a);
                a=ProgramNode("RTHIRD",a);
                $$ = a;

                // update type
                strcpy($$->HelperType, $1->HelperType);

                // int & update vector
                $$->v = $1->v;
                $$->v.push_back($3);

                fprintf(errout, "Line# %d: Non-integer Array Size\n", line_count);
                err_count++;
            }
            | declaration_list error COMMA ID LTHIRD CONST_FLOAT RTHIRD {

                    /***
                        THIS IS AS EXTRA RULE ADDED TO CATCH ERROR
                        
                        Also,
                        To handle errors like :
                        int x-y,z[10.5];
                    ***/

                print_grammar_rule("declaration_list","declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD");

                tnode *a = CreateTnode();
                strcpy(a->token,"declaration_list");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($4,a);
                a=ProgramNode("LTHIRD",a);
                a=ProgramNode($6,a);
                a=ProgramNode("RTHIRD",a);
                $$ = a;

                // update type
                strcpy($$->HelperType, $1->HelperType);

                // int & update vector
                $$->v = $1->v;
                $$->v.push_back($4);

                fprintf(errout, "Line# %d: Non-integer Array Size\n", line_count);
                err_count++;
            }
 		    | ID {     
                    print_grammar_rule("declaration_list","ID");

                    tnode *a = CreateTnode();
                    strcpy(a->token,"declaration_list");
                    a=ProgramNode($1,a);
                    $$ = a;

                    // init vector
                    $$->v.push_back($1);
            }
 		    | ID LTHIRD CONST_INT RTHIRD {

                    print_grammar_rule("declaration_list","ID LTHIRD CONST_INT RTHIRD");

                    tnode *a = CreateTnode();
                    strcpy(a->token,"declaration_list");
                    a=ProgramNode($1,a);
                    a=ProgramNode("LTHIRD",a);
                    a=ProgramNode($3,a);
                    a=ProgramNode("RTHIRD",a);
                    $$ = a;

                    // init vector
                    $1->token = "ARRAY";
                    $1->data_type ="array";
                    $$->v.push_back($1);
            }
            | ID LTHIRD CONST_FLOAT RTHIRD {

                    /***
                        THIS IS AS EXTRA RULE ADDED TO CATCH ERROR
                    ***/

                    print_grammar_rule("declaration_list","ID LTHIRD CONST_FLOAT RTHIRD");

                    tnode *a = CreateTnode();
                    strcpy(a->token,"declaration_list");
                    a=ProgramNode($1,a);
                    a=ProgramNode("LTHIRD",a);
                    a=ProgramNode($3,a);
                    a=ProgramNode("RTHIRD",a);
                    $$ = a;

                    // init vector
                    $$->v.push_back($1);

                    fprintf(errout, "Line# %d: Non-integer Array Size\n", line_count);
                    err_count++;
           }
 		  ;
 		  
statements: statement {
            print_grammar_rule("statements","statement");
            tnode *a = CreateTnode();
            strcpy(a->token,"statements");
            a=ProgramNode($1,a);
            $$ = a;
            
        }
	   | statements statement {
            print_grammar_rule("statements","statements statement");
        
            tnode *a = CreateTnode();
            strcpy(a->token,"statements");
            a=ProgramNode($1,a);
            a=ProgramNode($2,a);
            $$ = a;
        }
        | statements error statement {
            print_grammar_rule("statements","statements statement");
        
            tnode *a = CreateTnode();
            strcpy(a->token,"statements");
            a=ProgramNode($1,a);
            a=ProgramNode($3,a);
            $$ = a;
        }
        
	   ;
	   
statement: var_declaration {
            print_grammar_rule("statement","var_declaration");

            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1,a);
            $$ = a;
        }
      | func_definition {
            print_grammar_rule("statement","func_definition");

            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1,a);
            $$ = a;

            fprintf(errout, "Line# %d: A function cannot be defined inside another function\n", line_count);
            err_count++;
      }
      | func_declaration {
            print_grammar_rule("statement","func_declaration");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1,a);
            $$ = a;

            fprintf(errout, "Line# %d: A function cannot be defined inside another function\n", line_count);
            err_count++;
      }
	  | expression_statement {
            print_grammar_rule("statement","expression_statement");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1,a);
            $$ = a;
        }
	  | compound_statement {
            print_grammar_rule("statement","compound_statement");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1,a);
            $$ = a;
        }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement {
            print_grammar_rule("statement","FOR LPAREN expression_statement expression_statement expression RPAREN statement");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1, "FOR", a);
            a=ProgramNode("LPAREN", a);
            a=ProgramNode($3, a);
            a=ProgramNode($4, a);
            a=ProgramNode($5, a);
            a=ProgramNode("RPAREN", a);
            a=ProgramNode($7, a);
            $$ = a;
        }
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE { 
            print_grammar_rule("statement","IF LPAREN expression RPAREN statement");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1, "IF",a);
            a=ProgramNode("LPAREN",a);
            a=ProgramNode($3,a);
            a=ProgramNode("RPAREN",a);
            a=ProgramNode($5,a);
            $$ = a;
        }
	  | IF LPAREN expression RPAREN statement ELSE statement {

            print_grammar_rule("statement","IF LPAREN expression RPAREN statement ELSE statement");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1, "IF",a);
            a=ProgramNode("LPAREN",a);
            a=ProgramNode($3,a);
            a=ProgramNode("RPAREN",a);
            a=ProgramNode($1, "ELSE",a);
            a=ProgramNode($5,a);
            $$ = a;
        
        }
	  | WHILE LPAREN expression RPAREN statement {
            print_grammar_rule("statement","WHILE LPAREN expression RPAREN statement");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1,"WHILE",a);
            $$ = a;
        }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON {
            print_grammar_rule("statement","PRINTLN LPAREN ID RPAREN SEMICOLON");

            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode($1,"PRINTLN",a);
            a=ProgramNode("LPAREN",a);
            a=ProgramNode($3,a);
            a=ProgramNode("RPAREN",a);
            a=ProgramNode("SEMICOLON",a);
            $$ = a;

            // check error
            SymbolInfo* ret_symbol = sym_tab->_search($3->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared variable \'%s\'\n", line_count, ($3->lexeme).data());
                err_count++;
                strcpy($$->HelperType,"NULL");
            }
            
        }
	  | RETURN expression SEMICOLON {
            print_grammar_rule("statement","RETURN expression SEMICOLON");
            tnode *a = CreateTnode();
            strcpy(a->token,"statement");
            a=ProgramNode("RETURN",a);
            a=ProgramNode($2,a);
            a=ProgramNode("SEMICOLON",a);
            $$ = a;
        }
	  ;
	  
expression_statement: SEMICOLON	{
                    print_grammar_rule("expression_statement","SEMICOLON");
                    tnode *a = CreateTnode();
                    strcpy(a->token,"expression_statement");
                    a=ProgramNode("SEMICOLON",a);
                    $$ = a;
                }		
			| expression SEMICOLON {
                    print_grammar_rule("expression_statement","expression SEMICOLON");
                    tnode *a = CreateTnode();
                    strcpy(a->token,"expression_statement");
                    a=ProgramNode($1,a);
                    a=ProgramNode("SEMICOLON",a);
                    $$ = a;
                }
			;
	  
variable: ID { 
            print_grammar_rule("variable","ID");

            tnode *a = CreateTnode();
            strcpy(a->token,"variable");
            a=ProgramNode($1,a);
            $$ = a;

            // check error
            SymbolInfo* ret_symbol = sym_tab->_search($1->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared variable \'%s\'\n", line_count, ($1->lexeme).data());
                err_count++;
                strcpy($$->HelperType, "NULL");
            }
            else
            {
                if(ret_symbol->data_type == "int_array" || ret_symbol->data_type == "float_array")
                {
                    error_type_mismatch(ret_symbol->lexeme);
                    strcpy($$->HelperType, "NULL");
                }
                else{
                    strcpy($$->HelperType, ret_symbol->data_type.c_str());
                }
            }

        }		
	 | ID LTHIRD expression RTHIRD {
            print_grammar_rule("variable","ID LTHIRD expression RTHIRD");

            tnode *a = CreateTnode();
            strcpy(a->token,"variable");
            a=ProgramNode($1,a);
            a=ProgramNode("LTHIRD",a);
            a=ProgramNode($3,a);
            a=ProgramNode("RTHIRD",a);
            $$ = a;

            // check error
            SymbolInfo* ret_symbol = sym_tab->_search($1->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared variable \'%s\'\n", line_count, ($1->lexeme).data());
                err_count++;
                strcpy($$->HelperType, "NULL");
            }
            else
            {
                if(ret_symbol->data_type == "int" || ret_symbol->data_type == "float")
                {
                    fprintf(errout, "Line# %d: \'%s\' is not an array\n", line_count, (ret_symbol->lexeme).data());
                    err_count++;
                    strcpy($$->HelperType, "NULL");
                }
                else{
                    strcpy($$->HelperType, ret_symbol->data_type.c_str());
                }
            }

            if(strcmp($3->HelperType, "int"))
            {
                fprintf(errout, "Line# %d: Array subscript is not an integer\n", line_count);
                err_count++;
            }
         }
	 ;
	 
 expression: logic_expression	{
                print_grammar_rule("expression","logic_expression");
                tnode *a = CreateTnode();
                strcpy(a->token,"expression");
                a=ProgramNode($1,a);
                $$ = a;

                // update vector : push up
                strcpy($$->HelperType, $1->HelperType);
            }
	   | variable ASSIGNOP logic_expression {
                print_grammar_rule("expression","variable ASSIGNOP logic_expression");

                tnode *a = CreateTnode();
                strcpy(a->token,"expression");
                a=ProgramNode($1,a);
                a=ProgramNode("ASSIGNOP",a);
                a=ProgramNode($3,a);
                $$ = a;

                if(!check_assignop($1->HelperType,$3->HelperType))
                {
                    if(strcmp($1->HelperType,"void")==0 || strcmp($3->HelperType,"void")==0)
                    {
                        error_type_cast_void();
                    }
                    else if((!strcmp($1->HelperType,"int") || !strcmp($1->HelperType,"int_array")) && 
                            (!strcmp($3->HelperType,"float") || !strcmp($3->HelperType,"float_array")))
                    {
                        fprintf(errout, "Line# %d: Warning: possible loss of data in assignment of FLOAT to INT\n", line_count);
                        err_count++;
                    }
                }
            }	
	   ;


			 
logic_expression: rel_expression {
                print_grammar_rule("logic_expression","rel_expression"); 
                tnode *a = CreateTnode();
                strcpy(a->token,"logic_expression");
                a=ProgramNode($1,a);
                $$ = a;

                // update vector : push up
                strcpy($$->HelperType, $1->HelperType);
            }	
		 | rel_expression LOGICOP rel_expression {
                print_grammar_rule("logic_expression","rel_expression LOGICOP rel_expression");

                tnode *a = CreateTnode();
                strcpy(a->token,"logic_expression");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode($3,a);
                $$ = a;

                // do implicit typecast
                string typecast_ret = do_implicit_typecast($1->HelperType,$3->HelperType);

                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") strcpy($$->HelperType,"int"); // ALWAYS INT
                    else {

                        if(!strcmp($1->HelperType, "void") || !strcmp($3->HelperType, "void"))
                        {
                            error_type_cast_void();
                        }
                        else
                        {
                            error_type_cast();
                        }

                        strcpy($$->HelperType, "NULL");
                    }
                }
                else
                {
                    strcpy($$->HelperType, "NULL");
                }
            }	
		 ;
			
rel_expression: simple_expression {
                print_grammar_rule("rel_expression","simple_expression");
                tnode *a = CreateTnode();
                strcpy(a->token,"rel_expression");
                a=ProgramNode($1,a);
                $$ = a;

                // update vector : push up
                strcpy($$->HelperType, $1->HelperType);
            }
		| simple_expression RELOP simple_expression	{
                print_grammar_rule("rel_expression","simple_expression RELOP simple_expression");

                tnode *a = CreateTnode();
                strcpy(a->token,"rel_expression");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                a=ProgramNode($3,a);
                $$ = a;

                // do implicit typecast
                string typecast_ret = do_implicit_typecast($1->HelperType,$3->HelperType);

                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") strcpy($$->HelperType, "int"); // ALWAYS INT
                    else {

                        if(!strcmp($1->HelperType, "void") || !strcmp($3->HelperType, "void"))
                        {
                            error_type_cast_void();
                        }
                        else
                        {
                            error_type_cast();
                        }

                        strcpy($$->HelperType, "NULL");
                    }
                }
                else
                {
                    strcpy($$->HelperType, "NULL");
                }
            }
		;
				
simple_expression: term {

                    print_grammar_rule("simple_expression","term");
                    tnode *a = CreateTnode();
                    strcpy(a->token,"simple_expression");
                    a=ProgramNode($1,a);
                    $$ = a;

                    // update vector : push up
                    strcpy($$->HelperType, $1->HelperType);
            }
		    |   simple_expression ADDOP term {
                    print_grammar_rule("simple_expression","simple_expression ADDOP term");

                    tnode *a = CreateTnode();
                    strcpy(a->token,"simple_expression");
                    a=ProgramNode($1,a);
                    a=ProgramNode($2,a);
                    a=ProgramNode($3,a);
                    $$ = a;

                    string typecast_ret = do_implicit_typecast($1->HelperType,$3->HelperType);

                    if(typecast_ret != "NULL")
                    {
                        if(typecast_ret != "error")     strcpy($$->HelperType, typecast_ret.c_str());
                        else {
                            if(!strcmp($1->HelperType, "void") || !strcmp($3->HelperType, "void"))
                            {
                                error_type_cast_void();
                            }
                            else
                            {
                                error_type_cast();
                            }

                            strcpy($$->HelperType, "NULL");
                        }
                    }
                    else
                    {
                        strcpy($$->HelperType, "NULL");
                    }
            }
		    ;
					
term:	unary_expression {

            print_grammar_rule("term","unary_expression");
            tnode *a = CreateTnode();
            strcpy(a->token,"term");
            a=ProgramNode($1,a);
            $$ = a;

            // update vector : push up
            strcpy($$->HelperType, $1->HelperType);
    }
    |  term MULOP unary_expression {

            print_grammar_rule("term","term MULOP unary_expression");

            tnode *a = CreateTnode();
            strcpy(a->token,"term");
            a=ProgramNode($1,a);
            a=ProgramNode($2,a);
            a=ProgramNode($3,a);
            $$ = a;

            // implicit typecast
            string typecast_ret = do_implicit_typecast($1->HelperType,$3->HelperType);

            if($2->lexeme == "%")
            {
                if(!strcmp($3->valname,"0"))
                {
                    fprintf(errout, "Line# %d: Warning: modulus by zero\n", line_count);
                    err_count++;
                    strcpy($$->HelperType, "NULL");
                }
                else
                {
                    if(typecast_ret != "int")
                    {
                        fprintf(errout, "Line# %d: Operands of modulus must be integers\n", line_count);
                        err_count++;
                        strcpy($$->HelperType, "NULL");
                    }
                    else{
                        strcpy($$->HelperType, "int");
                    }
                }
            }
            else
            {
                if(typecast_ret != "NULL")
                {
                    if(typecast_ret != "error") strcpy($$->HelperType, typecast_ret.c_str());
                    else {
                        if(strcmp($1->HelperType, "void")==0 || strcmp($3->HelperType, "void")==0)
                        {
                            error_type_cast_void();
                        }
                        else
                        {
                            error_type_cast();
                        }

                        strcpy($$->HelperType, "NULL");
                    }
                }
                else
                {
                    strcpy($$->HelperType, "NULL");
                }
            }
    }
    ;

unary_expression: ADDOP unary_expression  {
                print_grammar_rule("unary_expression","ADDOP unary_expression");
                tnode *a = CreateTnode();
                strcpy(a->token,"unary_expression");
                a=ProgramNode($1,a);
                a=ProgramNode($2,a);
                $$ = a;
                // implicit typecast
                strcpy($$->HelperType, $2->HelperType);
            }
		    | NOT unary_expression {
                print_grammar_rule("unary_expression","NOT unary_expression");
                tnode *a = CreateTnode();
                strcpy(a->token,"unary_expression");
                a=ProgramNode("NOT",a);
                a=ProgramNode($2,a);
                $$ = a;
                // implicit typecast
                strcpy($$->HelperType, $2->HelperType);
            }
		    | factor  { 
                print_grammar_rule("unary_expression","factor");
                tnode *a = CreateTnode();
                strcpy(a->token,"unary_expression");
                a=ProgramNode($1,a);
                $$ = a;
                strcpy($$->valname, $1->valname);
                // implicit typecast
                strcpy($$->HelperType, $1->HelperType);
            }
		 ;
	
factor: variable {

            print_grammar_rule("factor","variable");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode($1,a);
            $$ = a;

            // implicit typecast
            strcpy($$->HelperType, $1->HelperType);
        }
	| ID LPAREN argument_list RPAREN {

            print_grammar_rule("factor","ID LPAREN argument_list RPAREN");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode($1,a);
            a=ProgramNode("LPAREN",a);
            a=ProgramNode($3,a);
            a=ProgramNode("RPAREN",a);
            $$ = a;

            // check error
            SymbolInfo* ret_symbol = sym_tab->_search($1->lexeme);

            if(ret_symbol == NULL)
            {
                fprintf(errout, "Line# %d: Undeclared function \'%s\'\n", line_count, ($1->lexeme).data());
                err_count++;
                strcpy($$->HelperType,"NULL");
            }
            else
            {
                if(ret_symbol->isFunc == false)
                {
                    strcpy($$->HelperType,"NULL");
                    fprintf(errout, "Line# %d: %s not a function\n", line_count, ($1->lexeme).data());
                    err_count++;
                }

                //convert data_type to lowercase and pass it up as HelperType of 'factor'
                char *temp = new char(100);
                strcpy(temp, ret_symbol->data_type.c_str());
                for(int i = 0; i < 100; i++)    temp[i] = tolower(temp[i]);
                strcpy($$->HelperType, temp);
                free(temp);
                //

                if(ret_symbol->isFuncDecl)
                {
                    fprintf(errout, "Line# %d: Function declared, but not defined\n", line_count);
                    err_count++;
                }
                else // other errors
                {
                    if(($3->args_list.size() - ret_symbol->args_list.size()) > 0){
                        fprintf(errout, "Line# %d: Too many arguments to function \'%s\'\n", line_count, (ret_symbol->lexeme).data());
                        err_count++;
                    }
                    else if(($3->args_list.size() - ret_symbol->args_list.size()) < 0){
                        fprintf(errout, "Line# %d: Too few arguments to function \'%s\'\n", line_count, (ret_symbol->lexeme).data());
                        err_count++;
                    }
                    else
                    {
                        for(int i=0;i<ret_symbol->args_list.size();i++)
                        {
                            if(!is_param_typecast_ok(ret_symbol->args_list[i], $3->args_list[i])){
                                fprintf(errout, "Line# %d: Type mismatch for argument %d of \'%s\'\n", line_count, (i+1), (ret_symbol->lexeme).data());
                                err_count++;
                            }
                        }
                    }
                }
            }
        }
	| LPAREN expression RPAREN {

            print_grammar_rule("factor","LPAREN expression RPAREN");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode("LPAREN",a);
            a=ProgramNode($2,a);
            a=ProgramNode("RPAREN",a);
            $$ = a;
        
            strcpy($$->HelperType, $2->HelperType);
        }
	| CONST_INT  { 
            print_grammar_rule("factor","CONST_INT");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode($1,a);
            $$ = a;
            strcpy($$->valname, $1->lexeme.c_str());
            // pass up
            strcpy($$->HelperType, "int");
        }
	| CONST_FLOAT  { 
            print_grammar_rule("factor","CONST_FLOAT");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode($1,a);
            $$ = a;
            strcpy($$->valname, $1->lexeme.c_str());
            // pass up
            strcpy($$->HelperType, "float");
        }
    | ERROR_FLOAT  { 
            print_grammar_rule("factor","ERROR_FLOAT");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode($1,a);
            $$ = a;
            strcpy($$->valname, $1->lexeme.c_str());
            // pass up
            strcpy($$->HelperType, "NULL");
        }
	| variable INCOP {
            print_grammar_rule("factor","variable INCOP");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode($1,a);
            a=ProgramNode("INCOP",a);
            $$ = a;
        }
	| variable DECOP {
            print_grammar_rule("factor","variable DECOP");
            tnode *a = CreateTnode();
            strcpy(a->token,"factor");
            a=ProgramNode($1,a);
            a=ProgramNode("DECOP",a);
            $$ = a;
        }
	;
	
argument_list: arguments {

                    print_grammar_rule("argument_list","arguments");
                    tnode *a = CreateTnode();
                    strcpy(a->token,"argument_list");
                    a=ProgramNode($1,a);
                    $$ = a;

                    $$->args_list = $1->args_list; 
                }
			| {
                print_grammar_rule("argument_list","");
                tnode *a = CreateTnode();
                strcpy(a->token,"argument_list");
                $$ = a;
            }   
			;
	
arguments: arguments COMMA logic_expression {

                print_grammar_rule("arguments","arguments COMMA logic_expression");
                tnode *a = CreateTnode();
                strcpy(a->token,"arguments");
                a=ProgramNode($1,a);
                a=ProgramNode("COMMA",a);
                a=ProgramNode($3,a);
                $$ = a;

                // update vector
                $$->args_list = $1->args_list; 
                $$->args_list.push_back($3->HelperType);
            }
	    | logic_expression {

                print_grammar_rule("arguments","logic_expression");
                tnode *a = CreateTnode();
                strcpy(a->token,"arguments");
                a=ProgramNode($1,a);
                $$ = a;

                // update helper type
                strcpy($$->HelperType, $1->HelperType);
                $$->args_list.push_back($1->HelperType);
            }
	    ;

%%

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
    if(NTnode != NULL) printtree(NTnode);

    fprintf(logout, "Total lines: %d\n", line_count);
    fprintf(logout, "Total errors: %d\n", err_count);

    fclose(yyin);
    fclose(parseout);
    fclose(logout);
	fclose(errout);

    exit(0);
}
