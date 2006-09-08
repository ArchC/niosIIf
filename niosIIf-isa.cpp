/**
 * @file      niosIIf-isa.cpp
 * @author    Richard Maciel
 *            Guilherme Quentel Melo
 *
 * @author    System Design Automation Lab (LAPS)
 *            INE-UFSC
 *            http://www.laps.inf.ufsc.br
 *
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   1.0
 * @date      Fri, 08 Sep 2006 00:53:24 -0300
 *
 * @brief     The ArchC Nios II functional model
 *
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 *
 */

#include  "niosIIf-isa.H"
#include  "ac_isa_init.cpp"
#include "ac_parms.H"

using namespace std;

//#define DEBUG_MODEL

#ifdef DEBUG_MODEL
#include <stdio.h>
#include <stdarg.h>
inline int dprintf(const char *format, ...)
{
  int ret;
  va_list args;
  va_start(args, format);
  ret = vfprintf(ac_err, format, args);
  va_end(args);
  return ret;
}

inline void print_binary(int c){
	int i;
	//int c0 = *(&c - 4);

	for(i = 0; i < 32; i++){
		if ((c << i) & 0x80000000){
			printf("1");
		}
		else{
			printf("0");
		}
		if(((i + 1) % 8) == 0) printf(" ");
	}
	printf("\n");
}
#else
inline void dprintf(const char *format, ...) {}
inline void print_binary(int c) {}
#endif

// Useful constants
#define BA 30 // Breakpoint return address
#define EA 29 // Exception return address
#define RA 31 // Return address
#define MAXNEGINT -2147483648 // Maximum negative integer
#define EADDR 0x00F0 //AC_RAM_END // Exception address
#define BADDR 0x00F0 //AC_RAM_END // Break address
#define ZERO 0 // Register zero

// Rotate word n positions. Positive numbers rotate left,
// negative rotate left.
ac_Uword rot_word( ac_Uword word, ac_Uword n, unsigned char left = 1)
{
  ac_Uword temp = word;
  
  // Rotate with the 5 first bits.
  n &= 0x1F;
  
  if(left)
  {
    temp <<= n;
    word >>= AC_WORDSIZE - n;
    word |= temp;
  }
  else
  {
    temp >>= n;
    word <<= AC_WORDSIZE - n;
    word |= temp;
  }

  return word;
}
 
//!Generic instruction behavior method.
void ac_behavior( instruction )
{
  dprintf("----- PC=%#x ----- %lld\n", (int) ac_pc, ac_instr_counter);
  ac_pc += 4;
};

//!Behavior called before starting simulation
void ac_behavior(begin)
{
  dprintf("@@@ begin behavior @@@\n");
  RB.write(ZERO, 0);
}

//!Behavior called after finishing simulation
void ac_behavior(end)
{
  dprintf("@@@ end behavior @@@\n");
}
 
//! Instruction Format behavior methods.
void ac_behavior( Type_I ){}
void ac_behavior( Type_R ){}
void ac_behavior( Type_J ){}
void ac_behavior( Type_C ){}
 
