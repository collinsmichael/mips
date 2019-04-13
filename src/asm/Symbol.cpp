#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdint.h>
#include "Symbol.h"

char   *Ascii;
uint32_t  NumSymbols;
char  **SymbolText;
uint32_t *SymbolLength;
uint32_t *SymbolFlags;
uint32_t *SymbolMetaData;
uint32_t *SymbolAddress;

uint32_t SymbolInit(void)
{
	SymbolText     = (char** )malloc(sizeof(char**));  if (!SymbolText    ) return false;
	SymbolLength   = (uint32_t*)malloc(sizeof(uint32_t*)); if (!SymbolLength  ) return false;
	SymbolFlags    = (uint32_t*)malloc(sizeof(uint32_t*)); if (!SymbolFlags   ) return false;
	SymbolMetaData = (uint32_t*)malloc(sizeof(uint32_t*)); if (!SymbolMetaData) return false;
	SymbolAddress  = (uint32_t*)malloc(sizeof(uint32_t*)); if (!SymbolAddress ) return false;
	NumSymbols = 0;
	return true;
}

uint32_t SymbolFree(void)
{
	free(SymbolText);     SymbolText     = 0;
	free(SymbolLength);   SymbolLength   = 0;
	free(SymbolFlags);    SymbolFlags    = 0;
	free(SymbolMetaData); SymbolMetaData = 0;
	free(SymbolAddress);  SymbolAddress  = 0;
	NumSymbols = 0;
	return true;
}

uint32_t SymbolAdd(char *Text, uint32_t Length, uint32_t Flags, uint32_t MetaData, uint32_t Address)
{
	SymbolText     = (char** )realloc(SymbolText    , (NumSymbols+1)*sizeof(char**));
	SymbolLength   = (uint32_t*)realloc(SymbolLength  , (NumSymbols+1)*sizeof(uint32_t*));
	SymbolFlags    = (uint32_t*)realloc(SymbolFlags   , (NumSymbols+1)*sizeof(uint32_t*));
	SymbolMetaData = (uint32_t*)realloc(SymbolMetaData, (NumSymbols+1)*sizeof(uint32_t*));
	SymbolAddress  = (uint32_t*)realloc(SymbolAddress , (NumSymbols+1)*sizeof(uint32_t*));
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

uint32_t SymbolGetNumSymbols(void)
{
	return NumSymbols;
}

char *SymbolGetText(uint32_t SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolText[SymbolNum];
	return 0;
}

uint32_t SymbolGetLength(uint32_t SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolLength[SymbolNum];
	return 0;
}

uint32_t SymbolGetFlags(uint32_t SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolFlags[SymbolNum];
	return 0;
}

uint32_t SymbolGetMetaData(uint32_t SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolMetaData[SymbolNum];
	return 0;
}

uint32_t SymbolGetAddress(uint32_t SymbolNum)
{
	if (SymbolNum >= 0 && SymbolNum < NumSymbols) return SymbolAddress[SymbolNum];
	return 0;
}

uint32_t SymbolSetText(uint32_t SymbolNum, char *Text)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolText[SymbolNum] = Text;
	return true;
}

uint32_t SymbolSetLength(uint32_t SymbolNum, uint32_t Length)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolLength[SymbolNum] = Length;
	return true;
}

uint32_t SymbolSetFlags(uint32_t SymbolNum, uint32_t Flags)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolFlags[SymbolNum] = Flags;
	return true;
}

uint32_t SymbolSetMetaData(uint32_t SymbolNum, uint32_t MetaData)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolMetaData[SymbolNum] = MetaData;
	return true;
}

uint32_t SymbolSetAddress(uint32_t SymbolNum, uint32_t Address)
{
	if (SymbolNum < 0 || SymbolNum >= NumSymbols) return false;
	SymbolAddress[SymbolNum] = Address;
	return true;
}
