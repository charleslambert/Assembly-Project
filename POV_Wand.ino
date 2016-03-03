



//////////////////////////////////////////////////////////////////////////

#define buttonPin 2 // analog input pin to use as a digital input

#define debounce 20 // ms debounce period to prevent flickering when pressing or releasing the button
#define holdTime 2000 // ms hold period: how long to wait for press+hold event




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
  if (led_on) {
  buttonVal = digitalRead(buttonPin);

  cli();
  check_short_press(buttonVal,buttonLast);
  check_long_press(buttonVal);
  sei();
  buttonLast = buttonVal;
  }
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


void setup() {
  message_to_dis = "fame";
  r = 0;
  g = 50;
  b = 0;
  led_on = 1;

  ledsetup();
  pinMode(buttonPin, INPUT);
  
}

void loop() {

  button_press();

  parse(message_to_dis,r,g,b,framerate);
  
}

