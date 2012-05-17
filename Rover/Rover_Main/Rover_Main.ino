#include "RoverDefaults.h"
#include "RoverProcessing.h"

#include "Arduino.h"

void setup()
{
  Serial.begin(9600);
  analogReference(DEFAULT);
}

void loop()
{

  RoverProcessingRoutines();
}
