#include <Arduino.h>
#include <LedController.hpp>

#define LED_CONTROL_CLK_PIN 18
#define LED_CONTROL_CS_PIN 19
#define LED_CONTROL_DATA_PIN 23
#define LED_CONTROL_NUM_SEGMENTS 1

/*
Rotary Encoder Demo
rot-encode-demo.ino
Demonstrates operation of Rotary Encoder
https://Dronebotworkshop.com
*/

//Rotary Encoder Inputs
#define inputClk 14
#define inputDT 13
#define digitsPerSegment 8
#define positionOffset 0
#define Segments 1


int posF = 0;
int posN = 0;
int currentStateCLK;
int previousStateCLK;
int currentStateDT;
int previousStateDT;
unsigned int segNum;
//int segmentBNumber() = 1;

LedController ledController(LED_CONTROL_DATA_PIN, LED_CONTROL_CLK_PIN,
                            LED_CONTROL_CS_PIN, LED_CONTROL_NUM_SEGMENTS);

//This function calculates the largest number that can be displayed
unsigned long long getLargestNumber() {
  return (unsigned long long) pow(10,Segments*digitsPerSegment);
}

//this function sets the Segments to a given number
void setLEDs (int number) {
  //the loop is used to split the given number and set the right digit on the Segments
  int i = 0;
  while (number > 0) {
    int digit = number%10;
    ledController.setChar(segNum, i, (char)digit, false);
    number /= 10;
    i++;
    ledController.setChar(segNum, i, ' ', false);
    }
  }
void setup(){
  //set encoder pins as inputs
  pinMode (inputClk, INPUT);
  pinMode (inputDT, INPUT);

  Serial.begin (115200);

  previousStateCLK = digitalRead(inputClk);
  previousStateDT = digitalRead(inputDT);
  
  Serial.println("Init LEDs");
  ledController.activateAllSegments();
  ledController.setIntensity(15);
  ledController.clearMatrix();
  Serial.println("Display Numbers");
  delay(500);
  for (unsigned int i = 0; i < 8; i++) {
   ledController.setChar(segNum, i, (char)8 - i, false);
  }
delay (500);
ledController.clearMatrix();
ledController.setChar(segNum, 0, (char)0, false);
}

void loop () {
  currentStateCLK = digitalRead(inputClk);
  if (currentStateCLK != previousStateCLK) {
      if (digitalRead(inputDT) != currentStateCLK){
          posF ++;
      } else {
          posF --;
      }
  if(posF >= 0) {
    ledController.setChar(segNum, 7, ' ', false);
    setLEDs(posF);
    } else {
      posN = posF * -1;
      setLEDs(posN);
      ledController.setChar(segNum, 7, '-', false);
    }
  }
  previousStateCLK = currentStateCLK;
}