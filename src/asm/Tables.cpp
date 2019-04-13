#include <stdint.h>
#include "Symbol.h"

// CPU General Purpose Registers
const char *CpuReg[0x40] = {
	"R0",   "R1",   "R2",   "R3",   "R4",   "R5",   "R6",   "R7",
	"R8",   "R9",   "R10",  "R11",  "R12",  "R13",  "R14",  "R15",
	"R16",  "R17",  "R18",  "R19",  "R20",  "R21",  "R22",  "R23",
	"R24",  "R25",  "R26",  "R27",  "R28",  "R29",  "R30",  "R31",
	"ZERO", "AT",   "V0",   "V1",   "A0",   "A1",   "A2",   "A3",
	"T0",   "T1",   "T2",   "T3",   "T4",   "T5",   "T6",   "T7",
	"S0",   "S1",   "S2",   "S3",   "S4",   "S5",   "S6",   "S7",
	"T8",   "T9",   "K0",   "K1",   "GP",   "SP",   "S8",   "RA"
};

// GPU Control Registers
const char *GpuReg[0x60] = {
	"COP0CR00", "COP0CR01", "COP0CR02", "COP0CR03", "COP0CR04", "COP0CR05", "COP0CR06", "COP0CR07",
	"COP0CR08", "COP0CR09", "COP0CR10", "COP0CR11", "COP0CR12", "COP0CR13", "COP0CR14", "COP0CR15",
	"COP0CR16", 0,          0,          0,          0,          0,          0,          0,
	0,          0,          0,          0,          0,          0,          0,          0,
	"INDEX",    "RANDOM",   "TLBLO",    "BPC",      "CTXT",     "BDA",      "PIDMASK",  "DCIC",
	"BADVADDR", "BDAM",     "TLBHI",    "BPCM",     "SR",       "CAUSE",    "EPC",      "PRID",
	"ERREG",    0,          0,          0,          0,          0,          0,          0,
	0,          0,          0,          0,          0,          0,          0,          0,
	"COP0CR00_INDEX",       "COP0CR01_RANDOM",      "COP0CR02_TLBLO",       "COP0CR03_BPC",
	"COP0CR04_CTXT",        "COP0CR05_BDA",         "COP0CR06_PIDMASK",     "COP0CR07_DCIC",
	"COP0CR08_BADVADDR",    "COP0CR09_BDAM",        "COP0CR10_TLBHI",       "COP0CR11_BPCM",
	"COP0CR12_SR",          "COP0CR13_CAUSE",       "COP0CR14_EPC",         "COP0CR15_PRID",
	"COP0CR16_ERREG",       0,                      0,                      0,
	0,                      0,                      0,                      0,
	0,                      0,                      0,                      0,
	0,                      0,                      0,                      0
};

// GTE Control Registers
const char *GteCReg[0x60] = {
	"GTECR00", "GTECR01", "GTECR02", "GTECR03", "GTECR04", "GTECR05", "GTECR06", "GTECR07",
	"GTECR08", "GTECR09", "GTECR10", "GTECR11", "GTECR12", "GTECR13", "GTECR14", "GTECR15",
	"GTECR16", "GTECR17", "GTECR18", "GTECR19", "GTECR20", "GTECR21", "GTECR22", "GTECR23",
	"GTECR24", "GTECR25", "GTECR26", "GTECR27", "GTECR28", "GTECR29", "GTECR30", "GTECR31",
	"R11R12",  "R13R21",  "R22R23",  "R31R32",  "R33",     "TRX",     "TRY",     "TRZ",
	"L11L12",  "L13L21",  "L22L23",  "L31L32",  "L33",     "RBK",     "BBK",     "GBK",
	"LR1LR2",  "LR3LG1",  "LG2LG3",  "LB1LB2",  "LB3",     "RFC",     "GFC",     "BFC",
	"OFX",     "OFY",     "H",       "DQA",     "DQB",     "ZSF3",    "ZSF4",    "FLAG",
	"GTECR00_R11R12", "GTECR01_R13R21", "GTECR02_R22R23", "GTECR03_R31R32",
	"GTECR04_R33",    "GTECR05_TRX",    "GTECR06_TRY",    "GTECR07_TRZ",
	"GTECR08_L11L12", "GTECR09_L13L21", "GTECR10_L22L23", "GTECR11_L31L32",
	"GTECR12_L33",    "GTECR13_RBK",    "GTECR14_BBK",    "GTECR15_GBK",
	"GTECR16_LR1LR2", "GTECR17_LR3LG1", "GTECR18_LG2LG3", "GTECR19_LB1LB2",
	"GTECR20_LB3",    "GTECR21_RFC",    "GTECR22_GFC",    "GTECR23_BFC",
	"GTECR24_OFX",    "GTECR25_OFY",    "GTECR26_H",      "GTECR27_DQA",
	"GTECR28_DQB",    "GTECR29_ZSF3",   "GTECR30_ZSF4",   "GTECR31_FLAG"
};

