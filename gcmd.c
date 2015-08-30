#include <stdio.h>
#include <stdlib.h>

#ifdef __ET__
#  include <sys/timer.h>
#  include <mb/vga.h>
#  include <mb/beep.h>
#  include <mb/leds.h>
#else
#  include <unistd.h>
#endif 

#include "stack.h"
#include "fifth.h"
#include "cmd.h"
#include "gcmd.h"

#define MIN(a,b) ( (a) < (b) ? (a) : (b) )                                           


/** take width, x, y and color from stack and draw rect
 */
int fifth_rect( struct fifth *f, uint8_t size )
{
  uint8_t color = stack_pop( &f->stack ); 

  uint8_t y = stack_pop( &f->stack );
  uint8_t x = stack_pop( &f->stack );

  x = MIN( x, FIFTH_SCREEN_X_MAX );
  y = MIN( y, FIFTH_SCREEN_Y_MAX );

#ifdef __ET__
  vga_c_fill_rect( x, 
                   y, 
                   MIN( x + size, FIFTH_SCREEN_X_MAX ), 
                   MIN( y + size, FIFTH_SCREEN_Y_MAX ), 
                   color );

  disp_refresh( );
#else
  printf( "Draw square (size = %u at %u, %u of color %u\n", 
           size, x, y, color );
#endif
  return 0;
}

int fifth_lrect( struct fifth *f )
{
  return fifth_rect( f, 10 );
}

int fifth_mrect( struct fifth *f )
{
  return fifth_rect( f, 20 );
}

int fifth_brect( struct fifth *f )
{
  return fifth_rect( f, 50 );
}

/** print character 
 */
int fifth_char( struct fifth *f )
{
  uint8_t c = stack_pop( &f->stack );

  uint8_t bg_color = stack_pop( &f->stack ); 
  uint8_t fg_color = stack_pop( &f->stack ); 

  uint8_t y = stack_pop( &f->stack );
  uint8_t x = stack_pop( &f->stack );

#ifdef __ET__
  int _fg_color = vga_fg_color( );
  int _bg_color = vga_bg_color( );

  vga_set_colors( fg_color, bg_color );
  vga_putchar( c, x, y );
  vga_set_colors( _fg_color, _bg_color );

#else
  printf( "Character %c at %u, %u color %u\n", c, x, y, fg_color );
#endif
  return 0;
}

/** take color and fill screen with it
 */
int fifth_fill( struct fifth *f )
{
  uint8_t color = stack_pop( &f->stack );

#ifdef __ET__
  vga_c_fill_rect( FIFTH_SCREEN_X_MIN, 
                   FIFTH_SCREEN_Y_MIN, 
                   FIFTH_SCREEN_X_MAX, 
                   FIFTH_SCREEN_Y_MAX, 
                   color );
  //FIXME: hack to avoid blinking
  //disp_refresh( );
#else
  printf( "fill with color %d\n", color );
#endif
  return 0;
}

/** take beep duration from stack and then beep
 *
 */
int fifth_sound( struct fifth *f )
{
//  uint8_t tone = stack_pop( &f->stack );  
  uint8_t dur = stack_pop( &f->stack );

  //printf( "\007" ); 

#ifdef __ET__
  beep( (unsigned short)dur * 10 );
#endif

  return 0;
}

/** take led number and it's color from stack
 *  and then burn led 
 */
int fifth_led( struct fifth *f )
{
  uint8_t led = stack_pop( &f->stack );
  uint8_t color = stack_pop( &f->stack );
#ifdef __ET__
  led_burn( led, color );

  //FIXME: hack to avoid blinking
  disp_refresh( );
#else  
  printf( "burn led #%d with color %d\n", led, color );
#endif

  return 0;
}

/** take stack top and sleep for this number of milliseconds
 */
int fifth_delay( struct fifth *f )
{
#ifdef __ET__
  NutSleep( 10 * stack_pop( &f->stack ) );
#else 
  usleep( 10 * 1000 * stack_pop( &f->stack ) );
#endif 

  return 0;
}

