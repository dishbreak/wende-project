#include "RoverDefaults.h"
#include "RoverProcessing.h"

#include "Arduino.h"

void setup()
{
  Serial.begin(9600);
  analogReference(DEFAULT);
  pinMode(LED_READY_PIN, OUTPUT);
  pinMode(LED_LOW_LIGHT_PIN, OUTPUT);
  pinMode(LED_MID_LIGHT_PIN, OUTPUT);
  pinMode(LED_HIGH_LIGHT_PIN, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}

void loop()
{

  RoverProcessingRoutines();
}
