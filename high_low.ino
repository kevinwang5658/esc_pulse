#include <math.h>

volatile int state = 0;
volatile bool isForward = true;

void setup() {
  Serial.begin(9600);
  DDRD = DDRD | B11111100;

  cli(); //stops interrupts

  //Adjust timer2 to be 8kHz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 600;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1<<CS12)|(1<<CS10);
  TIMSK1 |= (1 << OCIE1A);

  sei(); // allow interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (isForward) {
    switch(state) {
      case 0: 
        PORTD = B11000100;
        break;
      case 1:
        PORTD = B11010000;
        break;
      case 2:
        PORTD = B01110000;
        break;
      case 3:
        PORTD = B00110100;
        break;
      case 4:
        PORTD = B00011100;
        break;
      case 5:
        PORTD = B01001100;
        break;
    }
  } else {
    switch(state) {
      case 0: 
        PORTD = B01100000;
        break;
      case 1:
        PORTD = B00100100;
        break;
      case 2:
        PORTD = B10000100;
        break;
      case 3:
        PORTD = B10010000;
        break;
      case 4:
        PORTD = B00011000;
        break;
      case 5:
        PORTD = B01001000;
        break;
    }
  }
  //OCR1A = OCR1A - 4;
  state = (state + 1) % 6;
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    Serial.println(input);
    if (input[0] != '\0') {
      //OCR1A = 400;
      isForward = !isForward;
    }
  }
}
