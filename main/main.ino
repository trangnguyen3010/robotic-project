#include <LiquidCrystal.h>  //the liquid crystal library contains commands for printing to the display

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  // tell the RedBoard what pins are connected to the display

struct Button {
  String name;
  int pin;
};

struct Person {
  String name;
  Button button;
};

const int trigPin = A4;           //connects to the trigger pin on the distance sensor
const int echoPin = A5;           //connects to the echo pin on the distance sensor
float distance = 0;               //stores the distance measured by the distance sensor

/********************************************************************************/
Button yellowButton = { "Yellow", A0 };
Button greenButton = { "Green", A1 };
Button redButton = { "Red", A2 };
Button blueButton = { "Blue", A3 };
Button buttons[] = { yellowButton, greenButton, redButton, blueButton };

// Initializing persons
Person trang = { "Trang", yellowButton };
Person minh = { "Minh", greenButton };
Person anh = { "Anh", redButton };

// Using pointers to Person objects
Person* roommates[] = { &trang, &minh, &anh };
int roommateNo = sizeof(roommates) / sizeof(roommates[0]);
int currentPerson = 0;

//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 13;  //control pin 1 on the motor driver for the right motor
const int AIN2 = 12;  //control pin 2 on the motor driver for the right motor
const int PWMA = 11;  //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 10;  //speed control pin on the motor driver for the left motor
const int BIN2 = 9;   //control pin 2 on the motor driver for the left motor
const int BIN1 = 8;   //control pin 1 on the motor driver for the left motor

const int driveTime = 20;  //this is the number of milliseconds that it takes the robot to drive 1 inch
                           //it is set so that if you tell the robot to drive forward 25 units, the robot drives about 25 inches
const int DISTANCE_DISPENSE_ONE_BAG = 300;

/********************************************************************************/
void setup() {
  //set all of the button pins to input_pullup (use the built-in pull-up resistors)
  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  pinMode(trigPin, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  lcd.begin(16, 2);  //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(9600);  //begin serial communication with the computer
}

/********************************************************************************/
void loop() {
  if (buttonCheck()) {
    lcd.clear();
  }
  displayOnLcd();

  distance = getDistance();   //variable to store the distance measured by the sensor
  Serial.print(distance);     //print the distance that was measured
  Serial.println(" in");      //print units after the distance

  if (distance < 2) {
    bagIsDispensed();
    dispenseBag();
    delay(200);
  }

  delay(50);
}

void displayOnLcd() {
  lcd.setCursor(0, 0);                                      //set the cursor to the 0,0 position (top left corner)
  lcd.print("Next turn:" + roommates[currentPerson]->name);  //print hello, world! starting at that position
  lcd.setCursor(0, 1);
  lcd.print("T.   M.   A.");
}

void displayOnLcd(String customized) {
  lcd.setCursor(0, 0);                                      //set the cursor to the 0,0 position (top left corner)
  lcd.print(customized);
  lcd.setCursor(0, 1);
  lcd.print("T.   M.   A.");
}

/********************************************************************************/
//CHECK WHICH BUTTON IS PRESSED
bool buttonCheck() {
  // Check if any buttons are being pressed
  // Set all of the button pins to input_pullup (use the built-in pull-up resistors)
  if (digitalRead(yellowButton.pin) == LOW) {
    checkButtonPressed(trang);
    while (digitalRead(yellowButton.pin) == LOW ) {}
    return true;
  } else if (digitalRead(greenButton.pin) == LOW) {
    checkButtonPressed(minh);
    while (digitalRead(greenButton.pin) == LOW ) {}
    return true;
  } else if (digitalRead(redButton.pin) == LOW) {
    checkButtonPressed(anh);
    while (digitalRead(redButton.pin) == LOW ) {}
    return true;
  } else if (digitalRead(blueButton.pin) == LOW) {
    Serial.println("blue");
    bagIsDispensed();
    dispenseBag();
    while (digitalRead(blueButton.pin) == LOW ) {}
    return true;
  }
  return false;
}
/********************************************************************************/
// DISPENSE TRASH BAGS
void bagIsDispensed(){
  displayOnLcd("Bag is cominggg!");
}

void dispenseBag() {
    rightMotor(-200);                     //drive the right wheel forward
    leftMotor(-200);                      //drive the left wheel forward
    delay(driveTime * DISTANCE_DISPENSE_ONE_BAG);  //drive the motors long enough travel the entered distance
    rightMotor(0);                        //turn the right motor off
    leftMotor(0);                         //turn the left motor off
}
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
/********************************************************************************/
// CHECK PERSON
void checkButtonPressed(Person& person) {
  if (comparePersonsByReference(roommates[currentPerson], &person)) {
    nextPerson();
  } else {
    wrongPerson();
  }
}

void nextPerson(){
  displayOnLcd("Good job, love!");
  delay(200);
  if (currentPerson == roommateNo - 1) {
    currentPerson = 0;
  } else {
    currentPerson += 1;
  }
  Serial.println("Change Person!!!");
}

void wrongPerson() {
  displayOnLcd("Wrong personnn!");
  delay(200);
  Serial.println("WRONG");
}

bool comparePersonsByReference(Person* p1, Person* p2) {
  return (p1 == p2);
}

/********************************************************************************/
//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  
  return calculatedDistance;              //send back the distance that was calculated
}
