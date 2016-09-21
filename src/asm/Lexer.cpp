#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "Lexer.h"
#include "Symbol.h"

extern char  *CpuReg[0x40];
extern char  *GpuReg[0x60];
extern char  *GteCReg[0x60];
extern char  *GteDReg[0x60];
extern char  *Mnemonic[0x60];
extern char  *Directive[0x20];

static char *Text;
static int   Length;
static int   Flags;
static int   MetaData;
static int   Address;
static char  Ascii[0x10000];

int LexerSimplify(char *FileData, int FileSize)
{
	// simplify newline byte sequences
	for (int i = 0; i < FileSize; i++) {
		if (FileData[i] == 0x0D) {
			if (FileData[i+1] == 0x0A) FileData[i+1] = ' ';
			FileData[i] = 0x0A;
		}
	}

	// simplify white space byte sequences
	for (int i = 0; i < FileSize; i++) {
		if (FileData[i] > 0 && FileData[i] != 0x0A && FileData[i] <= ' ') FileData[i] = ' ';
	}

	// strip out commentation
	for (int i = 0; i < FileSize; i++) {
		if (FileData[i] == '#') {
			FileData[i] = ' ';
			while (FileData[i] > 0 && FileData[i] != 0x0A) {
				FileData[i] = ' ';
				i++;
			}
		}
	}
	return true;
}

int LexerSkipWhiteSpace(char *Text)
{
	int Length = 0;
	while (Text[Length] > 0 && Text[Length] == ' ') Length++;
	return Length;
}

int LexerIsNewLine(char *Text)
{
	if (*Text == 0x0A) return 1;
	return 0;
}

int LexerIsText(char *Text)
{
	int  Length;
	char c;

	if (*Text != '"' && *Text != '\'') return 0;
	Length = 0;
	c = *Text;
	while (Text[Length] != c) {
		if (Text[Length] == 0x0A) {
			printf("Lexer Error : String literal not terminated before end of line!\r\n"); return 0;
		} else if (Text[Length] == 0x00) {
			printf("Lexer Error : String literal not terminated before end of file!\r\n"); return 0;
		}
		Length++;
	}
	Length++; // consumes the terminating quote
	return 0;
}

int LexerIsAlpha(char *Text)
{
	int Length = 0;
	if ((*Text >= 'A' && *Text <= 'Z') || (*Text >= 'a' && *Text <= 'z') || (*Text == '$' || *Text == '@') || (*Text == '.' || *Text == '_')) {
		while ((Text[Length] >= '0' && Text[Length] <= '9') || (Text[Length] >= 'A' && Text[Length] <= 'Z') || (Text[Length] >= 'a' && Text[Length] <= 'z') || (Text[Length] == '$' || Text[Length] == '@') || (Text[Length] == '.' || Text[Length] == '_')) { Length++; }
	}
	return Length;
}

int LexerIsGrammar(char *Text)
{
	switch (*Text) {
	case '[': case ']': case '(': case ')':
	case '{': case '}': case '<': case '>':
	case '.': case ',': case ':': case ';':
	case '&': case '|': case '^': case '!':
	case '+': case '-': case '*': case '/':
	case '=': case '?': case '%': case '~':
	case '\\':
		return 1;
	}
	return 0;
}

