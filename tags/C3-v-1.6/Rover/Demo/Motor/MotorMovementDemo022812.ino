//Motor Movement Demo

int leftMotorPinEnable =  3;    // One motor wire connected to digital pin 5
int leftMotorPinDir =  2;    // One motor wire connected to digital pin 6
int rightMotorPinEnable =  5;    
int rightMotorPinDir =  4;    
int slow = 120;
int fast = 255;

// The setup() method runs once, when the sketch starts

void setup()   {                
  // initialize the digital pins as an output:
  pinMode(leftMotorPinEnable, OUTPUT); 
  pinMode(leftMotorPinDir, OUTPUT); 
  pinMode(rightMotorPinEnable, OUTPUT); 
  pinMode(rightMotorPinDir, OUTPUT); 
  pinMode(13, OUTPUT);
}

// the loop() method runs over and over again,
// as long as the Arduino has power
void loop()                     
{
  motorForward(slow, 2000);
  delay(200);
  motorReverse(slow, 2000);
  delay(200);
  motorForward(fast, 2000);
  delay(200);
  motorReverse(fast, 2000);
  digitalWrite(13, HIGH);   // set the LED on
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(1000);              // wait for a second
}

void motorForward(int motorSpeed, int runtime){
  digitalWrite(leftMotorPinDir, HIGH);    // set the Pin motorPin2 LOW
  digitalWrite(rightMotorPinDir, LOW);    // set the Pin motorPin2 LOW
  analogWrite(leftMotorPinEnable, motorSpeed); //rotates motor
  analogWrite(rightMotorPinEnable, motorSpeed); //rotates motor
  delay(runtime); //waits
  digitalWrite(leftMotorPinEnable, LOW);    // set the Pin motorPin1 LOW
  digitalWrite(rightMotorPinEnable, LOW);    // set the Pin motorPin1 LOW
}

void motorReverse(int motorSpeed, int runtime){
  digitalWrite(leftMotorPinDir, LOW);    // set the Pin motorPin2 LOW
  digitalWrite(rightMotorPinDir, HIGH);    // set the Pin motorPin2 LOW
  analogWrite(leftMotorPinEnable, motorSpeed); //rotates motor
  analogWrite(rightMotorPinEnable, motorSpeed); //rotates motor
  delay(runtime); //waits
  digitalWrite(leftMotorPinEnable, LOW);    // set the Pin motorPin1 LOW
  digitalWrite(rightMotorPinEnable, LOW);    // set the Pin motorPin1 LOW
}

