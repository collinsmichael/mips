//
// File Name : Dasm.cpp
// File Date : 15 March 2012
// Author(s) : Valendian
// Purpose   : Disassembles Binary Binary.File to MIPS assembler listing.
//
// Pass 1: Identify file format.
// Pass 2: Determine instruction and assign internal id.
// Pass 3: Identify jumps and mark targets as labels.
// Pass 4: Identify memory access and mark targets with appropriate data types.
// Pass 5: Identify jump tables and mark destinations as labels.
// Pass 6: Print output to file.
//

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Main.h"
#include "Dasm.h"

extern int pass4(void);

#define Reg(registerfile, argument) (registerfile[argument*4 + Naming])

extern bool volatile ShowAddress;
extern bool volatile ShowOpcode;
extern bool volatile ShowLabel;
extern bool volatile ShowMnemonic;
extern bool volatile ShowAscii;

// =========================
// options and user settings
// =========================
uint32_t Address = 0;
uint32_t Entry   = 0;
uint32_t Naming  = 3;
char  *Type;

// ==================
// Binary File loader
// ==================
//uint8_t   *flag;
uint32_t    *Code;
uint32_t    *Flag;
uint32_t     Pointer;
binary     Binary;
char       Text[0x100];
char       Number[0x100];
PsxExeHdr *Hdr;
uint32_t     LenHTML;
bool       JrReg;

// ===========================
// instruction decoding feilds
// ===========================
uint32_t Pc;      // program counter register.
uint32_t Opcode;  // current instruction.
uint32_t Op;      // FC000000  6 bits (bit 0x1A to bit 0x1F)
uint32_t Rs;      // 03E00000  5 bits (bit 0x15 to bit 0x19)
uint32_t Format;  // 03E00000  5 bits (bit 0x15 to bit 0x19)
uint32_t Rt;      // 001F0000  5 bits (bit 0x10 to bit 0x14)
uint32_t Ft;      // 001F0000  5 bits (bit 0x10 to bit 0x14)
uint32_t Rd;      // 0000F800  5 bits (bit 0x0B to bit 0x0F)
uint32_t Fs;      // 0000F800  5 bits (bit 0x0B to bit 0x0F)
uint32_t Sa;      // 000007C0  5 bits (bit 0x06 to bit 0x0A)
uint32_t Fd;      // 000007C0  5 bits (bit 0x06 to bit 0x0A)
uint32_t Func;    // 0000003F  6 bits (bit 0x00 to bit 0x05)
uint32_t Target;  // 03FFFFFF 26 bits (bit 0x00 to bit 0x19) added to Pc
uint32_t Imm;     // 0000FFFF 16 bits (bit 0x00 to bit 0x0F)
uint32_t Label;   // 0000FFFF 16 bits (bit 0x00 to bit 0x0F) sign extended and added to Pc
uint32_t SysCall; // 03FFFFC0 20 bits (bit 0x06 to bit 0x1A)
uint32_t Fop;     // 03E0003F concatenation of Func and Format feilds

// =============
// register File
// =============
const char *cpu[32*4] = {
//   both       name  numeric  pSX
	"r0_zero", "zero", "r0",  "r0",  // constant 0.
	"r1_at",   "at",   "r1",  "r1",  // assembler temporary.
	"r2_v0",   "v0",   "r2",  "r2",  // return value.
	"r3_v1",   "v1",   "r3",  "r3",  // return value.
	"r4_a0",   "a0",   "r4",  "r4",  // function argument.
	"r5_a1",   "a1",   "r5",  "r5",  // function argument.
	"r6_a2",   "a2",   "r6",  "r6",  // function argument.
	"r7_a3",   "a3",   "r7",  "r7",  // function argument.
	"r8_t0",   "t0",   "r8",  "r8",  // temporary.
	"r9_t1",   "t1",   "r9",  "r9",  // temporary.
	"r10_t2",  "t2",   "r10", "r10", // temporary.
	"r11_t3",  "t3",   "r11", "r11", // temporary.
	"r12_t4",  "t4",   "r12", "r12", // temporary.
	"r13_t5",  "t5",   "r13", "r13", // temporary.
	"r14_t6",  "t6",   "r14", "r14", // temporary.
	"r15_t7",  "t7",   "r15", "r15", // temporary.
	"r16_s0",  "s0",   "r16", "r16", // saved temporary.
	"r17_s1",  "s1",   "r17", "r17", // saved temporary.
	"r18_s2",  "s2",   "r18", "r18", // saved temporary.
	"r19_s3",  "s3",   "r19", "r19", // saved temporary.
	"r20_s4",  "s4",   "r20", "r20", // saved temporary.
	"r21_s5",  "s5",   "r21", "r21", // saved temporary.
	"r22_s6",  "s6",   "r22", "r22", // saved temporary.
	"r23_s7",  "s7",   "r23", "r23", // saved temporary.
	"r24_t8",  "t8",   "r24", "r24", // temporary.
	"r25_t9",  "t9",   "r25", "r25", // temporary.
	"r26_k0",  "k0",   "r26", "r26", // reserved for kernel.
	"r27_k1",  "k1",   "r27", "r27", // reserved for kernel.
	"r28_gp",  "gp",   "r28", "r28", // global Pointer.
	"r29_sp",  "sp",   "r29", "r29", // stack Pointer.
	"r30_s8",  "s8",   "r30", "r30", // frame Pointer.
	"r31_ra",  "ra",   "r31", "r31"  // return Address.
};

