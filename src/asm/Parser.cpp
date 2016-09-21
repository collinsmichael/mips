#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include "Parser.h"
#include "Symbol.h"

#pragma warning(disable:4996)

extern char  *CpuReg[0x40];
extern char  *GpuCReg[0x60];
extern char  *GteCReg[0x60];
extern char  *GteDReg[0x60];
extern char  *Mnemonic[0x60];
extern uint32 Opcode[0x60];

static uint32 NumSymbols;
static uint32 Sym;
static char  *Text;
static uint32 Length;
static uint32 Flags;
static uint32 MetaData;
static uint32 Address;

uint32 Param0(uint32 mnemonic)
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	return (OpCode);
}

uint32 Param1(uint32 mnemonic) // op CpuRt, GpuRd
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 GpuRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GPUREG)  GpuRd = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRt|GpuRd);
}

uint32 Param2(uint32 mnemonic) // op CpuRt, GtecRd
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 GtecRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GTECTRL) GtecRd = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRt|GtecRd);
}

uint32 Param3(uint32 mnemonic) // op CpuRt, GtedRd
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 GtedRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GTEDATA) GtedRd = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRt|GtedRd);
}

uint32 Param4(uint32 mnemonic) // op CpuRd
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRd = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRd);
}

uint32 Param5(uint32 mnemonic) // op CpuRd, CpuRs, CpuRt
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRd = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRd = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRd = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRs = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRd|CpuRs|CpuRt);
}

uint32 Param6(uint32 mnemonic) // op CpuRd, CpuRt, CpuRs
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRd = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRd = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRd = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRd|CpuRt|CpuRs);
}

uint32 Param7(uint32 mnemonic) // op CpuRd, CpuRt, Sa
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRd = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRd = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRd = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 Sa = SymbolGetMetaData(Sym) << 0x06;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Sa = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRd|CpuRt|Sa);
}

uint32 Param8(uint32 mnemonic) // op CpuRs
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRs);
}

uint32 Param9(uint32 mnemonic) // op CpuRs, CpuRd
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRs = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRd = SymbolGetMetaData(Sym) << 0x0B;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRd = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRs|CpuRd);
}

uint32 ParamA(uint32 mnemonic) // op CpuRs, CpuRt
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRs = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRs|CpuRt);
}

uint32 ParamB(uint32 mnemonic) // op CpuRs, CpuRt, Imm
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRs = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 Imm = SymbolGetMetaData(Sym);
	Imm = ((Imm-Address)/4) & 0x0000FFFF;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;

	//printf("%08X %08X %08X\r\n", (OpCode|CpuRs|CpuRt|Imm), Imm, Address);
	return (OpCode|CpuRs|CpuRt|Imm);
}

uint32 ParamC(uint32 mnemonic) // op CpuRs, Imm
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRs = 0xFFFFFFFF;
	Sym++;

	uint32 Imm = SymbolGetMetaData(Sym);
	Imm = ((Imm-Address)/4) & 0x0000FFFF;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRs|Imm);
}

uint32 ParamD(uint32 mnemonic) // op CpuRt, Imm(CpuRs)
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 Imm = SymbolGetMetaData(Sym) & 0x0000FFFF;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) Imm = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != '(')              Imm = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ')')              CpuRs = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRt|Imm|CpuRs);
}

uint32 ParamE(uint32 mnemonic) // op CpuRt, CpuRs, Imm
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRs = 0xFFFFFFFF;
	Sym++;

	uint32 Imm = SymbolGetMetaData(Sym) & 0x0000FFFF;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRt|CpuRs|Imm);
}

uint32 ParamF(uint32 mnemonic) // op CpuRt, Imm
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 CpuRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              CpuRt = 0xFFFFFFFF;
	Sym++;

	uint32 Imm = SymbolGetMetaData(Sym) & 0x0000FFFF;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;

	return (OpCode|CpuRt|Imm);
}

uint32 ParamG(uint32 mnemonic) // op GtedRt, Imm(CpuRs)
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 GtedRt = SymbolGetMetaData(Sym) << 0x10;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GTEDATA) GtedRt = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) GtedRt = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ',')              GtedRt = 0xFFFFFFFF;
	Sym++;

	uint32 Imm = SymbolGetMetaData(Sym) & 0x0000FFFF;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) Imm = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != '(')              Imm = 0xFFFFFFFF;
	Sym++;

	uint32 CpuRs = SymbolGetMetaData(Sym) << 0x15;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_CPUREG)  CpuRs = 0xFFFFFFFF;
	Sym++;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_GRAMMAR) CpuRs = 0xFFFFFFFF;
	if (SymbolGetMetaData(Sym) != ')')              CpuRs = 0xFFFFFFFF;
	Sym++;

	return (OpCode|GtedRt|Imm|CpuRs);
}

