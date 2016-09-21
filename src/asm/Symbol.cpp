#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "Symbol.h"

char   *Ascii;
uint32  NumSymbols;
char  **SymbolText;
uint32 *SymbolLength;
uint32 *SymbolFlags;
uint32 *SymbolMetaData;
uint32 *SymbolAddress;

uint32 SymbolInit(void)
{
	SymbolText     = (char** )malloc(sizeof(char**));  if (!SymbolText    ) return false;
	SymbolLength   = (uint32*)malloc(sizeof(uint32*)); if (!SymbolLength  ) return false;
	SymbolFlags    = (uint32*)malloc(sizeof(uint32*)); if (!SymbolFlags   ) return false;
	SymbolMetaData = (uint32*)malloc(sizeof(uint32*)); if (!SymbolMetaData) return false;
	SymbolAddress  = (uint32*)malloc(sizeof(uint32*)); if (!SymbolAddress ) return false;
	NumSymbols = 0;
	return true;
}

uint32 SymbolFree(void)
{
	free(SymbolText);     SymbolText     = 0;
	free(SymbolLength);   SymbolLength   = 0;
	free(SymbolFlags);    SymbolFlags    = 0;
	free(SymbolMetaData); SymbolMetaData = 0;
	free(SymbolAddress);  SymbolAddress  = 0;
	NumSymbols = 0;
	return true;
}

uint32 SymbolAdd(char *Text, uint32 Length, uint32 Flags, uint32 MetaData, uint32 Address)
{
	SymbolText     = (char** )realloc(SymbolText    , (NumSymbols+1)*sizeof(char**));
	SymbolLength   = (uint32*)realloc(SymbolLength  , (NumSymbols+1)*sizeof(uint32*));
	SymbolFlags    = (uint32*)realloc(SymbolFlags   , (NumSymbols+1)*sizeof(uint32*));
	SymbolMetaData = (uint32*)realloc(SymbolMetaData, (NumSymbols+1)*sizeof(uint32*));
	SymbolAddress  = (uint32*)realloc(SymbolAddress , (NumSymbols+1)*sizeof(uint32*));
	if (!SymbolText    ) return false;
	if (!SymbolLength  ) return false;
	if (!SymbolFlags   ) return false;
	if (!SymbolMetaData) return false;
	if (!SymbolAddress ) return false;
	SymbolText[NumSymbols]     = Text;
	SymbolLength[NumSymbols]   = Length;
	SymbolFlags[NumSymbols]    = Flags;
	SymbolMetaData[NumSymbols] = MetaData;
	SymbolAddress[NumSymbols]  = Address;
	NumSymbols++;

	/*
	if (Flags != SYMBOL_FLAG_ENDOFFILE && Flags != SYMBOL_FLAG_NEWLINE) {
		Ascii = (char*)malloc(Length+1);
		if (Ascii) {
			memset(Ascii, 0, Length+1);
			memcpy(Ascii, Text, Length);
			printf("symbol %08X %08X %08X %08X '%s'\r\n", Length, Flags, MetaData, Address, Ascii);
			free(Ascii);
			Ascii = 0;
		}
	} else {
			printf("symbol %08X %08X %08X %08X ''\r\n", Length, Flags, MetaData, Address);
	}
	*/

	return true;
}

uint32 SymbolGetNumSymbols(void)
{
	return NumSymbols;
}

char *SymbolGetText(uint32 SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolText[SymbolNum];
	return 0;
}

uint32 SymbolGetLength(uint32 SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolLength[SymbolNum];
	return 0;
}

uint32 SymbolGetFlags(uint32 SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolFlags[SymbolNum];
	return 0;
}

uint32 SymbolGetMetaData(uint32 SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolMetaData[SymbolNum];
	return 0;
}

uint32 SymbolGetAddress(uint32 SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolAddress[SymbolNum];
	return 0;
}

uint32 SymbolSetText(uint32 SymbolNum, char *Text)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolText[SymbolNum] = Text;
	return true;
}

uint32 SymbolSetLength(uint32 SymbolNum, uint32 Length)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolLength[SymbolNum] = Length;
	return true;
}

uint32 SymbolSetFlags(uint32 SymbolNum, uint32 Flags)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolFlags[SymbolNum] = Flags;
	return true;
}

uint32 SymbolSetMetaData(uint32 SymbolNum, uint32 MetaData)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolMetaData[SymbolNum] = MetaData;
	return true;
}

uint32 SymbolSetAddress(uint32 SymbolNum, uint32 Address)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolAddress[SymbolNum] = Address;
	return true;
}