const char *cop0[32*4] = {
//   both                 name        numeric     pSX
	"cop0cr00_index",    "index",    "cop0cr00", "cop0cr00_index",    //
	"cop0cr01_random",   "random",   "cop0cr01", "cop0cr01_random",   //
	"cop0cr02_tlblo",    "tlblo",    "cop0cr02", "cop0cr02_tlblo",    //
	"cop0cr03_bpc",      "bpc",      "cop0cr03", "cop0cr03_bpc",      //
	"cop0cr04_ctxt",     "ctxt",     "cop0cr04", "cop0cr04_ctxt",     //
	"cop0cr05_bda",      "bda",      "cop0cr05", "cop0cr05_bda",      //
	"cop0cr06_pidmask",  "pidmask",  "cop0cr06", "cop0cr06_pidmask",  //
	"cop0cr07_dcic",     "dcic",     "cop0cr07", "cop0cr07_dcic",     //
	"cop0cr08_badvaddr", "badvaddr", "cop0cr08", "cop0cr08_badvaddr", //
	"cop0cr09_bdam",     "bdam",     "cop0cr09", "cop0cr09_bdam",     //
	"cop0cr10_tlbhi",    "tlbhi",    "cop0cr10", "cop0cr10_tlbhi",    //
	"cop0cr11_bpcm",     "bpcm",     "cop0cr11", "cop0cr11_bpcm",     //
	"cop0cr12_sr",       "sr",       "cop0cr12", "cop0cr12_sr",       //
	"cop0cr13_cause",    "cause",    "cop0cr13", "cop0cr13_cause",    //
	"cop0cr14_epc",      "epc",      "cop0cr14", "cop0cr14_epc",      //
	"cop0cr15_prid",     "prid",     "cop0cr15", "cop0cr15_prid",     //
	"cop0cr16_erreg",    "erreg",    "cop0cr16", "cop0cr16_erreg",    //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid",           //
	"invalid",           "invalid",  "invalid",  "invalid"            //
};

const char *gtec[32*4] = {
//   both              name      numeric    pSX
	"gtecr00_r11r12", "r11r12", "gtecr00", "gtecr00_r11r12", //
	"gtecr01_r13r21", "r13r21", "gtecr01", "gtecr01_r13r21", //
	"gtecr02_r22r23", "r22r23", "gtecr02", "gtecr02_r22r23", //
	"gtecr03_r31r32", "r31r32", "gtecr03", "gtecr03_r31r32", //
	"gtecr04_r33",    "r33",    "gtecr04", "gtecr04_r33",    //
	"gtecr05_trx",    "trx",    "gtecr05", "gtecr05_trx",    //
	"gtecr06_try",    "try",    "gtecr06", "gtecr06_try",    //
	"gtecr07_trz",    "trz",    "gtecr07", "gtecr07_trz",    //
	"gtecr08_l11l12", "l11l12", "gtecr08", "gtecr08_l11l12", //
	"gtecr09_l13l21", "l13l21", "gtecr09", "gtecr09_l13l21", //
	"gtecr10_l22l23", "l22l23", "gtecr10", "gtecr10_l22l23", //
	"gtecr11_l31l32", "l31l32", "gtecr11", "gtecr11_l31l32", //
	"gtecr12_l33",    "l33",    "gtecr12", "gtecr12_l33",    //
	"gtecr13_rbk",    "rbk",    "gtecr13", "gtecr13_rbk",    //
	"gtecr14_bbk",    "bbk",    "gtecr14", "gtecr14_bbk",    //
	"gtecr15_gbk",    "gbk",    "gtecr15", "gtecr15_gbk",    //
	"gtecr16_lr1lr2", "lr1lr2", "gtecr16", "gtecr16_lr1lr2", //
	"gtecr17_lr3lg1", "lr3lg1", "gtecr17", "gtecr17_lr3lg1", //
	"gtecr18_lg2lg3", "lg2lg3", "gtecr18", "gtecr18_lg2lg3", //
	"gtecr19_lb1lb2", "lb1lb2", "gtecr19", "gtecr19_lb1lb2", //
	"gtecr20_lb3",    "lb3",    "gtecr20", "gtecr20_lb3",    //
	"gtecr21_rfc",    "rfc",    "gtecr21", "gtecr21_rfc",    //
	"gtecr22_gfc",    "gfc",    "gtecr22", "gtecr22_gfc",    //
	"gtecr23_bfc",    "bfc",    "gtecr23", "gtecr23_bfc",    //
	"gtecr24_ofx",    "ofx",    "gtecr24", "gtecr24_ofx",    //
	"gtecr25_ofy",    "ofy",    "gtecr25", "gtecr25_ofy",    //
	"gtecr26_h",      "h",      "gtecr26", "gtecr26_h",      //
	"gtecr27_dqa",    "dqa",    "gtecr27", "gtecr27_dqa",    //
	"gtecr28_dqb",    "dqb",    "gtecr28", "gtecr28_dqb",    //
	"gtecr29_zsf3",   "zsf3",   "gtecr29", "gtecr29_zsf3",   //
	"gtecr30_zsf4",   "zsf4",   "gtecr30", "gtecr30_zsf4",   //
	"gtecr31_flag",   "flag",   "gtecr31", "gtecr31_flag"    //
};

const char *gted[32*4] = {
//   both            name    numeric    pSX
	"gtedr00_vxy0", "vxy0", "gtedr00", "gtedr00_vxy0", //
	"gtedr01_vz0",  "vz0",  "gtedr01", "gtedr01_vz0",  //
	"gtedr02_vxy1", "vxy1", "gtedr02", "gtedr02_vxy1", //
	"gtedr03_vz1",  "vz1",  "gtedr03", "gtedr03_vz1",  //
	"gtedr04_vxy2", "vxy2", "gtedr04", "gtedr04_vxy2", //
	"gtedr05_vz2",  "vz2",  "gtedr05", "gtedr05_vz2",  //
	"gtedr06_rgb",  "rgb",  "gtedr06", "gtedr06_rgb",  //
	"gtedr07_otz",  "otz",  "gtedr07", "gtedr07_otz",  //
	"gtedr08_ir0",  "ir0",  "gtedr08", "gtedr08_ir0",  //
	"gtedr09_ir1",  "ir1",  "gtedr09", "gtedr09_ir1",  //
	"gtedr10_ir2",  "ir2",  "gtedr10", "gtedr10_ir2",  //
	"gtedr11_ir3",  "ir3",  "gtedr11", "gtedr11_ir3",  //
	"gtedr12_sxy0", "sxy0", "gtedr12", "gtedr12_sxy0", //
	"gtedr13_sxy1", "sxy1", "gtedr13", "gtedr13_sxy1", //
	"gtedr14_sxy2", "sxy2", "gtedr14", "gtedr14_sxy2", //
	"gtedr15_sxyp", "sxyp", "gtedr15", "gtedr15_sxyp", //
	"gtedr16_sz0",  "sz0",  "gtedr16", "gtedr16_sz0",  //
	"gtedr17_sz1",  "sz1",  "gtedr17", "gtedr17_sz1",  //
	"gtedr18_sz2",  "sz2",  "gtedr18", "gtedr18_sz2",  //
	"gtedr19_sz3",  "sz3",  "gtedr19", "gtedr19_sz3",  //
	"gtedr20_rgb0", "rgb0", "gtedr20", "gtedr20_rgb0", //
	"gtedr21_rgb1", "rgb1", "gtedr21", "gtedr21_rgb1", //
	"gtedr22_rgb2", "rgb2", "gtedr22", "gtedr22_rgb2", //
	"gtedr23_res1", "res1", "gtedr23", "gtedr23_res1", //
	"gtedr24_mac0", "mac0", "gtedr24", "gtedr24_mac0", //
	"gtedr25_mac1", "mac1", "gtedr25", "gtedr25_mac1", //
	"gtedr26_mac2", "mac2", "gtedr26", "gtedr26_mac2", //
	"gtedr27_mac3", "mac3", "gtedr27", "gtedr27_mac3", //
	"gtedr28_irgb", "irgb", "gtedr28", "gtedr28_irgb", //
	"gtedr29_orgb", "orgb", "gtedr29", "gtedr29_orgb", //
	"gtedr30_lzcs", "lzcs", "gtedr30", "gtedr30_lzcs", //
	"gtedr31_lzcr", "lzcr", "gtedr31", "gtedr31_lzcr"  //
};

