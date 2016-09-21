//
// File Name : Parse.cpp
// File Date : 19 March 2012
// Author(s) : Valendian
//
// ToDo List :
//
// [#] unhandled
// [#] exit
// [#] quit
// [#] command
// [#] open
// [-] link
// [#] close
// [#] register
// [#] print
// [#] show
// [#] hide
// [#] debug
// [#] address
// [#] entry
// [#] dasm
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>

#include "Main.h"
#include "Lexer.h"
#include "Parse.h"
#include "Command.h"
#include "Dasm.h"

const int NumNamingConventions = 4;
const char *NamingConvention[NumNamingConventions] = {
	"dasm",
	"psx",
	"common",
	"numeric"
};
const int NamingDasm    = 0;
const int NamingPsx     = 1;
const int NamingCommon  = 2;
const int NamingNumeric = 3;
int volatile RegisterNaming = NamingDasm;

const int NumDisplayOptions = 6;
const char *DisplayOption[NumDisplayOptions] = {
	"",
	"address",
	"opcode",
	"label",
	"mnemonic",
	"ascii"
};
const int DisplayAddress  = 1;
const int DisplayOpcode   = 2;
const int DisplayLabel    = 3;
const int DisplayMnemonic = 4;
const int DisplayAscii    = 5;

extern binary Binary;
extern uint32 Address;
extern uint32 Entry;

bool volatile ShowAddress  = true;
bool volatile ShowOpcode   = true;
bool volatile ShowLabel    = true;
bool volatile ShowMnemonic = true;
bool volatile ShowAscii    = true;

bool volatile DebugLexer   = false;
bool volatile DebugParser  = false;
bool volatile DebugCommand = false;

static char Buffer[0x1000];

int Close(binary *Binary, char *FilePath)
{
	if (!Binary) return false;
	if (!FilePath) return false;
	if (Binary->File) fclose(Binary->File);
	if (Binary->Data) free(Binary->Data);
	if (Binary->Copy) free(Binary->Copy);
	if (Binary->Code) free(Binary->Code);
	memset(Binary, 0, sizeof(Binary));
	return true;
}

int Open(binary *Binary, char *FilePath)
{
	if (!Binary) return false;
	if (!FilePath) return false;

	Close(Binary, FilePath);
	strcpy_s(Binary->Path, sizeof(Binary->Path), FilePath);
	_splitpath_s(Binary->Path, Binary->Drive, sizeof(Binary->Drive), Binary->Dir, sizeof(Binary->Dir),
		Binary->Fname, sizeof(Binary->Fname), Binary->Ext, sizeof(Binary->Ext));
	if (fopen_s(&Binary->File, FilePath, "rb")) {
		Close(Binary, FilePath);
		printf("error : failed to open file for reading! %s%s", Binary->Fname, Binary->Ext);
		return false;
	}

	fseek(Binary->File, 0, SEEK_END);
	Binary->Size = ftell(Binary->File);
	fseek(Binary->File, 0, SEEK_SET);

	Binary->Copy = malloc(Binary->Size*sizeof(uint32));
	if (!Binary->Copy) {
		Close(Binary, FilePath);
		printf("error : failed to allocate memory for file flags! %s%s", Binary->Fname, Binary->Ext);
		return false;
	}
	memset(Binary->Copy, 0, Binary->Size*sizeof(uint32));

	Binary->Code = malloc(Binary->Size);
	if (!Binary->Code) {
		Close(Binary, FilePath);
		printf("error : failed to allocate memory for file flags! %s%s", Binary->Fname, Binary->Ext);
		return false;
	}
	memset(Binary->Code, 0, Binary->Size);

	Binary->Data = malloc(Binary->Size);
	if (!Binary->Data) {
		Close(Binary, FilePath);
		printf("error : failed to allocate memory for file data! %s%s", Binary->Fname, Binary->Ext);
		return false;
	}
	if (fread(Binary->Data, Binary->Size, 1, Binary->File) != 1) {
		Close(Binary, FilePath);
		printf("error : failed to read file data into memory! %s%s", Binary->Fname, Binary->Ext);
		return false;
	}
	fclose(Binary->File);

	strcat_s(Binary->Path, sizeof(Binary->Path), ".LST");
	if (fopen_s(&Binary->File, Binary->Path, "wb")) {
		Close(Binary, FilePath);
		printf("error : failed to create assembler listing file! %s%s", Binary->Fname, Binary->Ext);
		return false;
	}
	fseek(Binary->File, 0, SEEK_SET);
	return true;
}

