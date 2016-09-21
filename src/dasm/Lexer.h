//
// File Name : Lexer.h
// File Date : 19 March 2012
// Author(s) : Valendian
//

#ifndef LEXER_HEADER
#define LEXER_HEADER

typedef struct token {
	char *Text;     // points to the text.
	int   Length;   // length of the text.
	int   Category; // lexical category classification.
	int   Value;    // either the value of a numeric text or the id of a keyword.
} token;

typedef struct table {
	int    NumTokens; // the total number of tokens in the token array
	int    CurToken;  // the current index in the token array
	char  *Source;    // either "CommandLine", "Console" or name of command file
	token *Token;     // array of tokens
} table;

const int TokenGrammer = 1;
const int TokenQuote   = 2;
const int TokenNumber  = 3;
const int TokenWord    = 4;
const int TokenKeyWord = 5;

const int KeyWordExit     = 0x01;
const int KeyWordQuit     = 0x02;
const int KeyWordCommand  = 0x03;
const int KeyWordOpen     = 0x04;
const int KeyWordLink     = 0x05;
const int KeyWordClose    = 0x06;
const int KeyWordRegister = 0x07;
const int KeyWordPrint    = 0x08;
const int KeyWordShow     = 0x09;
const int KeyWordHide     = 0x0A;
const int KeyWordDebug    = 0x0B;
const int KeyWordAddress  = 0x0C;
const int KeyWordEntry    = 0x0D;
const int KeyWordCode     = 0x0E;
const int KeyWordData     = 0x0F;
const int KeyWordDasm     = 0x10;

table *NewTokenTable(char *Source);
int DeleteTokenTable(table *Table);
int AddToken(table *Table, char *Text, int Length, int Category, int Value);
int GetNumTokens(table *Table);
int GetCurToken(table *Table);
int GetTokenLength(table *Table, int idToken);
int GetTokenCategory(table *Table, int idToken);
int GetTokenValue(table *Table, int idToken);
int GetTokenText(table *Table, int idToken, char *Buffer);
int SetCurToken(table *Table, int NewCurToken);

int PrintToken(table *Table, int idToken);
int SkipWhiteSpace(char *Text);
int SkipComment(char *Text);
int ScanGrammer(table *Table, char *Text);
int ScanQuotedText(table *Table, char *Text);
int ScanNumber(table *Table, char *Text);
int ScanWord(table *Table, char *Text);
int Lexer(table *Table, char *Text);

#endif
