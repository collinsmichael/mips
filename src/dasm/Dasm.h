//
// File Name : Dasm.h
// File Date : 15 March 2012
// Author(s) : Valendian
// Purpose   : Disassembles Binary File to MIPS assembler listing.
//

#ifndef DASM_HEADER
#define DASM_HEADER

typedef struct PsxExeHdr {
	uint8_t  id[8];   //
	uint32_t text;    // SCE only
	uint32_t data;    // SCE only
	uint32_t pc0;     //
	uint32_t gp0;     // SCE only
	uint32_t t_addr;  //
	uint32_t t_size;  //
	uint32_t d_addr;  // SCE only
	uint32_t d_size;  // SCE only
	uint32_t b_addr;  // SCE only
	uint32_t b_size;  // SCE only
	uint32_t s_addr;  //
	uint32_t s_size;  //
	uint32_t SavedSP; //
	uint32_t SavedFP; //
	uint32_t SavedGP; //
	uint32_t SavedRA; //
	uint32_t SavedS0; //
} PsxExeHdr;

const uint32_t FlagCode          = 0x80000000; // Byte is marked as code.
const uint32_t FlagData          = 0x40000000; // Byte is marked as data.
const uint32_t FlagFunction      = 0x20000000; // Byte marks the entry point to a function.
const uint32_t FlagLabel         = 0x10000000; // Byte marks the destination of a jump or conditional branch.
const uint32_t FlagUnconditional = 0x08000000;
const uint32_t FlagGreater       = 0x04000000;
const uint32_t FlagEqual         = 0x02000000;
const uint32_t Flaglesser        = 0x01000000;
const uint32_t FlagByte          = 0x00000001; // Byte is marked as byte type.
const uint32_t FlagHalf          = 0x00000002; // Byte is marked as half type.
const uint32_t FlagWord          = 0x00000003; // Byte is marked as word type.


const uint32_t id_add     = 0x01;
const uint32_t id_addi    = 0x02;
const uint32_t id_addiu   = 0x03;
const uint32_t id_addu    = 0x04;
const uint32_t id_and     = 0x05;
const uint32_t id_andi    = 0x06;
const uint32_t id_avsz3   = 0x07;
const uint32_t id_avsz4   = 0x08;
const uint32_t id_beq     = 0x09;
const uint32_t id_bgez    = 0x0A;
const uint32_t id_bgezal  = 0x0B;
const uint32_t id_bgtz    = 0x0C;
const uint32_t id_blez    = 0x0D;
const uint32_t id_bltz    = 0x0E;
const uint32_t id_bltzal  = 0x0F;
const uint32_t id_bne     = 0x10;
const uint32_t id_break   = 0x11;
const uint32_t id_cc      = 0x12;
const uint32_t id_cdp     = 0x13;
const uint32_t id_cfc2    = 0x14;
const uint32_t id_ctc2    = 0x15;
const uint32_t id_div     = 0x16;
const uint32_t id_divu    = 0x17;
const uint32_t id_dpcl    = 0x18;
const uint32_t id_dpcs    = 0x19;
const uint32_t id_dpct    = 0x1A;
const uint32_t id_gpf     = 0x1B;
const uint32_t id_gpl     = 0x1C;
const uint32_t id_intpl   = 0x1D;
const uint32_t id_j       = 0x1E;
const uint32_t id_jal     = 0x1F;
const uint32_t id_jalr    = 0x20;
const uint32_t id_jr      = 0x21;
const uint32_t id_lb      = 0x22;
const uint32_t id_lbu     = 0x23;
const uint32_t id_lh      = 0x24;
const uint32_t id_lhu     = 0x25;
const uint32_t id_lui     = 0x26;
const uint32_t id_lw      = 0x27;
const uint32_t id_lwc1    = 0x28;
const uint32_t id_lwc2    = 0x29;
const uint32_t id_lwl     = 0x2A;
const uint32_t id_lwr     = 0x2B;
const uint32_t id_mfc0    = 0x2C;
const uint32_t id_mfc2    = 0x2D;
const uint32_t id_mfhi    = 0x2E;
const uint32_t id_mflo    = 0x2F;
const uint32_t id_mtc0    = 0x30;
const uint32_t id_mtc2    = 0x31;
const uint32_t id_mthi    = 0x32;
const uint32_t id_mtlo    = 0x33;
const uint32_t id_mult    = 0x34;
const uint32_t id_multu   = 0x35;
const uint32_t id_mvmva   = 0x36;
const uint32_t id_nccs    = 0x37;
const uint32_t id_ncct    = 0x38;
const uint32_t id_ncds    = 0x39;
const uint32_t id_ncdt    = 0x3A;
const uint32_t id_nclip   = 0x3B;
const uint32_t id_ncs     = 0x3C;
const uint32_t id_nct     = 0x3D;
const uint32_t id_nop     = 0x3E;
const uint32_t id_nor     = 0x3F;
const uint32_t id_op      = 0x40;
const uint32_t id_or      = 0x41;
const uint32_t id_ori     = 0x42;
const uint32_t id_rfe     = 0x43;
const uint32_t id_rtps    = 0x44;
const uint32_t id_rtpt    = 0x45;
const uint32_t id_sb      = 0x46;
const uint32_t id_sh      = 0x47;
const uint32_t id_sll     = 0x48;
const uint32_t id_sllv    = 0x49;
const uint32_t id_slt     = 0x4A;
const uint32_t id_slti    = 0x4B;
const uint32_t id_sltiu   = 0x4C;
const uint32_t id_sltu    = 0x4D;
const uint32_t id_sqr     = 0x4E;
const uint32_t id_sra     = 0x4F;
const uint32_t id_srav    = 0x50;
const uint32_t id_srl     = 0x51;
const uint32_t id_srlv    = 0x52;
const uint32_t id_sub     = 0x53;
const uint32_t id_subu    = 0x54;
const uint32_t id_sw      = 0x55;
const uint32_t id_swc1    = 0x56;
const uint32_t id_swc2    = 0x57;
const uint32_t id_swl     = 0x58;
const uint32_t id_swr     = 0x59;
const uint32_t id_syscall = 0x5A;
const uint32_t id_tlbr    = 0x5B;
const uint32_t id_tlbwi   = 0x5C;
const uint32_t id_tlbwr   = 0x5D;
const uint32_t id_xor     = 0x5E;
const uint32_t id_xori    = 0x5F;
const uint32_t id_illegal = 0x60;

int Dasm(void);

#endif
