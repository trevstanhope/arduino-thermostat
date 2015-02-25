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
const int IDEAL_TEMP = 20;
const float K_P = 1.0;
const float K_I = 0.5;
const float K_D = 0.0;
const int SERVO_MIN = 0;
const int SERVO_CENTER = 90;
const int SERVO_MAX = 180;

/* --- Variables --- */
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint,K_P, K_I, K_D, DIRECT);
DHT dht(DHT_PIN, DHT_TYPE);
Servo servo;
int pwm;

void setup() {
  
  /* --- PID --- */
  Input = IDEAL_TEMP;
  Setpoint = IDEAL_TEMP;
  myPID.SetMode(AUTOMATIC);

  /* --- I/O --- */
  servo.attach(SERVO_PIN);
  dht.begin();
  Serial.begin(BAUD);
}

void loop() {
  
  Input = dht.readTemperature();
  myPID.Compute();
  pwm = Output + SERVO_CENTER;
  if (pwm > SERVO_MAX) {
     pwm = SERVO_MAX;
   }
  else if (pwm < SERVO_MAX) {
    pwm = SERVO_MIN;
  }
  servo.write(pwm);

}
