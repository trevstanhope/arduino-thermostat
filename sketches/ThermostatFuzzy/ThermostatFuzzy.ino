/*
ThermostatFuzzy.ino - Arduino rotary thermostat fuzzy controller
*/

/* --- Libraries --- */
#include <Servo.h> 
#include <DHT.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzySet.h>
#include <FuzzyInput.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyIO.h>

/* --- Constants --- */
const int DHT_PIN = 3;
const int SERVO_PIN = 9;
const int DHT_TYPE = 22;
const int BAUD = 9600;

/* --- Objects --- */
DHT dht(DHT_PIN, DHT_TYPE);
Servo servo;
Fuzzy* fuzzy = new Fuzzy();

void setup() {
  
  /* ---- Fuzzy --- */
  // Fuzzy Input is temperature
  FuzzyInput* temperature = new FuzzyInput(1);
  FuzzySet* cold = new FuzzySet(0, 20, 20, 40);
  FuzzySet* ideal = new FuzzySet(30, 50, 50, 70);
  FuzzySet* hot = new FuzzySet(60, 80, 80, 80);
  temperature->addFuzzySet(cold);
  temperature->addFuzzySet(ideal);
  temperature->addFuzzySet(hot);
  fuzzy->addFuzzyInput(temperature);
  
  // Fuzzy Output is rotor
  FuzzyOutput* rotor = new FuzzyOutput(1);
  FuzzySet* CCW = new FuzzySet(0, 10, 10, 20); 
  FuzzySet* zero = new FuzzySet(10, 20, 30, 40); 
  FuzzySet* CW = new FuzzySet(30, 40, 40, 50); 
  rotor->addFuzzySet(CCW);
  rotor->addFuzzySet(zero); 
  rotor->addFuzzySet(CW); 
  fuzzy->addFuzzyOutput(rotor); 
  
  // Fuzzy Fule for cold temp and CCW rotor
  FuzzyRuleAntecedent* ifTemperatureCold = new FuzzyRuleAntecedent(); // Antecedent
  ifTemperatureCold->joinSingle(cold);
  FuzzyRuleConsequent* thenRotorCCW = new FuzzyRuleConsequent(); // Consequence
  thenRotorCCW->addOutput(CCW);
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifTemperatureCold, thenRotorCCW); // Takes: (0) # ???, (1) antecedent, and (2) consequence
  fuzzy->addFuzzyRule(fuzzyRule01);
  
  // Fuzzy Rule for ideal temp and zero'd rotor
  FuzzyRuleAntecedent* ifTemperatureIdeal = new FuzzyRuleAntecedent(); // Antecedent
  ifTemperatureIdeal->joinSingle(ideal);
  FuzzyRuleConsequent* thenRotorZero = new FuzzyRuleConsequent(); // Consequence
  thenRotorZero->addOutput(zero);
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifTemperatureIdeal, thenRotorZero); // Takes: (0) # ???, (1) antecedent, and (2) consequence
  fuzzy->addFuzzyRule(fuzzyRule02);
  
  // Fuzzy Rule for hot temperature and CW rotor
  FuzzyRuleAntecedent* ifTemperatureHot = new FuzzyRuleAntecedent();  // Antecedent
  ifTemperatureHot->joinSingle(hot);
  FuzzyRuleConsequent* thenRotorCW = new FuzzyRuleConsequent(); // Consequence
  thenRotorCW->addOutput(CW); 
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifTemperatureHot, thenRotorCW); // Takes: (0) # ???, (1) antecedent, and (2) consequence
  fuzzy->addFuzzyRule(fuzzyRule03);
  
  /* --- I/O --- */
  servo.attach(SERVO_PIN);
  dht.begin();
  Serial.begin(BAUD);
}

void loop() {
   float input = dht.readTemperature();
   fuzzy->setInput(1, input);
   fuzzy->fuzzify();
   float output = fuzzy->defuzzify(1);
   servo.write(output);
}
