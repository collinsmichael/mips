#ifndef PARSER_H
#define PARSER_H
#include "Symbol.h"

	// CPU General Purpose Registers
	const uint32_t R0   = 0x00; const uint32_t ZERO = 0x00;
	const uint32_t R1   = 0x01; const uint32_t AT   = 0x01;
	const uint32_t R2   = 0x02; const uint32_t V0   = 0x02;
	const uint32_t R3   = 0x03; const uint32_t V1   = 0x03;
	const uint32_t R4   = 0x04; const uint32_t A0   = 0x04;
	const uint32_t R5   = 0x05; const uint32_t A1   = 0x05;
	const uint32_t R6   = 0x06; const uint32_t A2   = 0x06;
	const uint32_t R7   = 0x07; const uint32_t A3   = 0x07;
	const uint32_t R8   = 0x08; const uint32_t T0   = 0x08;
	const uint32_t R9   = 0x09; const uint32_t T1   = 0x09;
	const uint32_t R10  = 0x0A; const uint32_t T2   = 0x0A;
	const uint32_t R11  = 0x0B; const uint32_t T3   = 0x0B;
	const uint32_t R12  = 0x0C; const uint32_t T4   = 0x0C;
	const uint32_t R13  = 0x0D; const uint32_t T5   = 0x0D;
	const uint32_t R14  = 0x0E; const uint32_t T6   = 0x0E;
	const uint32_t R15  = 0x0F; const uint32_t T7   = 0x0F;
	const uint32_t R16  = 0x10; const uint32_t S0   = 0x10;
	const uint32_t R17  = 0x11; const uint32_t S1   = 0x11;
	const uint32_t R18  = 0x12; const uint32_t S2   = 0x12;
	const uint32_t R19  = 0x13; const uint32_t S3   = 0x13;
	const uint32_t R20  = 0x14; const uint32_t S4   = 0x14;
	const uint32_t R21  = 0x15; const uint32_t S5   = 0x15;
	const uint32_t R22  = 0x16; const uint32_t S6   = 0x16;
	const uint32_t R23  = 0x17; const uint32_t S7   = 0x17;
	const uint32_t R24  = 0x18; const uint32_t T8   = 0x18;
	const uint32_t R25  = 0x19; const uint32_t T9   = 0x19;
	const uint32_t R26  = 0x1A; const uint32_t K0   = 0x1A;
	const uint32_t R27  = 0x1B; const uint32_t K1   = 0x1B;
	const uint32_t R28  = 0x1C; const uint32_t GP   = 0x1C;
	const uint32_t R29  = 0x1D; const uint32_t SP   = 0x1D;
	const uint32_t R30  = 0x1E; const uint32_t S8   = 0x1E;
	const uint32_t R31  = 0x1F; const uint32_t RA   = 0x1F;

	// GPU Control Registers
	const uint32_t COP0CR00 = 0x00; const uint32_t INDEX    = 0x00; const uint32_t COP0CR00_INDEX    = 0x00;
	const uint32_t COP0CR01 = 0x01; const uint32_t RANDOM   = 0x01; const uint32_t COP0CR01_RANDOM   = 0x01;
	const uint32_t COP0CR02 = 0x02; const uint32_t TLBLO    = 0x02; const uint32_t COP0CR02_TLBLO    = 0x02;
	const uint32_t COP0CR03 = 0x03; const uint32_t BPC      = 0x03; const uint32_t COP0CR03_BPC      = 0x03;
	const uint32_t COP0CR04 = 0x04; const uint32_t CTXT     = 0x04; const uint32_t COP0CR04_CTXT     = 0x04;
	const uint32_t COP0CR05 = 0x05; const uint32_t BDA      = 0x05; const uint32_t COP0CR05_BDA      = 0x05;
	const uint32_t COP0CR06 = 0x06; const uint32_t PIDMASK  = 0x06; const uint32_t COP0CR06_PIDMASK  = 0x06;
	const uint32_t COP0CR07 = 0x07; const uint32_t DCIC     = 0x07; const uint32_t COP0CR07_DCIC     = 0x07;
	const uint32_t COP0CR08 = 0x08; const uint32_t BADVADDR = 0x08; const uint32_t COP0CR08_BADVADDR = 0x08;
	const uint32_t COP0CR09 = 0x09; const uint32_t BDAM     = 0x09; const uint32_t COP0CR09_BDAM     = 0x09;
	const uint32_t COP0CR10 = 0x0A; const uint32_t TLBHI    = 0x0A; const uint32_t COP0CR10_TLBHI    = 0x0A;
	const uint32_t COP0CR11 = 0x0B; const uint32_t BPCM     = 0x0B; const uint32_t COP0CR11_BPCM     = 0x0B;
	const uint32_t COP0CR12 = 0x0C; const uint32_t SR       = 0x0C; const uint32_t COP0CR12_SR       = 0x0C;
	const uint32_t COP0CR13 = 0x0D; const uint32_t CAUSE    = 0x0D; const uint32_t COP0CR13_CAUSE    = 0x0D;
	const uint32_t COP0CR14 = 0x0E; const uint32_t EPC      = 0x0E; const uint32_t COP0CR14_EPC      = 0x0E;
	const uint32_t COP0CR15 = 0x0F; const uint32_t PRID     = 0x0F; const uint32_t COP0CR15_PRID     = 0x0F;
	const uint32_t COP0CR16 = 0x10; const uint32_t ERREG    = 0x10; const uint32_t COP0CR16_ERREG    = 0x10;

	// GTE Control Registers
	const uint32_t GTECR00 = 0x00; const uint32_t R11R12 = 0x00; const uint32_t GTECR00_R11R12 = 0x00;
	const uint32_t GTECR01 = 0x01; const uint32_t R13R21 = 0x01; const uint32_t GTECR01_R13R21 = 0x01;
	const uint32_t GTECR02 = 0x02; const uint32_t R22R23 = 0x02; const uint32_t GTECR02_R22R23 = 0x02;
	const uint32_t GTECR03 = 0x03; const uint32_t R31R32 = 0x03; const uint32_t GTECR03_R31R32 = 0x03;
	const uint32_t GTECR04 = 0x04; const uint32_t R33    = 0x04; const uint32_t GTECR04_R33    = 0x04;
	const uint32_t GTECR05 = 0x05; const uint32_t TRX    = 0x05; const uint32_t GTECR05_TRX    = 0x05;
	const uint32_t GTECR06 = 0x06; const uint32_t TRY    = 0x06; const uint32_t GTECR06_TRY    = 0x06;
	const uint32_t GTECR07 = 0x07; const uint32_t TRZ    = 0x07; const uint32_t GTECR07_TRZ    = 0x07;
	const uint32_t GTECR08 = 0x08; const uint32_t L11L12 = 0x08; const uint32_t GTECR08_L11L12 = 0x08;
	const uint32_t GTECR09 = 0x09; const uint32_t L13L21 = 0x09; const uint32_t GTECR09_L13L21 = 0x09;
	const uint32_t GTECR10 = 0x0A; const uint32_t L22L23 = 0x0A; const uint32_t GTECR10_L22L23 = 0x0A;
	const uint32_t GTECR11 = 0x0B; const uint32_t L31L32 = 0x0B; const uint32_t GTECR11_L31L32 = 0x0B;
	const uint32_t GTECR12 = 0x0C; const uint32_t L33    = 0x0C; const uint32_t GTECR12_L33    = 0x0C;
	const uint32_t GTECR13 = 0x0D; const uint32_t RBK    = 0x0D; const uint32_t GTECR13_RBK    = 0x0D;
	const uint32_t GTECR14 = 0x0E; const uint32_t BBK    = 0x0E; const uint32_t GTECR14_BBK    = 0x0E;
	const uint32_t GTECR15 = 0x0F; const uint32_t GBK    = 0x0F; const uint32_t GTECR15_GBK    = 0x0F;
	const uint32_t GTECR16 = 0x10; const uint32_t LR1LR2 = 0x10; const uint32_t GTECR16_LR1LR2 = 0x10;
	const uint32_t GTECR17 = 0x11; const uint32_t LR3LG1 = 0x11; const uint32_t GTECR17_LR3LG1 = 0x11;
	const uint32_t GTECR18 = 0x12; const uint32_t LG2LG3 = 0x12; const uint32_t GTECR18_LG2LG3 = 0x12;
	const uint32_t GTECR19 = 0x13; const uint32_t LB1LB2 = 0x13; const uint32_t GTECR19_LB1LB2 = 0x13;
	const uint32_t GTECR20 = 0x14; const uint32_t LB3    = 0x14; const uint32_t GTECR20_LB3    = 0x14;
	const uint32_t GTECR21 = 0x15; const uint32_t RFC    = 0x15; const uint32_t GTECR21_RFC    = 0x15;
	const uint32_t GTECR22 = 0x16; const uint32_t GFC    = 0x16; const uint32_t GTECR22_GFC    = 0x16;
	const uint32_t GTECR23 = 0x17; const uint32_t BFC    = 0x17; const uint32_t GTECR23_BFC    = 0x17;
	const uint32_t GTECR24 = 0x18; const uint32_t OFX    = 0x18; const uint32_t GTECR24_OFX    = 0x18;
	const uint32_t GTECR25 = 0x19; const uint32_t OFY    = 0x19; const uint32_t GTECR25_OFY    = 0x19;
	const uint32_t GTECR26 = 0x1A; const uint32_t H      = 0x1A; const uint32_t GTECR26_H      = 0x1A;
	const uint32_t GTECR27 = 0x1B; const uint32_t DQA    = 0x1B; const uint32_t GTECR27_DQA    = 0x1B;
	const uint32_t GTECR28 = 0x1C; const uint32_t DQB    = 0x1C; const uint32_t GTECR28_DQB    = 0x1C;
	const uint32_t GTECR29 = 0x1D; const uint32_t ZSF3   = 0x1D; const uint32_t GTECR29_ZSF3   = 0x1D;
	const uint32_t GTECR30 = 0x1E; const uint32_t ZSF4   = 0x1E; const uint32_t GTECR30_ZSF4   = 0x1E;
	const uint32_t GTECR31 = 0x1F; const uint32_t FLAG   = 0x1F; const uint32_t GTECR31_FLAG   = 0x1F;

	// GTE Data Registers
	const uint32_t GTEDR00 = 0x00; const uint32_t VXY0 = 0x00; const uint32_t GTEDR00_VXY0 = 0x00;
	const uint32_t GTEDR01 = 0x01; const uint32_t VZ0  = 0x01; const uint32_t GTEDR01_VZ0  = 0x01;
	const uint32_t GTEDR02 = 0x02; const uint32_t VXY1 = 0x02; const uint32_t GTEDR02_VXY1 = 0x02;
	const uint32_t GTEDR03 = 0x03; const uint32_t VZ1  = 0x03; const uint32_t GTEDR03_VZ1  = 0x03;
	const uint32_t GTEDR04 = 0x04; const uint32_t VXY2 = 0x04; const uint32_t GTEDR04_VXY2 = 0x04;
	const uint32_t GTEDR05 = 0x05; const uint32_t VZ2  = 0x05; const uint32_t GTEDR05_VZ2  = 0x05;
	const uint32_t GTEDR06 = 0x06; const uint32_t RGB  = 0x06; const uint32_t GTEDR06_RGB  = 0x06;
	const uint32_t GTEDR07 = 0x07; const uint32_t OTZ  = 0x07; const uint32_t GTEDR07_OTZ  = 0x07;
	const uint32_t GTEDR08 = 0x08; const uint32_t IR0  = 0x08; const uint32_t GTEDR08_IR0  = 0x08;
	const uint32_t GTEDR09 = 0x09; const uint32_t IR1  = 0x09; const uint32_t GTEDR09_IR1  = 0x09;
	const uint32_t GTEDR10 = 0x0A; const uint32_t IR2  = 0x0A; const uint32_t GTEDR10_IR2  = 0x0A;
	const uint32_t GTEDR11 = 0x0B; const uint32_t IR3  = 0x0B; const uint32_t GTEDR11_IR3  = 0x0B;
	const uint32_t GTEDR12 = 0x0C; const uint32_t SXY0 = 0x0C; const uint32_t GTEDR12_SXY0 = 0x0C;
	const uint32_t GTEDR13 = 0x0D; const uint32_t SXY1 = 0x0D; const uint32_t GTEDR13_SXY1 = 0x0D;
	const uint32_t GTEDR14 = 0x0E; const uint32_t SXY2 = 0x0E; const uint32_t GTEDR14_SXY2 = 0x0E;
	const uint32_t GTEDR15 = 0x0F; const uint32_t SXYP = 0x0F; const uint32_t GTEDR15_SXYP = 0x0F;
	const uint32_t GTEDR16 = 0x10; const uint32_t SZ0  = 0x10; const uint32_t GTEDR16_SZ0  = 0x10;
	const uint32_t GTEDR17 = 0x11; const uint32_t SZ1  = 0x11; const uint32_t GTEDR17_SZ1  = 0x11;
	const uint32_t GTEDR18 = 0x12; const uint32_t SZ2  = 0x12; const uint32_t GTEDR18_SZ2  = 0x12;
	const uint32_t GTEDR19 = 0x13; const uint32_t SZ3  = 0x13; const uint32_t GTEDR19_SZ3  = 0x13;
	const uint32_t GTEDR20 = 0x14; const uint32_t RGB0 = 0x14; const uint32_t GTEDR20_RGB0 = 0x14;
	const uint32_t GTEDR21 = 0x15; const uint32_t RGB1 = 0x15; const uint32_t GTEDR21_RGB1 = 0x15;
	const uint32_t GTEDR22 = 0x16; const uint32_t RGB2 = 0x16; const uint32_t GTEDR22_RGB2 = 0x16;
	const uint32_t GTEDR23 = 0x17; const uint32_t RES1 = 0x17; const uint32_t GTEDR23_RES1 = 0x17;
	const uint32_t GTEDR24 = 0x18; const uint32_t MAC0 = 0x18; const uint32_t GTEDR24_MAC0 = 0x18;
	const uint32_t GTEDR25 = 0x19; const uint32_t MAC1 = 0x19; const uint32_t GTEDR25_MAC1 = 0x19;
	const uint32_t GTEDR26 = 0x1A; const uint32_t MAC2 = 0x1A; const uint32_t GTEDR26_MAC2 = 0x1A;
	const uint32_t GTEDR27 = 0x1B; const uint32_t MAC3 = 0x1B; const uint32_t GTEDR27_MAC3 = 0x1B;
	const uint32_t GTEDR28 = 0x1C; const uint32_t IRGB = 0x1C; const uint32_t GTEDR28_IRGB = 0x1C;
	const uint32_t GTEDR29 = 0x1D; const uint32_t ORGB = 0x1D; const uint32_t GTEDR29_ORGB = 0x1D;
	const uint32_t GTEDR30 = 0x1E; const uint32_t LZCS = 0x1E; const uint32_t GTEDR30_LZCS = 0x1E;
	const uint32_t GTEDR31 = 0x1F; const uint32_t LZCR = 0x1F; const uint32_t GTEDR31_LZCR = 0x1F;

	// Opcode Mnemonics
	const uint32_t CDP     = 0x00; //
	const uint32_t RFE     = 0x01; //
	const uint32_t TLBWI   = 0x02; //
	const uint32_t GPF     = 0x03; //
	const uint32_t CC      = 0x04; //
	const uint32_t RTPT    = 0x05; //
	const uint32_t AVSZ3   = 0x06; //
	const uint32_t TLBR    = 0x07; //
	const uint32_t NCLIP   = 0x08; //
	const uint32_t SQR     = 0x09; //
	const uint32_t RTPS    = 0x0A; //
	const uint32_t DPCT    = 0x0B; //
	const uint32_t NCDS    = 0x0C; //
	const uint32_t DPCS    = 0x0D; //
	const uint32_t MVMVA   = 0x0E; //
	const uint32_t NOP     = 0x0F; //
	const uint32_t NCCS    = 0x10; //
	const uint32_t NCDT    = 0x11; //
	const uint32_t INTPL   = 0x12; //
	const uint32_t NCS     = 0x13; //
	const uint32_t OP      = 0x14; //
	const uint32_t NCT     = 0x15; //
	const uint32_t DPCL    = 0x16; //
	const uint32_t GPL     = 0x17; //
	const uint32_t TLBWR   = 0x18; //
	const uint32_t AVSZ4   = 0x19; //
	const uint32_t NCCT    = 0x1A; //
	const uint32_t MFC0    = 0x1B; // CPU_RT, COP0RD
	const uint32_t MTC0    = 0x1C; // CPU_RT, COP0RD
	const uint32_t CTC2    = 0x1D; // CPU_RT, GTECRD
	const uint32_t CFC2    = 0x1E; // CPU_RT, GTECRD
	const uint32_t MFC2    = 0x1F; // CPU_RT, GTEDRD
	const uint32_t MTC2    = 0x20; // CPU_RT, GTEDRD
	const uint32_t MFLO    = 0x21; // CPU_RD
	const uint32_t MFHI    = 0x22; // CPU_RD
	const uint32_t NOR     = 0x23; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t ADD     = 0x24; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t ADDU    = 0x25; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t SLTU    = 0x26; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t XOR     = 0x27; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t OR      = 0x28; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t SLT     = 0x29; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t SUB     = 0x2A; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t AND     = 0x2B; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t SUBU    = 0x2C; // CPU_RD, CPU_RS, CPU_RT
	const uint32_t SLLV    = 0x2D; // CPU_RD, CPU_RT, CPU_RS
	const uint32_t SRLV    = 0x2E; // CPU_RD, CPU_RT, CPU_RS
	const uint32_t SRAV    = 0x2F; // CPU_RD, CPU_RT, CPU_RS
	const uint32_t SRL     = 0x30; // CPU_RD, CPU_RT, SA
	const uint32_t SLL     = 0x31; // CPU_RD, CPU_RT, SA
	const uint32_t SRA     = 0x32; // CPU_RD, CPU_RT, SA
	const uint32_t MTLO    = 0x33; // CPU_RS
	const uint32_t MTHI    = 0x34; // CPU_RS
	const uint32_t JR      = 0x35; // CPU_RS
	const uint32_t JALR    = 0x36; // CPU_RS, CPU_RD
	const uint32_t MULTU   = 0x37; // CPU_RS, CPU_RT
	const uint32_t MULT    = 0x38; // CPU_RS, CPU_RT
	const uint32_t DIV     = 0x39; // CPU_RS, CPU_RT
	const uint32_t DIVU    = 0x3A; // CPU_RS, CPU_RT
	const uint32_t BNE     = 0x3B; // CPU_RS, CPU_RT, LABEL
	const uint32_t BEQ     = 0x3C; // CPU_RS, CPU_RT, LABEL
	const uint32_t BGEZ    = 0x3D; // CPU_RS, LABEL
	const uint32_t BLEZ    = 0x3E; // CPU_RS, LABEL
	const uint32_t BLTZ    = 0x3F; // CPU_RS, LABEL
	const uint32_t BGEZAL  = 0x40; // CPU_RS, LABEL
	const uint32_t BLTZAL  = 0x41; // CPU_RS, LABEL
	const uint32_t BGTZ    = 0x42; // CPU_RS, LABEL
	const uint32_t SW      = 0x43; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t SB      = 0x44; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LWL     = 0x45; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t SWR     = 0x46; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LB      = 0x47; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t SWL     = 0x48; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LHU     = 0x49; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LH      = 0x4A; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t SH      = 0x4B; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LWR     = 0x4C; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LBU     = 0x4D; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LWC1    = 0x4E; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t LW      = 0x4F; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t SWC1    = 0x50; // CPU_RT, NUMBER(CPU_RS)
	const uint32_t ANDI    = 0x51; // CPU_RT, CPU_RS, NUMBER
	const uint32_t ADDIU   = 0x52; // CPU_RT, CPU_RS, NUMBER
	const uint32_t XORI    = 0x53; // CPU_RT, CPU_RS, NUMBER
	const uint32_t ORI     = 0x54; // CPU_RT, CPU_RS, NUMBER
	const uint32_t SLTIU   = 0x55; // CPU_RT, CPU_RS, NUMBER
	const uint32_t SLTI    = 0x56; // CPU_RT, CPU_RS, NUMBER
	const uint32_t ADDI    = 0x57; // CPU_RT, CPU_RS, NUMBER
	const uint32_t LUI     = 0x58; // CPU_RT, NUMBER
	const uint32_t SWC2    = 0x59; // GTEDRT, NUMBER(CPU_RS)
	const uint32_t LWC2    = 0x5A; // GTEDRT, NUMBER(CPU_RS)
	const uint32_t SYSCALL = 0x5B; // SYSCALL
	const uint32_t BREAK   = 0x5C; // SYSCALL
	const uint32_t JAL     = 0x5D; // TARGET
	const uint32_t J       = 0x5E; // TARGET

	// Directives
	const uint32_t DIRECTIVE_BYTE = 0x00;
	const uint32_t DIRECTIVE_HALF = 0x01;
	const uint32_t DIRECTIVE_WORD = 0x02;

	// Functions
	uint32_t Parser(char *FilePath);

#endif
