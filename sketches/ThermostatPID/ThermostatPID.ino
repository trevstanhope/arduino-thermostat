/*
ThermostatPID.ino - Arduino rotary thermostat PID controller

Input: DHT22 Temperature/Humidity Sensor
System: PID
Ouptut: Servo motor

*/

/* --- Libraries --- */
#include <PID_v1.h>
#include <DHT.h>
#include <Servo.h> 

/* --- Constants --- */
const int DHT_PIN = 3;
const int SERVO_PIN = 9;
const int DHT_TYPE = 22;
const int BAUD = 9600;

/* --- Variables --- */
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);
DHT dht(DHT_PIN, DHT_TYPE);
Servo servo;

void setup() {
  
  /* --- PID --- */
  Input = 
  Setpoint = 100;
  myPID.SetMode(AUTOMATIC);

  /* --- I/O --- */
  servo.attach(SERVO_PIN);
  dht.begin();
  Serial.begin(BAUD);
}

void loop() {
  Input = dht.readTemperature();
  myPID.Compute();
  servo.write(Output);

}
