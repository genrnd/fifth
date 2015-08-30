#ifndef FIFTH__H
#define FIFTH__H

#include <stdint.h>
#include "vma.h"

#define FIFTH_STACK_SIZE 32
#define FIFTH_PRG_SIZE   256

#ifdef __ET__
#  define ERR_STREAM stdout
#else 
#  define ERR_STREAM stderr
#endif


struct fifth {
  struct vma stack;
  struct vma text;
};

void fifth_inc_pc( struct fifth *f );
int fifth_set_pc( struct fifth *f, uint16_t pc );
uint16_t fifth_pc( struct fifth *f );
uint8_t fifth_curr_text( struct fifth *f );
int fifth_set_text( struct fifth *f, uint8_t addr, uint8_t data );
void fifth_print_pc( struct fifth *f );
void fifth_fprint_text( FILE *stream, struct fifth *f );
void fifth_print_text( struct fifth *f );

#endif // FIFTH__H

