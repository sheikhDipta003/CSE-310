%option noyywrap

%{
#include "lexBase.h"
%}

ANYTHING_EXCEPT_NEWLINE [^\n]

WHITESPACE [ \t\f\r\v]+ 
LETTER [a-zA-Z]
DIGIT [0-9]
DIGITS {DIGIT}+
NEWLINE \n
DECPOINT (\.)*
FRACTION (\.{DIGITS})

FLOAT ({DIGITS})?(\.?{DIGITS})(E[+-]?{DIGITS})?
TOO_DECPOINT {DIGIT}*((\.)+{DIGITS})+(E{DECPOINT}[+-]?{DECPOINT}{DIGITS})?
ILL_NUM ({DIGITS}|{FLOAT})[E]*{FLOAT}*


%%
{NEWLINE} {line_count++;}
{WHITESPACE}+ {}

{DIGITS}	{addToken_const_int();}
{FLOAT}		{addToken_const_float();}
(\'.\')		|
(\'\\n\')	|
(\'\\t\')	|
(\'\\a\')	|	
(\'\\b\')	|
(\'\\r\')	|
(\'\\f\')	|
(\'\\v\')	|
(\'\\\'\')	|
(\'\\\"\')	|
(\'\\\\')	|
(\'\\0\')	{addToken_const_char();}

{TOO_DECPOINT}			{printError("Too many decimal point");}
{ILL_NUM}				{printError("Ill formed number");}


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