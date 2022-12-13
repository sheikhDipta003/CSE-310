#ifndef LEXICALANALYZER_LEXBASE_H
#define LEXICALANALYZER_LEXBASE_H

#define SYMBOL_TABLE_SIZE 10

#define _addop_ "ADDOP"
#define _mulop_ "MULOP"
#define _incop_ "INCOP"
#define _relop_ "RELOP"
#define _asgnop_ "ASSIGNOP"
#define _logop_ "LOGICOP"
#define _bitop_ "BITOP"
#define _not_ "NOT"
#define _lparen_ "LPAREN"
#define _rparen_ "RPAREN"
#define _lcurl_ "LCURL"
#define _rcurl_ "RCURL"
#define _lsq_ "LSQUARE"
#define _rsq_ "RSQUARE"
#define _comma_ "COMMA"
#define _semicolon_ "SEMICOLON"

#include "sym_table.h"
#include "Utils.h"
#include <locale>

SymbolTable symTable(SYMBOL_TABLE_SIZE);
FILE *logout, *tokenout;
int line_count = 1;
int keyword_count = 0;
int err_count = 0;


void insertTosymTable(string token_symbol,string token_name) {
	symTable.__insert(token_symbol, token_name);
	symTable.__print(logout, "C");
}


// void addToken_keyword(string token_name) {
// 	fprintf(tokenout, "<%s>", token_name.data());
//  fprintf(logout, "\nLine no %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name, yytext);
// 	keyword_count++;
// }

void addToken_keyword() {
	string token_name = StringParser::toUpperCase(yytext);
	fprintf(tokenout, "<%s>", token_name.data());
	fprintf(logout, "Line# %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name.data(), yytext);
	keyword_count++;
}

void addToken_operator(string token_name) {
	fprintf(tokenout, "<%s,%s>", token_name.data(), yytext);
	fprintf(logout, "Line# %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name.data(), yytext);
}

void addToken_const_int() {
	string token_name = "CONST_INT";
	fprintf(tokenout, "<%s,%s>", token_name.data(), yytext);
	fprintf(logout, "Line# %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name.data(), yytext);
}

void addToken_const_float() {
	string token_name = "CONST_FLOAT";
	fprintf(tokenout, "<%s,%s>", token_name.data(), yytext);
	fprintf(logout, "Line# %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name.data(), yytext);
}

void addToken_const_char() {
	string token_name = "CONST_CHAR";
	string char_literal = StringParser::parse(yytext);
	StringUtils::replaceFirst(char_literal, "\'", "");
	StringUtils::replaceLast(char_literal, "\'", "");

	fprintf(tokenout, "<%s,%s>", token_name.data(), char_literal.data());
	fprintf(logout, "Line# %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name.data(), yytext);
}

void addToken_string() {
	string token_name = "STRING";

	string string_literal = StringParser::parse(yytext);
	StringUtils::replaceFirst(string_literal, "\"", "");
	StringUtils::replaceLast(string_literal, "\"", "");
	fprintf(tokenout,  "<%s,%s>", token_name.data(), string_literal.data());
	fprintf(logout, "Line# %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name.data(), yytext);

	line_count += StringUtils::occCount(yytext, '\n');
}

void addToken_identifier() {
	string token_name = "ID";
	fprintf(tokenout, "<%s,%s>", token_name.data(), yytext);
	fprintf(logout, "Line# %d: TOKEN <%s> Lexeme <%s> found\n", line_count, token_name.data(), yytext);
	insertTosymTable(token_name, yytext);
}

void comment() {

	string cmnt = yytext;
	// cout << cmnt;

	if(cmnt[1]=='/'){
		StringUtils::replaceFirst(cmnt,"//","");
		 StringUtils::replaceAll(cmnt, "\\\n", "");
	} else{
		StringUtils::replaceFirst(cmnt,"/*","");
		StringUtils::replaceFirst(cmnt,"*/","");
	}

	fprintf(logout, "\nLine# %d: TOKEN <COMMENT> Lexeme <%s> found\n", line_count, cmnt.data());

	line_count += StringUtils::occCount(yytext, '\n');
}

void printError(string msg) {
	fprintf(logout, "Error at line# <%d>: <%s>\n", line_count, msg.data());
	err_count++;
	line_count += StringUtils::occCount(yytext, '\n');
}

#endif //LEXICALANALYZER_LEXBASE_H