#ifndef _SENSORS_MANAGER_H
#define _SENSORS_MANAGER_H


#include "Arduino.h"

#include <DHTesp.h>
#include "Adafruit_CCS811.h"


#define DHT_PIN D6
#define CCS811_WAKE_PIN D3

class SensorsManager 
{
  public:
   SensorsManager( bool aDHT_EN = false, bool aHDC1080_EN = false, bool aCCS811_EN = false );
   void init ( void );
   void sampleSensors( void ) ;
   float getTemperature ( void );
   float getHumidity ( void );
   float getECO2 ( void );
   float getTVOC ( void );
   

 private:
  uint8_t DHT_EN;
  uint8_t DHT_Pin;
  uint8_t HDC1080_EN;
  uint8_t CCS811_EN;

  float lastHumidity;
  float lastTemperature;
  float lastECO2;
  float lastTVOC;
  
  
  DHTesp dht;
  Adafruit_CCS811 ccs;
  
  
    

};

#endif
