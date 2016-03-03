
//Functions for controlling the LED Strip
//====================================================================================================================================================

////////////////////////////////////////////////////////////////////////////////
// Actually send a bit to the string. We must to drop to asm to enusre that the complier does
// not reorder things and make it so the delay happens in the wrong place.

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
////////////////////////////////////////////////////////////////////////////////

//Functions for parsing text and updating the display
//====================================================================================================================================================

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

void parse(String message, unsigned char r, unsigned char g, unsigned char b, int framerate) {
  int mes_len = message.length();
  for (int k=0; k<mes_len; k++){
     dis_letter(r,g,b,framerate,message[k]);
  }
}

// Button variables
int buttonVal = 0; // value read from button
int buttonLast = 0; // buffered value of the button's previous state
long btnDnTime; // time the button was pressed down
long btnUpTime; // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered

//Function for sleeping and waking on interupt
//====================================================================================================================================================

void shutoff() {
  asm volatile(
    "ldi r16,(1<<%[ASM_INT0]) \n\t" //Turn on external interrupts 
    "out %[ASM_GIMSK], r16 \n\t"    /////////////////////////////
    "ldi r16,(1<<%[ASM_SE])|(1<<%[ASM_SM1])|(0<<%[ASM_SM0])|(0<<%[ASM_ISC01])|(0<<%[ASM_ISC00]) \n\t" //set sleep mode to "power down" and set interrupt to low level
    "out %[ASM_MCUCR], r16 \n\t"                                                                      ///////////////////////////////////////////////////////////////
    "ldi r16, (0<<%[ASM_ADEN]) \n\t" // turn off ADC
    "out %[ASM_ADCSRA], r16 \n\t"    ///////////////
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