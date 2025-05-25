#include <RH_ASK.h>
#include <SoftwareSerial.h>
#include <SPI.h>  // Not actually used but needed to compile
#include "Wire.h"
RH_ASK driver;
SoftwareSerial bluetooth(8, 9); 
//SoftwareSerial myblue(0,1);
//motor initializations..
//motor left side
int in1 = 7;
int in2 = 4;
//motor right
int in3 = 3;
int in4 = 2;
//speed
int ena = 5;
int enb = 6;
//ultrasonic..
int trigPin = 12;  // Trigger
int echoPin = 13;
long duration;
float distance;



void setup() {

  //driver.init();
  if (!driver.init()) {
    Serial.println("Radio module initialization failed");
  }

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(ena, 0);
  analogWrite(enb, 0);




  //ultrasonic
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  // driver.init();
  // Set baud rate to 9600

  //   // Send AT commands to configure HC-05 module
  //   myblue.println("AT+UART=115200,0,0"); // Set baud rate to 9600
  //   delay(500);
  //   myblue.println("AT+NAME=MyBluetooth"); // Set device name to "MyBluetooth"
  //   delay(500);

  //   Serial.println("HC-05 configured.");
  // //motor setup
  Serial.begin(9600);
  bluetooth.begin(9600);
}



void backward() {

  analogWrite(ena, 100);
  analogWrite(enb, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void forward() {
  analogWrite(ena, 100);
  analogWrite(enb, 100);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void right() {
  analogWrite(ena, 60);
  analogWrite(enb, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void left() {
  analogWrite(ena, 100);
  analogWrite(enb, 60);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {


  //ultrasonic part

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  delay(200);
    if (distance <= 20) {
      Serial.println(distance);
      stop();

    } 
  //bluetooth part
    char receivedChar;
  if (bluetooth.available() > 0) {

    receivedChar = bluetooth.read();
            if(distance <=20) {
          stop();
        }
    else if ( receivedChar == 'f' || receivedChar == 'F') {
        Serial.println(receivedChar);
        forward();

      } else if ( receivedChar == 'b' || receivedChar == 'B') {
        backward();
        //Serial.println("backward");


      } else if (receivedChar == 'l' || receivedChar == 'L') {
        left();
        //Serial.println("left");


      } else if (receivedChar == 'r' || receivedChar == 'R') {
        right();
        // Serial.println("right");


      } else if (receivedChar == 's' || receivedChar == 'S') {
        // Serial.println("stop");
        stop();
      } else {
        stop();
      }
  }
    char *cmd;
      if (distance <= 20) {
        //Serial.println(distance);
        stop();

      } 
    //receive message from RF module first
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) {  // Non-blocking
      buf[buflen] = '\0';
      cmd = (char *)buf;
      Serial.println(cmd);
        if(distance <=20) {
          stop();
        }

         else if (strcmp(cmd, "F") == 0 || receivedChar == 'f' || receivedChar == 'F') {
          //Serial.println("forward");
          forward();




        } else if (strcmp(cmd, "B") == 0 || receivedChar == 'b' || receivedChar == 'B') {
          backward();
          //Serial.println("backward");


        } else if (strcmp(cmd, "L") == 0 || receivedChar == 'l' || receivedChar == 'L') {
          left();
          //Serial.println("left");


        } else if (strcmp(cmd, "R") == 0 || receivedChar == 'r' || receivedChar == 'R') {
          right();
          // Serial.println("right");


        } else if (strcmp(cmd, "S") == 0 || receivedChar == 's' || receivedChar == 'S') {
          // Serial.println("stop");
          stop();
        } else {
          stop();
        }
      }
}
