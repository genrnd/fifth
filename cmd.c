#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "fifth.h"
#include "cmd.h"
#include "gcmd.h"

/** push value to stack.
 *  value is stored at program after 'push' command code.
 */
int fifth_push( struct fifth *f )
{
  stack_push( &f->stack, fifth_curr_text( f ) );
  fifth_inc_pc( f );
  return 0;
}

/** remove top item from stack
 */
int fifth_drop( struct fifth *f )
{
  stack_pop( &f->stack );
  return 0;
}

int fifth_add( struct fifth *f )
{
  stack_push( &f->stack, 
              stack_pop( &f->stack ) + stack_pop( &f->stack ) );
  return 0;
}

int fifth_sub( struct fifth *f )
{
  uint8_t b = stack_pop( &f->stack );  
  uint8_t a = stack_pop( &f->stack );

  stack_push( &f->stack, a - b );
  return 0;
}

int fifth_mul( struct fifth *f )
{
  stack_push( &f->stack, 
              stack_pop( &f->stack ) * stack_pop( &f->stack ) );
  return 0;
}

int fifth_inc( struct fifth *f )
{
  stack_push( &f->stack, 
              stack_pop( &f->stack ) + 1 );
  return 0;
}

int fifth_dec( struct fifth *f )
{
  stack_push( &f->stack, 
              stack_pop( &f->stack ) - 1 );
  return 0;
}

int fifth_lt( struct fifth *f )
{
  uint8_t b = stack_pop( &f->stack );
  uint8_t a = stack_pop( &f->stack );

  stack_push( &f->stack, !!( a < b ) );
  return 0;
}

int fifth_gt( struct fifth *f )
{
  uint8_t b = stack_pop( &f->stack );
  uint8_t a = stack_pop( &f->stack );

  stack_push( &f->stack, !!( a > b ) );
  return 0;
}

int fifth_eq( struct fifth *f )
{
  stack_push( &f->stack, 
              !!( stack_pop( &f->stack ) == stack_pop( &f->stack ) ) );
  return 0;
}

int fifth_ne( struct fifth *f )
{
  stack_push( &f->stack, 
              !!( stack_pop( &f->stack ) != stack_pop( &f->stack ) ) );
  return 0;
}

int fifth_cjump( struct fifth *f )
{
  uint8_t new_pc = stack_pop( &f->stack );
  uint8_t val = stack_pop( &f->stack );

  if ( val ) {
    fifth_set_pc( f, new_pc );
  }

  return 0;
}

int fifth_goto( struct fifth *f )
{
  fifth_set_pc( f, stack_pop( &f->stack ) );
  return 0;
}

int fifth_nop( struct fifth *f)
{
  return 0;
}

// a -- a a
int fifth_dup( struct fifth *f )
{
  stack_push( &f->stack, stack_val( &f->stack, 0 ) );
  return 0;
}

// a b -- b a
int fifth_swap( struct fifth *f )
{
  uint8_t a = stack_pop( &f->stack );
  uint8_t b = stack_pop( &f->stack );

  stack_push( &f->stack, a );
  stack_push( &f->stack, b );
  return 0;
}

// a b -- a b a
int fifth_over( struct fifth *f )
{
  uint8_t b = stack_pop( &f->stack );
  uint8_t a = stack_pop( &f->stack );

  stack_push( &f->stack, a );
  stack_push( &f->stack, b );
  stack_push( &f->stack, a );
  return 0;
}

// a b c -- b c a
int fifth_rot( struct fifth *f )
{
  uint8_t c = stack_pop( &f->stack );
  uint8_t b = stack_pop( &f->stack );
  uint8_t a = stack_pop( &f->stack );

  stack_push( &f->stack, b );
  stack_push( &f->stack, c );
  stack_push( &f->stack, a );
  return 0;
}



int fifth_print( struct fifth *f )
{
  printf( "%d\n", stack_val( &f->stack, 0 ) );
  return 0;
}

