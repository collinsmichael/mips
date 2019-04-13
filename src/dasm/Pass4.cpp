
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "Main.h"
#include "Dasm.h"

extern uint32_t  Address;
extern uint32_t  Entry;
extern uint32_t  Naming;
extern uint32_t *Code;
extern uint32_t *Flag;
extern uint32_t  Pointer;
extern binary  Binary;

extern uint32_t  Pc;      // program counter register.
extern uint32_t  Opcode;  // current instruction.
extern uint32_t  Op;      // FC000000  6 bits (bit 0x1A to bit 0x1F)
extern uint32_t  Rs;      // 03E00000  5 bits (bit 0x15 to bit 0x19)
extern uint32_t  Format;  // 03E00000  5 bits (bit 0x15 to bit 0x19)
extern uint32_t  Rt;      // 001F0000  5 bits (bit 0x10 to bit 0x14)
extern uint32_t  Ft;      // 001F0000  5 bits (bit 0x10 to bit 0x14)
extern uint32_t  Rd;      // 0000F800  5 bits (bit 0x0B to bit 0x0F)
extern uint32_t  Fs;      // 0000F800  5 bits (bit 0x0B to bit 0x0F)
extern uint32_t  Sa;      // 000007C0  5 bits (bit 0x06 to bit 0x0A)
extern uint32_t  Fd;      // 000007C0  5 bits (bit 0x06 to bit 0x0A)
extern uint32_t  Func;    // 0000003F  6 bits (bit 0x00 to bit 0x05)
extern uint32_t  Target;  // 03FFFFFF 26 bits (bit 0x00 to bit 0x19) added to Pc
extern uint32_t  Imm;     // 0000FFFF 16 bits (bit 0x00 to bit 0x0F)
extern uint32_t  Label;   // 0000FFFF 16 bits (bit 0x00 to bit 0x0F) sign extended and added to Pc
extern uint32_t  SysCall; // 03FFFFC0 20 bits (bit 0x06 to bit 0x1A)
extern uint32_t  Fop;     // 03E0003F concatenation of Func and Format feilds

uint32_t r[32];
uint32_t pc;
uint32_t hi;
uint32_t lo;

