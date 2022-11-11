#include "temperature_system.h"
#include "Arduino.h"
#include "main.h"


void init_temperature_system(void)
{
    pinMode(COOLER, OUTPUT);
    digitalWrite(cooler_control, LOW);
    pinMode(HEATER, OUTPUT);
    digitalWrite(heater_control, LOW);
    
}

void cooler_control(bool control)
{
  digitalWrite(COOLER, control);
}
float read_temperature(void)
{
  float temperature;
  temperature =(((analogRead(A0)*(float)5/1023)) /(float)0.01);
  return temperature;
}


void heater_control(bool control)
{
    digitalWrite(HEATER, control);
}
