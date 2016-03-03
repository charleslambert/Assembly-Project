#include "POV_Wand_Helper.h"

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

