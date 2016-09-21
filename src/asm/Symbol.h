#ifndef SYMBOL_H
#define SYMBOL_H

	// type definitions
	typedef signed   __int8  int8;
	typedef unsigned __int8  uint8;
	typedef signed   __int16 int16;
	typedef unsigned __int16 uint16;
	typedef signed   __int32 int32;
	typedef unsigned __int32 uint32;
	typedef signed   __int64 int64;
	typedef unsigned __int64 uint64;

	// symbol flag definitions
	const uint32 SYMBOL_FLAG_ENDOFFILE  = 0x00000000;
	const uint32 SYMBOL_FLAG_NEWLINE    = 0x00000001;
	const uint32 SYMBOL_FLAG_GRAMMAR    = 0x00000002;
	const uint32 SYMBOL_FLAG_STRING     = 0x00000003;
	const uint32 SYMBOL_FLAG_NUMERIC    = 0x00000004;
	const uint32 SYMBOL_FLAG_IDENTIFIER = 0x00000005;
	const uint32 SYMBOL_FLAG_ADDRESS    = 0x00000006;
	const uint32 SYMBOL_FLAG_DIRECTIVE  = 0x00000007;
	const uint32 SYMBOL_FLAG_MNEMONIC   = 0x00000008;
	const uint32 SYMBOL_FLAG_CPUREG     = 0x00000009;
	const uint32 SYMBOL_FLAG_GPUREG     = 0x0000000A;
	const uint32 SYMBOL_FLAG_GTECTRL    = 0x0000000B;
	const uint32 SYMBOL_FLAG_GTEDATA    = 0x0000000C;
	const uint32 SYMBOL_FLAG_ERROR      = 0xDEADC0DE;

	// functions
	uint32 SymbolInit(void);
	uint32 SymbolFree(void);
	uint32 SymbolAdd(char *Text, uint32 Length, uint32 Flags, uint32 MetaData, uint32 Address);
	uint32 SymbolGetNumSymbols(void);
	char  *SymbolGetText(uint32 SymbolNum);
	uint32 SymbolGetLength(uint32 SymbolNum);
	uint32 SymbolGetFlags(uint32 SymbolNum);
	uint32 SymbolGetMetaData(uint32 SymbolNum);
	uint32 SymbolGetAddress(uint32 SymbolNum);
	uint32 SymbolSetText(uint32 SymbolNum, char *Text);
	uint32 SymbolSetLength(uint32 SymbolNum, uint32 Length);
	uint32 SymbolSetFlags(uint32 SymbolNum, uint32 Flags);
	uint32 SymbolSetMetaData(uint32 SymbolNum, uint32 MetaData);
	uint32 SymbolSetAddress(uint32 SymbolNum, uint32 Address);

#endif
