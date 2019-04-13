#ifndef SYMBOL_H
#define SYMBOL_H

	// symbol flag definitions
	const uint32_t SYMBOL_FLAG_ENDOFFILE  = 0x00000000;
	const uint32_t SYMBOL_FLAG_NEWLINE    = 0x00000001;
	const uint32_t SYMBOL_FLAG_GRAMMAR    = 0x00000002;
	const uint32_t SYMBOL_FLAG_STRING     = 0x00000003;
	const uint32_t SYMBOL_FLAG_NUMERIC    = 0x00000004;
	const uint32_t SYMBOL_FLAG_IDENTIFIER = 0x00000005;
	const uint32_t SYMBOL_FLAG_ADDRESS    = 0x00000006;
	const uint32_t SYMBOL_FLAG_DIRECTIVE  = 0x00000007;
	const uint32_t SYMBOL_FLAG_MNEMONIC   = 0x00000008;
	const uint32_t SYMBOL_FLAG_CPUREG     = 0x00000009;
	const uint32_t SYMBOL_FLAG_GPUREG     = 0x0000000A;
	const uint32_t SYMBOL_FLAG_GTECTRL    = 0x0000000B;
	const uint32_t SYMBOL_FLAG_GTEDATA    = 0x0000000C;
	const uint32_t SYMBOL_FLAG_ERROR      = 0xDEADC0DE;

	// functions
	uint32_t SymbolInit(void);
	uint32_t SymbolFree(void);
	uint32_t SymbolAdd(char *Text, uint32_t Length, uint32_t Flags, uint32_t MetaData, uint32_t Address);
	uint32_t SymbolGetNumSymbols(void);
	char    *SymbolGetText(uint32_t SymbolNum);
	uint32_t SymbolGetLength(uint32_t SymbolNum);
	uint32_t SymbolGetFlags(uint32_t SymbolNum);
	uint32_t SymbolGetMetaData(uint32_t SymbolNum);
	uint32_t SymbolGetAddress(uint32_t SymbolNum);
	uint32_t SymbolSetText(uint32_t SymbolNum, char *Text);
	uint32_t SymbolSetLength(uint32_t SymbolNum, uint32_t Length);
	uint32_t SymbolSetFlags(uint32_t SymbolNum, uint32_t Flags);
	uint32_t SymbolSetMetaData(uint32_t SymbolNum, uint32_t MetaData);
	uint32_t SymbolSetAddress(uint32_t SymbolNum, uint32_t Address);

#endif
