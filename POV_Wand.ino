#include <avr/interrupt.h>

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
// Note that you could also include the DigitalWriteFast header file to not need to to this lookup.

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




// 
//  Font data for Minecraft 16pt
// 

// Character bitmaps for Minecraft 16pt
const static unsigned char letters[] PROGMEM= 
{
  // @0 'A' (17 pixels wide)
  //      #######     
  //      #######     
  //      #######     
  //   ####     ####  
  //   ####     ####  
  //   #############  
  //   #############  
  //   #############  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0xF8, 0xF8, 0xF8, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @34 'B' (17 pixels wide)
  //  ############    
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ############    
  //  ############    
  //  ############    
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0xFF, 0xFF, 0x78, 0x78, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xFF, 0xFF, 0x1E, 0x1E, 0x00, 0x00, 

  // @68 'C' (17 pixels wide)
  //     ###########  
  //     ###########  
  //     ###########  
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####        ### 
  //  ####        ### 
  //     ###########  
  //     ###########  
  //     ###########  
  0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x3F, 0x3F, 0x38, 0x00, 
  0x00, 0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF8, 0xF8, 0x18, 0x00, 

  // @102 'D' (17 pixels wide)
  //  ############    
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ############    
  //  ############    
  //  ############    
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xFF, 0xF8, 0xF8, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0x1F, 0x1F, 0x00, 0x00, 

  // @136 'E' (17 pixels wide)
  //  ##############  
  //  ##############  
  //  ##############  
  //  ####            
  //  ####            
  //  ########        
  //  ########        
  //  ########        
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ##############  
  //  ##############  
  //  ##############  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 

  // @170 'F' (17 pixels wide)
  //   #############  
  //   #############  
  //   #############  
  //   ####           
  //   ####           
  //   #########      
  //   #########      
  //   #########      
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @204 'G' (17 pixels wide)
  //     ############ 
  //     ############ 
  //     ############ 
  //  ####            
  //  ####            
  //  ####     ###### 
  //  ####     ###### 
  //  ####     ###### 
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //     ##########   
  //     ##########   
  //     ##########   
  0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x00, 
  0x00, 0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x00, 

  // @238 'H' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ##############  
  //  ##############  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @272 'I' (17 pixels wide)
  //       #######    
  //       #######    
  //       #######    
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //       #######    
  //       #######    
  //       #######    
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 

  // @306 'J' (17 pixels wide)
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //    ####   ###    
  //    ####   ###    
  //       #####      
  //       #####      
  //       #####      
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xF8, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 

  // @340 'K' (17 pixels wide)
  //   ####           
  //   ####           
  //   ####     ####  
  //   ####     ####  
  //   ####   ###     
  //   ####   ###     
  //   ####   ###     
  //   ########       
  //   ########       
  //   ####   ###     
  //   ####   ###     
  //   ####   ###     
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0xF0, 0x70, 0x7C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x01, 0x0F, 0x0E, 0xFE, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 

  // @374 'L' (17 pixels wide)
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   #############  
  //   #############  
  //   #############  
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 

  // @408 'M' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ######  ######  
  //  ######  ######  
  //  ##############  
  //  #### #### ####  
  //  #### #### ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x38, 0xF8, 0xE0, 0xE0, 0xF8, 0x38, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @442 'N' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ######    ####  
  //  ######    ####  
  //  #### #### ####  
  //  #### #### ####  
  //  #### #### ####  
  //  ####    ######  
  //  ####    ######  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x0C, 0x7C, 0x70, 0x70, 0xF0, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @476 'O' (17 pixels wide)
  //    ###########   
  //    ###########   
  //    ###########   
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  //    ###########   
  //    ###########   
  //    ###########   
  0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xF8, 0xF8, 0xF8, 
  0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x1F, 

  // @510 'P' (17 pixels wide)
  //  ############    
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ##############  
  //  ############    
  //  ############    
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0xFF, 0x38, 0x38, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @544 'Q' (17 pixels wide)
  //    ###########   
  //    ###########   
  //    ###########   
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####    #### ####
  //    ###########   
  //    ###########   
  //    ##############
  0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xF8, 0xF8, 0xF8, 
  0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xFF, 0x9F, 0x9F, 0x9F, 

  // @578 'R' (17 pixels wide)
  // #############    
  // #############    
  // #############    
  // ####        ###  
  // ####        ###  
  // ###############  
  // #############    
  // #############    
  // ####    ####     
  // ####    ####     
  // ####    ####     
  // ####      ####   
  // ####      ####   
  // ####         ####
  // ####         ####
  // ####         ####
  0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x38, 0x38, 0x00, 0x00, 
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x1F, 0x1F, 0x18, 0xF8, 0xE0, 0xE0, 0xE0, 

  // @612 'S' (17 pixels wide)
  //      ##########  
  //      ##########  
  //      ##########  
  //   ####           
  //   ####           
  //   ##########     
  //      #######     
  //      #######     
  //            ####  
  //            ####  
  //            ####  
  //            ####  
  //            ####  
  //   ##########     
  //   ##########     
  //   ##########     
  0x00, 0x00, 0x38, 0x38, 0x38, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 

  // @646 'T' (17 pixels wide)
  //  ##############  
  //  ##############  
  //  ##############  
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @680 'U' (17 pixels wide)
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //      ########    
  //      ########    
  //      ########    
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0x00, 0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0x1F, 0x1F, 0x00, 0x00, 

  // @714 'V' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //     ###  ####    
  //     ###  ####    
  //     ###  ####    
  //     ###  ####    
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  0x00, 0x7F, 0x7F, 0x7F, 0xFF, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0xFF, 0xFF, 0x7F, 0x7F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0xFF, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 

  // @748 'W' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  #### #### ####  
  //  #### #### ####  
  //  #### #### ####  
  //  ######  ######  
  //  ######  ######  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x18, 0x1F, 0x07, 0x07, 0x1F, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @782 'X' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //     ###  ####    
  //     ###  ####    
  //     #########    
  //       ####       
  //       ####       
  //     ###  ####    
  //     ###  ####    
  //     ###  ####    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0x07, 0x07, 0x07, 0x3F, 0x38, 0xF8, 0xE0, 0xE0, 0xF8, 0x38, 0x3F, 0x3F, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0xFF, 0xFF, 0xF8, 0xF8, 0x00, 0x00, 

  // @816 'Y' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //     ###  ####    
  //     ###  ####    
  //     #########    
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  0x00, 0x07, 0x07, 0x07, 0x3F, 0x38, 0xF8, 0xE0, 0xE0, 0xF8, 0x38, 0x3F, 0x3F, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @850 'Z' (17 pixels wide)
  //  ##############  
  //  ##############  
  //  ##############  
  //            ####  
  //            ####  
  //          ######  
  //          ####    
  //          ####    
  //       ####       
  //       ####       
  //       ####       
  //     ###          
  //     ###          
  //  ##############  
  //  ##############  
  //  ##############  
  0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xE7, 0xE7, 0xFF, 0xFF, 0x3F, 0x3F, 0x00, 0x00, 
  0x00, 0xE0, 0xE0, 0xE0, 0xF8, 0xF8, 0xFF, 0xE7, 0xE7, 0xE7, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 
};

