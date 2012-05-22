//PID motor control for WENDE rover subsystem
//Code by Trevor Owen 03/11/2012

// define motor pins
#define LM_enable   5  //left motor pins
#define LM_dir      4
#define LM_enc      2
#define RM_enable   6  //right motor pins
#define RM_dir      7
#define RM_enc      3

// define loop variables
int loopTime = 100;  //time between recalculating PID control

// define speed variables
int speedRef = 200;        //desired reference speed (mm/s) of both motors
#define Forward   LOW // for left motor direction
#define Reverse   HIGH

// set variables
unsigned long lastMilli = 0;  //saved last time value
int LMspeedRef = speedRef;   // desired reference speed for left motor
int LMspeedMeas = 0;   // measured speed for left motor
int RMspeedRef = 0;   // desired reference speed for right motor
int RMspeedMeas = 0;   // measured speed for right motor
int LM_PWM = 0;   // full speed 255
int RM_PWM = 0;   // full speed 255
volatile long LM_count = 0; // Interrupt variables should be volatile (stored in RAM)
volatile long RM_count = 0;
long oldLMcount = 0;
long oldRMcount = 0;
int cumError = 0;
int lastError = 0;
// fast speed
float Kp = 0.2; // proportional gain value
float Ki = .001; // integrative gain value
float Kd = 0; // derivative gain value
// slow speed
//float Kp = 0.15; // proportional gain value
//float Ki = .0009; // integrative gain value
//float Kd = 0; // derivative gain value

//Create spiral variable
  double Xspiral = 0;
  int SpiralTightness = 200;

void setup() {
  // set pin modes
  pinMode(LM_enable, OUTPUT);
  pinMode(LM_dir, OUTPUT);
  pinMode(LM_enc, INPUT);
  pinMode(RM_enable, OUTPUT);
  pinMode(RM_dir, OUTPUT);
  pinMode(RM_enc, INPUT);
  
  // set interrupt pull up resistors for consistancy
  digitalWrite(LM_enc, HIGH);
  digitalWrite(RM_enc, HIGH);
  
  // initialize motors
  analogWrite(LM_enable, LM_PWM);
  digitalWrite(LM_dir, Forward);
  analogWrite(RM_enable, RM_PWM);
  digitalWrite(RM_dir, !Forward);
  
  //initialize interrupts
  attachInterrupt(0,LMcounting, FALLING);
  attachInterrupt(1,RMcounting, FALLING);
}

void loop() {
    if (millis()-lastMilli >= loopTime) {
      lastMilli = millis();
      calcSpeed();
      LM_PWM = calcPWM(LM_PWM, LMspeedRef, LMspeedMeas);
      RM_PWM = calcPWM(RM_PWM, RMspeedRef, RMspeedMeas);
      analogWrite(LM_enable, LM_PWM);
      analogWrite(RM_enable, RM_PWM);
      RMspeedRef = (127*atan(Xspiral/SpiralTightness));
      Xspiral++;
    }
}

void calcSpeed() {
  LMspeedMeas = (LM_count-oldLMcount)*(1000/(3*53))*(204/loopTime); //speed in mm/s
  RMspeedMeas = (RM_count-oldRMcount)*(1000/(3*53))*(204/loopTime);
  oldLMcount = LM_count;
  oldRMcount = RM_count;
}

int calcPWM(int oldPWM, int targetSpeed, int actualSpeed){
  float PIDdelta = 0;
  int error = 0;
  error = abs(targetSpeed)-abs(actualSpeed); // calculate error
  cumError += error;     // calculate cumulative error
  PIDdelta = Kp*error-Kd*(error-lastError)+Ki*cumError;
  lastError = error; // set last error
  return constrain(oldPWM+int(PIDdelta),0,255); // constrain the new value to range
}

void LMcounting() {
  LM_count++;
}

void RMcounting() {
  RM_count++;
}

