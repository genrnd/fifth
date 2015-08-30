#ifndef GCMD__H
#define GCMD__H 

#define FIFTH_SCREEN_X_MIN (0)
#define FIFTH_SCREEN_Y_MIN (0)
#define FIFTH_SCREEN_X_MAX (240)
#define FIFTH_SCREEN_Y_MAX (224)

struct fifth;

int fifth_lrect( struct fifth *f );
int fifth_mrect( struct fifth *f );
int fifth_brect( struct fifth *f );
int fifth_fill( struct fifth *f );
int fifth_sound( struct fifth *f );
int fifth_char( struct fifth *f );
int fifth_led( struct fifth *f );
int fifth_delay( struct fifth *f );

#endif // GCMD__H

