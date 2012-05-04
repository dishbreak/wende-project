#include <Servo.h>

#define MIN_PULSE_WIDTH       900     // the shortest pulse sent to a servo 
#define MAX_PULSE_WIDTH      2100     // the longest pulse sent to a servo
#define LASER_PIN 2
#define AZ_SERVO_PIN 5
#define EL_SERVO_PIN 6
#define LED_PIN 13

Servo myservoAz;  // create servo object to control a servo  - Azimuth
Servo myservoEl; //elevation servo object

int temp = 0;
int blinkDelay = 500;  // blink delay used to indicate servo movement imminent
int blinkDelayDone = 250;  // blink delay to indicate moving to next test

int delayed = 5000; //milliseconds

//1 = Servo Sweep Test (Response Time Measurement)
//2 = Azimuth Resolution Test
//3 = Elevation Resolution Test
int mode = 1;

// Function used to strobe the on-board LED
void strobeLED(int strobeCount, int ledDelay)
{
  // Set the for loop based on the strobe count
  for( int Inx=0; Inx < strobeCount; Inx++ )
  {
    // Strobe the LED on then off
    digitalWrite(LED_PIN,HIGH); //LED on
    delay(ledDelay);
    digitalWrite(LED_PIN,LOW); //LED off
    delay(ledDelay);
  }
}

// Function used to initialize the servos to their neutral positions
void initServos()
{
  //initialize servos
  myservoAz.write(1500); //us
  myservoEl.write(1774); //us
}

// Function used to sweep the servos between their min and max values
void servoSweep()
{
  // Sweep code for Demo 2
  myservoEl.writeMicroseconds(1050); // Min El position
  myservoAz.writeMicroseconds(950);  // Min Az position
  
  // 5 second delay
  delay(5000);
  
  // Strobe LED 3 times
  strobeLED(3, blinkDelay);
  
  myservoEl.writeMicroseconds(1950); // Max El position
  myservoAz.writeMicroseconds(2050); // Min Az position
  
  // 5 second delay
  delay(5000);
}

// Function used to demonstrate the resolution of the Az servo
void azServoRes()
{
  // Turn the laser on
  digitalWrite(LASER_PIN, HIGH);
  
  for(temp = 0; temp < 15; temp += 1)
  {
    // Move the azimuth servo by 1 step and wait 4 seconds
    myservoAz.write(1500+temp);
    delay(4000);
  }

  // Turn the laser off
  digitalWrite(LASER_PIN, LOW);
}

// Function used to demonstrate the resolution of the El servo
void elServoRes()
{
  // Turn the laser on
  digitalWrite(LASER_PIN, HIGH);
  
  for(temp = 0; temp < 15; temp += 1)
  {
    // Move the Elevation servo by 1 step and wait 4 seconds
    myservoEl.write(1774-temp);
    delay(4000);
  }
  
  // Turn the laser off
  digitalWrite(LASER_PIN, LOW);
}

void setup()
{
  myservoAz.attach(AZ_SERVO_PIN);  // attaches the Azimuth servo on pin 5 to the servo object
  myservoEl.attach(EL_SERVO_PIN);  // attaches the Elevation servo on pin 6 to the servo object
  pinMode(LED_PIN,OUTPUT); //LED
  pinMode(LASER_PIN,OUTPUT); //Laser

  // Move the servos to their initial position
  initServos();
  
  // Make sure the laser is off
  digitalWrite(LASER_PIN, LOW);
}

void loop()
{
  // Re-initialize the servos
  initServos();
  
  // Short delay before the next movement
  delay(delayed);

  //strobe LED 3 times
  strobeLED(3, blinkDelay);
  
  // If test mode is 1, run the servo sweep test
  if( mode == 1 )
  {
    // Run the servo sweep (reposition time) test
    servoSweep();
  }
  // else if the test mode is 2, run the Az Resolution test
  else if( mode == 2 )
  {
    azServoRes();
  }
  // else if the test mode is 3, run the El Resolution test
  else if( mode == 3 )
  {
    elServoRes();
  }
  
  //strobe LED 9 times to indicate test is done
  strobeLED(9, blinkDelayDone);
}