int pass4(void) {
	// step through code seperating code from data.
	if (Entry < Address || Entry > Address + Binary.Size) return false;

	for (int i = 0; i < 32; i++) r[i] = 0;
	pc = 0;
	hi = 0;
	lo = 0;
	r[29] = 0x801FFFF0; // set up stack pointer

	uint8_t *RAM = (uint8_t*)Code;

	//80069C6C: L80069C6C:  addiu   r29, r29, 0xFFE8                              # "...'" 27BDFFE8
	for (Pointer = 0; Pointer < Binary.Size; Pointer += sizeof(uint32_t)) {
		if ((Flag[Pointer] & FlagCode) == FlagCode) {
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

			uint32_t uImm = Imm;
			uint32_t sImm = Imm; if (sImm & 0x00008000) sImm |= 0xFFFF0000;

			if (Opcode == 0x03E00008) { // jr r31
				for (int i = 0; i < 32; i++) r[i] = 0;
				pc = 0;
				hi = 0;
				lo = 0;
				r[29] = 0x801FFFF0; // set up stack pointer
			}

			switch (Op) {
           	case 0x08:
				r[Rt] = r[Rs] + sImm;
				break;
				//addi    Rt, Rs, 0x0000
            case 0x09:
				r[Rt] = r[Rs] + uImm;
				break;
				//addiu   %s, %s, 0x0000
           	//case 0x0A: if (r[Rs] < sImm) r[Rt] = 1; else r[Rt] = 0; break; //slti    Rt, Rs, 0x0000
           	//case 0x0B: if (r[Rs] < uImm) r[Rt] = 1; else r[Rt] = 0; break; //sltiu   %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
           	//case 0x0C: //Print("andi    %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
           	//case 0x0D: //Print("ori     %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
           	//case 0x0E: //Print("xori    %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
           	case 0x0F: //lui     Rt, 0x0000
				r[Rt] = sImm << 0x10;
				break;

			case 0x20: //Print("lb      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x24: //Print("lbu     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x28: //Print("sb      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
				if (((r[Rs] + Imm) <  Address) || ((r[Rs] + Imm) >= Address + Binary.Size)) {
					r[Rt] = 0;
					break;
				}
				Flag[r[Rs] + Imm - Address] = (Flag[r[Rs] + Imm - Address] & 0xFFFFFFFC) | FlagLabel | FlagByte;
				r[Rt] = *(uint8_t*)&RAM[r[Rs] + Imm - Address];
				break;
			case 0x21: //Print("lh      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x25: //Print("lhu     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x29: //Print("sh      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
				if (((r[Rs] + Imm) <  Address) || ((r[Rs] + Imm) >= Address + Binary.Size)) {
					r[Rt] = 0;
					break;
				}
				Flag[r[Rs] + Imm - Address] = (Flag[r[Rs] + Imm - Address] & 0xFFFFFFFC) | FlagLabel | FlagHalf;
				r[Rt] = *(uint16_t*)&RAM[r[Rs] + Imm - Address];
				break;
           	
			case 0x23: //Print("lw      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x2B: //Print("sw      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x22: //Print("lwl     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x2A: //Print("swl     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x26: //Print("lwr     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x2E: //Print("swr     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
           	case 0x32: //Print("lwc2    %s, %s(%s)",     Reg(gted, Rt), Number, Reg(cpu, Rs)); break;
           	case 0x3A: //Print("swc2    %s, %s(%s)",     Reg(gted, Rt), Number, Reg(cpu, Rs)); break;
				if (((r[Rs] + Imm) <  Address) || ((r[Rs] + Imm) >= Address + Binary.Size)) break;
				Flag[r[Rs] + Imm - Address] = (Flag[r[Rs] + Imm - Address] & 0xFFFFFFFC) | FlagLabel;
				r[Rt] = *(uint32_t*)&RAM[r[Rs] + Imm - Address];
				break;
           	default:
				break;
			}

















			/*
			switch (Op) {
			case 0x00: //RType
            	switch (Func) {
            	case 0x00:
            		if (Opcode == 0x00000000) //Print("nop"); }
            		else   //Print("sll     %s, %s, 0x%02X", Reg(cpu, Rd), Reg(cpu, Rt), Sa); }
            		break;
            	case 0x02: //Print("srl     %s, %s, 0x%02X", Reg(cpu, Rd), Reg(cpu, Rt), Sa);           break;
            	case 0x03: //Print("sra     %s, %s, 0x%02X", Reg(cpu, Rd), Reg(cpu, Rt), Sa);           break;
            	case 0x04: //Print("sllv    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rt), Reg(cpu, Rs)); break;
            	case 0x06: //Print("srlv    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rt), Reg(cpu, Rs)); break;
            	case 0x07: //Print("srav    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rt), Reg(cpu, Rs)); break;
            	case 0x08: //JrReg = true; Print("jr      %s",             Reg(cpu, Rs));                             break;
            	case 0x09: //Print("jalr    %s, %s",         Reg(cpu, Rs), Reg(cpu, Rd));               break;
            	case 0x0C: //Print("syscall 0x%08X",         SysCall, SysCall);                                  break;
            	case 0x0D: //Print("break   0x%08X",         SysCall, SysCall);                                  break;
            	case 0x10: //Print("mfhi    %s",             Reg(cpu, Rd));                             break;
            	case 0x11: //Print("mthi    %s",             Reg(cpu, Rs));                             break;
            	case 0x12: //Print("mflo    %s",             Reg(cpu, Rd));                             break;
            	case 0x13: //Print("mtlo    %s",             Reg(cpu, Rs));                             break;
            	case 0x18: //Print("mult    %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
            	case 0x19: //Print("multu   %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
            	case 0x1A: //Print("div     %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
            	case 0x1B: //Print("divu    %s, %s",         Reg(cpu, Rs), Reg(cpu, Rt));               break;
            	case 0x20: //Print("add     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x21: //Print("addu    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x22: //Print("sub     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x23: //Print("subu    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x24: //Print("and     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x25: //Print("or      %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x26: //Print("xor     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x27: //Print("nor     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x2A: //Print("slt     %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	case 0x2B: //Print("sltu    %s, %s, %s",     Reg(cpu, Rd), Reg(cpu, Rs), Reg(cpu, Rt)); break;
            	default:   //Print("illegal");                                                          break;
            	}

	        case 0x02: //Print("j       0x%08X", Target, Target); break;
        	case 0x03: //Print("jal     0x%08X", Target, Target); break;
			case 0x10: //Cop_0
            	switch (Format) {
            	default:   //Print("illegal"); break;
            	case 0x00: //Print("mfc0    %s, %s", Reg(cpu, Ft), Reg(cop0, Fs)); break;
            	case 0x04: //Print("mtc0    %s, %s", Reg(cpu, Ft), Reg(cop0, Fs)); break;
            	    break;
            	case 0x10: case 0x11: case 0x12: case 0x13:
            	case 0x14: case 0x15: case 0x16: case 0x17:
            	case 0x18: case 0x19: case 0x1A: case 0x1B:
            	case 0x1C: case 0x1D: case 0x1E: case 0x1F:
            		switch (Func & 0x1F) {
            		case 0x01: //Print("tlbr");    break;
            		case 0x02: //Print("tlbwi");   break;
            		case 0x06: //Print("tlbwr");   break;
            		case 0x10: //Print("rfe");     break;
            		default:   //Print("illegal"); break;
            		    break;
            		}
            		break;
            	}

			case 0x12: //Cop_2
	            switch (Format) {
	            case 0x00: //Print("mfc2    %s, %s", Reg(cpu, Ft), Reg(gted, Fs)); break;
	            case 0x02: //Print("cfc2    %s, %s", Reg(cpu, Ft), Reg(gtec, Fs)); break;
	            case 0x04: //Print("mtc2    %s, %s", Reg(cpu, Ft), Reg(gted, Fs)); break;
	            case 0x06: //Print("ctc2    %s, %s", Reg(cpu, Ft), Reg(gtec, Fs)); break;
	            case 0x11: //Print("rtpt");  break;
	            case 0x1C: //Print("gpf");   break;
	            case 0x1D: //Print("gpl");   break;
	            case 0x14: //Print("intpl"); break;
	            case 0x15: //Print("sqr");   break;
	                break;
	            default:
	            	switch (Fop) {
	            	case 0x401: //Print("rtps");    break;
	            	case 0x406: //Print("nclip");   break;
	            	case 0x40C: //Print("op");      break;
	            	case 0x410: //Print("dpcs");    break;
	            	case 0x412: //Print("mvmva");   break;
	            	case 0x413: //Print("ncds");    break;
	            	case 0x414: //Print("cdp");     break;
	            	case 0x416: //Print("ncdt");    break;
	            	case 0x41B: //Print("nccs");    break;
	            	case 0x41C: //Print("cc");      break;
	            	case 0x41E: //Print("ncs");     break;
	            	case 0x420: //Print("nct");     break;
	            	case 0x429: //Print("dpcl");    break;
	            	case 0x42A: //Print("dpct");    break;
	            	case 0x42D: //Print("avsz3");   break;
	            	case 0x42E: //Print("avsz4");   break;
	            	case 0x43F: //Print("ncct");    break;
	            	default:    //Print("illegal"); break;
	            	    break;
	            	}
	            }

			default:   //IType
            	switch (Op) {
            	case 0x01:
            		if      (Rt == 0x00) //Print("bltz    %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
            		else if (Rt == 0x01) //Print("bgez    %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
            		else if (Rt == 0x10) //Print("bltzal  %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
            		else if (Rt == 0x11) //Print("bgezal  %s, 0x%08X", Reg(cpu, Rs), Label, Label); }
            		else                 //Print("illegal"); }
            		break;
            	case 0x04: //Print("beq     %s, %s, 0x%08X", Reg(cpu, Rs), Reg(cpu, Rt), Label, Label); break;
            	case 0x05: //Print("bne     %s, %s, 0x%08X", Reg(cpu, Rs), Reg(cpu, Rt), Label, Label); break;
            	case 0x06: //Print("blez    %s, 0x%08X",     Reg(cpu, Rs), Label, Label);                               break;
            	case 0x07: //Print("bgtz    %s, 0x%08X",     Reg(cpu, Rs), Label, Label);                               break;
            	case 0x08: //Print("addi    %s, %s, %s",     Reg(cpu, Rt), Reg(cpu, Rs), Number);  break;
            	case 0x09: //Print("addiu   %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
            	case 0x0A: //Print("slti    %s, %s, %s",     Reg(cpu, Rt), Reg(cpu, Rs), Number);  break;
            	case 0x0B: //Print("sltiu   %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
            	case 0x0C: //Print("andi    %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
            	case 0x0D: //Print("ori     %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
            	case 0x0E: //Print("xori    %s, %s, 0x%04X", Reg(cpu, Rt), Reg(cpu, Rs), Imm);     break;
            	case 0x0F: //Print("lui     %s, 0x%04X",     Reg(cpu, Rt), Imm);                   break;
            	case 0x20: //Print("lb      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x21: //Print("lh      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x22: //Print("lwl     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x23: //Print("lw      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x24: //Print("lbu     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x25: //Print("lhu     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x26: //Print("lwr     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x28: //Print("sb      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x29: //Print("sh      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x2A: //Print("swl     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x2B: //Print("sw      %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x2E: //Print("swr     %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	case 0x32: //Print("lwc2    %s, %s(%s)",     Reg(gted, Rt), Number, Reg(cpu, Rs)); break;
            	case 0x3A: //Print("swc2    %s, %s(%s)",     Reg(gted, Rt), Number, Reg(cpu, Rs)); break;
            //	case 0x31: //Print("lwc1    %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            //	case 0x39: //Print("swc1    %s, %s(%s)",     Reg(cpu, Rt), Number, Reg(cpu, Rs));  break;
            	default:   //Print("illegal"); break;
            	}
			}//*/
		}
	}
	return true;
}
