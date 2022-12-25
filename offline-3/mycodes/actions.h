#include "sym_info.h"
#include "y.tab.h" // SymbolInfo Used Here
#include <locale>

#define MAX_TABLE_SIZE 10

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
#define _lthird_ "LSQUARE"
#define _rthird_ "RSQUARE"
#define _comma_ "COMMA"
#define _semicolon_ "SEMICOLON"

SymbolTable symTable(MAX_TABLE_SIZE);
int num_lines = 1;
int num_err = 0;
extern ofstream logFile, errorFile, parserFile;
extern char *yytext;

void replace_help(string &str, string const &prev, string const &_new) {
	size_t pos = 0;
    while (pos += _new.length())
    {
        pos = str.find(prev, pos);
        if (pos == std::string::npos) {
            break;
        }
 
        str.erase(pos, prev.length());
        str.insert(pos, _new);
    }
}

string replace(const char* str) {
	string replaced = str;

	replace_help(replaced, "\\n", "\n");
	replace_help(replaced, "\\t", "\t");
	replace_help(replaced, "\\a", "\a");
	replace_help(replaced, "\\b", "\b");
	replace_help(replaced, "\\r", "\r");
	replace_help(replaced, "\\f", "\f");
	replace_help(replaced, "\\v", "\v");
	replace_help(replaced, "\\\'", "\'");
	replace_help(replaced, "\\\"", "\"");
	replace_help(replaced, "\\0", "\0");

	return replaced;
}

void add_punc_op(string token) {
	yylval.symbolValue = new SymbolInfo(yytext, token);

	if(token == _lcurl_)	symTable.enterScope();
	else if(token == _rcurl_)	symTable.exitScope();
}

void addConstInt() {
	string token = "CONST_INT";
	yylval.symbolValue = new SymbolInfo(yytext, token);
}

void addConstFloat() {
	string token = "CONST_FLOAT";
	yylval.symbolValue = new SymbolInfo(yytext, token);
}

void addConstChar() {
	string token = "CONST_CHAR";

	//replace with ascii representations
	string const_char = replace(yytext);

	//remove enclosing quotations
	const_char = "" + const_char.substr(1);
	const_char = const_char.substr(0, const_char.size() - 1) + "";

	yylval.symbolValue = new SymbolInfo(const_char, token);
}

void addString() {
	//replace with ascii representations
	string const_str = replace(yytext);

	//remove enclosing quotations
	const_str = "" + const_str.substr(1);
	const_str = const_str.substr(0, const_str.size() - 1) + "";
	int num_newline_char = std::count(const_str.begin(), const_str.end(), '\n');

	string token;
	if(num_newline_char == 0)	token = "SINGLE LINE STRING";
	else	token = "MULTI LINE STRING";

	const_str.erase(std::remove(const_str.begin(), const_str.end(), '\\'), const_str.end()); 	//remove backslash
	const_str.erase(std::remove(const_str.begin(), const_str.end(), '\n'), const_str.end()); 	//remove newline
	yylval.symbolValue = new SymbolInfo(const_str, token);

	num_lines += num_newline_char;
}

void installID() {
	SymbolInfo *s = symTable->__search(string(yytext));
	if(s == nullptr)
	{					
		yylval.symbolValue = new SymbolInfo(yytext, "ID");
	}
}

void ignoreComment(){
	string comment = replace(yytext);
	num_lines += std::count(comment.begin(), comment.end(), '\n');
}

void showError(const string &msg, std::ofstream &out) {
	string string_literal = replace(yytext);
	int err_line_count = std::count(string_literal.begin(), string_literal.end(), '\n');
	out << " >> LEXICAL Error @ Line no " << (num_lines + err_line_count) << ": " << msg.data() << ": <" << yytext << "> >>" << endl << endl;
	num_err++;
	num_lines += err_line_count;
}

void showError(const string& msg) {
	showError(msg,errorFile);
}

void showCommentError(const string &msg, string unf_cmnt, std::ofstream &out) {
	string string_literal = replace(unf_cmnt.data());
	int err_line_count = std::count(string_literal.begin(), string_literal.end(), '\n');
	out << " >> LEXICAL Error @ Line no " << (num_lines + err_line_count) << ": " << msg.data() << ": <" << yytext << "> >>" << endl << endl;
	num_err++;
	num_lines += err_line_count;
}

void showCommentError(const string& msg, string unf_cmnt) {
	showCommentError(msg, unf_cmnt, errorFile);
}