int DoUnhandled(table *Table)
{
	if (Table) {
		if (Table->Token) {
			int CurToken = GetCurToken(Table);
			GetTokenText(Table, CurToken, Buffer);
			printf("error : unhandled command! '%s'\n", Buffer);
			SetCurToken(Table, CurToken+1);
		}
	}
	return true;
}

int DoExit(table *Table)
{
	int Length = 1;
	if (Table) SetCurToken(Table, GetCurToken(Table)+Length);
	return false;
}

int DoQuit(table *Table)
{
	int Length = 1;
	if (Table) SetCurToken(Table, GetCurToken(Table)+Length);
	return false;
}

int DoCommand(table *Table)
{
	FILE *File;
	char *Data;
	int   Size, Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenNumber:  return Length;
		case TokenKeyWord: return Length;
		case TokenQuote:
		case TokenWord:
			GetTokenText(Table, CurToken+2, Buffer);
			if (fopen_s(&File, Buffer, "rb")) {
				printf("error : failed to open command file! '%s'\n", Buffer);
				return Length;
			}
			fseek(File, 0, SEEK_END);
			Size = ftell(File);
			fseek(File, 0, SEEK_SET);
			Data = (char*)malloc(Size+1);
			if (!Data) {
				fclose(File);
				printf("error : failed to allocate memory for command file! '%s'\n", Buffer);
				return Length;
			}
			if (fread(Data, Size, 1, File) != 1) {
				fclose(File);
				free(Data);
				printf("error : failed to read command file into momery! '%s'\n", Buffer);
				return Length;
			}
			fclose(File);
			Data[Size] = 0;

			strcpy_s(Buffer, sizeof(Buffer), "CommandFile ");
			GetTokenText(Table, CurToken+2, &Buffer[strlen(Buffer)]);
			Command(Buffer, Data);
			free(Data);
			Length += 4;

		}
	}
	return Length;
}

int DoOpen(table *Table)
{
	int Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;
		Length += 4;

		Category = GetTokenCategory(Table, CurToken+2);
		GetTokenText(Table, CurToken+2, Buffer);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer:
		case TokenNumber:
		case TokenKeyWord:
		case TokenQuote:
		case TokenWord:
			if (!Open(&Binary, Buffer)) return Length;
			break;
		}
	}
	return Length;
}

int DoLink(table *Table)
{
	int Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenNumber:  return Length;
		case TokenKeyWord: return Length;
		case TokenQuote:
		case TokenWord:
			Length += 4;
		}
	}
	return Length;
}

int DoClose(table *Table)
{
	int Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenNumber:  return Length;
		case TokenKeyWord: return Length;
		case TokenQuote:
		case TokenWord:
			Length += 4;
		}
	}
	return Length;
}

int DoRegister(table *Table)
{
	int Category, Naming = 0, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenKeyWord: return Length;
		case TokenQuote:
		case TokenWord:
			GetTokenText(Table, CurToken+2, Buffer);
			for (int i = 0; i < NumNamingConventions; i++) {
				if (_strcmpi(Buffer, NamingConvention[i]) == 0) Naming = i;
			}
			Length += 4;
			break;
		case TokenNumber:
			Naming = GetTokenValue(Table, CurToken+2);
			Length += 4;
		}

		switch (Naming) {
		default: return Length;
		case 0: RegisterNaming = NamingDasm;    break;
		case 1: RegisterNaming = NamingPsx;     break;
		case 2: RegisterNaming = NamingCommon;  break;
		case 3: RegisterNaming = NamingNumeric; break;
		}
	}
	return Length;
}

int DoPrint(table *Table)
{
	int CurToken, Category, Value, Length = 1;
	if (Table) {
		CurToken = GetCurToken(Table);

		// Parse the opening brace...
		Category = GetTokenCategory(Table, CurToken+Length);
		Value = GetTokenValue(Table, CurToken+Length);
		if (Category != TokenGrammer || Value != '(') return Length;

		// Count the number of arguments to be printed...
		do {
			Length++;
			Category = GetTokenCategory(Table, CurToken+2);
			switch (Category) {
			default:           return Length;
			case 0:            return Length;
			case TokenGrammer: return Length;
			case TokenKeyWord:
			case TokenNumber:
			case TokenQuote:
			case TokenWord:
				Length++;
			}
			Category = GetTokenCategory(Table, CurToken+Length);
			Value = GetTokenValue(Table, CurToken+Length);
		} while (Category == TokenGrammer && Value == ',');

		// Parse the closing brace and terminator...
		Category = GetTokenCategory(Table, CurToken+Length);
		Value = GetTokenValue(Table, CurToken+Length);
		if (Category != TokenGrammer || Value != ')') return Length;
		Length++;
		Category = GetTokenCategory(Table, CurToken+Length);
		Value = GetTokenValue(Table, CurToken+Length);
		if (Category != TokenGrammer || Value != ';') return Length;
		Length++;

		// Print the specified arguments...
		for (int i = 2; i < Length-2; i++) {
			GetTokenText(Table, CurToken+i, Buffer);
			printf("%s", Buffer);
		}
		printf("\n");
	}
	return Length;
}

