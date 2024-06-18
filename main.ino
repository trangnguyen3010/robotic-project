//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 13;  //control pin 1 on the motor driver for the right motor
const int AIN2 = 12;  //control pin 2 on the motor driver for the right motor
const int PWMA = 11;  //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 10;  //speed control pin on the motor driver for the left motor
const int BIN2 = 9;   //control pin 2 on the motor driver for the left motor
const int BIN1 = 8;   //control pin 1 on the motor driver for the left motor

int switchPin = 7;  //switch to turn the robot on and off

const int driveTime = 20;  //this is the number of milliseconds that it takes the robot to drive 1 inch
                           //it is set so that if you tell the robot to drive forward 25 units, the robot drives about 25 inches

String distance;  //the distance to travel in each direction

/********************************************************************************/
void setup() {
  pinMode(switchPin, INPUT_PULLUP);  //set this as a pullup to sense whether the switch is flipped

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(9600);  //begin serial communication with the computer

  //prompt the user to enter a command
  Serial.println("Enter a direction followed by a distance.");
  Serial.println("f = forward, b = backward, r = turn right, l = turn left");
  Serial.println("Example command: f 50");
}

/********************************************************************************/
void loop() {
  if (digitalRead(7) == LOW) {   //if the switch is in the ON position
    if (Serial.available() > 0)  //if the user has sent a command to the RedBoard
    {
      distance = Serial.readStringUntil(' ');  //read the characters in the command until you reach the second space

      //print the command that was just received in the serial monitor
      Serial.print(" ");
      Serial.println(distance.toInt());

      rightMotor(-200);                     //drive the right wheel forward
      leftMotor(-200);                      //drive the left wheel forward
      delay(driveTime * distance.toInt());  //drive the motors long enough travel the entered distance
      rightMotor(0);                        //turn the right motor off
      leftMotor(0);                         //turn the left motor off
    }
  }
}

/********************************************************************************/
void rightMotor(int motorSpeed)  //function for driving the right motor
{
  if (motorSpeed > 0)  //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);  //set pin 1 to high
    digitalWrite(AIN2, LOW);   //set pin 2 to low
  } else if (motorSpeed < 0)   //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);   //set pin 1 to low
    digitalWrite(AIN2, HIGH);  //set pin 2 to high
  } else                       //if the motor should stop
  {
    digitalWrite(AIN1, LOW);  //set pin 1 to low
    digitalWrite(AIN2, LOW);  //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));  //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void leftMotor(int motorSpeed)  //function for driving the left motor
{
  if (motorSpeed > 0)  //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);  //set pin 1 to high
    digitalWrite(BIN2, LOW);   //set pin 2 to low
  } else if (motorSpeed < 0)   //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);   //set pin 1 to low
    digitalWrite(BIN2, HIGH);  //set pin 2 to high
  } else                       //if the motor should stop
  {
    digitalWrite(BIN1, LOW);  //set pin 1 to low
    digitalWrite(BIN2, LOW);  //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));  //now that the motor direction is set, drive it at the entered speed
}