//!Instruction addi behavior method.
void ac_behavior( addi )
{
  dprintf("addi r%d, r%d, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.write(rB, RB.read(rA) + (ac_Sword)imm16);
  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction andhi behavior method.
void ac_behavior( andhi )
{
  dprintf("andhi r%d, r%d, %d\n", rB, rA, imm16 << AC_WORDSIZE/2);
  RB.write(rB, RB.read(rA) & (imm16 << AC_WORDSIZE/2));
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction andi behavior method.
void ac_behavior( andi )
{
  dprintf("andi r%d, r%d, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.write(rB, RB.read(rA) & (imm16 & 0xFFFF));
  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction beq behavior method.
void ac_behavior( beq )
{ 
  dprintf("beq r%u, r%u, %d\n", rA, rB, imm16 & 0xFFFF);
  if(RB.read(rA) == RB.read(rB))
  {
    ac_pc = ac_pc + (ac_Sword)imm16, 1;
    //addr_align_test(ac_pc);
  }
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction bge behavior method.
void ac_behavior( bge )
{
  ac_Sword v1 = RB.read(rA);
  ac_Sword v2 = RB.read(rB);

  dprintf("bge r%d, r%d, %d\n", rA, rB, imm16 & 0xFFFF);

  if(v1 >= v2)
  {
    ac_pc = ac_pc + (ac_Sword)imm16, 1;
    //addr_align_test(ac_pc);
  }
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction bgeu behavior method.
void ac_behavior( bgeu )
{
  dprintf("bgeu r%u, r%u, %d\n", rA, rB, imm16 & 0xFFFF);
  if(RB.read(rA) >= RB.read(rB))
  {
    ac_pc = ac_pc + (ac_Sword)imm16, 1;
//     addr_align_test(ac_pc);
  }
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction blt behavior method.
void ac_behavior( blt )
{
  ac_Sword v1 = RB.read(rA);
  ac_Sword v2 = RB.read(rB);

  dprintf("blt r%u, r%u, %d\n", rA, rB, imm16 & 0xFFFF);
  if(v1 < v2)
  {
    ac_pc = ac_pc + (ac_Sword)imm16, 1;
//     addr_align_test(ac_pc);
  }
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction bltu behavior method.
void ac_behavior( bltu )
{
  dprintf("bltu r%u, r%u, %d\n", rA, rB, imm16 & 0xFFFF);
  if(RB.read(rA) < RB.read(rB))
  {
    ac_pc = ac_pc + (ac_Sword)imm16, 1;
//     addr_align_test(ac_pc);
  }
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction bne behavior method.
void ac_behavior( bne )
{
  dprintf("bne r%u, r%u, %d\n", rA, rB, imm16 & 0xFFFF);
  if(RB.read(rA) != RB.read(rB))
  {
    ac_pc = ac_pc + (ac_Sword)imm16, 1;
//     addr_align_test(ac_pc);
  }
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction br behavior method.
void ac_behavior( br )
{
  dprintf("br %d\n", imm16 & 0xFFFF);

  //Detects a infinite loop. Altera's linker adds this in its binaries.
  if(imm16 != -4){
	ac_pc = ac_pc + (ac_Sword)imm16;
//   	addr_align_test(ac_pc);
  }
  //Stop simulation.
  else ac_pc = -1;

  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction cmplti behavior method.
void ac_behavior( cmplti )
{
  ac_Sword v1 = RB.read(rA);

  dprintf("cmplti r%u, r%u, %d\n", rB, rA, imm16 & 0xFFFF);
  v1 < (ac_Sword)imm16 ? RB.write(rB, 1):RB.write(rB, 0);
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction flushd behavior method.
void ac_behavior( flushd )
{
  /*ac_Uword addr = RB.read(rA) + (ac_Sword)imm16;

  // Copy datum from cache to memory.
  dprintf("flushd %d(r%u)\n", rA, imm16 & 0xFFFF);
  DC.write(addr, DC.read(addr));
  dprintf("Mem value = %#x\n", DC.read(addr));
  // No valid bit reset!! - Must be done.*/
}

//!Instruction initd behavior method.
void ac_behavior( initd )
{
  // A fazer
}

//!Instruction ldb behavior method.
void ac_behavior( ldb )
{
  ac_Sword regv = RB.read(rA);
  ac_Sword bv; 

  dprintf("ldb r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);

  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF;
    bv = (ac_Sword)DC.read_byte(regv + (ac_Sword)imm16);
  }
  else*/
    bv = (ac_Sword)DC.read_byte(regv + (ac_Sword)imm16);

  if(bv & 0x80) // Sign extend
    bv |= 0xFFFFFF00;

  RB.write(rB, bv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldbio behavior method.
void ac_behavior( ldbio )
{
  ac_Sword regv = RB.read(rA);
  ac_Sword bv; 

  dprintf("ldbio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.

  bv = DC.read_byte(regv + (ac_Sword)imm16);

  if(bv & 0x80) // Sign extend
    bv |= 0xFFFFFF00;

  RB.write(rB, bv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldbu behavior method.
void ac_behavior( ldbu )
{
  ac_Sword regv = RB.read(rA);
  ac_Uword bv; 

  dprintf("ldbu r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF; // Clear bit 31.
    bv = DC.read_byte(regv + (ac_Sword)imm16);
  }
  else*/
    bv = DC.read_byte(regv + (ac_Sword)imm16);

  RB.write(rB, bv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldbuio behavior method.
void ac_behavior( ldbuio )
{
  ac_Sword regv = RB.read(rA);
  ac_Uword bv; 

  dprintf("ldbuio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.
  bv = DC.read_byte(regv + (ac_Sword)imm16);

  RB.write(rB, bv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldh behavior method.
void ac_behavior( ldh )
{
  ac_Sword regv = RB.read(rA);
  ac_Sword hwv; 

  dprintf("ldh r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF; // Clear bit 31.
    hwv = DC.read_half(regv + (ac_Sword)imm16);  
  }  
  else*/
    hwv = DC.read_half(regv + (ac_Sword)imm16);

  if(hwv & 0x8000) // Sign extend
    hwv |= 0xFFFF0000;

  RB.write(rB, hwv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldhio behavior method.
void ac_behavior( ldhio )
{
  ac_Sword regv = RB.read(rA);
  ac_Sword hwv; 

  dprintf("ldhio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.
  hwv = DC.read_half(regv + (ac_Sword)imm16);

  if(hwv & 0x8000) // Sign extend
    hwv |= 0xFFFF0000;

  RB.write(rB, hwv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldhu behavior method.
void ac_behavior( ldhu )
{
  ac_Sword regv = RB.read(rA);
  ac_Uword hwv;

  dprintf("ldhu r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF; // Clear bit 31.
    hwv = DC.read_half(regv + (ac_Sword)imm16);
  }
  else*/
    hwv = DC.read_half(regv + (ac_Sword)imm16);


  RB.write(rB, hwv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldhuio behavior method.
void ac_behavior( ldhuio )
{
  ac_Sword regv = RB.read(rA);
  ac_Uword hwv; 

  dprintf("ldhuio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.
  hwv = DC.read_half(regv + (ac_Sword)imm16);

  RB.write(rB, hwv);

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldw behavior method.
void ac_behavior( ldw )
{
  ac_Sword regv = RB.read(rA);

  dprintf("ldw r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF;
    RB.write(rB, DC.read(regv + (ac_Sword)imm16));
  }
  else*/
    RB.write(rB, DC.read(regv + (ac_Sword)imm16));

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction ldwio behavior method.
void ac_behavior( ldwio )
{
  ac_Sword regv = RB.read(rA);

  dprintf("ldwio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);
  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.

  RB.write(rB, DC.read(regv + (ac_Sword)imm16));

  dprintf("Result = %#x\n", RB.read(rB));
  dprintf("Result = %d\n", RB.read(rB));
}

//!Instruction stb behavior method.
void ac_behavior( stb )
{
  ac_Sword regv = RB.read(rA);

  dprintf("stb r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);

  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF; // Clear bit 31.
    DC.write_byte(regv + (ac_Sword)imm16, RB.read(rB));
    dprintf("Mem: %#x\n", DC.read_byte(regv + (ac_Sword)imm16));
  }
  else
  {*/
    DC.write_byte(regv + (ac_Sword)imm16, RB.read(rB));
    dprintf("Mem: %#x\n", DC.read_byte(regv + (ac_Sword)imm16));
  //}
}

//!Instruction stbio behavior method.
void ac_behavior( stbio )
{
  ac_Sword regv = RB.read(rA);

  dprintf("stbio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);

  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.
  DC.write_byte(regv + (ac_Sword)imm16, RB.read(rB));
  dprintf("Mem: %#x\n", DC.read_byte(regv + (ac_Sword)imm16));
}

//!Instruction sth behavior method.
void ac_behavior( sth )
{
  ac_Sword regv = RB.read(rA);

  dprintf("sth r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);

  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF; // Clear bit 31.
    DC.write_half(regv + (ac_Sword)imm16, RB.read(rB));
    dprintf("Mem: %#x\n", DC.read_half(regv + (ac_Sword)imm16));
  }
  else
  {*/
    DC.write_half(regv + (ac_Sword)imm16, RB.read(rB));
    dprintf("Mem: %#x\n", DC.read_half(regv + (ac_Sword)imm16));
  //}
}

//!Instruction sthio behavior method.
void ac_behavior( sthio )
{
  ac_Sword regv = RB.read(rA);

  dprintf("sthio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);

  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.
  DC.write_half(regv + (ac_Sword)imm16, RB.read(rB));
  dprintf("Mem: %#x\n", DC.read_half(regv + (ac_Sword)imm16));
}

//!Instruction stw behavior method.
void ac_behavior( stw )
{
  ac_Sword regv = RB.read(rA);

  dprintf("stw r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);

  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  // If 32th bit is set, bypass dcache.
  /*if(regv & 0x80000000)
  {
    regv &= 0x7FFFFFFF;
    DC.write(regv + (ac_Sword)imm16, RB.read(rB));
    dprintf("Mem: %#x\n", DC.read(regv + (ac_Sword)imm16));
  }
  else
  {*/
    DC.write(regv + (ac_Sword)imm16, RB.read(rB));
    dprintf("Mem: %#x\n", DC.read(regv + (ac_Sword)imm16));
  //}

}

//!Instruction stwio behavior method.
void ac_behavior( stwio )
{
  ac_Sword regv = RB.read(rA);

  dprintf("stwio r%d, %hd(r%d)\n", rB, (ac_SHword)(imm16 & 0xFFFF), rA);

  dprintf("Address: %#x\n", regv + (ac_Sword)imm16);

  //regv &= 0x7FFFFFFF; // Clear bit 31.
  DC.write(regv + (ac_Sword)imm16, RB.read(rB));
  dprintf("Mem: %#x\n", DC.read(regv + (ac_Sword)imm16));
}

//!Instruction xorhi behavior method.
void ac_behavior( xorhi )
{
  dprintf("xorhi r%d, r%d, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.write(rB, RB.read(rA) ^ (imm16 << AC_WORDSIZE/2));
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction xori behavior method.
void ac_behavior( xori )
{
  dprintf("xori r%d, r%d, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.write(rB, RB.read(rA) ^ (imm16 & 0xFFFF));
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction add behavior method.
void ac_behavior( add )
{
  dprintf("add r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, RB.read(rA) + RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
  dprintf("Result = %d\n", RB.read(rC));
}

//!Instruction and behavior method.
void ac_behavior( And )
{
  dprintf("and r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, RB.read(rA) & RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction break behavior method.
void ac_behavior( Break )
{
  // Saves status register, disables interruptions
  // and goes to break address
  dprintf("break\n");

  bstatus_reg_user = status_reg_user;
  bstatus_reg_pie = status_reg_pie;
  status_reg_pie = 0;
  RB.write(BA, ac_pc);
  ac_pc = BADDR;

  dprintf("ba: %#x\n", RB.read(BA));
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction bret behavior method.
void ac_behavior( bret )
{
  dprintf("bret\n");
  // Restores status register and load return address to pc.
  status_reg_user = bstatus_reg_user;
  status_reg_pie = bstatus_reg_pie;
  //status_reg_pie = 0;
  ac_pc = RB.read(BA);

  dprintf("New address = %#x", ac_pc.read());
}

//!Instruction callr behavior method.
void ac_behavior( callr )
{
  dprintf("callr r%d\n", rA);
  RB.write(RA, ac_pc);
  ac_pc = RB.read(rA), 1;
  dprintf("r31: %#x\n", RB.read(RA));
  dprintf("New address = %#x", ac_pc.read());
}

//!Instruction cmpeq behavior method.
void ac_behavior( cmpeq )
{
  dprintf("cmpeq r%d, r%d, r%d\n", rC, rA, rB);
  RB.read(rA) == RB.read(rB) ? RB.write(rC, 1):RB.write(rC, 0);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction cmpeqi behavior method.
void ac_behavior( cmpeqi )
{
  dprintf("cmpeqi r%u, r%u, %d\n", rB, rA, imm16 & 0xFFFF);
  (ac_Sword)RB.read(rA) == (ac_Sword)imm16 ? RB.write(rB, 1):RB.write(rB, 0);
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction cmpge behavior method.
void ac_behavior( cmpge )
{
  ac_Sword v1 = RB.read(rA);
  ac_Sword v2 = RB.read(rB);

  dprintf("cmpge r%d, r%d, r%d\n", rC, rA, rB);
  v1 >= v2 ? RB.write(rC, 1):RB.write(rC, 0);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction cmpgei behavior method.
void ac_behavior( cmpgei )
{
  ac_Sword v1 = RB.read(rA);

  dprintf("cmpgei r%u, r%u, %d\n", rB, rA, imm16 & 0xFFFF);
  v1 >= imm16 ? RB.write(rB, 1):RB.write(rB, 0);
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction cmpgeu behavior method.
void ac_behavior( cmpgeu )
{
  dprintf("cmpgeu r%d, r%d, r%d\n", rC, rA, rB);
  RB.read(rA) >= RB.read(rB) ? RB.write(rC, 1):RB.write(rC, 0);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction cmpgeui behavior method.
void ac_behavior( cmpgeui )
{
  dprintf("cmpgeui r%u, r%u, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.read(rA) >= (ac_Uword)imm16 ? RB.write(rB, 1):RB.write(rB, 0);
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction cmplt behavior method.
void ac_behavior( cmplt )
{
  ac_Sword v1 = RB.read(rA);
  ac_Sword v2 = RB.read(rB);

  dprintf("cmplt r%d, r%d, r%d\n", rC, rA, rB);
  v1 < v2 ? RB.write(rC, 1):RB.write(rC, 0);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction cmpltu behavior method.
void ac_behavior( cmpltu )
{
  dprintf("cmpltu r%d, r%d, r%d\n", rC, rA, rB);
  RB.read(rA) < RB.read(rB) ? RB.write(rC, 1):RB.write(rC, 0);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction cmpltui behavior method.
void ac_behavior( cmpltui )
{
  dprintf("cmpltui r%u, r%u, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.read(rA) < (ac_Uword)imm16 ? RB.write(rB, 1):RB.write(rB, 0);
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction cmpne behavior method.
void ac_behavior( cmpne )
{
  dprintf("cmpne r%d, r%d, r%d\n", rC, rA, rB);
  RB.read(rA) != RB.read(rB) ? RB.write(rC, 1):RB.write(rC, 0);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction cmpnei behavior method.
void ac_behavior( cmpnei )
{
  dprintf("cmpnei r%u, r%u, %d\n", rB, rA, imm16 & 0xFFFF);
  (ac_Sword)RB.read(rA) != (ac_Sword)imm16 ? RB.write(rB, 1):RB.write(rB, 0);
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction custom behavior method.
void ac_behavior( custom )
{
  // Cada implementacao define a sua propria instrucao.
}

//!Instruction div behavior method.
void ac_behavior( div )
{
  ac_Sword v1 = RB.read(rA);
  ac_Sword v2 = RB.read(rB);

  dprintf("div r%d, r%d, r%d\n", rC, rA, rB);

  if(v2 == 0)
    return;
  else if(v1 == (ac_Sword)(MAXNEGINT) && v2 == (ac_Sword)(-1))
    return;

  RB.write(rC,  v1 / v2);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction divu behavior method.
void ac_behavior( divu )
{

  dprintf("divu r%d, r%d, r%d\n", rC, rA, rB);

  if(RB.read(rB) == 0)
    return;

  RB.write(rC, RB.read(rA) / RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction eret behavior method.
void ac_behavior( eret )
{
  dprintf("eret\n");
  // Returns from exception address. 
  // Copies estatus_reg to status_reg_
  status_reg_user = estatus_reg_user;
  status_reg_pie = estatus_reg_pie;
  //status_reg_pie = 0;
  ac_pc = RB.read(EA);

  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction flushi behavior method.
void ac_behavior( flushi )
{
  // A fazer
}

//!Instruction flushp behavior method.
void ac_behavior( flushp )
{
  // Models doesn't have a pipeline, yet!
}

//!Instruction initi behavior method.
void ac_behavior( initi )
{
  // A fazer
}

//!Instruction jmp behavior method.
void ac_behavior( jmp )
{
  dprintf("jmp r%d\n", rA);
//   addr_align_test(RB.read(rA));
  ac_pc = RB.read(rA);
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction mul behavior method.
void ac_behavior( mul )
{
  ac_UDword result = RB.read(rA) * RB.read(rB);

  dprintf("mul r%u, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, (ac_Uword)result);  
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction muli behavior method.
void ac_behavior( muli )
{
  ac_Sword v1 = imm16; // Sign-extend imm16 to 32 bits.
  ac_UDword result = RB.read(rA) * v1;

  dprintf("muli r%u, r%u, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.write(rB, (ac_Uword)result);
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction mulxss behavior method.
void ac_behavior( mulxss )
{
  ac_SDword v1 = RB.read(rA);
  ac_SDword v2 = RB.read(rB);

  dprintf("rA: %#x\n", (ac_Sword)RB.read(rA));
  dprintf("rB: %#x\n", (ac_Sword)RB.read(rB));

  if(v1 & 0x80000000) // Sign-extend to 64 bits
    v1 |= (ac_SDword)0xFFFFFFFF << 32;
  if(v2 & 0x80000000) // Sign-extend to 64 bits
    v2 |= (ac_SDword)0xFFFFFFFF << 32;

  dprintf("v1: %#llx\n", v1);
  dprintf("v2: %#llx\n", v2);

  ac_SDword result =  v1 * v2;

  dprintf("mulxss r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, result >> AC_WORDSIZE);
  dprintf("Result64 = %#llx\n", result);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction mulxsu behavior method.
void ac_behavior( mulxsu )
{
  ac_SDword v1;
  ac_UDword v2;

  dprintf("rA: %#x\n", RB.read(rA));
  dprintf("rB: %#x\n", RB.read(rB));

  v1 = RB.read(rA);
  v2 = RB.read(rB);

  if(v1 & 0x80000000) // Sign-extend to 64 bits
    v1 |= (ac_SDword)0xFFFFFFFF << 32;
  
  dprintf("v1: %#llx\n", v1);
  dprintf("v2: %#llx\n", v2);

  ac_SDword result = v1 * v2;
  
  dprintf("mulxsu r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, result >> AC_WORDSIZE);
  dprintf("Result64 = %#llx\n", result);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction mulxuu behavior method.
void ac_behavior( mulxuu )
{
  ac_UDword v1, v2;

  dprintf("rA: %#x\n", RB.read(rA));
  dprintf("rB: %#x\n", RB.read(rB));

  v1 = RB.read(rA);
  v2 = RB.read(rB);

  dprintf("v1: %#llx\n", v1);
  dprintf("v2: %#llx\n", v2);

  ac_UDword result = v1 * v2;

  dprintf("mulxuu r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, result >> AC_WORDSIZE);
  dprintf("Result64 = %#llx\n", result);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction nextpc behavior method.
void ac_behavior( nextpc )
{
  dprintf("nextpc r%d\n", rC);
  RB.write(rC, ac_pc);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction nor behavior method.
void ac_behavior( nor )
{
  dprintf("nor r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, ~(RB.read(rA) | RB.read(rB)));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction or behavior method.
void ac_behavior( Or )
{
  dprintf("or r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, RB.read(rA) | RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction orhi behavior method.
void ac_behavior( orhi )
{
  dprintf("orhi r%d, r%d, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.write(rB, RB.read(rA) | (imm16 << (AC_WORDSIZE / 2)));
  dprintf("Result = %#x\n", RB.read(rB));
}

//!Instruction ori behavior method.
void ac_behavior( ori )
{
  dprintf("ori r%d, r%d, %d\n", rB, rA, imm16 & 0xFFFF);
  RB.write(rB, RB.read(rA) | (imm16 & 0xFFFF));
  dprintf("Result = %#x\n", RB.read(rB));

}

//!Instruction rdctl behavior method.
void ac_behavior( rdctl )
{
  // Reads control register to a GPR.
  
  dprintf("rdctl r%d, %d\n", rC, imm5);
  
  switch(imm5)
  {
  case 0:
    // Reads status_reg
    RB.write(rC, status_reg_user.read() << 1 | status_reg_pie.read());
    break;
  case 1:
    RB.write(rC, estatus_reg_user.read() << 1 | estatus_reg_pie.read());
    break;
  case 2:
    RB.write(rC, bstatus_reg_user.read() << 1 | bstatus_reg_pie.read());
    break;
  case 3:
    RB.write(rC, ienable_reg.read());
    break;
  case 4:
    RB.write(rC, ipending_reg.read());
    break;
  }

  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction ret behavior method.
void ac_behavior( ret )
{
  dprintf("ret\n");
  ac_pc = RB.read(RA);
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction rol behavior method.
void ac_behavior( rol )
{
  dprintf("rol r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, rot_word(RB.read(rA), RB.read(rB)));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction roli behavior method.
void ac_behavior( roli )
{
  dprintf("roli r%d, r%d, %d\n", rC, rA, imm5);
  RB.write(rC, rot_word(RB.read(rA), imm5));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction ror behavior method.
void ac_behavior( ror )
{
  dprintf("ror r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, rot_word(RB.read(rA), RB.read(rB), 0));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction sll behavior method.
void ac_behavior( sll )
{
  dprintf("sll r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, RB.read(rA) << RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction slli behavior method.
void ac_behavior( slli )
{
  dprintf("slli r%d, r%d, %d\n", rC, rA, imm5);
  RB.write(rC, RB.read(rA) << imm5);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction sra behavior method.
void ac_behavior( sra )
{
  ac_Sword temp = RB.read(rA);

  dprintf("sra r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, temp >> RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction srai behavior method.
void ac_behavior( srai )
{
  ac_Sword temp = RB.read(rA);

  dprintf("srai r%d, r%d, %d\n", rC, rA, imm5);
  RB.write(rC, temp >> imm5);
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction srl behavior method.
void ac_behavior( srl )
{
  dprintf("srl r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, RB.read(rA) >> RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction srli behavior method.
void ac_behavior( srli )
{
  dprintf("srli r%d, r%d, %d\n", rC, rA, imm5);
  RB.write(rC, RB.read(rA) >> imm5);
  dprintf("Result = %#x\n", RB.read(rC));  
}

//!Instruction sub behavior method.
void ac_behavior( sub )
{
  dprintf("sub r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, RB.read(rA) - RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
  dprintf("Result = %d\n", RB.read(rC));
}

//!Instruction sync behavior method.
void ac_behavior( sync )
{
  // Supports only in-order memory access.
  // Do no operation.
}

//!Instruction trap behavior method.
void ac_behavior( trap )
{
  // Saves status register, disable interrupts
  // and goes to exception address.
  dprintf("trap\n");
  
  estatus_reg_user = status_reg_user;
  estatus_reg_pie = status_reg_pie;
  status_reg_pie = 0;
  RB.write(EA, ac_pc);
  ac_pc = EADDR;

  dprintf("ea: %#x\n", RB.read(EA));
  dprintf("New address = %#x\n", ac_pc.read());
}

//!Instruction wrctl behavior method.
void ac_behavior( wrctl )
{
  // Writes a register to a control register.
  
  dprintf("wrctl %d, r%d\n", imm5, rA);
  
  switch(imm5)
  {
  case 0:
    // Reads status_reg
    status_reg_user.write(RB.read(rA) >> 1 & 0x1);
    status_reg_pie.write(RB.read(rA) & 0x1);
    dprintf("Status register: user=%d, pie=%d\n", status_reg_user.read(), status_reg_pie.read());
    break;
  case 1:
    estatus_reg_user.write(RB.read(rA) >> 1 & 0x1);
    estatus_reg_pie.write(RB.read(rA) & 0x1);
    dprintf("Exception register: user=%d, pie=%d\n", estatus_reg_user.read(), estatus_reg_pie.read());
    break;
  case 2:
    bstatus_reg_user.write(RB.read(rA) >> 1 & 0x1);
    bstatus_reg_pie.write(RB.read(rA) & 0x1);
    dprintf("Break register: user=%d, pie=%d\n", bstatus_reg_user.read(), bstatus_reg_pie.read());
    break;
  case 3:
    ienable_reg.write(RB.read(rA));
    dprintf("Interrupt Enable: %#x\n", ienable_reg.read());
    break;
  case 4:
    ipending_reg.write(RB.read(rA));
    dprintf("Interrupt Pending: %#x\n", ipending_reg.read());
    break;
  }

  
}

//!Instruction xor behavior method.
void ac_behavior( Xor )
{
  dprintf("xor r%d, r%d, r%d\n", rC, rA, rB);
  RB.write(rC, RB.read(rA) ^ RB.read(rB));
  dprintf("Result = %#x\n", RB.read(rC));
}

//!Instruction call behavior method.
void ac_behavior( call )
{
  ac_Uword pc_31_28 = ac_pc & 0xF0000000;
  ac_Uword pc_27_0 = imm26 << 2; // Always aligned.

  dprintf("call %d\n", imm26);
  RB.write(31, ac_pc);
  ac_pc = pc_31_28 | pc_27_0;
  dprintf("ra: %#x\n", RB.read(RA));
  dprintf("New address = %#x\n", ac_pc.read());
}