static char Ascii[0x10];
static char Buffer[0x1000];

int Print(const char *Format, ...)
{
	static va_list list;
	va_start(list, Format);
	vsprintf(Text, Format, list);
	va_end(list);

	//fprintf(Binary.File, "%08X: %08X %s\r\n", Address+Pointer, Opcode, Text);
	*(uint32_t*)Ascii = Opcode;
	if (Ascii[0] < ' ') Ascii[0] = '.';
	if (Ascii[1] < ' ') Ascii[1] = '.';
	if (Ascii[2] < ' ') Ascii[2] = '.';
	if (Ascii[3] < ' ') Ascii[3] = '.';
	Ascii[4] = 0;

	int Width = 0;
	int Length = 0;
	if (ShowAddress) {
		sprintf(&Buffer[Length], "%08X:", Address+Pointer);
		Length = strlen(Buffer);
		//Width += 0x09;
		Width += 0x20;
	}

	//if (ShowOpcode) {
	//	sprintf_s(&Buffer[Length], sizeof(Buffer)-Length, " %08X",  Opcode);
	//	Length = strlen(Buffer);
	//	Width += 0x09;
	//}

	if (ShowLabel)  {
		if (Address+Pointer == Entry) {
			sprintf(&Buffer[Length], " Entry:     ");
		} else {
			if ((Flag[Pointer] & FlagLabel) == FlagLabel) {
				sprintf(&Buffer[Length], " L%08X: ", Address+Pointer);
			} else {
				sprintf(&Buffer[Length], "            ");
			}
		}
		Length = strlen(Buffer);
		Width += 0x0B;
	}

	if (ShowMnemonic) {
		sprintf(&Buffer[Length], " %s", Text);
		Length = strlen(Buffer);
		//Width += 0x21;
		Width += 0x21;
	}

	//Width += LenHTML;
	if (ShowAscii) {
		for (int i = Length; i < Width; i++) Buffer[i] = ' ';
		sprintf(&Buffer[Width], "# \"%s\"", Ascii);
		Length = strlen(Buffer);
	}

	if (ShowOpcode) {
		sprintf(&Buffer[Length], " %08X",  Opcode);
		Length = strlen(Buffer);
		Width += 0x09;
	}

	fprintf(Binary.File, "%s\r\n", Buffer);
	return true;
}

