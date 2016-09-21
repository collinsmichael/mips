//
// File Name : Dasm.h
// File Date : 15 March 2012
// Author(s) : Valendian
// Purpose   : Disassembles Binary File to MIPS assembler listing.
//

#ifndef DASM_HEADER
#define DASM_HEADER

#ifndef int8
typedef signed __int8 int8;
#endif
#ifndef uint8
typedef unsigned __int8 uint8;
#endif
#ifndef int16
typedef signed __int16 int16;
#endif
#ifndef uint16
typedef unsigned __int16 uint16;
#endif
#ifndef int32
typedef signed __int32 int32;
#endif
#ifndef uint32
typedef unsigned __int32 uint32;
#endif
#ifndef int64
typedef signed __int64 int64;
#endif
#ifndef uint64
typedef unsigned __int64 uint64;
#endif

typedef struct PsxExeHdr {
	uint8  id[8];   //
	uint32 text;    // SCE only
	uint32 data;    // SCE only
	uint32 pc0;     //
	uint32 gp0;     // SCE only
	uint32 t_addr;  //
	uint32 t_size;  //
	uint32 d_addr;  // SCE only
	uint32 d_size;  // SCE only
	uint32 b_addr;  // SCE only
	uint32 b_size;  // SCE only
	uint32 s_addr;  //
	uint32 s_size;  //
	uint32 SavedSP; //
	uint32 SavedFP; //
	uint32 SavedGP; //
	uint32 SavedRA; //
	uint32 SavedS0; //
} PsxExeHdr;

const uint32 FlagCode          = 0x80000000; // Byte is marked as code.
const uint32 FlagData          = 0x40000000; // Byte is marked as data.
const uint32 FlagFunction      = 0x20000000; // Byte marks the entry point to a function.
const uint32 FlagLabel         = 0x10000000; // Byte marks the destination of a jump or conditional branch.
const uint32 FlagUnconditional = 0x08000000;
const uint32 FlagGreater       = 0x04000000;
const uint32 FlagEqual         = 0x02000000;
const uint32 Flaglesser        = 0x01000000;
const uint32 FlagByte          = 0x00000001; // Byte is marked as byte type.
const uint32 FlagHalf          = 0x00000002; // Byte is marked as half type.
const uint32 FlagWord          = 0x00000003; // Byte is marked as word type.


const uint32 id_add     = 0x01;
const uint32 id_addi    = 0x02;
const uint32 id_addiu   = 0x03;
const uint32 id_addu    = 0x04;
const uint32 id_and     = 0x05;
const uint32 id_andi    = 0x06;
const uint32 id_avsz3   = 0x07;
const uint32 id_avsz4   = 0x08;
const uint32 id_beq     = 0x09;
const uint32 id_bgez    = 0x0A;
const uint32 id_bgezal  = 0x0B;
const uint32 id_bgtz    = 0x0C;
const uint32 id_blez    = 0x0D;
const uint32 id_bltz    = 0x0E;
const uint32 id_bltzal  = 0x0F;
const uint32 id_bne     = 0x10;
const uint32 id_break   = 0x11;
const uint32 id_cc      = 0x12;
const uint32 id_cdp     = 0x13;
const uint32 id_cfc2    = 0x14;
const uint32 id_ctc2    = 0x15;
const uint32 id_div     = 0x16;
const uint32 id_divu    = 0x17;
const uint32 id_dpcl    = 0x18;
const uint32 id_dpcs    = 0x19;
const uint32 id_dpct    = 0x1A;
const uint32 id_gpf     = 0x1B;
const uint32 id_gpl     = 0x1C;
const uint32 id_intpl   = 0x1D;
const uint32 id_j       = 0x1E;
const uint32 id_jal     = 0x1F;
const uint32 id_jalr    = 0x20;
const uint32 id_jr      = 0x21;
const uint32 id_lb      = 0x22;
const uint32 id_lbu     = 0x23;
const uint32 id_lh      = 0x24;
const uint32 id_lhu     = 0x25;
const uint32 id_lui     = 0x26;
const uint32 id_lw      = 0x27;
const uint32 id_lwc1    = 0x28;
const uint32 id_lwc2    = 0x29;
const uint32 id_lwl     = 0x2A;
const uint32 id_lwr     = 0x2B;
const uint32 id_mfc0    = 0x2C;
const uint32 id_mfc2    = 0x2D;
const uint32 id_mfhi    = 0x2E;
const uint32 id_mflo    = 0x2F;
const uint32 id_mtc0    = 0x30;
const uint32 id_mtc2    = 0x31;
const uint32 id_mthi    = 0x32;
const uint32 id_mtlo    = 0x33;
const uint32 id_mult    = 0x34;
const uint32 id_multu   = 0x35;
const uint32 id_mvmva   = 0x36;
const uint32 id_nccs    = 0x37;
const uint32 id_ncct    = 0x38;
const uint32 id_ncds    = 0x39;
const uint32 id_ncdt    = 0x3A;
const uint32 id_nclip   = 0x3B;
const uint32 id_ncs     = 0x3C;
const uint32 id_nct     = 0x3D;
const uint32 id_nop     = 0x3E;
const uint32 id_nor     = 0x3F;
const uint32 id_op      = 0x40;
const uint32 id_or      = 0x41;
const uint32 id_ori     = 0x42;
const uint32 id_rfe     = 0x43;
const uint32 id_rtps    = 0x44;
const uint32 id_rtpt    = 0x45;
const uint32 id_sb      = 0x46;
const uint32 id_sh      = 0x47;
const uint32 id_sll     = 0x48;
const uint32 id_sllv    = 0x49;
const uint32 id_slt     = 0x4A;
const uint32 id_slti    = 0x4B;
const uint32 id_sltiu   = 0x4C;
const uint32 id_sltu    = 0x4D;
const uint32 id_sqr     = 0x4E;
const uint32 id_sra     = 0x4F;
const uint32 id_srav    = 0x50;
const uint32 id_srl     = 0x51;
const uint32 id_srlv    = 0x52;
const uint32 id_sub     = 0x53;
const uint32 id_subu    = 0x54;
const uint32 id_sw      = 0x55;
const uint32 id_swc1    = 0x56;
const uint32 id_swc2    = 0x57;
const uint32 id_swl     = 0x58;
const uint32 id_swr     = 0x59;
const uint32 id_syscall = 0x5A;
const uint32 id_tlbr    = 0x5B;
const uint32 id_tlbwi   = 0x5C;
const uint32 id_tlbwr   = 0x5D;
const uint32 id_xor     = 0x5E;
const uint32 id_xori    = 0x5F;
const uint32 id_illegal = 0x60;

int Dasm(void);

#endif
