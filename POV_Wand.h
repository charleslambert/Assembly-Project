#include <avr/interrupt.h>

#include <stdlib.h>
#define i_ADCSRA ADCSRA
#define i_ADEN ADEN
#define i_INT0 INT0
#define i_GIMSK GIMSK
#define i_MCUCR MCUCR
#define i_SE SE
#define i_SM0 SM0
#define i_SM1 SM1
#define i_ISC00 ISC00
#define i_ISC01 ISC01

////////////////////////////////////////////////////////////////////////////////
#define buttonPin 2 // analog input pin to use as a digital input

#define debounce 20 // ms debounce period to prevent flickering when pressing or releasing the button
#define holdTime 2000 // ms hold period: how long to wait for press+hold event
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Change this to be at least as long as your pixel string (too long will work fine, just be a little slower)

#include <util/delay.h>
#include <stdint.h>
#define PIXELS 16  // Number of pixels in the string

// These values depend on which pin your string is connected to and what board you are using 

// More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation

// These values are for digital pin 8 on an Arduino Yun or digital pin 12 on a DueMilinove/UNO
// Note that you could also include the DigitalWriteFast hneader file to not need to to this lookup.

#define PIXEL_PORT  PORTB  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRB   // Port of the pin the pixels are connected to
#define PIXEL_BIT   1      // Bit of the pin the pixels are connected to

// These are the timing constraints taken mostly from the WS2812 datasheets 
// These are chosen to be conservative and avoid problems rather than for maximum throughput 

#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns

#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns

#define RES 6000    // Width of the low gap between bits to cause a frame to latch

// Here are some convience defines for using nanoseconds specs to generate actual CPU delays

#define NS_PER_SEC (1000000000L)          // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )

#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )

// Actually send a bit to the string. We must to drop to asm to enusre that the complier does
// not reorder things and make it so the delay happens in the wrong place.
////////////////////////////////////////////////////////////////////////////////

#define FRAMERATE 1/24
struct MES;