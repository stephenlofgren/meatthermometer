#include <Arduino.h>
#include "headers/temp.h"

int _sensorPin;
int Vo, Vsum, Vcount; 
float R1 = 4700;
float R2, logR2, T, outputVoltage;
float c1 = 1.096240521e-03, c2 = 1.778683147e-04, c3 = 2.824818659e-07;

void setSensorPin(int pin){
    _sensorPin = pin;
}

float readResistance() {
  Vsum = 0;
  for(Vcount =0; Vcount < 500; Vcount++)
  {
    Vo = analogRead(_sensorPin);
    Vsum = Vsum + Vo;
    delay(10);
    //Serial.print("reading ");
    //Serial.println(Vo);
  }
  
  Vo = Vsum / Vcount;
  //I calibrated the voltage range to get as close to the actual voltage measurment as possible
  outputVoltage = (float)Vo * (float)(3.125 / 1023.0);
  //I am not sure this is the right equation but I have calibrated everything to the resistor output using this equation
  R2 = R1 * (3.3/outputVoltage - 1);
  return R2;
}

float calculateTempF(float r){
  logR2 = log(r);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  
  T = T - 273.15;
  T = (T * 9.0)/ 5 + 32.0; 

  println("Resistance: " + String(r) + " works out to " + String(T) + "F");
  return T;
}

float readTemperature(){
  //calculateTempF(27639.83);
  //calculateTempF(18068.4);
  //calculateTempF(12056.94);
  //calculateTempF(8186.68);
  return calculateTempF(readResistance());
}