// Character descriptors for Minecraft 16pt
// { [Char width in bits], [Offset into minecraft_16ptCharBitmaps in bytes] }
const int minecraft_16ptDescriptors[][2] = 
{
  {17, 0},    // A 
  {17, 34},     // B 
  {17, 68},     // C 
  {17, 102},    // D 
  {17, 136},    // E 
  {17, 170},    // F 
  {17, 204},    // G 
  {17, 238},    // H 
  {17, 272},    // I 
  {17, 306},    // J 
  {17, 340},    // K 
  {17, 374},    // L 
  {17, 408},    // M 
  {17, 442},    // N 
  {17, 476},    // O 
  {17, 510},    // P 
  {17, 544},    // Q 
  {17, 578},    // R 
  {17, 612},    // S 
  {17, 646},    // T 
  {17, 680},    // U 
  {17, 714},    // V 
  {17, 748},    // W 
  {17, 782},    // X 
  {17, 816},    // Y 
  {17, 850},    // Z 
};

//Functions for operating the LED string
//==============================================================================

////////////////////////////////////////////////////////////////////////////////
/*
 This is an example of how simple driving a Neopixel can be
 This code is optimized for understandability and changability rather than raw speed
 More info at http://wp.josh.com/2014/05/11/ws2812-neopixels-made-easy/
*/