// GTE Data Registers
const char *GteDReg[0x60] = {
	"GTEDR00", "GTEDR01", "GTEDR02", "GTEDR03", "GTEDR04", "GTEDR05", "GTEDR06", "GTEDR07",
	"GTEDR08", "GTEDR09", "GTEDR10", "GTEDR11", "GTEDR12", "GTEDR13", "GTEDR14", "GTEDR15",
	"GTEDR16", "GTEDR17", "GTEDR18", "GTEDR19", "GTEDR20", "GTEDR21", "GTEDR22", "GTEDR23",
	"GTEDR24", "GTEDR25", "GTEDR26", "GTEDR27", "GTEDR28", "GTEDR29", "GTEDR30", "GTEDR31",
	"VXY0",    "VZ0",     "VXY1",    "VZ1",     "VXY2",    "VZ2",     "RGB",     "OTZ",
	"IR0",     "IR1",     "IR2",     "IR3",     "SXY0",    "SXY1",    "SXY2",    "SXYP",
	"SZ0",     "SZ1",     "SZ2",     "SZ3",     "RGB0",    "RGB1",    "RGB2",    "RES1",
	"MAC0",    "MAC1",    "MAC2",    "MAC3",    "IRGB",    "ORGB",    "LZCS",    "LZCR",
	"GTEDR00_VXY0", "GTEDR01_VZ0",  "GTEDR02_VXY1", "GTEDR03_VZ1",
	"GTEDR04_VXY2", "GTEDR05_VZ2",  "GTEDR06_RGB",  "GTEDR07_OTZ",
	"GTEDR08_IR0",  "GTEDR09_IR1",  "GTEDR10_IR2",  "GTEDR11_IR3",
	"GTEDR12_SXY0", "GTEDR13_SXY1", "GTEDR14_SXY2", "GTEDR15_SXYP",
	"GTEDR16_SZ0",  "GTEDR17_SZ1",  "GTEDR18_SZ2",  "GTEDR19_SZ3",
	"GTEDR20_RGB0", "GTEDR21_RGB1", "GTEDR22_RGB2", "GTEDR23_RES1",
	"GTEDR24_MAC0", "GTEDR25_MAC1", "GTEDR26_MAC2", "GTEDR27_MAC3",
	"GTEDR28_IRGB", "GTEDR29_ORGB", "GTEDR30_LZCS", "GTEDR31_LZCR"
};