int fifth_rand( struct fifth *f )
{
  uint8_t a = stack_pop( &f->stack );
  stack_push( &f->stack, rand( ) % a );
  return 0;
}

// addr -- value
int fifth_peek( struct fifth *f )
{
  uint8_t addr = stack_pop( &f->stack );
  stack_push( &f->stack, f->stack.data[addr] );

  return 0;
}

// value addr --
int fifth_poke( struct fifth *f )
{
  uint8_t addr = stack_pop( &f->stack );
  uint8_t value = stack_pop( &f->stack );

  f->stack.data[addr] = value;

  return 0;
}

// addr -- value
int fifth_tpeek( struct fifth *f )
{
  uint8_t addr = stack_pop( &f->stack );
  stack_push( &f->stack, f->text.data[addr] );

  return 0;
}

// value addr --
int fifth_tpoke( struct fifth *f )
{
  uint8_t addr = stack_pop( &f->stack );
  uint8_t value = stack_pop( &f->stack );

  f->text.data[addr] = value;

  return 0;
}

struct fifth_cmd cmds[] = {
  // stack ops
  [0x0] =  { "push",    0x00, 1, 0, fifth_push },
  [0x1] =  { "drop",    0x01, 0, 0, fifth_drop },
  [0x2] =  { "dup",     0x02, 0, 1, fifth_dup },
  [0x3] =  { "swap",    0x03, 0, 2, fifth_swap },
  [0x4] =  { "over",    0x04, 0, 2, fifth_over },
  [0x5] =  { "rot",     0x05, 0, 3, fifth_rot },

  // math
  [0x10] = { "add",     0x10, 0, 2, fifth_add },
  [0x11] = { "mul",     0x11, 0, 2, fifth_mul },
  [0x12] = { "sub",     0x12, 0, 2, fifth_sub },
  [0x13] = { "inc",     0x13, 0, 1, fifth_inc },
  [0x14] = { "dec",     0x14, 0, 1, fifth_dec },

  // comparators
  [0x20] = { "lt",      0x20, 0, 2, fifth_lt },
  [0x21] = { "gt",      0x21, 0, 2, fifth_gt },
  [0x22] = { "eq",      0x22, 0, 2, fifth_eq },
  [0x23] = { "ne",      0x23, 0, 2, fifth_ne },

  // jumps
  [0x30] = { "cjump",   0x30, 0, 2, fifth_cjump },
  [0x31] = { "goto",    0x31, 0, 1, fifth_goto }, 

  // graphical and UI
  [0x40] = { "lrect",   0x40, 0, 3, fifth_lrect },
  [0x41] = { "mrect",   0x41, 0, 3, fifth_mrect },
  [0x42] = { "brect",   0x42, 0, 3, fifth_brect },
  [0x43] = { "fill",    0x43, 0, 1, fifth_fill  },
  [0x44] = { "char",    0x44, 0, 3, fifth_char  },
  [0x45] = { "delay",   0x45, 0, 1, fifth_delay },
  [0x46] = { "led",     0x46, 0, 2, fifth_led   },
  [0x47] = { "sound",   0x47, 0, 2, fifth_sound },

  // other
  [0x50] = { "nop",     0x50, 0, 0, fifth_nop },
  [0x51] = { "print",   0x51, 0, 1, fifth_print }, 
  [0x52] = { "rand",    0x52, 0, 1, fifth_rand }, 

  // ram
  [0x60] = { "peek",    0x60, 0, 1, fifth_peek },
  [0x61] = { "poke",    0x61, 0, 2, fifth_poke },
  [0x62] = { "tpeek",   0x62, 0, 1, fifth_tpeek },
  [0x63] = { "tpoke",   0x63, 0, 2, fifth_tpoke },
};

uint8_t fifth_cmd_num( )
{
  return ARRAY_SIZE( cmds );
}