void sendBit( bool bitVal ) {
  
    if (  bitVal ) {        // 0 bit
      
    asm volatile (
      "sbi %[port], %[bit] \n\t"        // Set the output bit
      ".rept %[onCycles] \n\t"                                // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      "cbi %[port], %[bit] \n\t"                              // Clear the output bit
      ".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      ::
      [port]    "I" (_SFR_IO_ADDR(PIXEL_PORT)),
      [bit]   "I" (PIXEL_BIT),
      [onCycles]  "I" (NS_TO_CYCLES(T1H) - 2),    // 1-bit width less overhead  for the actual bit setting, note that this delay could be longer and everything would still work
      [offCycles]   "I" (NS_TO_CYCLES(T1L) - 2)     // Minimum interbit delay. Note that we probably don't need this at all since the loop overhead will be enough, but here for correctness

    );
                                  
    } else {          // 1 bit

    // **************************************************************************
    // This line is really the only tight goldilocks timing in the whole program!
    // **************************************************************************


    asm volatile (
      "sbi %[port], %[bit] \n\t"        // Set the output bit
      ".rept %[onCycles] \n\t"        // Now timing actually matters. The 0-bit must be long enough to be detected but not too long or it will be a 1-bit
      "nop \n\t"                                              // Execute NOPs to delay exactly the specified number of cycles
      ".endr \n\t"
      "cbi %[port], %[bit] \n\t"                              // Clear the output bit
      ".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      ::
      [port]    "I" (_SFR_IO_ADDR(PIXEL_PORT)),
      [bit]   "I" (PIXEL_BIT),
      [onCycles]  "I" (NS_TO_CYCLES(T0H) - 2),
      [offCycles] "I" (NS_TO_CYCLES(T0L) - 2)

    );
      
    }
    
    // Note that the inter-bit gap can be as long as you want as long as it doesn't exceed the 5us reset timeout (which is A long time)
    // Here I have been generous and not tried to squeeze the gap tight but instead erred on the side of lots of extra time.
    // This has thenice side effect of avoid glitches on very long strings becuase 

    
}  

  
void sendByte( unsigned char byte ) {
    
    for( unsigned char bit = 0 ; bit < 8 ; bit++ ) {
      
      sendBit( bitRead( byte , 7 ) );                // Neopixel wants bit in highest-to-lowest order
                                                     // so send highest bit (bit #7 in an 8-bit byte since they start at 0)
      byte <<= 1;                                    // and then shift left so bit 6 moves into 7, 5 moves into 6, etc
      
    }           
} 
// Set the specified pin up as digital out

void ledsetup() {
  
  bitSet( PIXEL_DDR , PIXEL_BIT );
  
}

void sendPixel( unsigned char r, unsigned char g , unsigned char b )  {  
  
  sendByte(g);          // Neopixel wants colors in green then red then blue order
  sendByte(r);
  sendByte(b);
  
}


// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame

void show() {
  _delay_us( (RES / 1000UL) + 1);       // Round up since the delay must be _at_least_ this long (too short might not work, too long not a problem)
}

//Functions for translating text into bitmap and then updating the LED display
//==============================================================================

int translate(char c) {
  if (c != ' ') {
    return c - 'A';
  }
  else {
    return 0;
  }
}

void update_display(unsigned char r, unsigned char g, unsigned char b, uint16_t slice) {
   cli();

    for( uint16_t bit = 0 ; bit < 16; bit++ ) {
      
      bitRead( slice , 15) ? sendPixel(r,g,b) : sendPixel(0,0,0);                
      slice <<= 1;
      
    }
    sei();
    show();
}

