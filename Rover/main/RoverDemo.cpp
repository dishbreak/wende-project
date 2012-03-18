#include "Arduino.h"
#include "RoverProcessing.h"
#include "RoverDefaults.h"
#include "RoverLaserDetection.h"


void RoverLightSensingDemo()
{

  static unsigned int demo_state = 0;
  static unsigned int sampleCount = 1;
  static unsigned int avgSampleCount = 1;
  static unsigned int photoDetectorCycleAvg = 0;
  static unsigned long photoDetectorCycleSum = 0;
  static unsigned long  detectorSum = 0;
  static unsigned int photoDetectorAvg = 0;
  static unsigned int photoDetectorBias = 0;
  boolean calibrated = true;
  
  // read the analog in value:
  int photodetectorVal = (1024-analogRead(PHOTO_DETECTOR_PIN));
  detectorSum+=photodetectorVal;
  
  if(demo_state == 0)
  { 
    if(sampleCount == 100)
    {
      //initialize to lighting conditions...
      photoDetectorBias = detectorSum/sampleCount;
      detectorSum = photodetectorVal;
      demo_state = 1;
      sampleCount = 1;
      digitalWrite(LED_READY_PIN,HIGH);
//      Serial.print("Initialized to current lighting conditions...current lighting is at ");
//      Serial.println(photoDetectorBias);
    }
    
  }
  else
  {  
      if(sampleCount == 100)
      {
        photoDetectorAvg = detectorSum/sampleCount;
        detectorSum = photodetectorVal;
        photoDetectorCycleSum += photoDetectorAvg; 
        sampleCount = 1;
        avgSampleCount++;
        
        // print the results to the serial monitor:
       // Serial.print("bias = ");
       // Serial.print(photoDetectorBias);
       // Serial.print("\tsensor = " );                       
       // Serial.println(photoDetectorAvg);      
      }
      
      if(avgSampleCount == 200)
      {
        photoDetectorCycleAvg = photoDetectorCycleSum/avgSampleCount;
        photoDetectorCycleSum = photoDetectorAvg;
        
        //rebias
        //if(abs(photoDetectorCycleAvg - photoDetectorBias) > 30)
        //{
        //  digitalWrite(LED_READY_PIN,LOW);       
        //  photoDetectorBias = photoDetectorCycleAvg;
        //  calibrated = false;
        //}
        avgSampleCount = 1;
      }
      
        
      
      // map it to the range of the analog out:
      int ledPWM = map(photoDetectorAvg, 0, 100, 0, 255);  
      // change the analog out value:
      analogWrite(LED_ACTUAL_LIGHT_PIN, ledPWM);           
    
      //check for large light increase...
      if(calibrated)
      {
        digitalWrite(LED_READY_PIN,HIGH);  
        if(photoDetectorAvg > photoDetectorBias)
        {
          if(photoDetectorAvg-photoDetectorBias > 150)
          {
            //light high LED
            digitalWrite(LED_LOW_LIGHT_PIN,LOW);
            digitalWrite(LED_MID_LIGHT_PIN,LOW);
            digitalWrite(LED_HIGH_LIGHT_PIN,HIGH);
          }
          //the change we WANT to see based on laser testing...600-1000mV
          else if(photoDetectorAvg-photoDetectorBias > 50)
          {
            //light mid LED
            digitalWrite(LED_LOW_LIGHT_PIN,LOW);
            digitalWrite(LED_MID_LIGHT_PIN,HIGH);
            digitalWrite(LED_HIGH_LIGHT_PIN,LOW);
          }
          else if(photoDetectorAvg-photoDetectorBias > 20)
          {
            //light low LED
            digitalWrite(LED_LOW_LIGHT_PIN,HIGH);
            digitalWrite(LED_MID_LIGHT_PIN,LOW);
            digitalWrite(LED_HIGH_LIGHT_PIN,LOW);
          }
          else
          {
            //light no LED
            digitalWrite(LED_LOW_LIGHT_PIN,LOW);
            digitalWrite(LED_MID_LIGHT_PIN,LOW);
            digitalWrite(LED_HIGH_LIGHT_PIN,LOW);
          }
        }
      }
      else
      {
          //light no LED
          digitalWrite(LED_LOW_LIGHT_PIN,LOW);
          digitalWrite(LED_MID_LIGHT_PIN,LOW);
          digitalWrite(LED_HIGH_LIGHT_PIN,LOW);
      }
  }
  sampleCount++;
}

void RoverLaserDetectionDemo()
{
	DetectionProcessing();
}
void RoverMotorDemo(void)
{
}
void RoverDrivingDemo(void)
{
}
