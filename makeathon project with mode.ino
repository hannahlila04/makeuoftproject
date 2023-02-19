#include <Keypad.h>
#include <Wire.h>     // include Arduino Wire library
#include "rgb_lcd.h"  // include Seeed Studio LCD library

#define LED1_PIN 12
#define LED2_PIN 2
#define LED3_PIN 5
#define SENSOR1  A0
#define buttonPin 3
#define S1 7
#define S2 8
#define S3 9

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

int s1state;
int s2state;
int s3state;

void ISR_button()
{
  buttonState = 1;
}

void setup() {
  SerialUSB.begin(9600);
  SerialUSB.print("Set up succeeded\n");
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // switches
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button, CHANGE);

}

void loop() {

  LED2_State = LOW;
  LED3_State = LOW;

  bool jump_exit = true;
  while (jump_exit) {
  

    s1state = digitalRead(S1);
    s2state = digitalRead(S2);
    s3state = digitalRead(S3);

    if (s1state) {
      delayCount = (TEMPO * 1000) / 2;
    } else if (s2state) {
      delayCount = (TEMPO * 1000) / 4;
    } else if (s3state) {
      delayCount = (TEMPO * 1000) / 8;
    } else {
      delayCount = (TEMPO * 1000);
    }
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




