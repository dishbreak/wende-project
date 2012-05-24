#include "Arduino.h"
#include "RoverInterfaces.h"
#include "RoverUI.h"

int roverMode = 0;

/**
* RoverUIRoutines: 
* Description: switch states for user interface
* @return true for ready, false otherwise
*/
boolean RoverUIRoutines()
{
  static int uiState = 0;
  
  switch(uiState)
  {
    case UI_INIT_STATE:
      //read the rover mode
      roverMode = readRoverMode();
      
      //reset wait
      RoverWait(true);
      uiState = UI_WAIT_STATE;
      break;
      
    case UI_WAIT_STATE:
      if(RoverWait(false))
      {
        uiState = UI_READY_STATE;
      }
        
      break;
      
    case UI_READY_STATE:
      
      //in case arduino does not properly handle a reset
      //with static variables...
      
      
      break;
  }
  
  return (uiState == UI_READY_STATE);
  
}

/**
* RoverWait: 
* Description: allow rover to delay before rovering
* @return true for ready, false otherwise
*/
boolean RoverWait(boolean reset)
{
  static int waitCounter = 0;
  
  if(reset)
  {
    waitCounter = INIT_DELAY_CYCLES;
  }
  else if(waitCounter > 0)
  {
    waitCounter--;
  }
  return (waitCounter == 0);
}
