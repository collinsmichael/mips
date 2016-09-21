#ifndef PARSER_H
#define PARSER_H
#include "Symbol.h"

	// CPU General Purpose Registers
	const uint32 R0   = 0x00; const uint32 ZERO = 0x00;
	const uint32 R1   = 0x01; const uint32 AT   = 0x01;
	const uint32 R2   = 0x02; const uint32 V0   = 0x02;
	const uint32 R3   = 0x03; const uint32 V1   = 0x03;
	const uint32 R4   = 0x04; const uint32 A0   = 0x04;
	const uint32 R5   = 0x05; const uint32 A1   = 0x05;
	const uint32 R6   = 0x06; const uint32 A2   = 0x06;
	const uint32 R7   = 0x07; const uint32 A3   = 0x07;
	const uint32 R8   = 0x08; const uint32 T0   = 0x08;
	const uint32 R9   = 0x09; const uint32 T1   = 0x09;
	const uint32 R10  = 0x0A; const uint32 T2   = 0x0A;
	const uint32 R11  = 0x0B; const uint32 T3   = 0x0B;
	const uint32 R12  = 0x0C; const uint32 T4   = 0x0C;
	const uint32 R13  = 0x0D; const uint32 T5   = 0x0D;
	const uint32 R14  = 0x0E; const uint32 T6   = 0x0E;
	const uint32 R15  = 0x0F; const uint32 T7   = 0x0F;
	const uint32 R16  = 0x10; const uint32 S0   = 0x10;
	const uint32 R17  = 0x11; const uint32 S1   = 0x11;
	const uint32 R18  = 0x12; const uint32 S2   = 0x12;
	const uint32 R19  = 0x13; const uint32 S3   = 0x13;
	const uint32 R20  = 0x14; const uint32 S4   = 0x14;
	const uint32 R21  = 0x15; const uint32 S5   = 0x15;
	const uint32 R22  = 0x16; const uint32 S6   = 0x16;
	const uint32 R23  = 0x17; const uint32 S7   = 0x17;
	const uint32 R24  = 0x18; const uint32 T8   = 0x18;
	const uint32 R25  = 0x19; const uint32 T9   = 0x19;
	const uint32 R26  = 0x1A; const uint32 K0   = 0x1A;
	const uint32 R27  = 0x1B; const uint32 K1   = 0x1B;
	const uint32 R28  = 0x1C; const uint32 GP   = 0x1C;
	const uint32 R29  = 0x1D; const uint32 SP   = 0x1D;
	const uint32 R30  = 0x1E; const uint32 S8   = 0x1E;
	const uint32 R31  = 0x1F; const uint32 RA   = 0x1F;

	// GPU Control Registers
	const uint32 COP0CR00 = 0x00; const uint32 INDEX    = 0x00; const uint32 COP0CR00_INDEX    = 0x00;
	const uint32 COP0CR01 = 0x01; const uint32 RANDOM   = 0x01; const uint32 COP0CR01_RANDOM   = 0x01;
	const uint32 COP0CR02 = 0x02; const uint32 TLBLO    = 0x02; const uint32 COP0CR02_TLBLO    = 0x02;
	const uint32 COP0CR03 = 0x03; const uint32 BPC      = 0x03; const uint32 COP0CR03_BPC      = 0x03;
	const uint32 COP0CR04 = 0x04; const uint32 CTXT     = 0x04; const uint32 COP0CR04_CTXT     = 0x04;
	const uint32 COP0CR05 = 0x05; const uint32 BDA      = 0x05; const uint32 COP0CR05_BDA      = 0x05;
	const uint32 COP0CR06 = 0x06; const uint32 PIDMASK  = 0x06; const uint32 COP0CR06_PIDMASK  = 0x06;
	const uint32 COP0CR07 = 0x07; const uint32 DCIC     = 0x07; const uint32 COP0CR07_DCIC     = 0x07;
	const uint32 COP0CR08 = 0x08; const uint32 BADVADDR = 0x08; const uint32 COP0CR08_BADVADDR = 0x08;
	const uint32 COP0CR09 = 0x09; const uint32 BDAM     = 0x09; const uint32 COP0CR09_BDAM     = 0x09;
	const uint32 COP0CR10 = 0x0A; const uint32 TLBHI    = 0x0A; const uint32 COP0CR10_TLBHI    = 0x0A;
	const uint32 COP0CR11 = 0x0B; const uint32 BPCM     = 0x0B; const uint32 COP0CR11_BPCM     = 0x0B;
	const uint32 COP0CR12 = 0x0C; const uint32 SR       = 0x0C; const uint32 COP0CR12_SR       = 0x0C;
	const uint32 COP0CR13 = 0x0D; const uint32 CAUSE    = 0x0D; const uint32 COP0CR13_CAUSE    = 0x0D;
	const uint32 COP0CR14 = 0x0E; const uint32 EPC      = 0x0E; const uint32 COP0CR14_EPC      = 0x0E;
	const uint32 COP0CR15 = 0x0F; const uint32 PRID     = 0x0F; const uint32 COP0CR15_PRID     = 0x0F;
	const uint32 COP0CR16 = 0x10; const uint32 ERREG    = 0x10; const uint32 COP0CR16_ERREG    = 0x10;

	// GTE Control Registers
	const uint32 GTECR00 = 0x00; const uint32 R11R12 = 0x00; const uint32 GTECR00_R11R12 = 0x00;
	const uint32 GTECR01 = 0x01; const uint32 R13R21 = 0x01; const uint32 GTECR01_R13R21 = 0x01;
	const uint32 GTECR02 = 0x02; const uint32 R22R23 = 0x02; const uint32 GTECR02_R22R23 = 0x02;
	const uint32 GTECR03 = 0x03; const uint32 R31R32 = 0x03; const uint32 GTECR03_R31R32 = 0x03;
	const uint32 GTECR04 = 0x04; const uint32 R33    = 0x04; const uint32 GTECR04_R33    = 0x04;
	const uint32 GTECR05 = 0x05; const uint32 TRX    = 0x05; const uint32 GTECR05_TRX    = 0x05;
	const uint32 GTECR06 = 0x06; const uint32 TRY    = 0x06; const uint32 GTECR06_TRY    = 0x06;
	const uint32 GTECR07 = 0x07; const uint32 TRZ    = 0x07; const uint32 GTECR07_TRZ    = 0x07;
	const uint32 GTECR08 = 0x08; const uint32 L11L12 = 0x08; const uint32 GTECR08_L11L12 = 0x08;
	const uint32 GTECR09 = 0x09; const uint32 L13L21 = 0x09; const uint32 GTECR09_L13L21 = 0x09;
	const uint32 GTECR10 = 0x0A; const uint32 L22L23 = 0x0A; const uint32 GTECR10_L22L23 = 0x0A;
	const uint32 GTECR11 = 0x0B; const uint32 L31L32 = 0x0B; const uint32 GTECR11_L31L32 = 0x0B;
	const uint32 GTECR12 = 0x0C; const uint32 L33    = 0x0C; const uint32 GTECR12_L33    = 0x0C;
	const uint32 GTECR13 = 0x0D; const uint32 RBK    = 0x0D; const uint32 GTECR13_RBK    = 0x0D;
	const uint32 GTECR14 = 0x0E; const uint32 BBK    = 0x0E; const uint32 GTECR14_BBK    = 0x0E;
	const uint32 GTECR15 = 0x0F; const uint32 GBK    = 0x0F; const uint32 GTECR15_GBK    = 0x0F;
	const uint32 GTECR16 = 0x10; const uint32 LR1LR2 = 0x10; const uint32 GTECR16_LR1LR2 = 0x10;
	const uint32 GTECR17 = 0x11; const uint32 LR3LG1 = 0x11; const uint32 GTECR17_LR3LG1 = 0x11;
	const uint32 GTECR18 = 0x12; const uint32 LG2LG3 = 0x12; const uint32 GTECR18_LG2LG3 = 0x12;
	const uint32 GTECR19 = 0x13; const uint32 LB1LB2 = 0x13; const uint32 GTECR19_LB1LB2 = 0x13;
	const uint32 GTECR20 = 0x14; const uint32 LB3    = 0x14; const uint32 GTECR20_LB3    = 0x14;
	const uint32 GTECR21 = 0x15; const uint32 RFC    = 0x15; const uint32 GTECR21_RFC    = 0x15;
	const uint32 GTECR22 = 0x16; const uint32 GFC    = 0x16; const uint32 GTECR22_GFC    = 0x16;
	const uint32 GTECR23 = 0x17; const uint32 BFC    = 0x17; const uint32 GTECR23_BFC    = 0x17;
	const uint32 GTECR24 = 0x18; const uint32 OFX    = 0x18; const uint32 GTECR24_OFX    = 0x18;
	const uint32 GTECR25 = 0x19; const uint32 OFY    = 0x19; const uint32 GTECR25_OFY    = 0x19;
	const uint32 GTECR26 = 0x1A; const uint32 H      = 0x1A; const uint32 GTECR26_H      = 0x1A;
	const uint32 GTECR27 = 0x1B; const uint32 DQA    = 0x1B; const uint32 GTECR27_DQA    = 0x1B;
	const uint32 GTECR28 = 0x1C; const uint32 DQB    = 0x1C; const uint32 GTECR28_DQB    = 0x1C;
	const uint32 GTECR29 = 0x1D; const uint32 ZSF3   = 0x1D; const uint32 GTECR29_ZSF3   = 0x1D;
	const uint32 GTECR30 = 0x1E; const uint32 ZSF4   = 0x1E; const uint32 GTECR30_ZSF4   = 0x1E;
	const uint32 GTECR31 = 0x1F; const uint32 FLAG   = 0x1F; const uint32 GTECR31_FLAG   = 0x1F;

	// GTE Data Registers
	const uint32 GTEDR00 = 0x00; const uint32 VXY0 = 0x00; const uint32 GTEDR00_VXY0 = 0x00;
	const uint32 GTEDR01 = 0x01; const uint32 VZ0  = 0x01; const uint32 GTEDR01_VZ0  = 0x01;
	const uint32 GTEDR02 = 0x02; const uint32 VXY1 = 0x02; const uint32 GTEDR02_VXY1 = 0x02;
	const uint32 GTEDR03 = 0x03; const uint32 VZ1  = 0x03; const uint32 GTEDR03_VZ1  = 0x03;
	const uint32 GTEDR04 = 0x04; const uint32 VXY2 = 0x04; const uint32 GTEDR04_VXY2 = 0x04;
	const uint32 GTEDR05 = 0x05; const uint32 VZ2  = 0x05; const uint32 GTEDR05_VZ2  = 0x05;
	const uint32 GTEDR06 = 0x06; const uint32 RGB  = 0x06; const uint32 GTEDR06_RGB  = 0x06;
	const uint32 GTEDR07 = 0x07; const uint32 OTZ  = 0x07; const uint32 GTEDR07_OTZ  = 0x07;
	const uint32 GTEDR08 = 0x08; const uint32 IR0  = 0x08; const uint32 GTEDR08_IR0  = 0x08;
	const uint32 GTEDR09 = 0x09; const uint32 IR1  = 0x09; const uint32 GTEDR09_IR1  = 0x09;
	const uint32 GTEDR10 = 0x0A; const uint32 IR2  = 0x0A; const uint32 GTEDR10_IR2  = 0x0A;
	const uint32 GTEDR11 = 0x0B; const uint32 IR3  = 0x0B; const uint32 GTEDR11_IR3  = 0x0B;
	const uint32 GTEDR12 = 0x0C; const uint32 SXY0 = 0x0C; const uint32 GTEDR12_SXY0 = 0x0C;
	const uint32 GTEDR13 = 0x0D; const uint32 SXY1 = 0x0D; const uint32 GTEDR13_SXY1 = 0x0D;
	const uint32 GTEDR14 = 0x0E; const uint32 SXY2 = 0x0E; const uint32 GTEDR14_SXY2 = 0x0E;
	const uint32 GTEDR15 = 0x0F; const uint32 SXYP = 0x0F; const uint32 GTEDR15_SXYP = 0x0F;
	const uint32 GTEDR16 = 0x10; const uint32 SZ0  = 0x10; const uint32 GTEDR16_SZ0  = 0x10;
	const uint32 GTEDR17 = 0x11; const uint32 SZ1  = 0x11; const uint32 GTEDR17_SZ1  = 0x11;
	const uint32 GTEDR18 = 0x12; const uint32 SZ2  = 0x12; const uint32 GTEDR18_SZ2  = 0x12;
	const uint32 GTEDR19 = 0x13; const uint32 SZ3  = 0x13; const uint32 GTEDR19_SZ3  = 0x13;
	const uint32 GTEDR20 = 0x14; const uint32 RGB0 = 0x14; const uint32 GTEDR20_RGB0 = 0x14;
	const uint32 GTEDR21 = 0x15; const uint32 RGB1 = 0x15; const uint32 GTEDR21_RGB1 = 0x15;
	const uint32 GTEDR22 = 0x16; const uint32 RGB2 = 0x16; const uint32 GTEDR22_RGB2 = 0x16;
	const uint32 GTEDR23 = 0x17; const uint32 RES1 = 0x17; const uint32 GTEDR23_RES1 = 0x17;
	const uint32 GTEDR24 = 0x18; const uint32 MAC0 = 0x18; const uint32 GTEDR24_MAC0 = 0x18;
	const uint32 GTEDR25 = 0x19; const uint32 MAC1 = 0x19; const uint32 GTEDR25_MAC1 = 0x19;
	const uint32 GTEDR26 = 0x1A; const uint32 MAC2 = 0x1A; const uint32 GTEDR26_MAC2 = 0x1A;
	const uint32 GTEDR27 = 0x1B; const uint32 MAC3 = 0x1B; const uint32 GTEDR27_MAC3 = 0x1B;
	const uint32 GTEDR28 = 0x1C; const uint32 IRGB = 0x1C; const uint32 GTEDR28_IRGB = 0x1C;
	const uint32 GTEDR29 = 0x1D; const uint32 ORGB = 0x1D; const uint32 GTEDR29_ORGB = 0x1D;
	const uint32 GTEDR30 = 0x1E; const uint32 LZCS = 0x1E; const uint32 GTEDR30_LZCS = 0x1E;
	const uint32 GTEDR31 = 0x1F; const uint32 LZCR = 0x1F; const uint32 GTEDR31_LZCR = 0x1F;

	// Opcode Mnemonics
	const uint32 CDP     = 0x00; //
	const uint32 RFE     = 0x01; //
	const uint32 TLBWI   = 0x02; //
	const uint32 GPF     = 0x03; //
	const uint32 CC      = 0x04; //
	const uint32 RTPT    = 0x05; //
	const uint32 AVSZ3   = 0x06; //
	const uint32 TLBR    = 0x07; //
	const uint32 NCLIP   = 0x08; //
	const uint32 SQR     = 0x09; //
	const uint32 RTPS    = 0x0A; //
	const uint32 DPCT    = 0x0B; //
	const uint32 NCDS    = 0x0C; //
	const uint32 DPCS    = 0x0D; //
	const uint32 MVMVA   = 0x0E; //
	const uint32 NOP     = 0x0F; //
	const uint32 NCCS    = 0x10; //
	const uint32 NCDT    = 0x11; //
	const uint32 INTPL   = 0x12; //
	const uint32 NCS     = 0x13; //
	const uint32 OP      = 0x14; //
	const uint32 NCT     = 0x15; //
	const uint32 DPCL    = 0x16; //
	const uint32 GPL     = 0x17; //
	const uint32 TLBWR   = 0x18; //
	const uint32 AVSZ4   = 0x19; //
	const uint32 NCCT    = 0x1A; //
	const uint32 MFC0    = 0x1B; // CPU_RT, COP0RD
	const uint32 MTC0    = 0x1C; // CPU_RT, COP0RD
	const uint32 CTC2    = 0x1D; // CPU_RT, GTECRD
	const uint32 CFC2    = 0x1E; // CPU_RT, GTECRD
	const uint32 MFC2    = 0x1F; // CPU_RT, GTEDRD
	const uint32 MTC2    = 0x20; // CPU_RT, GTEDRD
	const uint32 MFLO    = 0x21; // CPU_RD
	const uint32 MFHI    = 0x22; // CPU_RD
	const uint32 NOR     = 0x23; // CPU_RD, CPU_RS, CPU_RT
	const uint32 ADD     = 0x24; // CPU_RD, CPU_RS, CPU_RT
	const uint32 ADDU    = 0x25; // CPU_RD, CPU_RS, CPU_RT
	const uint32 SLTU    = 0x26; // CPU_RD, CPU_RS, CPU_RT
	const uint32 XOR     = 0x27; // CPU_RD, CPU_RS, CPU_RT
	const uint32 OR      = 0x28; // CPU_RD, CPU_RS, CPU_RT
	const uint32 SLT     = 0x29; // CPU_RD, CPU_RS, CPU_RT
	const uint32 SUB     = 0x2A; // CPU_RD, CPU_RS, CPU_RT
	const uint32 AND     = 0x2B; // CPU_RD, CPU_RS, CPU_RT
	const uint32 SUBU    = 0x2C; // CPU_RD, CPU_RS, CPU_RT
	const uint32 SLLV    = 0x2D; // CPU_RD, CPU_RT, CPU_RS
	const uint32 SRLV    = 0x2E; // CPU_RD, CPU_RT, CPU_RS
	const uint32 SRAV    = 0x2F; // CPU_RD, CPU_RT, CPU_RS
	const uint32 SRL     = 0x30; // CPU_RD, CPU_RT, SA
	const uint32 SLL     = 0x31; // CPU_RD, CPU_RT, SA
	const uint32 SRA     = 0x32; // CPU_RD, CPU_RT, SA
	const uint32 MTLO    = 0x33; // CPU_RS
	const uint32 MTHI    = 0x34; // CPU_RS
	const uint32 JR      = 0x35; // CPU_RS
	const uint32 JALR    = 0x36; // CPU_RS, CPU_RD
	const uint32 MULTU   = 0x37; // CPU_RS, CPU_RT
	const uint32 MULT    = 0x38; // CPU_RS, CPU_RT
	const uint32 DIV     = 0x39; // CPU_RS, CPU_RT
	const uint32 DIVU    = 0x3A; // CPU_RS, CPU_RT
	const uint32 BNE     = 0x3B; // CPU_RS, CPU_RT, LABEL
	const uint32 BEQ     = 0x3C; // CPU_RS, CPU_RT, LABEL
	const uint32 BGEZ    = 0x3D; // CPU_RS, LABEL
	const uint32 BLEZ    = 0x3E; // CPU_RS, LABEL
	const uint32 BLTZ    = 0x3F; // CPU_RS, LABEL
	const uint32 BGEZAL  = 0x40; // CPU_RS, LABEL
	const uint32 BLTZAL  = 0x41; // CPU_RS, LABEL
	const uint32 BGTZ    = 0x42; // CPU_RS, LABEL
	const uint32 SW      = 0x43; // CPU_RT, NUMBER(CPU_RS)
	const uint32 SB      = 0x44; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LWL     = 0x45; // CPU_RT, NUMBER(CPU_RS)
	const uint32 SWR     = 0x46; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LB      = 0x47; // CPU_RT, NUMBER(CPU_RS)
	const uint32 SWL     = 0x48; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LHU     = 0x49; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LH      = 0x4A; // CPU_RT, NUMBER(CPU_RS)
	const uint32 SH      = 0x4B; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LWR     = 0x4C; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LBU     = 0x4D; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LWC1    = 0x4E; // CPU_RT, NUMBER(CPU_RS)
	const uint32 LW      = 0x4F; // CPU_RT, NUMBER(CPU_RS)
	const uint32 SWC1    = 0x50; // CPU_RT, NUMBER(CPU_RS)
	const uint32 ANDI    = 0x51; // CPU_RT, CPU_RS, NUMBER
	const uint32 ADDIU   = 0x52; // CPU_RT, CPU_RS, NUMBER
	const uint32 XORI    = 0x53; // CPU_RT, CPU_RS, NUMBER
	const uint32 ORI     = 0x54; // CPU_RT, CPU_RS, NUMBER
	const uint32 SLTIU   = 0x55; // CPU_RT, CPU_RS, NUMBER
	const uint32 SLTI    = 0x56; // CPU_RT, CPU_RS, NUMBER
	const uint32 ADDI    = 0x57; // CPU_RT, CPU_RS, NUMBER
	const uint32 LUI     = 0x58; // CPU_RT, NUMBER
	const uint32 SWC2    = 0x59; // GTEDRT, NUMBER(CPU_RS)
	const uint32 LWC2    = 0x5A; // GTEDRT, NUMBER(CPU_RS)
	const uint32 SYSCALL = 0x5B; // SYSCALL
	const uint32 BREAK   = 0x5C; // SYSCALL
	const uint32 JAL     = 0x5D; // TARGET
	const uint32 J       = 0x5E; // TARGET

	// Directives
	const uint32 DIRECTIVE_BYTE = 0x00;
	const uint32 DIRECTIVE_HALF = 0x01;
	const uint32 DIRECTIVE_WORD = 0x02;

	// Functions
	uint32 Parser(char *FilePath);

#endif