int RType(void)
{
	//Print("RType %02X %02X %02X %02X %02X %02X", Op, Rs, Rt, Rd, Sa, Func);
	Type = (char*)"RType";
	switch (Func) {
	case 0x00:
		if (Opcode == 0x00000000) { LenHTML = 14; Print("nop"); }
		else { LenHTML = 42; Print("sll     %s, %s, 0x%02X", Reg(cpu, Rd), Reg(cpu, Rt), Sa); }
		break;
	case 0x02: LenHTML = 42; Print("srl     %s, %s, 0x%02X", Reg(cpu, Rd), Reg(cpu, Rt), Sa);           break;
	case 0x03: LenHTML = 42; Print("sra     %s, %s, 0x%02X", Reg(cpu, Rd), Reg(cpu, Rt), Sa);           break;
	case 0x04: LenHTML = 56; Print("sllv    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rt), Reg(cpu, Rs)); break;
	case 0x06: LenHTML = 56; Print("srlv    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rt), Reg(cpu, Rs)); break;
	case 0x07: LenHTML = 56; Print("srav    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rt), Reg(cpu, Rs)); break;
	case 0x08: LenHTML = 28; JrReg = true; Print("jr      %s",             Reg(cpu, Rs));                             break;
	case 0x09: LenHTML = 42; Print("jalr    %s, %s",         Reg(cpu, Rs), Reg(cpu, Rd));               break;
	case 0x0C: LenHTML = 46; Print("syscall 0x%08X",         SysCall, SysCall);                                  break;
	case 0x0D: LenHTML = 46; Print("break   0x%08X",         SysCall, SysCall);                                  break;
	case 0x10: LenHTML = 28; Print("mfhi    %s",             Reg(cpu, Rd));                             break;
	case 0x11: LenHTML = 28; Print("mthi    %s",             Reg(cpu, Rs));                             break;
	case 0x12: LenHTML = 28; Print("mflo    %s",             Reg(cpu, Rd));                             break;
	case 0x13: LenHTML = 28; Print("mtlo    %s",             Reg(cpu, Rs));                             break;
	case 0x18: LenHTML = 42; Print("mult    %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
	case 0x19: LenHTML = 42; Print("multu   %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
	case 0x1A: LenHTML = 42; Print("div     %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
	case 0x1B: LenHTML = 42; Print("divu    %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
	case 0x20: LenHTML = 56; Print("add     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x21: LenHTML = 56; Print("addu    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x22: LenHTML = 56; Print("sub     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x23: LenHTML = 56; Print("subu    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x24: LenHTML = 56; Print("and     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x25: LenHTML = 56; Print("or      %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x26: LenHTML = 56; Print("xor     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x27: LenHTML = 56; Print("nor     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x2A: LenHTML = 56; Print("slt     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	case 0x2B: LenHTML = 56; Print("sltu    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
	default:   LenHTML = 14; Print("illegal");                                                          break;
	}
	return true;
}

int IType(void)
{
	//Print("IType %02X %02X %04X", Op, Rs, Rt, Imm);
	Type = (char*)"IType";
	switch (Op) {
	case 0x01:
		if      (Rt == 0x00) { LenHTML = 52; Print("bltz    %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
		else if (Rt == 0x01) { LenHTML = 52; Print("bgez    %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
		else if (Rt == 0x10) { LenHTML = 52; Print("bltzal  %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
		else if (Rt == 0x11) { LenHTML = 52; Print("bgezal  %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
		else                 { LenHTML = 14; Print("illegal"); }
		break;

	case 0x04: LenHTML = 66; Print("beq     %s, %s, 0x%08X", Reg(cpu, Rs), Reg(cpu, Rt), Label, Label); break;
	case 0x05: LenHTML = 66; Print("bne     %s, %s, 0x%08X", Reg(cpu, Rs), Reg(cpu, Rt), Label, Label); break;
	case 0x06: LenHTML = 52; Print("blez    %s, 0x%08X",     Reg(cpu, Rs), Label, Label);                               break;
	case 0x07: LenHTML = 52; Print("bgtz    %s, 0x%08X",     Reg(cpu, Rs), Label, Label);                               break;
	case 0x08: LenHTML = 56; Print("addi    %s, %s, %s",     Reg(cpu, Rt), Reg(cpu, Rs), Number);  break;
	case 0x09: LenHTML = 42; Print("addiu   %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
	case 0x0A: LenHTML = 56; Print("slti    %s, %s, %s",     Reg(cpu, Rt), Reg(cpu, Rs), Number);  break;
	case 0x0B: LenHTML = 42; Print("sltiu   %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
	case 0x0C: LenHTML = 42; Print("andi    %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
	case 0x0D: LenHTML = 42; Print("ori     %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
	case 0x0E: LenHTML = 42; Print("xori    %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
	case 0x0F: LenHTML = 28; Print("lui     %s, 0x%04X",     Reg(cpu, Rt), Imm);                   break;
	case 0x20: LenHTML = 42; Print("lb      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x21: LenHTML = 42; Print("lh      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x22: LenHTML = 42; Print("lwl     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x23: LenHTML = 42; Print("lw      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x24: LenHTML = 42; Print("lbu     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x25: LenHTML = 42; Print("lhu     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x26: LenHTML = 42; Print("lwr     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x28: LenHTML = 42; Print("sb      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x29: LenHTML = 42; Print("sh      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x2A: LenHTML = 42; Print("swl     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x2B: LenHTML = 42; Print("sw      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x2E: LenHTML = 42; Print("swr     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	case 0x32: LenHTML = 42; Print("lwc2    %s, %s(%s)",     Reg(gted, Rt), Number, Reg(cpu, Rs)); break;
	case 0x3A: LenHTML = 42; Print("swc2    %s, %s(%s)",     Reg(gted, Rt), Number, Reg(cpu, Rs)); break;
//	case 0x31: LenHTML = 42; Print("lwc1    %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
//	case 0x39: LenHTML = 42; Print("swc1    %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
	default:   LenHTML = 14; Print("illegal"); break;
	}
	return true;
}

int JType(void)
{
	//Print("JType %07X", Op, Target);
	Type = (char*)"JType";
	switch (Op) {
	case 0x02: LenHTML = 38; Print("j       0x%08X", Target, Target); break;
	case 0x03: LenHTML = 38; Print("jal     0x%08X", Target, Target); break;
	default:   LenHTML = 14; Print("illegal");                        break;
	}
	return true;
}

int Cop_0(void)
{
	// Print("Coprocessor 0 %02X %02X %02X %02X %02X %02X", Op, Format, Ft, Fs, Fd, Func); break;
	Type = (char*)"Cop_0";
	switch (Format) {
	default:   LenHTML = 14; Print("illegal"); break;
	case 0x00: LenHTML = 42; Print("mfc0    %s, %s", Reg(cpu, Ft), Reg(cop0, Fs)); break;
	case 0x04: LenHTML = 42; Print("mtc0    %s, %s", Reg(cpu, Ft), Reg(cop0, Fs)); break;
	case 0x10: case 0x11: case 0x12: case 0x13:
	case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B:
	case 0x1C: case 0x1D: case 0x1E: case 0x1F:
		LenHTML = 14;
		switch (Func & 0x1F) {
		case 0x01: Print("tlbr");    break;
		case 0x02: Print("tlbwi");   break;
		case 0x06: Print("tlbwr");   break;
		case 0x10: Print("rfe");     break;
		default:   Print("illegal"); break;
		}
		break;
	}
	return true;
}

int Cop_1(void)
{
	//Print("Coprocessor 1 %02X %02X %02X %02X %02X %02X", Op, Format, Ft, Fs, Fd, Func);
	Type = (char*)"Cop_1";
	LenHTML = 14;
	Print("illegal");
	return true;
}

int Cop_2(void)
{
	//Print("Coprocessor 2 %02X %02X %02X %02X %02X %02X", Op, Format, Ft, Fs, Fd, Func);
	Type = (char*)"Cop_2";
	switch (Format) {
	case 0x00: LenHTML = 42; Print("mfc2    %s, %s", Reg(cpu, Ft), Reg(gted, Fs)); break;
	case 0x02: LenHTML = 42; Print("cfc2    %s, %s", Reg(cpu, Ft), Reg(gtec, Fs)); break;
	case 0x04: LenHTML = 42; Print("mtc2    %s, %s", Reg(cpu, Ft), Reg(gted, Fs)); break;
	case 0x06: LenHTML = 42; Print("ctc2    %s, %s", Reg(cpu, Ft), Reg(gtec, Fs)); break;
	case 0x11: LenHTML = 14; Print("rtpt");  break;
	case 0x1C: LenHTML = 14; Print("gpf");   break;
	case 0x1D: LenHTML = 14; Print("gpl");   break;
	case 0x14: LenHTML = 14; Print("intpl"); break;
	case 0x15: LenHTML = 14; Print("sqr");   break;
	default:
		LenHTML = 14;
		switch (Fop) {
		case 0x401: Print("rtps");    break;
		case 0x406: Print("nclip");   break;
		case 0x40C: Print("op");      break;
		case 0x410: Print("dpcs");    break;
		case 0x412: Print("mvmva");   break;
		case 0x413: Print("ncds");    break;
		case 0x414: Print("cdp");     break;
		case 0x416: Print("ncdt");    break;
		case 0x41B: Print("nccs");    break;
		case 0x41C: Print("cc");      break;
		case 0x41E: Print("ncs");     break;
		case 0x420: Print("nct");     break;
		case 0x429: Print("dpcl");    break;
		case 0x42A: Print("dpct");    break;
		case 0x42D: Print("avsz3");   break;
		case 0x42E: Print("avsz4");   break;
		case 0x43F: Print("ncct");    break;
		default:    Print("illegal"); break;
		}
	}
	return true;
}

int Cop_3(void)
{
	//Print("Coprocessor 3 %02X %02X %02X %02X %02X %02X", Op, Format, Ft, Fs, Fd, Func);
	Type = (char*)"Cop_3";
	LenHTML = 14;
	Print("illegal");
	return true;
}

int Follow(uint32_t Pc)
{
	if (Pc < Address || Pc > Address + Binary.Size) return true;
	uint32_t *Opcodes = (uint32_t*)Binary.Code;

	while ((Pc >= Address) && (Pc <= (Address + Binary.Size))) {
		if ((Flag[Pc-Address  ] & FlagCode) == FlagCode) return true;
		if ((Flag[Pc-Address  ] & FlagData) == FlagData) return true;

		Opcode  = Code[(Pc-Address)/sizeof(uint32_t)];
		Rs      = ((Opcode & 0x03E00000) >> 0x15);
		Target  = ((Opcode & 0x01FFFFFF) >> 0x00);
		Label   = ((Opcode & 0x0000FFFF) >> 0x00);
		if ((Label & 0x00008000) == 0x00008000) Label |= 0xFFFF0000;
		Target = (Target << 0x02) | (Pc & 0xF0000000);
		Label  = (Label << 0x02) + Pc;

		switch (Opcodes[(Pc-Address)/sizeof(uint32_t)]) {
		default:
			Flag[Pc-Address  ] |= FlagCode;
			break;
		case id_beq:
		case id_bgez:
		case id_bgezal:
		case id_bgtz:
		case id_blez:
		case id_bltz:
		case id_bltzal:
		case id_bne:
			Flag[Pc-Address  ] |= FlagCode;
			//Flag[Pc-Address+4] |= FlagCode;
			Follow(Label);
			break;
		case id_j:
			Flag[Pc-Address  ] |= FlagCode;
			Flag[Pc-Address+4] |= FlagCode;
			Follow(Target);
			return true;
		case id_jal:
			Flag[Pc-Address  ] |= FlagCode;
			//Flag[Pc-Address+4] |= FlagCode;
			Follow(Target);
			break;
		case id_jalr:
			Flag[Pc-Address  ] |= FlagCode;
			//Flag[Pc-Address+4] |= FlagCode;
			break;
		case id_jr:
			Flag[Pc-Address  ] |= FlagCode;
			if (Rs == 0x31) {
				Flag[Pc-Address+4] |= FlagCode;
				return true;
			}
			break;
		case id_illegal:
			Flag[Pc-Address] |= FlagData;
			return false;
		}
		Pc += 4;
	}
	return true;
}

int pass1(void)
{
	// identify executable file format...
	if (Binary.Size < sizeof(PsxExeHdr)) return false;
	Hdr = (PsxExeHdr*)Binary.Data;
	if (!Hdr) return false;
	if (memcmp(Hdr->id, "PS-X EXE", 8) != 0) return false;
	Entry   = Hdr->pc0;
	Address = Hdr->t_addr - 0x800;
	return true;
}

int pass2(void)
{
	// identify opcodes
	uint32_t *Opcodes = (uint32_t*)Binary.Code;
	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		Pc      = Address + Pointer + 4;
		Opcode  = Code[Pointer/sizeof(uint32_t)];
		Op      = ((Opcode & 0xFC000000) >> 0x1A);
		Rs      = ((Opcode & 0x03E00000) >> 0x15);
		Format  = ((Opcode & 0x03E00000) >> 0x15);
		Rt      = ((Opcode & 0x001F0000) >> 0x10);
		Ft      = ((Opcode & 0x001F0000) >> 0x10);
		Rd      = ((Opcode & 0x0000F800) >> 0x0B);
		Fs      = ((Opcode & 0x0000F800) >> 0x0B);
		Sa      = ((Opcode & 0x000007C0) >> 0x06);
		Fd      = ((Opcode & 0x000007C0) >> 0x06);
		Func    = ((Opcode & 0x0000003F) >> 0x00);
		SysCall = ((Opcode & 0x03FFFFFC) >> 0x06);
		//Target  = ((Opcode & 0x03FFFFFF) >> 0x00);
		Target  = ((Opcode & 0x01FFFFFF) >> 0x00);
		Imm     = ((Opcode & 0x0000FFFF) >> 0x00);
		Label   = ((Opcode & 0x0000FFFF) >> 0x00);
		if ((Label & 0x00008000) == 0x00008000) Label |= 0xFFFF0000;
		Target = (Target << 0x02) | (Pc & 0xF0000000);
		Label  = (Label << 0x02) + Pc;
		Fop    = ((Format & 0x10) << 0x06) | Func;

		Opcodes[Pointer/sizeof(uint32_t)] = id_illegal;
		switch (Op) {
		case 0x00:
			switch (Func) {
			case 0x00:
				if (Opcode == 0x00000000) Opcodes[Pointer/sizeof(uint32_t)] = id_nop;
				else                      Opcodes[Pointer/sizeof(uint32_t)] = id_sll;
				break;
			case 0x02: Opcodes[Pointer/sizeof(uint32_t)] = id_srl    ; break;
			case 0x03: Opcodes[Pointer/sizeof(uint32_t)] = id_sra    ; break;
			case 0x04: Opcodes[Pointer/sizeof(uint32_t)] = id_sllv   ; break;
			case 0x06: Opcodes[Pointer/sizeof(uint32_t)] = id_srlv   ; break;
			case 0x07: Opcodes[Pointer/sizeof(uint32_t)] = id_srav   ; break;
			case 0x08: Opcodes[Pointer/sizeof(uint32_t)] = id_jr     ; break;
			case 0x09: Opcodes[Pointer/sizeof(uint32_t)] = id_jalr   ; break;
			case 0x0C: Opcodes[Pointer/sizeof(uint32_t)] = id_syscall; break;
			case 0x0D: Opcodes[Pointer/sizeof(uint32_t)] = id_break  ; break;
			case 0x10: Opcodes[Pointer/sizeof(uint32_t)] = id_mfhi   ; break;
			case 0x11: Opcodes[Pointer/sizeof(uint32_t)] = id_mthi   ; break;
			case 0x12: Opcodes[Pointer/sizeof(uint32_t)] = id_mflo   ; break;
			case 0x13: Opcodes[Pointer/sizeof(uint32_t)] = id_mtlo   ; break;
			case 0x18: Opcodes[Pointer/sizeof(uint32_t)] = id_mult   ; break;
			case 0x19: Opcodes[Pointer/sizeof(uint32_t)] = id_multu  ; break;
			case 0x1A: Opcodes[Pointer/sizeof(uint32_t)] = id_div    ; break;
			case 0x1B: Opcodes[Pointer/sizeof(uint32_t)] = id_divu   ; break;
			case 0x20: Opcodes[Pointer/sizeof(uint32_t)] = id_add    ; break;
			case 0x21: Opcodes[Pointer/sizeof(uint32_t)] = id_addu   ; break;
			case 0x22: Opcodes[Pointer/sizeof(uint32_t)] = id_sub    ; break;
			case 0x23: Opcodes[Pointer/sizeof(uint32_t)] = id_subu   ; break;
			case 0x24: Opcodes[Pointer/sizeof(uint32_t)] = id_and    ; break;
			case 0x25: Opcodes[Pointer/sizeof(uint32_t)] = id_or     ; break;
			case 0x26: Opcodes[Pointer/sizeof(uint32_t)] = id_xor    ; break;
			case 0x27: Opcodes[Pointer/sizeof(uint32_t)] = id_nor    ; break;
			case 0x2A: Opcodes[Pointer/sizeof(uint32_t)] = id_slt    ; break;
			case 0x2B: Opcodes[Pointer/sizeof(uint32_t)] = id_sltu   ; break;
			}
			break;
		case 0x02: Opcodes[Pointer/sizeof(uint32_t)] = id_j;   break;
		case 0x03: Opcodes[Pointer/sizeof(uint32_t)] = id_jal; break;
		case 0x10:
			switch (Format) {
			case 0x00: Opcodes[Pointer/sizeof(uint32_t)] = id_mfc0; break;
			case 0x04: Opcodes[Pointer/sizeof(uint32_t)] = id_mtc0; break;
			case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
			case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F:
				switch (Func & 0x1F) {
				case 0x01: Opcodes[Pointer/sizeof(uint32_t)] = id_tlbr;    break;
				case 0x02: Opcodes[Pointer/sizeof(uint32_t)] = id_tlbwi;   break;
				case 0x06: Opcodes[Pointer/sizeof(uint32_t)] = id_tlbwr;   break;
				case 0x10: Opcodes[Pointer/sizeof(uint32_t)] = id_rfe;     break;
				}
				break;
			}
			break;

		case 0x12:
			switch (Format) {
			case 0x00: Opcodes[Pointer/sizeof(uint32_t)] = id_mfc2;  break;
			case 0x02: Opcodes[Pointer/sizeof(uint32_t)] = id_cfc2;  break;
			case 0x04: Opcodes[Pointer/sizeof(uint32_t)] = id_mtc2;  break;
			case 0x06: Opcodes[Pointer/sizeof(uint32_t)] = id_ctc2;  break;
			case 0x11: Opcodes[Pointer/sizeof(uint32_t)] = id_rtpt;  break;
			case 0x1C: Opcodes[Pointer/sizeof(uint32_t)] = id_gpf;   break;
			case 0x1D: Opcodes[Pointer/sizeof(uint32_t)] = id_gpl;   break;
			case 0x14: Opcodes[Pointer/sizeof(uint32_t)] = id_intpl; break;
			case 0x15: Opcodes[Pointer/sizeof(uint32_t)] = id_sqr;   break;
			default:
				switch (Fop) {
				case 0x401: Opcodes[Pointer/sizeof(uint32_t)] = id_rtps;    break;
				case 0x406: Opcodes[Pointer/sizeof(uint32_t)] = id_nclip;   break;
				case 0x40C: Opcodes[Pointer/sizeof(uint32_t)] = id_op;      break;
				case 0x410: Opcodes[Pointer/sizeof(uint32_t)] = id_dpcs;    break;
				case 0x412: Opcodes[Pointer/sizeof(uint32_t)] = id_mvmva;   break;
				case 0x413: Opcodes[Pointer/sizeof(uint32_t)] = id_ncds;    break;
				case 0x414: Opcodes[Pointer/sizeof(uint32_t)] = id_cdp;     break;
				case 0x416: Opcodes[Pointer/sizeof(uint32_t)] = id_ncdt;    break;
				case 0x41B: Opcodes[Pointer/sizeof(uint32_t)] = id_nccs;    break;
				case 0x41C: Opcodes[Pointer/sizeof(uint32_t)] = id_cc;      break;
				case 0x41E: Opcodes[Pointer/sizeof(uint32_t)] = id_ncs;     break;
				case 0x420: Opcodes[Pointer/sizeof(uint32_t)] = id_nct;     break;
				case 0x429: Opcodes[Pointer/sizeof(uint32_t)] = id_dpcl;    break;
				case 0x42A: Opcodes[Pointer/sizeof(uint32_t)] = id_dpct;    break;
				case 0x42D: Opcodes[Pointer/sizeof(uint32_t)] = id_avsz3;   break;
				case 0x42E: Opcodes[Pointer/sizeof(uint32_t)] = id_avsz4;   break;
				case 0x43F: Opcodes[Pointer/sizeof(uint32_t)] = id_ncct;    break;
				}
			}
			break;
		default:
			switch (Op) {
			case 0x01:
				if      (Rt == 0x00) Opcodes[Pointer/sizeof(uint32_t)] = id_bltz   ;
				else if (Rt == 0x01) Opcodes[Pointer/sizeof(uint32_t)] = id_bgez   ;
				else if (Rt == 0x10) Opcodes[Pointer/sizeof(uint32_t)] = id_bltzal ;
				else if (Rt == 0x11) Opcodes[Pointer/sizeof(uint32_t)] = id_bgezal ;
				break;
			case 0x04: Opcodes[Pointer/sizeof(uint32_t)] = id_beq    ; break;
			case 0x05: Opcodes[Pointer/sizeof(uint32_t)] = id_bne    ; break;
			case 0x06: Opcodes[Pointer/sizeof(uint32_t)] = id_blez   ; break;
			case 0x07: Opcodes[Pointer/sizeof(uint32_t)] = id_bgtz   ; break;
			case 0x08: Opcodes[Pointer/sizeof(uint32_t)] = id_addi   ; break;
			case 0x09: Opcodes[Pointer/sizeof(uint32_t)] = id_addiu  ; break;
			case 0x0A: Opcodes[Pointer/sizeof(uint32_t)] = id_slti   ; break;
			case 0x0B: Opcodes[Pointer/sizeof(uint32_t)] = id_sltiu  ; break;
			case 0x0C: Opcodes[Pointer/sizeof(uint32_t)] = id_andi   ; break;
			case 0x0D: Opcodes[Pointer/sizeof(uint32_t)] = id_ori    ; break;
			case 0x0E: Opcodes[Pointer/sizeof(uint32_t)] = id_xori   ; break;
			case 0x0F: Opcodes[Pointer/sizeof(uint32_t)] = id_lui    ; break;
			case 0x20: Opcodes[Pointer/sizeof(uint32_t)] = id_lb     ; break;
			case 0x21: Opcodes[Pointer/sizeof(uint32_t)] = id_lh     ; break;
			case 0x22: Opcodes[Pointer/sizeof(uint32_t)] = id_lwl    ; break;
			case 0x23: Opcodes[Pointer/sizeof(uint32_t)] = id_lw     ; break;
			case 0x24: Opcodes[Pointer/sizeof(uint32_t)] = id_lbu    ; break;
			case 0x25: Opcodes[Pointer/sizeof(uint32_t)] = id_lhu    ; break;
			case 0x26: Opcodes[Pointer/sizeof(uint32_t)] = id_lwr    ; break;
			case 0x28: Opcodes[Pointer/sizeof(uint32_t)] = id_sb     ; break;
			case 0x29: Opcodes[Pointer/sizeof(uint32_t)] = id_sh     ; break;
			case 0x2A: Opcodes[Pointer/sizeof(uint32_t)] = id_swl    ; break;
			case 0x2B: Opcodes[Pointer/sizeof(uint32_t)] = id_sw     ; break;
			case 0x2E: Opcodes[Pointer/sizeof(uint32_t)] = id_swr    ; break;
			case 0x32: Opcodes[Pointer/sizeof(uint32_t)] = id_lwc2   ; break;
			case 0x3A: Opcodes[Pointer/sizeof(uint32_t)] = id_swc2   ; break;
			}
			break;
		}
	}
	return true;
}

int pass3(void)
{
	// step through code seperating code from data.
	if (Entry < Address || Entry > Address + Binary.Size) return false;
	Follow(Entry);

	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		if ((Flag[Pointer] & FlagCode) != FlagCode) {
			Flag[Pointer  ] |= FlagData;
			Flag[Pointer+1] |= FlagData;
			Flag[Pointer+2] |= FlagData;
			Flag[Pointer+3] |= FlagData;
		}
	}
	return true;
}

int Dasm(void)
{
	if (!Binary.Data) return false;
	if (!Binary.File) return false;
	if (!Binary.Size) return false;
	Flag = (uint32_t*)Binary.Copy;
	Code = (uint32_t*)Binary.Data;

	// multiple passes over file...
	pass1(); // identify file format & extract loading address and entry point.
	//pass2(); // identify each opcode and record it's internal id.
	pass3(); // step through the code seperating code from data.
	pass4(); // further step through the code searching for lui type data access

	// Print out the header to the assembler listing
	fprintf(Binary.File, "# dasm version 1.0 March 2012\r\n");
	fprintf(Binary.File, "# file name : %s%s\r\n", Binary.Fname, Binary.Ext);
	fprintf(Binary.File, "# file size : 0x%08X\r\n", Binary.Size);
	fprintf(Binary.File, "# address   : 0x%08X\r\n", Address);
	if ((Entry >= Address) && (Entry <= Address + Binary.Size)) {
		fprintf(Binary.File, "# entry     : 0x%08X\r\n", Entry);
	}
	switch (Naming) {
	case 0: fprintf(Binary.File, "# naming    : both common and numeric register names\r\n"); break;
	case 1: fprintf(Binary.File, "# naming    : common register names\r\n"); break;
	case 2: fprintf(Binary.File, "# naming    : numeric register names\r\n"); break;
	case 3: fprintf(Binary.File, "# naming    : pSX register naming convention\r\n"); break;
	}
	fprintf(Binary.File, "\r\n");

	// find jump targets...
	if ((Entry >= Address) && (Entry <= Address+Binary.Size)) Flag[Entry-Address] |= FlagLabel;
	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		if (Flag[Pointer] & FlagCode) {
			// instruction decoding
			Pc      = Address + Pointer + 4;
			Opcode  = Code[Pointer/sizeof(uint32_t)];
			Op      = ((Opcode & 0xFC000000) >> 0x1A);
			Target  = ((Opcode & 0x01FFFFFF) >> 0x00);
			Target  = (Target << 0x02) | (Pc & 0xF0000000);
			Label   = ((Opcode & 0x0000FFFF) >> 0x00);
			if ((Label & 0x00008000) == 0x00008000) Label |= 0xFFFF0000;
			Label  = (Label << 0x02) + Pc;

			// disassemble Opcode
			switch (Op) {
			default:
				break;

			case 0x00: case 0x10: case 0x11: case 0x12: case 0x13:
				break;

			case 0x02: case 0x03:
				if ((Target >= Address) && (Target <= Address+Binary.Size)) {
					Flag[Target-Address] |= FlagLabel;
					//fprintf(Binary.File, "local 0x%08X\r\n", Target, Target);
				} else {
					fprintf(Binary.File, "extern 0x%08X\r\n", Target);
				}
				break;

			case 0x01: case 0x04: case 0x05: case 0x06: case 0x07:
				if ((Label >= Address) && (Label <= Address+Binary.Size)) {
					Flag[Label-Address] |= FlagLabel;
					//fprintf(Binary.File, "local 0x%08X\r\n", Label, Label);
				} else {
					fprintf(Binary.File, "extern 0x%08X\r\n", Label);
				}
				break;
			}
		} else {
			// instruction decoding
			Pc      = Address + Pointer;
			Opcode  = Code[Pointer/sizeof(uint32_t)];
			if ((Opcode >= Address) && (Opcode <= Address+Binary.Size)) {
				Flag[Opcode-Address] |= FlagLabel;
			} else if ((Opcode >= 0x80000000) && (Opcode < 0x80200000)) {
				fprintf(Binary.File, "extern 0x%08X\r\n", Target);
			}
		}

	}

	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		if ((Flag[Pointer    ] & FlagCode) != FlagCode) continue; // if this word is an opcode
		if ((Flag[Pointer - 4] & FlagCode) == FlagCode) continue; // and the previous word is data
		Flag[Pointer] |= FlagLabel; // mark this location as a label
	}
	fprintf(Binary.File, "\r\n");
	fprintf(Binary.File, "# local functions\r\n");
	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		if ((Flag[Pointer] & FlagCode ) != FlagCode) continue;
		if ((Flag[Pointer] & FlagLabel) != FlagLabel) continue;
		Pc = Address + Pointer;
		fprintf(Binary.File, "local  0x%08X\r\n", Pc);
	}
	fprintf(Binary.File, "\r\n");
	fprintf(Binary.File, "# local data references\r\n");
	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		if ((Flag[Pointer] & FlagData ) != FlagData) continue;
		if ((Flag[Pointer] & FlagLabel) != FlagLabel) continue;
		Pc = Address + Pointer;
		fprintf(Binary.File, "local  0x%08X\r\n", Pc);
	}
	fprintf(Binary.File, "\r\n");


	// disassembly should be performed as a series of passes
	JrReg = false; // flag stating that we should terminate a proc
	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		bool TerminateProc = false;

		// instruction decoding
		Pc      = Address + Pointer + 4;
		Opcode  = Code[Pointer/sizeof(uint32_t)];
		Op      = ((Opcode & 0xFC000000) >> 0x1A);
		Rs      = ((Opcode & 0x03E00000) >> 0x15);
		Format  = ((Opcode & 0x03E00000) >> 0x15);
		Rt      = ((Opcode & 0x001F0000) >> 0x10);
		Ft      = ((Opcode & 0x001F0000) >> 0x10);
		Rd      = ((Opcode & 0x0000F800) >> 0x0B);
		Fs      = ((Opcode & 0x0000F800) >> 0x0B);
		Sa      = ((Opcode & 0x000007C0) >> 0x06);
		Fd      = ((Opcode & 0x000007C0) >> 0x06);
		Func    = ((Opcode & 0x0000003F) >> 0x00);
		SysCall = ((Opcode & 0x03FFFFFC) >> 0x06);
		//Target  = ((Opcode & 0x03FFFFFF) >> 0x00);
		Target  = ((Opcode & 0x01FFFFFF) >> 0x00);
		Imm     = ((Opcode & 0x0000FFFF) >> 0x00);
		Label   = ((Opcode & 0x0000FFFF) >> 0x00);
		if ((Label & 0x00008000) == 0x00008000) Label |= 0xFFFF0000;
		Target = (Target << 0x02) | (Pc & 0xF0000000);
		Label  = (Label << 0x02) + Pc;
		Fop    = ((Format & 0x10) << 0x06) | Func;

		if (JrReg == true) {
			TerminateProc = true;
			JrReg = false;
		}
		if (Imm == 0x00008000) {
			sprintf(Number, "-0xFFFF%04X", Imm);
		} else if (Imm & 0x00008000) {
			sprintf(Number, "-0x%04X", (Imm^0x0000FFFF)+1);
		} else {
			sprintf(Number, "0x%04X", Imm);
		}

		// disassemble Opcode
		if (Flag[Pointer] & FlagData) {
			uint16_t Half;
			uint8_t  Byte;
			switch (Flag[Pointer] & 0x03) {
			case FlagByte:
				Byte = Opcode & 0x000000FF;
				Print("byte    0x%02X", Byte);
				Byte = (Opcode & 0x0000FF00) >> 0x08;
				Pointer += 1;
				Print("byte    0x%02X", Byte);
				Byte = (Opcode & 0x00FF0000) >> 0x10;
				Pointer += 1;
				Print("byte    0x%02X", Byte);
				Byte = (Opcode & 0xFF000000) >> 0x18;
				Pointer += 1;
				Print("byte    0x%02X", Byte);
				Pointer -= 3;
				break;
			case FlagHalf:
				Half = Opcode & 0x0000FFFF;
				Print("half    0x%04X", Half);
				Half = Opcode >> 0x10;
				Pointer += 2;
				Print("half    0x%04X", Half);
				Pointer -= 2;
				break;
			default:
				Print("word    0x%08X", Opcode);
				break;
			}
		} else {
			LenHTML = 0;
			switch (Op) {
			case 0x00: RType(); break; // Register Type Opcode
			case 0x02: JType(); break; // Jump Type Opcode
			case 0x03: JType(); break; // Jump Type Opcode
			case 0x10: Cop_0(); break; // Coprocessor 0 Opcode
			case 0x11: Cop_1(); break; // Coprocessor 1 Opcode
			case 0x12: Cop_2(); break; // Coprocessor 2 Opcode
			case 0x13: Cop_3(); break; // Coprocessor 3 Opcode
			default:   IType(); break; // Immediate Type Opcode
			}
		}

		if (TerminateProc == true) {
			LenHTML = 0;
			Print("");
		}
	}
	JrReg = false;
	return true;
}