int DoShow(table *Table)
{
	int Category, Display = 0, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenNumber:  return Length;
		case TokenKeyWord: //return Length;
		case TokenQuote:
		case TokenWord:
			GetTokenText(Table, CurToken+2, Buffer);
			for (int i = 0; i < NumDisplayOptions; i++) {
				if (_strcmpi(Buffer, DisplayOption[i]) == 0) Display = i;
			}
			Length += 4;
		}

		switch (Display) {
		case 0: break;
		case DisplayAddress:  ShowAddress  = true; break;
		case DisplayOpcode:   ShowOpcode   = true; break;
		case DisplayLabel:    ShowLabel    = true; break;
		case DisplayMnemonic: ShowMnemonic = true; break;
		case DisplayAscii:    ShowAscii    = true; break;
		}
	}
	return Length;
}

int DoHide(table *Table)
{
	int Category, Display = 0, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenNumber:  return Length;
		case TokenKeyWord: //return Length;
		case TokenQuote:
		case TokenWord:
			GetTokenText(Table, CurToken+2, Buffer);
			for (int i = 0; i < NumDisplayOptions; i++) {
				if (_strcmpi(Buffer, DisplayOption[i]) == 0) Display = i;
			}
			Length += 4;
		}

		switch (Display) {
		case 0: break;
		case DisplayAddress:  ShowAddress  = false; break;
		case DisplayOpcode:   ShowOpcode   = false; break;
		case DisplayLabel:    ShowLabel    = false; break;
		case DisplayMnemonic: ShowMnemonic = false; break;
		case DisplayAscii:    ShowAscii    = false; break;
		}
	}
	return Length;
}

int DoDebug(table *Table)
{
	int Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenNumber:  return Length;
		case TokenKeyWord:
		case TokenQuote:
		case TokenWord:
			GetTokenText(Table, CurToken+2, Buffer);
			printf("debugging %s\n", Buffer);
			if      (_strcmpi(Buffer, "lexer") == 0) DebugLexer = true;
			else if (_strcmpi(Buffer, "parser") == 0) DebugParser = true;
			else if (_strcmpi(Buffer, "command") == 0) DebugCommand = true;
			else return Length;
			Length += 4;
		}
	}
	return Length;
}

int DoAddress(table *Table)
{
	int Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;
		Length += 4;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenKeyWord: return Length;
		case TokenQuote:   return Length;
		case TokenWord:    return Length;
		case TokenNumber:
			Address = GetTokenValue(Table, CurToken+2);
		}
	}
	return Length;
}

int DoEntry(table *Table)
{
	int Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;
		Length += 4;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenKeyWord: return Length;
		case TokenQuote:   return Length;
		case TokenWord:    return Length;
		case TokenNumber:
			Entry = GetTokenValue(Table, CurToken+2);
		}
	}
	return Length;
}

int DoCode(table *Table)
{
	uint32 *Byte;
	uint32 Start, Finish;
	int    Category, Length = 1;

	Byte = (uint32*)Binary.Copy;
	if (!Byte) return Length;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+5) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+6) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ',') return Length;
		if (GetTokenValue(Table, CurToken+5) != ')') return Length;
		if (GetTokenValue(Table, CurToken+6) != ';') return Length;
		Length += 6;

		Category = GetTokenCategory(Table, CurToken+2);
		if (Category != TokenNumber) return Length;
		Start = GetTokenValue(Table, CurToken+2);
		if ((Start < Address) || (Start > Address+Binary.Size)) return Length;
		Start -= Address;

		Category = GetTokenCategory(Table, CurToken+4);
		if (Category != TokenNumber) return Length;
		Finish = GetTokenValue(Table, CurToken+4);
		if ((Finish < Address) || (Finish > Address+Binary.Size)) return Length;
		Finish -= Address;

		for (uint32 i = Start; i <= Finish; i++) Byte[i] |= FlagCode;
	}
	return Length;
}

