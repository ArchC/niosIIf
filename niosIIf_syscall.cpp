/**
 * @file      niosIIf_syscall.cpp
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

#include "niosIIf_syscall.H"
#include "ac_resources.H"

//#define DEBUG_SYSC

#ifdef DEBUG_SYSC
#include <stdio.h>
#include <stdarg.h>
inline int dsyscallprintf(const char *format, ...)
{
  int ret;
  va_list args;
  va_start(args, format);
  ret = vfprintf(ac_err, format, args);
  va_end(args);
  return ret;
}
#else
inline void dsyscallprintf(const char *format, ...) {}
#endif

void niosIIf_syscall::get_buffer(int argn, unsigned char* buf, unsigned int size)
{
  dsyscallprintf("niosIIf_syscall::get_buffer begin\n");
  unsigned int addr = RB[4+argn];

  for (unsigned int i = 0; i<size; i++, addr++) {
    buf[i] = DC.read_byte(addr);
  }
  dsyscallprintf("niosIIf_syscall::get_buffer end\n");
}

void niosIIf_syscall::set_buffer(int argn, unsigned char* buf, unsigned int size)
{
  dsyscallprintf("niosIIf_syscall::set_buffer begin\n");
  unsigned int addr = RB[4+argn];

  for (unsigned int i = 0; i<size; i++, addr++) {
    DC.write_byte(addr, buf[i]);
  }
  dsyscallprintf("niosIIf_syscall::set_buffer end\n");
}

void niosIIf_syscall::set_buffer_noinvert(int argn, unsigned char* buf, unsigned int size)
{
  dsyscallprintf("niosIIf_syscall::set_buffer_noinvert begin\n");
  unsigned int addr = RB[4+argn];

  for (unsigned int i = 0; i<size; i+=4, addr+=4) {
    DC.write(addr, *(unsigned int *) &buf[i]);
  }
  dsyscallprintf("niosIIf_syscall::set_buffer_noinvert end\n");
}

int niosIIf_syscall::get_int(int argn)
{
  dsyscallprintf("niosIIf_syscall::get_int\n");
  dsyscallprintf("niosIIf_syscall::get_int:  argn = %d, reg = %d\n", argn, RB[4+argn]);
  return RB[4+argn];
}

void niosIIf_syscall::set_int(int argn, int val)
{
  dsyscallprintf("niosIIf_syscall::set_int\n");
  dsyscallprintf("niosIIf_syscall::set_int: argn = %d, val = %d\n", argn, val);
  RB[2+argn] = val;
}

void niosIIf_syscall::return_from_syscall()
{
  dsyscallprintf("niosIIf_syscall::return_from_syscall\n");
  ac_pc = RB[31];
}

void niosIIf_syscall::set_prog_args(int argc, char **argv)
{
  dsyscallprintf("niosIIf_syscall::set_prog_args begin\n");
  int i, j, base;

  unsigned int ac_argv[30];
  char ac_argstr[512];

  base = AC_RAM_END - 512;
  for (i=0, j=0; i<argc; i++) {
    int len = strlen(argv[i]) + 1;
    ac_argv[i] = base + j;
    memcpy(&ac_argstr[j], argv[i], len);
    j += len;
  }

  //Ajust %sp and write argument string
  RB[27] = AC_RAM_END-512;
  set_buffer(23, (unsigned char*) ac_argstr, 512);   //$23 = $27(sp) - 4 (set_buffer adds 4)

  //Ajust %sp and write string pointers
  RB[27] = AC_RAM_END-512-120;
  set_buffer_noinvert(23, (unsigned char*) ac_argv, 120);

  //Set %sp
  RB[27] = AC_RAM_END-512-128;

  //Set %o0 to the argument count
  RB[4] = argc;

  //Set %o1 to the string pointers
  RB[5] = AC_RAM_END-512-120;

  dsyscallprintf("niosIIf_syscall::set_prog_args end\n");
}
