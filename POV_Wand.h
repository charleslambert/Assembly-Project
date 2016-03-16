// I would like to attribute the LED Timing code to Josh Levine.
// and the button presss code to Eren Duchan. 
// I used a modified versino of their code.

#include <avr/interrupt.h>

#include <stdlib.h>
// required registers for sleep mode
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

// Button Configuration

#define debounce 20 // ms debounce period to prevent flickering when pressing or releasing the button
#define holdTime 2000 // ms hold period: how long to wait for press+hold event

// LED Register and Timing Congfiguration

#include <util/delay.h>
#include <stdint.h>

#define PIXEL_PORT  PORTB  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRB   // Port of the pin the pixels are connected to

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



// Configuration
#define FRAME_DELAY (1000000*(1/24))
#define PIXEL_BIT   1      // Bit of the pin the pixels are connected to
#define buttonPin 2 // analog input pin to use as a digital input
#define PIXELS 16  // Number of pixels in the string
struct MES;