// Opcode Mnemonics
const char *Mnemonic[0x60] = {
	"CDP",     "RFE",     "TLBWI",   "GPF",     "CC",      "RTPT",    "AVSZ3",   "TLBR",
	"NCLIP",   "SQR",     "RTPS",    "DPCT",    "NCDS",    "DPCS",    "MVMVA",   "NOP",
	"NCCS",    "NCDT",    "INTPL",   "NCS",     "OP",      "NCT",     "DPCL",    "GPL",
	"TLBWR",   "AVSZ4",   "NCCT",    "MFC0",    "MTC0",    "CTC2",    "CFC2",    "MFC2",
	"MTC2",    "MFLO",    "MFHI",    "NOR",     "ADD",     "ADDU",    "SLTU",    "XOR",
	"OR",      "SLT",     "SUB",     "AND",     "SUBU",    "SLLV",    "SRLV",    "SRAV",
	"SRL",     "SLL",     "SRA",     "MTLO",    "MTHI",    "JR",      "JALR",    "MULTU",
	"MULT",    "DIV",     "DIVU",    "BNE",     "BEQ",     "BGEZ",    "BLEZ",    "BLTZ",
	"BGEZAL",  "BLTZAL",  "BGTZ",    "SW",      "SB",      "LWL",     "SWR",     "LB",
	"SWL",     "LHU",     "LH",      "SH",      "LWR",     "LBU",     "LWC1",    "LW",
	"SWC1",    "ANDI",    "ADDIU",   "XORI",    "ORI",     "SLTIU",   "SLTI",    "ADDI",
	"LUI",     "SWC2",    "LWC2",    "SYSCALL", "BREAK",   "JAL",     "J",       0
};

// Opcode binary encodings
uint32_t Opcode[0x60] = {
//	0x4A000014, 0x40000010, 0x40000002, 0x4B800000, 0x4A00001C, 0x4A200000, 0x4A00002D, 0x40000001, 
	0x4A000014, 0x40000010, 0x40000002, 0x4B98003D, 0x4A00001C, 0x4A280030, 0x4B58002D, 0x40000001, 
//	0x4A000006, 0x4AA00000, 0x4A000001, 0x4A00002A, 0x4A000013, 0x4A000010, 0x4A000012, 0x00000000, 
	0x4B400006, 0x4AA00428, 0x4A180001, 0x4A00002A, 0x4A000013, 0x4A780010, 0x4A480012, 0x00000000, 
//	0x4A00001B, 0x4A000016, 0x4A800000, 0x4A00001E, 0x4A00000C, 0x4A000020, 0x4A000029, 0x4BA00000, 
	0x4A00001B, 0x4A000016, 0x4A800000, 0x4A00001E, 0x4B70000C, 0x4A000020, 0x4A000029, 0x4BA00000, 
	0x40000006, 0x4A00002E, 0x4A00003F, 0x40000000, 0x40800000, 0x48C00000, 0x48400000, 0x48000000, 
	0x48800000, 0x00000012, 0x00000010, 0x00000027, 0x00000020, 0x00000021, 0x0000002B, 0x00000026, 
	0x00000025, 0x0000002A, 0x00000022, 0x00000024, 0x00000023, 0x00000004, 0x00000006, 0x00000007, 
	0x00000002, 0x00000000, 0x00000003, 0x00000013, 0x00000011, 0x00000008, 0x00000009, 0x00000019, 
	0x00000018, 0x0000001A, 0x0000001B, 0x14000000, 0x10000000, 0x04010000, 0x18000000, 0x04000000, 
	0x04110000, 0x04100000, 0x1C000000, 0xAC000000, 0xA0000000, 0x88000000, 0xB8000000, 0x80000000, 
	0xA8000000, 0x94000000, 0x84000000, 0xA4000000, 0x98000000, 0x90000000, 0xC4000000, 0x8C000000, 
	0xE4000000, 0x30000000, 0x24000000, 0x38000000, 0x34000000, 0x2C000000, 0x28000000, 0x20000000, 
	0x3C000000, 0xE8000000, 0xC8000000, 0x0000000C, 0x0000000D, 0x0C000000, 0x08000000, 0xFFFFFFFF
};

const char *Directive[0x20] = {
	"BYTE", "HALF", "WORD", 0
};
