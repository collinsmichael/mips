//
// File Name : Lexer.cpp
// File Date : 19 March 2012
// Author(s) : Valendian
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include "Lexer.h"

const int NumKeyWords = 0x11;
const char *KeyWord[NumKeyWords] = {
	"",
	"exit",
	"quit",
	"command",
	"open",
	"link",
	"close",
	"register",
	"print",
	"show",
	"hide",
	"debug",
	"address",
	"entry",
	"code",
	"data",
	"dasm"
};

extern bool volatile DebugLexer; // can be switched to true at run time
static char Buffer[0x1000];

table *NewTokenTable(char *Source)
{
	table *Table;

	// allocate memory for the token table and initialize values...
	Table = (table*)malloc(sizeof(table));
	if (!Table) return 0;
	Table->NumTokens = 0;
	Table->CurToken = 0;
	Table->Source = Source;

	// allocate memory for the token array and initialize values of first entry...
	Table->Token = (token*)malloc(sizeof(token));
	if (!Table->Token) {
		free(Table);
		return 0;
	}
	Table->Token[Table->NumTokens].Text = 0;
	Table->Token[Table->NumTokens].Length = 0;
	Table->Token[Table->NumTokens].Category = 0;
	Table->Token[Table->NumTokens].Value = 0;
	return Table;
}

int DeleteTokenTable(table *Table)
{
	if (Table) {
		if (Table->Token) free(Table->Token);
		free(Table);
	}
	return true;
}

int AddToken(table *Table, char *Text, int Length, int Category, int Value)
{
	token *Token;
	if (!Table) return false;
	if (!Table->Token) return false;

	Token = (token*)realloc(Table->Token, (Table->NumTokens+1)*sizeof(token));
	if (!Token) return false;

	Table->Token = Token;
	Table->Token[Table->NumTokens].Text     = Text;
	Table->Token[Table->NumTokens].Length   = Length;
	Table->Token[Table->NumTokens].Category = Category;
	Table->Token[Table->NumTokens].Value    = Value;
	Table->NumTokens++;
	return true;
}

int GetNumTokens(table *Table)
{
	if (!Table) return 0;
	return Table->NumTokens;
}

int GetCurToken(table *Table)
{
	if (!Table) return 0;
	return Table->CurToken;
}

int GetTokenLength(table *Table, int idToken)
{
	if (!Table) return 0;
	if (!Table->Token) return 0;
	if (idToken < 0 || idToken > GetNumTokens(Table)) return 0;
	return Table->Token[idToken].Length;
}

int GetTokenCategory(table *Table, int idToken)
{
	if (!Table) return 0;
	if (!Table->Token) return 0;
	if (idToken < 0 || idToken > GetNumTokens(Table)) return 0;
	return Table->Token[idToken].Category;
}

int GetTokenValue(table *Table, int idToken)
{
	if (!Table) return 0;
	if (!Table->Token) return 0;
	if (idToken < 0 || idToken > GetNumTokens(Table)) return 0;
	return Table->Token[idToken].Value;
}

int GetTokenText(table *Table, int idToken, char *Buffer)
{
	if (!Table) return false;
	if (!Table->Token) return false;
	if (idToken < 0 || idToken > GetNumTokens(Table)) return false;
	memcpy(Buffer, Table->Token[idToken].Text, GetTokenLength(Table, idToken));
	Buffer[GetTokenLength(Table, idToken)] = 0;
	return true;
}

int SetCurToken(table *Table, int NewCurToken)
{
	if (!Table) return 0;
	Table->CurToken = NewCurToken;
	return Table->CurToken;
}

int PrintToken(table *Table, int idToken)
{
	if (!Table) return false;
	if (!Table->Token) return false;
	if (idToken < 0 || idToken > GetNumTokens(Table)) return false;
	memcpy(Buffer, Table->Token[idToken].Text, GetTokenLength(Table, idToken));
	Buffer[GetTokenLength(Table, idToken)] = 0;
	printf("%s", Buffer);
	return true;
}