void dis_letter ( unsigned char r, unsigned char g, unsigned char b,int frequency,char message) {
  
  int index = translate(message);
  int width = 17;
  int i = minecraft_16ptDescriptors[index][1];
  int x;
  uint16_t value;

  for (x=0;x<width;x++){
    value = (pgm_read_byte_near(letters+i+x+width) << 8) | pgm_read_byte_near(letters+i+x);
    update_display(r,g,b,value);
    delay(frequency*1000);
  } 
}

void parse(String message, unsigned char r, unsigned char g, unsigned char b, int framerate) {
  int mes_len = message.length();
  for (int k=0; k<mes_len; k++){
     dis_letter(r,g,b,framerate,message[k]);
  }
}
//Functions for button control
//==============================================================================

// Button variables
int buttonVal = 0; // value read from button
int buttonLast = 0; // buffered value of the button's previous state
long btnDnTime; // time the button was pressed down
long btnUpTime; // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered

//=================================================
// Events to trigger by click and press+hold

String message_to_dis;

int framerate = 1/30;
unsigned char r;
unsigned char g;
unsigned char b;

void shutoff() {
  asm volatile(
    "ldi r16,(1<<%[ASM_INT0]) \n\t"
    "out %[ASM_GIMSK], r16 \n\t"
    "ldi r16,(1<<%[ASM_SE])|(1<<%[ASM_SM1])|(0<<%[ASM_SM0])|(0<<%[ASM_ISC01])|(0<<%[ASM_ISC00]) \n\t"
    "out %[ASM_MCUCR], r16 \n\t"
    "ldi r16, (0<<%[ASM_ADEN]) \n\t"
    "out %[ASM_ADCSRA], r16 \n\t"
    "sei\n\t"
    "sleep\n\t"
    "cli\n\t"
    "ldi r16,(0<<%[ASM_INT0]) \n\t"
    "out %[ASM_GIMSK], r16 \n\t"
    "ldi r16,(0<<%[ASM_SE]) \n\t"
    "out %[ASM_MCUCR], r16 \n\t"
    "ldi r16, (1<<%[ASM_ADEN]) \n\t"
    "out %[ASM_ADCSRA], r16 \n\t"
    "sei\n\t"
    ::
    [ASM_SE] "I" (i_SE),
    [ASM_MCUCR] "I" (_SFR_IO_ADDR(i_MCUCR)),
    [ASM_GIMSK] "I" (_SFR_IO_ADDR(i_GIMSK)),
    [ASM_ADCSRA] "I" (_SFR_IO_ADDR(i_ADCSRA)),
    [ASM_ADEN] "I" (i_ADEN),
    [ASM_INT0] "I" (i_INT0),
    [ASM_ISC00] "I" (i_ISC00),
    [ASM_ISC01] "I" (i_ISC01),
    [ASM_SM0] "I" (i_SM0),
    [ASM_SM1] "I" (i_SM1)
    );
}
ISR(INT0_vect) {
}

void check_long_press(int buttonVal) {
    // Test for button held down for longer than the hold time
  if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime))
  {
    event2();
    ignoreUp = true;
    btnDnTime = millis();
  }
}

void check_short_press(int buttonVal, int buttonLast) {
  // Test for button pressed and store the down time
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounce))
  {
    btnDnTime = millis();
  }
  
  // Test for button release and store the up time
  if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounce))
  {
    if (ignoreUp == false) event1();
    else ignoreUp = false;
    btnUpTime = millis();
  }
}

void button_press(){
  buttonVal = digitalRead(buttonPin);

  cli();
  check_short_press(buttonVal,buttonLast);
  check_long_press(buttonVal);
  sei();
  buttonLast = buttonVal;
}

void event1()
{
message_to_dis = "hello";
r = 0;
g = 0;
b = 50;
}

void event2()
{
  update_display(r,g,b,0x00);
  delay(1000);
  shutoff();
}


//Main setup
//==============================================================================

void setup() {
  message_to_dis = "fame";
  r = 0;
  g = 50;
  b = 0;

  ledsetup();
  pinMode(buttonPin, INPUT);
  
}

void loop() {

  button_press();

  parse(message_to_dis,r,g,b,framerate);
  
}

