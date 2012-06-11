#include <SPI.h>
#include <Servo.h>

#define ON 1
#define OFF 0
#define MIN_SERVO 544
#define MAX_SERVO 2400

// Control pins
#define LASER_PIN 2
#define AZ_SERVO_PIN 5
#define EL_SERVO_PIN 6

// Communication pins
#define SS_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13

// Manual mode pins
#define LEFT_PIN 3
#define RIGHT_PIN 4
#define UP_PIN 7
#define DOWN_PIN 8
#define MANUAL_PIN 9
#define MANUAL_DELAY 250

Servo el_servo;
Servo az_servo;

// Manual mode, used to set the initial calibration
void manual_mode()
{
  boolean in_manual_mode = true;
  int x_coord = az_servo.readMicroseconds();
  int y_coord = el_servo.readMicroseconds();

  while( in_manual_mode )
  {
    // Get direction or mode change
    if( digitalRead( LEFT_PIN ) == HIGH )
      x_coord = x_coord - 1;
    else if( digitalRead( RIGHT_PIN ) == HIGH )
      x_coord = x_coord + 1;
    else if( digitalRead( UP_PIN ) == HIGH )
      y_coord = y_coord + 1;
    else if( digitalRead( DOWN_PIN ) == HIGH )
      y_coord = y_coord - 1;
    else if( digitalRead( MANUAL_PIN ) == HIGH )
      in_manual_mode = false;

    // Make sure position is within limits
    if( x_coord < MIN_SERVO )
      x_coord = MIN_SERVO;
    if( x_coord > MAX_SERVO )
      x_coord = MAX_SERVO;
    if( y_coord < MIN_SERVO )
      y_coord = MIN_SERVO;
    if( y_coord > MAX_SERVO )
      y_coord = MAX_SERVO;  

    // Set position  
    set_position( x_coord, y_coord );

    // Wait to prevent constant reading
    delay( MANUAL_DELAY );
  }
}

// Move to the specified position
void set_position( int x, int y) 
{
  // Set x coordinate
  az_servo.writeMicroseconds( x );

  // Set y coordinate
  el_servo.writeMicroseconds( y );
}

// Turn laser on or off
void set_laser( int on_off )
{
  if (on_off == ON)
    digitalWrite( LASER_PIN, HIGH);
  else
    digitalWrite( LASER_PIN, LOW);
}

// Get data from wireless
byte get_data()
{
  byte return_value;

  // Enable reading
  digitalWrite( SS_PIN, LOW );

  // Send nothing, return with the value read
  return_value = SPI.transfer( NULL );

  // Disable reading
  digitalWrite( SS_PIN, HIGH );

  return return_value;
}

// Send data over wireless
void send_data( byte value )
{
  // Enable writing
  digitalWrite( SS_PIN, LOW );

  // Send value
  SPI.transfer( value );

  // Disable writing
  digitalWrite( SS_PIN, HIGH );
}

// This is the initialization
void setup() 
{
  // Set outputs
  pinMode( LASER_PIN, OUTPUT );
  pinMode( AZ_SERVO_PIN, OUTPUT );
  pinMode( EL_SERVO_PIN, OUTPUT );

  // Initialize SPI
  // Sets SCK, MOSI, and SS to outputs
  // Pulls SCK and MOSI low, SS high
  SPI.begin();

  // Initialize servos
  el_servo.attach( EL_SERVO_PIN );
  az_servo.attach( AZ_SERVO_PIN );

}

// This runs the program
void loop() 
{
  boolean in_manual_mode;

  while(1)
  {
    in_manual_mode = analogRead( MANUAL_PIN );
    if( in_manual_mode )
      manual_mode();
  }
}







