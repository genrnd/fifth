#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "fifth.h"
#include "stack.h"
#include "cmd.h"

void fifth_inc_pc( struct fifth *f )
{
  if( f->text.idx <= FIFTH_PRG_SIZE ) {
    f->text.idx ++;
  } else {
    fprintf( ERR_STREAM, "Can't increment PC: end of program space\n" );
  }
}

uint16_t fifth_pc( struct fifth *f )
{
  return f->text.idx;
}

int fifth_set_pc( struct fifth *f, uint16_t pc )
{
  if( pc <= FIFTH_PRG_SIZE ) {
    f->text.idx = pc;
    return 0;
  }

  fprintf( ERR_STREAM, "Wrong PC: %d\n", pc );
  return 1;
}

/** prints program counter
 */
void fifth_print_pc( struct fifth *f )
{
  fprintf( ERR_STREAM, "PC: %d\n", fifth_pc( f ) );
}

/**
 *
 */
void fifth_fprint_text( FILE *stream, struct fifth *f )
{
  uint16_t i = 0;
  fprintf( stream, "TEXT: " );
  for ( i = fifth_pc( f ); i < f->text.size; i ++ ) {
    fprintf( stream, "%3d ", f->text.data[ i ] );
  }
  fprintf( stream, "\n" );
}

/**
 *
 */
int fifth_set_text( struct fifth *f, uint8_t addr, uint8_t data )
{
  if( addr < f->text.size ) { 
    f->text.data[ addr ] = data;
    return 0;
  }

  return 1;
}

/** prints program text from PC to the end
 */
void fifth_print_text( struct fifth *f )
{
  fifth_fprint_text( ERR_STREAM, f );
}

/** \return text contents at the PC
 */
uint8_t fifth_curr_text( struct fifth *f )
{
  return f->text.data[ fifth_pc( f ) ];
}

#ifndef __ET__
int main( int argc, char *argv[] )
{
  struct fifth f;
  f.text.data = malloc( FIFTH_PRG_SIZE );
  f.text.size = FIFTH_PRG_SIZE;
  f.text.idx = 0;

  f.stack.data = malloc( FIFTH_STACK_SIZE );
  f.stack.size = FIFTH_STACK_SIZE;
  f.stack.idx = 0;

  if ( !f.text.data || !f.stack.data ) {
    exit( ENOMEM );
  }

  memset( f.stack.data, 0, f.stack.size );
  memset( f.text.data, 0, f.text.size );

  f.text.size = fread( f.text.data, 1, f.text.size, stdin );
  if ( f.text.size > 0 ) {
    fprintf( ERR_STREAM, "INFO: loaded program, size = %u\n", f.text.size );
  } else {
    fprintf( ERR_STREAM, "ERR: program is empty\n" );
    exit( 1 );
  }

  while ( 1 ) {
    uint8_t t = fifth_curr_text( &f );
    fifth_print_pc( &f );
    fifth_print_text( &f );
    stack_print( &f.stack );
    if ( t < fifth_cmd_num() && cmds[t].func ) {
      fprintf( ERR_STREAM, "----------\n" );

      fprintf( ERR_STREAM, "INFO: %s(%d) ", cmds[t].name, t );
      uint16_t i = 0;
      for ( i = 0; i < cmds[t].n_txt_params; i++ ) {
        fprintf( ERR_STREAM, "%3d ", f.text.data[ fifth_pc( &f ) + 1 + i ] );
      }
      for ( i = 0; i < cmds[t].n_stack_params; i++ ) {
        fprintf( ERR_STREAM, "%3d ", stack_val( &f.stack, i ) );
      }
      fprintf( ERR_STREAM, "\n" );

      fifth_inc_pc( &f );
      if ( cmds[ t ].func( &f ) != 0 ) {
        fprintf( ERR_STREAM, "ERR: command %s failed\n", cmds[ t ].name );
        exit( EINVAL );
      }
    } else {
      fprintf( ERR_STREAM, "ERR: invalid command (%d) at PC=%d\n", 
                       fifth_curr_text( &f ),
                       fifth_pc( &f ) );
      exit( EINVAL );
    }
  }

  return 0; 
}
#endif
