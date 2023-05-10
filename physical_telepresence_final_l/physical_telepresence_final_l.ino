
//include the servo library
#include <Servo.h>

//create a servo object called myservo 
Servo myservo;
Servo otherservo;

//vairables from webserial code
const int inputPin = A0;
boolean newData = false;
unsigned char receivedChar;

int otherAngle = 0;
int thisAngle = 85;

//variables for "button"
int lightValue = 0;
bool buttonPressed = false;
bool isAwaitingButtonPress = true;
bool servoActivated = false;

bool isOn = true;

void setup() { // put your setup code here, to run once:

  pinMode(inputPin, INPUT); // set pin to input

  // set the servo pin, pin 9, as an servo output pin 
  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  otherservo.attach(6);// attaches the servo on pin 6 to the incoming data servo
  Serial.begin(9600); // initialize serial communications
}

void loop() { //  main code here, to runs repeatedly

  

  // read the light sensor value (input pin)
  int newLightValue = analogRead(inputPin);
  
  if (isAwaitingButtonPress && !buttonPressed && newLightValue < 150) {
    // button has been pressed!
    servoActivated = !servoActivated; // flip servo  state only once
    isAwaitingButtonPress = false; 
    if (servoActivated){
      myservo.write(thisAngle);
      delay(1500);
      otherservo.write(40);
      isOn = true;
    } else {
      myservo.write(0);
      delay(1500);
      otherservo.write(0);
      isOn = false;
    }
  } else if (newLightValue > 200) { // start awaiting next press
    isAwaitingButtonPress = true;
    buttonPressed = false;
  }
  
  // map the light readings to the angle possible by the servo motor 
  // int mappedLight = map (newLightValue, 0, 1023, 0, 180);

  // control the servo motor based on the light value read, adjust linearly by angles 
  
  
  // Print values.
  // send values out serial port as a JSON string,
  // the resulting JSON lookins like this: {"angle":mappedPot}
  Serial.print("{\"buttonPressed\":");
  Serial.print(isOn);
  Serial.println("}");


  // store the current light value for the next loop iteration
  lightValue = newLightValue;

  
  //copied over webserial code
  recvOneChar();
  updateServo();
  // slight delay to stabilize the ADC:
  delay(1);    

}

void updateServo() {
    if (newData == true) {
        int receivedAngle = receivedChar - '0';
          if (receivedAngle == 1){
          otherservo.write(120);
          } else if(receivedAngle == 0){
          otherservo.write(0);
          }
        //otherservo.write(receivedAngle);
        newData = false;
    }
}

// adding webserial code
void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}
