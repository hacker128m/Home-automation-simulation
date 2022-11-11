/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLTolsXimF"
#define BLYNK_DEVICE_NAME "Home automation"
#define BLYNK_AUTH_TOKEN "ykpBIP3AiujlLTKCtIpTU8kAQtunKKt-"


// Comment this out to disable prints 
#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
//#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw,inlet_sw,outlet_sw;
unsigned int tank_volume;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// This function is called every time the Virtual Pin 0 state changes
/*To turn ON and OFF cooler based virtual PIN value*/
BLYNK_WRITE(COOLER_V_PIN)
{
  int value = param.asInt();
  if(value)
  {
    cooler_control(ON);
    lcd.setCursor(7,0);
    lcd.print("CO_LR ON");
  }
  else
  {
    cooler_control(OFF);
    lcd.print("CO_LR OFF");
  }
  
}

//To turn ON and OFF heater based virtual PIN value
BLYNK_WRITE(HEATER_V_PIN )
{
  heater_sw= param.asInt();
  if(heater_sw)
  {
    heater_control(ON);
    lcd.setCursor(7,0);
    lcd.print("HT_R ON");
  }
  else
  {
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("HT_R OFF");
  }
}

/*To turn ON and OFF inlet vale based virtual PIN value*/
/*BLYNK_WRITE(INLET_V_PIN)
{
  inlet_sw=param.asInt();
  if(inlet_sw)
  {
    enable_inlet();
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON");
  }
  else
  {
    disable_inlet();
    lcd.setCursor(7,1);
    lcd.print("IN_FL_OFF");
    
  }
}

//To turn ON and OFF outlet value based virtual switch value
BLYNK_WRITE(OUTLET_V_PIN)
{
  outlet_sw=param.asInt();
  if (outlet_sw)
  {
    enable_outlet();
    disable_inlet();
    lcd.setCursor(7,1);
    lcd.print("OUT_FL_ON");
  }
  else
  {
    disable_outlet();
    disable_inlet();
    lcd.setCursor(7,1);
    lcd.print("OUT_FL_OFF");
  }
}
*/
/* To display temperature and water volume as gauge on the Blynk App*/  
void update_temperature_reading()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(TEMPERATURE_GAUGE,read_temperature());
  
}
void handle_temp(void)
{
  if ((read_temperature() > float(35))&& heater_sw)
  {
    heater_sw = 0;
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("HT_R  OFF");
    /*sending temperature for every one second*/
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"temp is greater than 35 ,turning off the heater");
    Blynk.virtualWrite(HEATER_V_PIN,0);
    /*Sending volume of water for every one second*/
    //Blynk.virtualWrite(WATER_VOL_GAUGE,volume());
    Blynk.virtualWrite(HEATER_V_PIN,0);
    
  }
}
/*To control water volume above 2000ltrs*/
/*void handle_tank(void)
{
  if water volume<2000 and inlet valve off
  if ((tank_volume<2000)&&(inlet_sw==0))
  {
    enable_inlet();
    inlet_sw==1;
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON");

    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"Volume of water is less than 2000 turning ON the water in flow");
    Blynk.virtualWrite(INLET_V_PIN,1);
    
  }
  if water volume=3000 and inlet valve on
  if ((tank_volume==3000)&&(inlet_sw==1))
  {
    disable_inlet();
    inlet_sw==0;
    lcd.setCursor(7,1);
    lcd.print("IN_FL_OFF");

    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN,"Volume of water full turning OFF the water in flow");
    Blynk.virtualWrite(INLET_V_PIN,0);
  }
}
*/
void setup(void)
{
    Blynk.begin(auth);
    lcd.init();                     
    lcd.backlight();
    lcd.clear();
    lcd.home(); 
    lcd.setCursor(0,0);
    lcd.print("T==");
    lcd.setCursor(0,1);
    lcd.print("V==");
    
    
    init_temperature_system();
    init_ldr();
    timer.setInterval(1000l, update_temperature_reading);

   //init_serial_tank();
}

void loop(void)
{
   /*To run BLYNK related functions*/
     Blynk.run();
    /*T call set intervals at a particular time*/
     timer.run();
     /*Read temp and display it on dashboard*/
     String temperature;
     temperature= String(read_temperature(),2);
     lcd.setCursor(2,0);
     lcd.print(temperature);
     /* To read and display tank volume*/
     //tank_volume=volume();
     lcd.setCursor(2,1);
     lcd.print(tank_volume);
    /* To control garden lights based on the intensity*/ 
    brightness_control();
    handle_temp();
   //handle_tank();
    
}
