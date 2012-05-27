#include "RoverInterfaces.h"
#include "RoverProcessing.h"

#include "Arduino.h"

void setup()
{
  //setup rover inputs/outputs/interrupts/defaults
  setupInterfaces();
}

void loop()
{

  RoverProcessingRoutines();
}
