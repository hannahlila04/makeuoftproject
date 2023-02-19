#include <Keypad.h>
#include <Wire.h>     // include Arduino Wire library
#include "rgb_lcd.h"  // include Seeed Studio LCD library

#define LED1_PIN 12
#define LED2_PIN 2
#define LED3_PIN 5
#define SENSOR1  A0
#define buttonPin 3
/*
There are X beats in a Y beat measure
Tempo = 120 BPM = 2 BPS
Beats are every a = 2 / Y seconds


Our modes will be duples: Keys 1 4 7 -> 2/2, 2/4, 6/8
          triples: Keys 2 5 8 -> 3/2, 3/4, 9/8
                  quadruples: Keys 3 6 9 -> 4/2, 4/4, 12/8
*/
unsigned long delayCount = 2000;
const unsigned int TEMPO = 2;

unsigned long previousBlink1 = 0;
byte LED1_State = HIGH;
byte LED2_State = HIGH;
byte LED3_State = HIGH;

int count = 0;
 
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

int countCorrect = 0;
int countTotal = 0;

void ISR_button()
{
  buttonState = 1;
  countCorrect = 0;
  countTotal = 0;
}

void setup() {
  SerialUSB.begin(9600);
  SerialUSB.print("Set up succeeded\n");
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button, CHANGE);

}

void loop() {

  LED2_State = LOW;
  LED3_State = LOW;

  bool jump_exit = true;
  while (jump_exit) {
  
    // read the state of the pushbutton value:
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
  
    if (buttonState == 1) {
        jump_exit = false;
        break;
    }
  
    int analogReading = analogRead(SENSOR1);

    if (LED1_State == HIGH) {
      LED1_State = LOW;
    } else {
      LED1_State = HIGH;
    }

    SerialUSB.print("Force sensor reading = ");
    SerialUSB.print(analogReading); // print the raw analog reading


    if (analogReading > 900) {
      SerialUSB.print("  pressed on time\n");
      countCorrect += 1;
      if (LED2_State == LOW) {
      LED2_State = HIGH;
    } else {
      LED2_State = HIGH;
    }
    if (LED3_State == LOW) {
      LED3_State = LOW;
    } else {
      LED3_State = LOW;
    }
    }
    else {
      SerialUSB.print("  missed\n");
       if (LED3_State == LOW) {
        LED3_State = HIGH;
        } else {
          LED3_State = HIGH;
      }
      if (LED2_State == LOW) {
        LED2_State = LOW;
      } else {
        LED2_State = LOW;
      }
    }

    delay(delayCount);

    digitalWrite(LED1_PIN, LED1_State);
    digitalWrite(LED2_PIN, LED2_State);
    digitalWrite(LED3_PIN, LED3_State);
    countTotal += 1;

  }

  // print text on the LCD
 
  char txt[] =  "Great job your score is:\0";
 
  SerialUSB.print(txt);       // print text array
  float percentageCorrect = (countCorrect/countTotal)*100;
  SerialUSB.print(percentageCorrect);
  SerialUSB.print("%");

  SerialUSB.println("Exit code ...");
  // end here
}




