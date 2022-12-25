%option noyywrap
%x COMMENT_ML

%{
#include "actions.h"
void yyerror(char *);
int newlines_in_cmnt = 0;
char* cmntStart;
%}

WHITESPACE 		[ \t\f\r\v]+
LETTER 			[a-zA-Z]
DIGIT 			[0-9]
DIGITS 			{DIGIT}+
NEWLINE 		\n
NOT_NEWLINE 	[^\n]
SLASH 			[\\]
SLASH_IN_QUOTES (\'\\\')

ID_START 		({LETTER}|_)
ID 				{ID_START}({LETTER}|{DIGIT}|_)*
INVALID_ID 		{DIGIT}({LETTER}|{DIGIT})*

CHAR 			\'{NOT_NEWLINE}\'
ESC 			((\'\\n\')|(\'\\t\')|(\'\\a\')|(\'\\b\')|(\'\\r\')|(\'\\f\')|(\'\\v\')|(\'\\\'\')|(\'\\\"\')|(\'\\\\')|(\'\\0\'))
INVALID_CHAR 	(\'{NOT_NEWLINE}{NOT_NEWLINE}+\')
UNF_CHAR 		\'{CHAR}*[^'\n]?
EMP_CHAR		\'\'

UNF_STR 		\"([^'"\\\n]|(\\)({NOT_NEWLINE}|{NEWLINE}))*
STR 			{UNF_STR}\"

DEC_POINT 		(\.)*
FLOAT 			({DIGITS})?(\.?{DIGITS})(E[+-]?{DIGITS})?
TOO_MANY_POINTS {DIGIT}*((\.)+{DIGITS})+(E{DEC_POINT}[+-]?{DEC_POINT}{DIGITS})?
ILL_FORMED_NUM 	({DIGITS}|{FLOAT})[E]*{FLOAT}*

CMNT_SL 		"//"({NOT_NEWLINE}*({SLASH}{NEWLINE}))*{NOT_NEWLINE}*

%%
{NEWLINE}		{num_lines++;}

{WHITESPACE}	{}

"if" 		{return IF;}
"for" 		{return FOR;}
"do" 		{return DO;}
"int" 		{return INT;}
"float" 	{return FLOAT;}
"void" 		{return VOID;}
"switch" 	{return SWITCH;}
"default" 	{return DEFAULT;}
"else" 		{return ELSE;}
"while" 	{return WHILE;}
"break" 	{return BREAK;}
"char" 		{return CHAR;}
"double" 	{return DOUBLE;}
"return"	{return RETURN;}
"case" 		{return CASE;}
"continue" 	{return CONTINUE;}

("+"|"-")						{add_punc_op(_addop_); return ADDOP;}
("*"|"/"|"%")					{add_punc_op(_mulop_);	return MULOP;}
("++"|"--")						{add_punc_op(_incop_);	return INCOP;}
("<"|"<="|">"|">="|"=="|"!=") 	{add_punc_op(_relop_);	return RELOP;}
("=")							{add_punc_op(_asgnop_);	return ASSIGNOP;}
("&&"|"||")						{add_punc_op(_logop_);	return LOGICOP;}
("&"|"|"|"^"|"<<"|">>")			{add_punc_op(_bitop_);	return BITOP;}
"!"								{add_punc_op(_not_); return NOT;}
"("								{add_punc_op(_lparen_);	return LPAREN;}
")"								{add_punc_op(_rparen_);	return RPAREN;}
"{"								{add_punc_op(_lcurl_);	return LCURL;}
"}"								{add_punc_op(_rcurl_);	return RCURL;}
"["								{add_punc_op(_lthird_);	return LTHIRD;}
"]"								{add_punc_op(_rthird_);	return RTHIRD;}
","								{add_punc_op(_comma_);	return COMMA;}
";"								{add_punc_op(_semicolon_);	return SEMICOLON;}


{DIGITS}			{addConstInt(); return CONST_INT;}
{FLOAT}				{addConstFloat(); return CONST_FLOAT;}
{TOO_MANY_POINTS}	{
						showError("TOO_MANY_DECIMAL_POINTS");
						yylval.symbolValue = new SymbolInfo(yytext, "CONST_FLOAT");
						return CONST_FLOAT;
					}
{ILL_FORMED_NUM}	{
						showError("ILLFORMED_NUMBER");
						yylval.symbolValue = new SymbolInfo("0.0", "CONST_FLOAT");
						return CONST_FLOAT;
					}

{SLASH_IN_QUOTES}	{
						showError("UNFINISHED_CONST_CHAR");
						yylval.symbolValue = new SymbolInfo("\0","CONST_CHAR");
						return CONST_CHAR;
					}
({CHAR}|{ESC})		{addConstChar(); return CONST_CHAR;}
{INVALID_CHAR}  	{
						showError("MULTICHAR_CONST_CHAR");
						yylval.symbolValue = new SymbolInfo("\0","CONST_CHAR");
						return CONST_CHAR;
					}
{UNF_CHAR}			{
						showError("UNFINISHED_CONST_CHAR");
						yylval.symbolValue = new SymbolInfo("\0","CONST_CHAR");
						return CONST_CHAR;
					}
{EMP_CHAR}			{
						showError("EMPTY_CONST_CHAR");
						yylval.symbolValue = new SymbolInfo("\0","CONST_CHAR");
						return CONST_CHAR;
					}

{STR}				{addString(); return STRING;}
{UNF_STR}			{showError("UNFINISHED_STRING");}


{ID}				{installID(); return ID;}
{INVALID_ID}		{showError("INVALID_ID_SUFFIX_NUM_PREFIX");}


{CMNT_SL}			{ignoreComment();}

<INITIAL>"/*" 		{ 
						cmntStart = yytext;
						yymore();
						BEGIN COMMENT_ML; 
					}
<COMMENT_ML>"*/" 	{ 
						num_lines += newlines_in_cmnt;
						newlines_in_cmnt = 0;
						BEGIN INITIAL;
					}
<COMMENT_ML>. 		{yymore();} 
<COMMENT_ML>\n 		{yymore(); newlines_in_cmnt++;} 
<COMMENT_ML><<EOF>> {showCommentError("UNFINISHED_COMMENT", cmntStart); return 0;}

.					{showError("UNRECOGNIZED_CHAR");}

<<EOF>> 			{return 0;}
%%