int DoData(table *Table)
{
	uint32 *Byte;
	uint32 Start, Finish;
	int    Category, Length = 1;

	Byte = (uint32*)Binary.Copy;
	if (!Byte) return Length;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+5) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+6) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ',') return Length;
		if (GetTokenValue(Table, CurToken+5) != ')') return Length;
		if (GetTokenValue(Table, CurToken+6) != ';') return Length;
		Length += 6;

		Category = GetTokenCategory(Table, CurToken+2);
		if (Category != TokenNumber) return Length;
		Start = GetTokenValue(Table, CurToken+2);
		if ((Start < Address) || (Start > Address+Binary.Size)) return Length;
		Start -= Address;

		Category = GetTokenCategory(Table, CurToken+4);
		if (Category != TokenNumber) return Length;
		Finish = GetTokenValue(Table, CurToken+4);
		if ((Finish < Address) || (Finish > Address+Binary.Size)) return Length;
		Finish -= Address;

		for (uint32 i = Start; i <= Finish; i++) Byte[i] |= FlagData;
	}
	return Length;
}

int DoDasm(table *Table)
{
	int Category, Length = 1;
	if (Table) {
		int CurToken = GetCurToken(Table);
		if (GetTokenCategory(Table, CurToken+1) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+3) != TokenGrammer) return Length;
		if (GetTokenCategory(Table, CurToken+4) != TokenGrammer) return Length;
		if (GetTokenValue(Table, CurToken+1) != '(') return Length;
		if (GetTokenValue(Table, CurToken+3) != ')') return Length;
		if (GetTokenValue(Table, CurToken+4) != ';') return Length;

		Category = GetTokenCategory(Table, CurToken+2);
		switch (Category) {
		default:           return Length;
		case 0:            return Length;
		case TokenGrammer: return Length;
		case TokenNumber:  return Length;
		case TokenKeyWord: return Length;
		case TokenQuote:
		case TokenWord:
			Dasm();
			Length += 4;
		}
	}
	return Length;
}

int Parse(table *Table)
{
	int CurToken, Length, Category, Value;
	if (!Table) return false;
	if (!Table->Token) return false;
	if (Table->NumTokens <= 1) return false;

	if (DebugParser) printf("\nParser\n");
	while ((CurToken = GetCurToken(Table)) < GetNumTokens(Table)) {
		Category = GetTokenCategory(Table, CurToken);
		switch (Category) {
		default:           return DoUnhandled(Table);
		case 0:            return true;
		case TokenGrammer: return DoUnhandled(Table);
		case TokenQuote:   return DoUnhandled(Table);
		case TokenNumber:  return DoUnhandled(Table);
		case TokenWord:    return DoUnhandled(Table);
		case TokenKeyWord:
			Value = GetTokenValue(Table, CurToken);
			switch (Value) {
			default:              return DoUnhandled(Table);
			case 0:               return true;
			case KeyWordExit:     return DoExit(Table);
			case KeyWordQuit:     return DoQuit(Table);
			case KeyWordCommand:  Length = DoCommand(Table);  break;
			case KeyWordOpen:     Length = DoOpen(Table);     break;
			case KeyWordLink:     Length = DoLink(Table);     break;
			case KeyWordClose:    Length = DoClose(Table);    break;
			case KeyWordRegister: Length = DoRegister(Table); break;
			case KeyWordPrint:    Length = DoPrint(Table);    break;
			case KeyWordShow:     Length = DoShow(Table);     break;
			case KeyWordHide:     Length = DoHide(Table);     break;
			case KeyWordDebug:    Length = DoDebug(Table);    break;
			case KeyWordAddress:  Length = DoAddress(Table);  break;
			case KeyWordEntry:    Length = DoEntry(Table);    break;
			case KeyWordCode:     Length = DoCode(Table);     break;
			case KeyWordData:     Length = DoData(Table);     break;
			case KeyWordDasm:     Length = DoDasm(Table);     break;
			}
			break;
		}
		if (DebugParser) {
			printf("%08X : ", Length);
			for (int i = 0; i < Length; i++) {
				GetTokenText(Table, CurToken+i, Buffer);
				if (Buffer[0]) {
					if (GetTokenCategory(Table, CurToken+i) == TokenQuote) {
						printf("'%s'", Buffer);
					} else {
						printf("%s", Buffer);
					}
				}
			}
			printf("\n");
		}
		SetCurToken(Table, CurToken+Length);
	}
	return true;
}
