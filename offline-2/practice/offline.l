%option noyywrap

%x COMMENT_SL COMMENT_ML

%{
#include "lexBase.h"
char* p;
%}

WHITESPACE [ \t\f\r\v]+
LETTER [a-zA-Z]
DIGIT [0-9]
DIGITS {DIGIT}+
NEWLINE \n
NOT_NEWLINE [^\n]
SLASH [\\]

ID_START ({LETTER}|_)
ID {ID_START}({LETTER}|{DIGIT}|_)*
INVALID_ID {DIGIT}({LETTER}|{DIGIT})*

CHAR (\'[^\n]\')
ESC ((\'\\n\')|(\'\\t\')|(\'\\a\')|(\'\\b\')|(\'\\r\')|(\'\\f\')|(\'\\v\')|(\'\\\'\')|(\'\\\"\')|(\'\\\\')|(\'\\0\'))
INVALID_CHAR (\'..+\')
UNF_CHAR ('{CHAR}*[^'\n]?)
EMP_CHAR	('')

UNF_STR \"([^'"\\\n]|(\\)({NOT_NEWLINE}|{NEWLINE}))*
STR {UNF_STR}\"

DECPOINT (\.)*
FLOAT ({DIGITS})?(\.?{DIGITS})(E[+-]?{DIGITS})?
TOO_DECPOINT {DIGIT}*((\.)+{DIGITS})+(E{DECPOINT}[+-]?{DECPOINT}{DIGITS})?
ILL_NUM ({DIGITS}|{FLOAT})[E]*{FLOAT}*

%%
{NEWLINE}		{num_lines++;}

{WHITESPACE}	{}
<COMMENT_SL>{WHITESPACE} {}
<COMMENT_ML>{WHITESPACE} {}

{DIGITS}		{addToken_const_int();}
{FLOAT}			{addToken_const_float();}

({CHAR}|{ESC})	{addToken_const_char();}
{INVALID_CHAR}  {printError("MULTICHAR_CONST_CHAR");}
{UNF_CHAR}		{printError("UNFINISHED_CONST_CHAR");}
{EMP_CHAR}		{printError("EMPTY_CONST_CHAR");}

{STR}			{addToken_string();}
{UNF_STR}		{printError("UNFINISHED_STRING");}


{TOO_DECPOINT}	{printError("TOO_MANY_DECIMAL_POINTS");}
{ILL_NUM}		{printError("ILLFORMED_NUMBER");}


"if" 		|
"for" 		|
"do" 		|
"int" 		|
"float" 	|
"void" 		|
"switch" 	|
"default" 	|
"else" 		|
"while" 	|
"break" 	|
"char" 		|
"double" 	|
"return"	|
"case" 		|
"continue" 	{addToken_keyword();}



("+"|"-")						{addToken_operator(_addop_);}
("*"|"/"|"%")					{addToken_operator(_mulop_);}
("++"|"--")						{addToken_operator(_incop_);}
("<"|"<="|">"|">="|"=="|"!=") 	{addToken_operator(_relop_);}
("=")							{addToken_operator(_asgnop_);}
("&&"|"||")						{addToken_operator(_logop_);}
("&"|"|"|"^"|"<<"|">>")			{addToken_operator(_bitop_);}
"!"								{addToken_operator(_not_);}
"("								{addToken_operator(_lparen_);}
")"								{addToken_operator(_rparen_);}
"{"								{addToken_operator(_lcurl_);}
"}"								{addToken_operator(_rcurl_);}
"["								{addToken_operator(_lsq_);}
"]"								{addToken_operator(_rsq_);}
","								{addToken_operator(_comma_);}
";"								{addToken_operator(_semicolon_);}

{ID}							{addToken_identifier();}
{INVALID_ID}					{printError("INVALID_ID_SUFFIX_NUM_PREFIX");}

<INITIAL>"/*" { 
  p = yytext;
  BEGIN(COMMENT_ML); 
}
<COMMENT_ML>"*/" { 
  char* cmnt = strndup(p, yytext - p + 2);
  fprintf(logout, "Line# %d: TOKEN <MULTI LINE COMMENT> Lexeme %s found\n", num_lines, cmnt);
  free(cmnt); 
  BEGIN(INITIAL); 
}
<COMMENT_ML>. {} 
<COMMENT_ML>\n {num_lines++;} 

<INITIAL>"//" { 
  p = yytext; 
  BEGIN(COMMENT_SL); 
}
<COMMENT_SL>{NEWLINE} { 
  char* cmnt = strndup(p, yytext - p + 2);
  fprintf(logout, "Line# %d: TOKEN <SINGLE LINE COMMENT> Lexeme %s found\n", num_lines, cmnt);
  free(cmnt); 
  BEGIN(INITIAL); 
}
<COMMENT_SL>. {}
<COMMENT_SL>({SLASH}{NEWLINE}) {num_lines++;}

.								{printError("UNRECOGNIZED_CHAR");}

<<EOF>> {fprintf(logout,"Total lines: %d\nTotal errors: %d\n",num_lines,num_err); return 0;}
%%

int main(int argc,char *argv[]){

	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

	logout= fopen("log.txt","w");
	tokenout= fopen("token.txt","w");

	yyin= fin;
	yylex();
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	return 0;
}