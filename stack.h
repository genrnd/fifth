#ifndef STACK__H
#define STACK__H

#include "vma.h"

#define STACK_MEM_OFFSET (0x100)

void stack_push( struct vma *, uint8_t b );
uint8_t stack_pop( struct vma *);
uint8_t stack_val( struct vma*, uint16_t i );
void stack_print( struct vma *v );
void stack_clear( struct vma *v );
int stack_write( struct vma *v, uint8_t addr, uint8_t data );

#endif // STACK__H