int LexerIsNumeric(char *Text)
{
	int BinLen, OctLen, DecLen, HexLen, HexPre;
	BinLen = OctLen = DecLen = HexLen = 0;
	if (*Text == '-' || *Text == '+') { BinLen = OctLen = DecLen = HexLen = 1; }

	// calculate the length of the binary interpretation
	if (Text[BinLen] >= '0' || Text[BinLen] <= '1' || Text[BinLen] == '_') {
		while ((Text[BinLen] >= '0' && Text[BinLen] <= '1') || Text[BinLen] == '_') BinLen++;
		if (Text[BinLen] == 'B' || Text[BinLen] == 'b') { BinLen++; } else { BinLen = 0; }
	}

	// calculate the length of the octal interpretation
	if ((Text[OctLen] >= '0' && Text[OctLen] <= '7') || Text[OctLen] == '_') {
		while ((Text[OctLen] >= '0' && Text[OctLen] <= '7') || Text[OctLen] == '_') OctLen++;
		if (Text[OctLen] == 'O' || Text[OctLen] == 'o') { OctLen++; } else { OctLen = 0; }
	}

	// calculate the length of the decimal interpretation
	if ((Text[DecLen] >= '0' && Text[DecLen] <= '9') || Text[DecLen] == '_') {
		while ((Text[DecLen] >= '0' && Text[DecLen] <= '9') || Text[DecLen] == '_') DecLen++;
	}

	// calculate the length of the hexidecimal interpretation
	if ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || Text[HexLen] == '_') {
		HexPre = HexLen;
		if (Text[HexLen] == '0' && (Text[HexLen+1] == 'X' || Text[HexLen+1] == 'x')) HexLen += 2;
		while ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') || (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') || Text[HexLen] == '_') HexLen++;
		if (Text[HexPre] == '0' && (Text[HexPre+1] == 'X' || Text[HexPre+1] == 'x')) { HexLen = HexLen; } else if (Text[HexLen] == 'H' || Text[HexLen] == 'h') { HexLen++; } else { HexLen = 0; }
	}

	// take the version that consumes the most characters
	if ((BinLen > OctLen) && (BinLen > DecLen) && (BinLen > HexLen)) return BinLen;
	if ((OctLen > BinLen) && (OctLen > DecLen) && (OctLen > HexLen)) return OctLen;
	if ((DecLen > BinLen) && (DecLen > OctLen) && (DecLen > HexLen)) return DecLen;
	if ((HexLen > BinLen) && (HexLen > OctLen) && (HexLen > OctLen)) return HexLen;
	return 0;
}

int LexerGetNumeric(char *Text, int Length)
{
	int BinLen, OctLen, DecLen, HexLen, BinVal, OctVal, DecVal, HexVal, HexPre, Negate;
	Negate = BinVal = BinLen = OctVal = OctLen = DecVal = DecLen = HexVal = HexLen = 0;
	if (*Text == '-' || *Text == '+') {
		BinLen = OctLen = DecLen = HexLen = 1;
		if (*Text == '-') Negate = true;
	}

	// calculate the length of the binary interpretation
	if (Text[BinLen] >= '0' || Text[BinLen] <= '1' || Text[BinLen] == '_') {
		while ((Text[BinLen] >= '0' && Text[BinLen] <= '1') || Text[BinLen] == '_') {
			if (Text[BinLen] >= '0' && Text[BinLen] <= '1') BinVal = (BinVal << 1) | (Text[BinLen] - '0');
			BinLen++;
		}
		if (Text[BinLen] == 'B' || Text[BinLen] == 'b') { BinLen++; } else { BinLen = 0; }
	}

	// calculate the length of the octal interpretation
	if ((Text[OctLen] >= '0' && Text[OctLen] <= '7') || Text[OctLen] == '_') {
		while ((Text[OctLen] >= '0' && Text[OctLen] <= '7') || Text[OctLen] == '_') {
			if (Text[OctLen] >= '0' && Text[OctLen] <= '7') OctVal = (OctVal << 3) | (Text[OctLen] - '0');
			OctLen++;
		}
		if (Text[OctLen] == 'O' || Text[OctLen] == 'o') { OctLen++; } else { OctLen = 0; }
	}

	// calculate the length of the decimal interpretation
	if ((Text[DecLen] >= '0' && Text[DecLen] <= '9') || Text[DecLen] == '_') {
		while ((Text[DecLen] >= '0' && Text[DecLen] <= '9') || Text[DecLen] == '_') {
			if (Text[DecLen] >= '0' && Text[DecLen] <= '9') DecVal = (DecVal * 10) + (Text[DecLen] - '0');
			DecLen++;
		}
	}

	// calculate the length of the hexidecimal interpretation
	if ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || Text[HexLen] == '_') {
		HexPre = HexLen;
		if (Text[HexLen] == '0' && (Text[HexLen+1] == 'X' || Text[HexLen+1] == 'x')) HexLen += 2;
		while ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') || (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') || Text[HexLen] == '_') {
			if (Text[HexLen] >= '0' && Text[HexLen] <= '9') HexVal = (HexVal << 4) | (Text[HexLen] - '0');
			if (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') HexVal = (HexVal << 4) | (Text[HexLen] - 'A' + 0x0A);
			if (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') HexVal = (HexVal << 4) | (Text[HexLen] - 'a' + 0x0A);
			HexLen++;
		}
		if (Text[HexPre] == '0' && (Text[HexPre+1] == 'X' || Text[HexPre+1] == 'x')) { HexLen = HexLen; } else if (Text[HexLen] == 'H' || Text[HexLen] == 'h') { HexLen++; } else { HexLen = 0; }
	}

	// take the version that consumes the most characters
	if (Negate) {
		BinVal = -BinVal;
		OctVal = -OctVal;
		DecVal = -DecVal;
		HexVal = -HexVal;
	}
	if ((BinLen > OctLen) && (BinLen > DecLen) && (BinLen > HexLen)) return BinVal;
	if ((OctLen > BinLen) && (OctLen > DecLen) && (OctLen > HexLen)) return OctVal;
	if ((DecLen > BinLen) && (DecLen > OctLen) && (DecLen > HexLen)) return DecVal;
	if ((HexLen > BinLen) && (HexLen > OctLen) && (HexLen > OctLen)) return HexVal;
	return 0;
}

