#ifndef FIFTHCMD__H
#define FIFTHCMD__H

#include <stdint.h>

struct fifth;

int fifth_push( struct fifth *f );
int fifth_drop( struct fifth *f );
int fifth_add( struct fifth *f );
int fifth_sub( struct fifth *f );
int fifth_mul( struct fifth *f );
int fifth_inc( struct fifth *f );
int fifth_dec( struct fifth *f );
int fifth_lt( struct fifth *f );
int fifth_gt( struct fifth *f );
int fifth_eq( struct fifth *f );
int fifth_ne( struct fifth *f );
int fifth_cjump( struct fifth *f );
int fifth_goto( struct fifth *f );
int fifth_nop( struct fifth *f);
int fifth_dup( struct fifth *f );
int fifth_swap( struct fifth *f );
int fifth_over( struct fifth *f );
int fifth_rot( struct fifth *f );
int fifth_print( struct fifth *f );
int fifth_rand( struct fifth *f );

struct fifth_cmd {
  const char *name;
  uint8_t     code;
  uint8_t     n_txt_params;     // for debugging
  uint8_t     n_stack_params;  // for debugging
  int         (*func)( struct fifth * );
};

extern struct fifth_cmd cmds[];
uint8_t fifth_cmd_num( );

#ifndef ARRAY_SIZE
#  define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif

#endif // FIFTHCMD__H