int SkipWhiteSpace(char *Text)
{
	int Length = 0;
	while (Text[Length] != 0 && Text[Length] <= ' ') Length++;
	return Length;
}

int SkipComment(char *Text)
{
	int Length = 0;
	if (Text[Length] == '#') {
		while (Text[Length] != 0x00 && Text[Length] != 0x0A && Text[Length] != 0x0D) Length++;
	}
	return Length;
}

int ScanGrammer(table *Table, char *Text)
{
	int Length = 0;
	switch (*Text) {
	case '!': case '$': case '%': case '&': case '\\':
	case '(': case ')': case '*': case '+': case ',':
	case '-': case '.': case '/': case ':': case ';':
	case '<': case '=': case '>': case '?': case '@':
	case '[': case ']': case '^': case '_': case '`':
	case '{': case '|': case '}': case '~':
		Length++;
		AddToken(Table, Text, Length, TokenGrammer, Text[0]);
	default:
		break;
	}

	return Length;
}

int ScanQuotedText(table *Table, char *Text)
{
	int Length = 0;
	char Terminator = Text[Length];
	
	if (Terminator == '"' || Terminator == '\'') {
		Text++; // Skip the opening quotation mark

		while (Text[Length] != Terminator) {
			switch (Text[Length]) {
			case 0x00:
			case 0x0A:
			case 0x0D:
				return Length+1;
			default:
				Length++;
				break;
			}
		}

		AddToken(Table, Text, Length, TokenQuote, 0);
		Length += 2; // skip both the opening and closing quotation mark
	}
	return Length;
}

int ScanNumber(table *Table, char *Text)
{
	char *EndText;
	int Value = strtoul(Text, &EndText, 0);
	int Length = EndText - Text;
	if (Length > 0) AddToken(Table, Text, Length, TokenNumber, Value);
	return Length;
}

int ScanWord(table *Table, char *Text)
{
	int Length = 0;

	if ((*Text >= 'A' && *Text <= 'Z') || (*Text >= 'a' && *Text <= 'z')) {
		while ((Text[Length] >= 'A' && Text[Length] <= 'Z') || (Text[Length] >= 'a' && Text[Length] <= 'z')) {
			Length++;
		}

		// check for matching keyword...
		memcpy(Buffer, Text, Length);
		Buffer[Length] = 0;
		for (int i = 0; i < NumKeyWords; i++) {
			if (_strcmpi(Buffer, KeyWord[i]) == 0) {
				AddToken(Table, Text, Length, TokenKeyWord, i);
				return Length;
			}
		}
		AddToken(Table, Text, Length, TokenWord, 0);
	}
	return Length;
}

int Lexer(table *Table, char *Text)
{
	int Length;
	if (!Text) return 0;
	if (!Table) return 0;
	if (!Table->Token) return 0;

	// divide the text into lexical units, skipping white space and commentation...
	while (*Text) {
		Text += SkipWhiteSpace(Text);
		Text += SkipComment(Text);

		if        (0 < (Length = ScanGrammer(Table, Text))) {
		} else if (0 < (Length = ScanQuotedText(Table, Text))) {
		} else if (0 < (Length = ScanNumber(Table, Text))) {
		} else if (0 < (Length = ScanWord(Table, Text))) {
		}

		Text += Length;
	}
	AddToken(Table, 0, 0, 0, 0);

	// print out all scanned tokens...
	if (DebugLexer == true) {
		printf("\nLexer\n");
		printf("Token    Value    Category Length    Text\n");
		for (int i = 0; i < GetNumTokens(Table); i++) {
			memcpy(Buffer, Table->Token[i].Text, GetTokenLength(Table, i));
			Buffer[GetTokenLength(Table, i)] = 0;
			if (Buffer[0]) {
				printf("%08X %08X %08X %08X '%s'\n", i, GetTokenValue(Table, i),
					GetTokenCategory(Table, i), GetTokenLength(Table, i), Buffer);
			} else {
				printf("%08X %08X %08X %08X\n", i, GetTokenValue(Table, i),
					GetTokenCategory(Table, i), GetTokenLength(Table, i));
			}
		}
	}
	return GetNumTokens(Table);
}