int LexerIsMnemonic(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (Mnemonic[i]) {
				if (memcmp(Ascii, Mnemonic[i], Length+1) == 0) return Length;
			}
		}
	}
	return 0;
}

int LexerGetMnemonic(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (Mnemonic[i]) {
				if (memcmp(Ascii, Mnemonic[i], Length+1) == 0) return i;
			}
		}
	}
	return 0;
}

int LexerIsCpuReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x40; i++) {
			if (CpuReg[i]) {
				if (memcmp(Ascii, CpuReg[i], Length+1) == 0) return Length;
			}
		}
	}
	return 0;
}

int LexerGetCpuReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x40; i++) {
			if (CpuReg[i]) {
				if (memcmp(Ascii, CpuReg[i], Length+1) == 0) return (i & 0x1F);
			}
		}
	}
	return 0;
}

int LexerIsGpuReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (GpuReg[i]) {
				if (memcmp(Ascii, GpuReg[i], Length+1) == 0) return Length;
			}
		}
	}
	return 0;
}

int LexerGetGpuReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (GpuReg[i]) {
				if (memcmp(Ascii, GpuReg[i], Length+1) == 0) return (i & 0x1F);
			}
		}
	}
	return 0;
}

int LexerIsGteCReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (GteCReg[i]) {
				if (memcmp(Ascii, GteCReg[i], Length+1) == 0) return Length;
			}
		}
	}
	return 0;
}

int LexerGetGteCReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (GteCReg[i]) {
				if (memcmp(Ascii, GteCReg[i], Length+1) == 0) return (i & 0x1F);
			}
		}
	}
	return 0;
}

int LexerIsGteDReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (GteDReg[i]) {
				if (memcmp(Ascii, GteDReg[i], Length+1) == 0) return Length;
			}
		}
	}
	return 0;
}

int LexerGetGteDReg(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x60; i++) {
			if (GteDReg[i]) {
				if (memcmp(Ascii, GteDReg[i], Length+1) == 0) return (i & 0x1F);
			}
		}
	}
	return 0;
}

int LexerIsDirective(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x20; i++) {
			if (Directive[i]) {
				if (memcmp(Ascii, Directive[i], Length+1) == 0) return Length;
			}
		}
	}
	return 0;
}

int LexerGetDirective(int Length)
{
	if (Length) {
		for (int i = 0; i < 0x20; i++) {
			if (Directive[i]) {
				if (memcmp(Ascii, Directive[i], Length+1) == 0) return (i & 0x1F);
			}
		}
	}
	return 0;
}

int LexerIsAddress(char *Text)
{
	int HexLen = 0;
	if ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') || (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') || Text[HexLen] == '_') {
		while ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') || (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') || Text[HexLen] == '_') HexLen++;
		if (Text[HexLen] != ':') HexLen = 0;
		else HexLen++;
	}
	return HexLen;
}

