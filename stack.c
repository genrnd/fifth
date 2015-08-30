#include <stdio.h>
#include <string.h>

#include "fifth.h"

#include "stack.h"

/*
 * we intentionally do not controlling boundaries.
 * invalid program should work incorrectly
 *
 * zero-th item in vma->data is not used.
 */

void stack_push( struct vma *v, uint8_t b )
{
  if ( v->idx <= FIFTH_STACK_SIZE ) {
    v->data[ ++v->idx ] = b;
  } else {
    fprintf( ERR_STREAM, "Stack is full\n" );
  }
}

uint8_t stack_pop( struct vma *v)
{
  return v->data[ v->idx -- ];
}

/** \param i -- index from top of stack. 0 means top of stack.
 */ 
uint8_t stack_val( struct vma *v, uint16_t i )
{
  return v->data[ v->idx - i ];
}

void stack_clear( struct vma *v )
{
  v->idx = 0;
  memset( v->data, 0, FIFTH_STACK_SIZE );
}

void stack_print( struct vma *v )
{
  int i = 0;
  fprintf( ERR_STREAM, "STACK: " );
  for ( i = v->idx; i > 0; i -- ) {
    fprintf( ERR_STREAM, "%3d ", v->data[ i ] );
  }
  fprintf( ERR_STREAM, "\n" );
}

/** \brief Function that writes to stack as to random access memory.
 *         It needs to close from user that data memory is stack.
 *
 */
int stack_write( struct vma *v, uint8_t addr, uint8_t data ) 
{
  if( addr > v->idx ) {
    return 1;
  }

  v->data[ addr ] = data;

  return 0;
}