uint32 ParamH(uint32 mnemonic) // op Imm
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 Imm = (SymbolGetMetaData(Sym) << 0x06) & 0x03FFFFFC;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;

	return (OpCode|Imm);
}

uint32 ParamI(uint32 mnemonic) // op Imm
{
	uint32 OpCode = Opcode[mnemonic];
	Sym++;

	uint32 Imm = (SymbolGetMetaData(Sym)>>2) & 0x03FFFFFF;
	if (SymbolGetFlags(Sym) != SYMBOL_FLAG_NUMERIC) Imm = 0xFFFFFFFF;
	Sym++;

	return (OpCode|Imm);
}

uint32 Assemble(FILE *File)
{
	uint32 OpCode = 0x00000000; // reset OpCode
	uint32 mnemonic = MetaData; // identify OpCode

	// consume arguments
	if      (mnemonic >= CDP     && mnemonic <= NCCT ) OpCode = Param0(mnemonic); // op
	else if (mnemonic >= MFC0    && mnemonic <= MTC0 ) OpCode = Param1(mnemonic); // op CpuRt, GpuRd
	else if (mnemonic >= CTC2    && mnemonic <= CFC2 ) OpCode = Param2(mnemonic); // op CpuRt, GtecRd
	else if (mnemonic >= MFC2    && mnemonic <= MTC2 ) OpCode = Param3(mnemonic); // op CpuRt, GtedRd
	else if (mnemonic >= MFLO    && mnemonic <= MFHI ) OpCode = Param4(mnemonic); // op CpuRd
	else if (mnemonic >= NOR     && mnemonic <= SUBU ) OpCode = Param5(mnemonic); // op CpuRd, CpuRs, CpuRt
	else if (mnemonic >= SLLV    && mnemonic <= SRAV ) OpCode = Param6(mnemonic); // op CpuRd, CpuRt, CpuRs
	else if (mnemonic >= SRL     && mnemonic <= SRA  ) OpCode = Param7(mnemonic); // op CpuRd, CpuRt, Sa
	else if (mnemonic >= MTLO    && mnemonic <= JR   ) OpCode = Param8(mnemonic); // op CpuRs
	else if (mnemonic == JALR                        ) OpCode = Param9(mnemonic); // op CpuRs, CpuRd
	else if (mnemonic >= MULTU   && mnemonic <= DIVU ) OpCode = ParamA(mnemonic); // op CpuRs, CpuRt
	else if (mnemonic >= BNE     && mnemonic <= BEQ  ) OpCode = ParamB(mnemonic); // op CpuRs, CpuRt, Imm
	else if (mnemonic >= BGEZ    && mnemonic <= BGTZ ) OpCode = ParamC(mnemonic); // op CpuRs, Imm
	else if (mnemonic >= SW      && mnemonic <= SWC1 ) OpCode = ParamD(mnemonic); // op CpuRt, Imm(CpuRs)
	else if (mnemonic >= ANDI    && mnemonic <= ADDI ) OpCode = ParamE(mnemonic); // op CpuRt, CpuRs, Imm
	else if (mnemonic == LUI                         ) OpCode = ParamF(mnemonic); // op CpuRt, Imm
	else if (mnemonic >= SWC2    && mnemonic <= LWC2 ) OpCode = ParamG(mnemonic); // op GtedRt, Imm(CpuRs)
	else if (mnemonic >= SYSCALL && mnemonic <= BREAK) OpCode = ParamH(mnemonic); // op Imm
	else if (mnemonic >= JAL     && mnemonic <= J    ) OpCode = ParamI(mnemonic); // op Imm
	else                                               OpCode = 0xFFFFFFFF;       // invalid mnemonic

	// ensure that there's no garbage remaining on this line of code after the instruction
	if ((SymbolGetFlags(Sym) != SYMBOL_FLAG_NEWLINE)
	&&  (SymbolGetFlags(Sym) != SYMBOL_FLAG_ENDOFFILE)) OpCode = 0xFFFFFFFF;

	//printf("%08X: %08X\r\n", Address, OpCode);
	if (File) {
		fprintf(File, "%c%c%c%c",
			((OpCode>>0x00)&0xFF), ((OpCode>>0x08)&0xFF),
			((OpCode>>0x10)&0xFF), ((OpCode>>0x18)&0xFF));
	}

	// return final OpCode
	return OpCode;
}