int LexerGetAddress(char *Text, int Length)
{
	int HexLen, HexVal;
	HexLen = HexVal = 0;

	if ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') || (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') || Text[HexLen] == '_') {
		while ((Text[HexLen] >= '0' && Text[HexLen] <= '9') || (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') || (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') || Text[HexLen] == '_') {
			if (Text[HexLen] >= '0' && Text[HexLen] <= '9') HexVal = (HexVal << 4) | (Text[HexLen] - '0');
			if (Text[HexLen] >= 'A' && Text[HexLen] <= 'F') HexVal = (HexVal << 4) | (Text[HexLen] - 'A' + 0x0A);
			if (Text[HexLen] >= 'a' && Text[HexLen] <= 'f') HexVal = (HexVal << 4) | (Text[HexLen] - 'a' + 0x0A);
			HexLen++;
		}
		if (Text[HexLen] != ':') { HexLen = HexVal = 0; } else { HexLen++; }
	}
	return HexVal;
}

int LexerTokenize(char *FileData, int FileSize)
{ 
	int   LenNewLine, LenGrammar, LenText, LenNumeric, LenAlpha, LenMnemonic, LenCpuReg, LenGpuReg, LenGteCReg, LenGteDReg, LenDirective, LenAddress;
	char *Text = FileData;
	//LenGrammar = LenKeyWord = LenAlpha = LenNumeric = LenText = LenNewLine = 0;
	while (*Text > 0) {
		Text += LexerSkipWhiteSpace(Text);
		LenNewLine = LexerIsNewLine(Text);
		LenGrammar = LexerIsGrammar(Text);
		LenText    = LexerIsText(Text);
		LenNumeric = LexerIsNumeric(Text);
		LenAlpha   = LexerIsAlpha(Text);
		LenAddress = LexerIsAddress(Text);

		// convert
		//Ascii = (char*)malloc(LenAlpha+1);
		//if (!Ascii) {
		//	printf("Lexer Error : Failed to allocate memory for identifier!\r\n");
		//	break;
		//}
		memset(Ascii, 0, LenAlpha+1);
		memcpy(Ascii, Text, LenAlpha);
		for (int i = 0; i < LenAlpha; i++) {
			if (Ascii[i] >= 'a' && Ascii[i] <= 'z') Ascii[i] = Ascii[i] - 'a' + 'A';
		}

		LenMnemonic  = LexerIsMnemonic(LenAlpha);
		LenCpuReg    = LexerIsCpuReg(LenAlpha);
		LenGpuReg    = LexerIsGpuReg(LenAlpha);
		LenGteCReg   = LexerIsGteCReg(LenAlpha);
		LenGteDReg   = LexerIsGteDReg(LenAlpha);
		LenDirective = LexerIsDirective(LenAlpha);

		// new line
		if ((LenNewLine > LenGrammar) && (LenNewLine > LenText) && (LenNewLine > LenNumeric) && (LenNewLine > LenAlpha) && (LenNewLine > LenMnemonic) && (LenNewLine > LenCpuReg) && (LenNewLine > LenGpuReg) && (LenNewLine > LenGteCReg) && (LenNewLine > LenGteDReg) && (LenNewLine > LenDirective) && (LenNewLine > LenAddress)) {
			SymbolAdd(Text, LenNewLine, SYMBOL_FLAG_NEWLINE, 0x00000000, 0x00000000);
			Text += LenNewLine;

		// grammar
		} else if ((LenGrammar > LenNewLine) && (LenGrammar > LenText) && (LenGrammar > LenNumeric) && (LenGrammar > LenAlpha) && (LenGrammar > LenMnemonic) && (LenGrammar > LenCpuReg) && (LenGrammar > LenGpuReg) && (LenGrammar > LenGteCReg) && (LenGrammar > LenGteDReg) && (LenGrammar > LenDirective) && (LenGrammar > LenAddress)) {
			SymbolAdd(Text, LenGrammar, SYMBOL_FLAG_GRAMMAR, (int)*Text, 0x00000000);
			Text += LenGrammar;

		// string literal
		} else if ((LenText > LenNewLine) && (LenText > LenGrammar) && (LenText > LenNumeric) && (LenText > LenAlpha) && (LenText > LenMnemonic) && (LenText > LenCpuReg) && (LenText > LenGpuReg) && (LenText > LenGteCReg) && (LenText > LenGteDReg) && (LenText > LenDirective) && (LenText > LenAddress)) {
			SymbolAdd(Text, LenText, SYMBOL_FLAG_STRING, 0x00000000, 0x00000000);
			Text += LenText;

		// numeric constant
		} else if ((LenNumeric > LenNewLine) && (LenNumeric > LenGrammar) && (LenNumeric > LenText) && (LenNumeric > LenAlpha) && (LenNumeric > LenMnemonic) && (LenNumeric > LenCpuReg) && (LenNumeric > LenGpuReg) && (LenNumeric > LenGteCReg) && (LenNumeric > LenGteDReg) && (LenNumeric > LenDirective) && (LenNumeric > LenAddress)) {
			SymbolAdd(Text, LenNumeric, SYMBOL_FLAG_NUMERIC, LexerGetNumeric(Text, LenNumeric), 0x00000000);
			Text += LenNumeric;
		
		// identifier
		} else if ((LenAlpha > LenNewLine) && (LenAlpha > LenGrammar) && (LenAlpha > LenText) && (LenAlpha > LenNumeric) && (LenAlpha > LenMnemonic) && (LenAlpha > LenCpuReg) && (LenAlpha > LenGpuReg) && (LenAlpha > LenGteCReg) && (LenAlpha > LenGteDReg) && (LenAlpha > LenDirective) && (LenAlpha > LenAddress)) {
			SymbolAdd(Text, LenAlpha, SYMBOL_FLAG_IDENTIFIER, 0x00000000, 0x00000000);
			Text += LenAlpha;

		// mnemonic
		} else if ((LenMnemonic > LenNewLine) && (LenMnemonic > LenGrammar) && (LenMnemonic > LenText) && (LenMnemonic > LenNumeric) && (LenMnemonic >= LenAlpha) && (LenMnemonic > LenCpuReg) && (LenMnemonic > LenGpuReg) && (LenMnemonic > LenGteCReg) && (LenMnemonic > LenGteDReg) && (LenMnemonic > LenDirective) && (LenMnemonic > LenAddress)) {
			SymbolAdd(Text, LenMnemonic, SYMBOL_FLAG_MNEMONIC, LexerGetMnemonic(LenMnemonic), 0x00000000);
			Text += LenMnemonic;

		// cpu register
		} else if ((LenCpuReg > LenNewLine) && (LenCpuReg > LenGrammar) && (LenCpuReg > LenText) && (LenCpuReg > LenNumeric) && (LenCpuReg >= LenAlpha) && (LenCpuReg > LenMnemonic) && (LenCpuReg > LenGpuReg) && (LenCpuReg > LenGteCReg) && (LenCpuReg > LenGteDReg) && (LenCpuReg > LenDirective) && (LenCpuReg > LenAddress)) {
			SymbolAdd(Text, LenCpuReg, SYMBOL_FLAG_CPUREG, LexerGetCpuReg(LenCpuReg), 0x00000000);
			Text += LenCpuReg;

		// gpu register
		} else if ((LenGpuReg > LenNewLine) && (LenGpuReg > LenGrammar) && (LenGpuReg > LenText) && (LenGpuReg > LenNumeric) && (LenGpuReg >= LenAlpha) && (LenGpuReg > LenMnemonic) && (LenGpuReg > LenCpuReg) && (LenGpuReg > LenGteCReg) && (LenGpuReg > LenGteDReg) && (LenGpuReg > LenDirective) && (LenGpuReg > LenAddress)) {
			SymbolAdd(Text, LenGpuReg, SYMBOL_FLAG_GPUREG, LexerGetGpuReg(LenGpuReg), 0x00000000);
			Text += LenGpuReg;

		// gte control register
		} else if ((LenGteCReg > LenNewLine) && (LenGteCReg > LenGrammar) && (LenGteCReg > LenText) && (LenGteCReg > LenNumeric) && (LenGteCReg >= LenAlpha) && (LenGteCReg > LenMnemonic) && (LenGteCReg > LenCpuReg) && (LenGteCReg > LenGpuReg) && (LenGteCReg > LenGteDReg) && (LenGteCReg > LenDirective) && (LenGteCReg > LenAddress)) {
			SymbolAdd(Text, LenGteCReg, SYMBOL_FLAG_GTECTRL, LexerGetGteCReg(LenGteCReg), 0x00000000);
			Text += LenGteCReg;

		// gte data register
		} else if ((LenGteDReg > LenNewLine) && (LenGteDReg > LenGrammar) && (LenGteDReg > LenText) && (LenGteDReg > LenNumeric) && (LenGteDReg >= LenAlpha) && (LenGteDReg > LenMnemonic) && (LenGteDReg > LenCpuReg) && (LenGteDReg > LenGpuReg) && (LenGteDReg > LenGteCReg) && (LenGteDReg > LenDirective) && (LenGteDReg > LenAddress)) {
			SymbolAdd(Text, LenGteDReg, SYMBOL_FLAG_GTEDATA, LexerGetGteDReg(LenGteDReg), 0x00000000);
			Text += LenGteDReg;

		// directives
		} else if ((LenDirective > LenNewLine) && (LenDirective > LenGrammar) && (LenDirective > LenText) && (LenDirective > LenNumeric) && (LenDirective >= LenAlpha) && (LenDirective > LenMnemonic) && (LenDirective > LenCpuReg) && (LenDirective > LenGpuReg) && (LenDirective > LenGteCReg) && (LenDirective > LenGteDReg) && (LenDirective > LenAddress)) {
			int DirectiveType = LexerGetDirective(LenDirective);
			SymbolAdd(Text, LenDirective, SYMBOL_FLAG_DIRECTIVE, DirectiveType, 0x00000000);
			Text += LenDirective;

		//	switch (DirectiveType) {
		//	case 0: // "BYTE"
		//	case 1: // "HALF"
		//	case 2: // "WORD"
		//		Text += LexerSkipWhiteSpace(Text);
		//		if ( *Text == ',' ) {
		//		}
		//	}

		// address column
		} else if ((LenAddress > LenNewLine) && (LenAddress > LenGrammar) && (LenAddress > LenText) && (LenAddress > LenNumeric) && (LenAddress > LenAlpha) && (LenAddress > LenMnemonic) && (LenAddress > LenCpuReg) && (LenAddress > LenGpuReg) && (LenAddress > LenGteCReg) && (LenAddress > LenGteDReg) && (LenAddress > LenDirective)) {
			SymbolAdd(Text, LenAddress, SYMBOL_FLAG_ADDRESS, LexerGetAddress(Text, LenAddress), 0x00000000);
			Text += LenAddress;

		// end of file
		} else if (*Text == 0) {
			SymbolAdd(0, 0, SYMBOL_FLAG_ENDOFFILE, 0x00000000, 0x00000000);
			break;

		// erronious token
		} else {
			SymbolAdd(Text, 1, SYMBOL_FLAG_ERROR, 0x00000000, 0x00000000);
			break;
		}
		//free(Ascii);
		//Ascii = 0;
	}

	//if (Ascii) {
	//	free(Ascii);
	//	Ascii = 0;
	//}
	return SymbolGetNumSymbols();
}

