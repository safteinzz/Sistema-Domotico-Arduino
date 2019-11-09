#include <Servo.h>

Servo servo;
const int sensorLuz = A0;
const int pinServo = 9;

void setup() {
  pinMode(pinServo,OUTPUT);
  pinMode(sensorLuz, INPUT);
  Serial.begin(9600);
  servo.attach(pinServo);
}

void loop() {
   float l = analogRead(sensorLuz);
   Serial.print(l);
   Serial.print(" ");
   int v = map(l,0,1024,0,179);
   servo.write(v);
   Serial.println(v);
  
}