uint32 Direct(FILE *File)
{
	uint32 OpCode;
	uint32 Flags;
	char  *Text;
	switch (MetaData) {
	case DIRECTIVE_BYTE:
	//	Sym++; OpCode = SymbolGetMetaData(Sym); Sym++;
	//	if (File) { fprintf(File, "%c", ((OpCode>>0x00)&0xFF)); }
		do {
			Sym++;
			OpCode = SymbolGetMetaData(Sym); Sym++;
			if (File) { fprintf(File, "%c", ((OpCode>>0x00)&0xFF)); }
			Flags  = SymbolGetFlags(Sym);
			Text = SymbolGetText(Sym);
		} while ( ( Flags == SYMBOL_FLAG_GRAMMAR ) && ( Text[0] == ',' ) );
		break;
	case DIRECTIVE_HALF:
	//	Sym++; OpCode = SymbolGetMetaData(Sym); Sym++;
	//	if (File) { fprintf(File, "%c%c", ((OpCode>>0x00)&0xFF), ((OpCode>>0x08)&0xFF)); }
		do {
			Sym++;
			OpCode = SymbolGetMetaData(Sym); Sym++;
			if (File) { fprintf(File, "%c%c", ((OpCode>>0x00)&0xFF), ((OpCode>>0x08)&0xFF)); }
			Flags  = SymbolGetFlags(Sym);
			Text = SymbolGetText(Sym);
		} while ( ( Flags == SYMBOL_FLAG_GRAMMAR ) && ( Text[0] == ',' ) );
		break;
	case DIRECTIVE_WORD:
	//	Sym++; OpCode = SymbolGetMetaData(Sym); Sym++;
	//	if (File) { fprintf(File, "%c%c%c%c", ((OpCode>>0x00)&0xFF), ((OpCode>>0x08)&0xFF), ((OpCode>>0x10)&0xFF), ((OpCode>>0x18)&0xFF)); }
		do {
			Sym++;
			OpCode = SymbolGetMetaData(Sym); Sym++;
			if (File) { fprintf(File, "%c%c%c%c", ((OpCode>>0x00)&0xFF), ((OpCode>>0x08)&0xFF), ((OpCode>>0x10)&0xFF), ((OpCode>>0x18)&0xFF)); }
			Flags  = SymbolGetFlags(Sym);
			Text = SymbolGetText(Sym);
		} while ( ( Flags == SYMBOL_FLAG_GRAMMAR ) && ( Text[0] == ',' ) );

		break;
	}
	return true;
}

uint32 Parser(char *FilePath)
{
	int    LenFilePath = strlen(FilePath);
	char  *FileName = (char*)malloc(LenFilePath+4);
	if (!FileName) return false;
	strcpy(FileName, FilePath);
	for (int i = LenFilePath; i > 0; i--) {
		if (FileName[i] == '.') {
			FileName[i] = 0;
			i = 0;
		}
	}
	strcat(FileName, ".PRG");
	FILE *File = fopen(FileName, "wb");

	uint32 OpCode;
	NumSymbols = SymbolGetNumSymbols();
	Sym = 0;

	while (Sym < NumSymbols) {
		Text     = SymbolGetText(Sym);
		Length   = SymbolGetLength(Sym);
		Flags    = SymbolGetFlags(Sym);
		MetaData = SymbolGetMetaData(Sym);

		switch (Flags) {
		case SYMBOL_FLAG_ENDOFFILE:
		case SYMBOL_FLAG_GRAMMAR:
		case SYMBOL_FLAG_STRING:
		case SYMBOL_FLAG_CPUREG:
		case SYMBOL_FLAG_GPUREG:
		case SYMBOL_FLAG_GTECTRL:
		case SYMBOL_FLAG_GTEDATA:
		case SYMBOL_FLAG_ERROR:
		case SYMBOL_FLAG_IDENTIFIER:
		case SYMBOL_FLAG_NEWLINE:
		case SYMBOL_FLAG_NUMERIC:
			Sym++;
			break;
		case SYMBOL_FLAG_ADDRESS:
			Address = MetaData + 0x04;
			Sym++;
			break;
		case SYMBOL_FLAG_MNEMONIC:
			OpCode = Assemble(File);
			break;
		case SYMBOL_FLAG_DIRECTIVE:
			Direct(File);
			break;
		}
	}
	return true;
}