int LexerCleanUp(void)
{
	int NumSymbols, Sym, Length, Flags, LineNum;
	char *Text, *Ascii;

	NumSymbols = SymbolGetNumSymbols();
	LineNum = Sym = 0;
	while (Sym < NumSymbols) {
		// retrieve symbol data
		Text     = SymbolGetText(Sym);
		Length   = SymbolGetLength(Sym);
		Flags    = SymbolGetFlags(Sym);

		// at this point we are examining a new line...
		switch (Flags) {
		default:
			break;
		case SYMBOL_FLAG_ERROR: // erronious token
			Ascii = (char*)malloc(Length+1);
			if (!Ascii) {
				printf("Lexer Error : Failed to allocate memory for identifier!\r\n");
				return 0;
			}
			memset(Ascii, 0, Length+1);
			memcpy(Ascii, Text, Length);
			printf("Lexer Error at Line %u unhandled token '%s'\r\n", LineNum, Ascii);
			free(Ascii);
			Ascii = 0;
			break;
		case SYMBOL_FLAG_NEWLINE: // new line
			LineNum++;
			break;
		}
		Sym++;
	}
	return true;
}

int Lexer(char *FileData, int FileSize)
{
	LexerSimplify(FileData, FileSize);
	LexerTokenize(FileData, FileSize);
	LexerCleanUp();
	return true;
}
