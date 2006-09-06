#include "niosIIf.H"

//#define DEBUG_GDB

#ifdef DEBUG_GDB
#include <stdio.h>
#include <stdarg.h>
inline int dgdb_printf(const char *format, ...)
{
  int ret;
  va_list args;
  va_start(args, format);
  ret = vfprintf(ac_err, format, args);
  va_end(args);
  return ret;
}
#else
inline void dgdb_printf(const char *format, ...) {}
#endif

int niosIIf::nRegs(void) {
  dgdb_printf("niosIIf::nRegs\n"); 
  return 37;
}

ac_word niosIIf::reg_read( int reg ) {
  dgdb_printf("niosIIf::reg_read begin\n"); 
  /* general purpose registers */
  if ( ( reg >= 0 ) && ( reg < 32 ) )
    return RB.read( reg );
  else {
    if ( ( reg >= 33 ) && ( reg < 35 ) )
      return 0; //TODO
    else
      /* pc */
      if ( reg == 37 )
        return ac_resources::ac_pc;
  }
  dgdb_printf("niosIIf::reg_read end\n"); 
  return 0;
}


void niosIIf::reg_write( int reg, ac_word value ) {
  dgdb_printf("niosIIf::reg_write begin\n"); 
  /* general purpose registers */
  if ( ( reg >= 0 ) && ( reg < 32 ) )
    RB.write( reg, value );
  else
    {
      /* lo, hi */
      if ( ( reg >= 33 ) && ( reg < 35 ) )
        {}//RB.write( reg - 1, value ); //TODO
      else
        /* pc */
        if ( reg == 37 )
          ac_resources::ac_pc = value;
    }
  dgdb_printf("niosIIf::reg_write end\n"); 
}


unsigned char niosIIf::mem_read( unsigned int address ) {
  dgdb_printf("niosIIf::mem_read\n"); 
  return IM->read_byte( address );
  return 0;
}


void niosIIf::mem_write( unsigned int address, unsigned char byte ) {
  dgdb_printf("niosIIf::mem_write\n"); 
  IM->write_byte( address, byte );
}
