#include "SensorsManager.h"


SensorsManager::SensorsManager( bool aDHT_EN, bool aHDC1080_EN, bool aCCS811_EN ) 
{       
  
  DHT_Pin = DHT_PIN;     
  DHT_EN = aDHT_EN;  
  HDC1080_EN = aHDC1080_EN;
  CCS811_EN = aCCS811_EN;  

  lastHumidity = 0;
  lastTemperature = 0;
}

void SensorsManager::init( void )
{
    if ( DHT_EN )
    {
        dht.setup(DHT_Pin); //Init DHT library 
    }
    
    if ( HDC1080_EN )
    {
    }
    
    if ( CCS811_EN )
    {    
        pinMode(CCS811_WAKE_PIN, OUTPUT);
        digitalWrite(CCS811_WAKE_PIN, LOW); // Set wake pin low to keep the CCS811 awake

        if(!ccs.begin())
        {
            Serial.println("Sensor CCS811 not working or not wired correctly");   
        }
    }
}

void SensorsManager::sampleSensors( void )
{
 
   
  if ( DHT_EN )
  { 
    lastHumidity = dht.getHumidity();
    lastTemperature = dht.getTemperature();
    /*
    Serial.print("Humedad: ");
    Serial.print(lastHumidity, 1);
    Serial.print("\tTemperatura: ");
    Serial.print(lastTemperature, 1);
    */
  }
  
  if ( HDC1080_EN )
  { 
    lastHumidity = dht.getHumidity();
    lastTemperature = dht.getTemperature();
    /*
    Serial.print("Humedad: ");
    Serial.print(lastHumidity, 1);
    Serial.print("\tTemperatura: ");
    Serial.print(lastTemperature, 1);*/
  }
  
  if ( CCS811_EN )
  {  
      if(!ccs.readData())
      { 
            lastECO2 = ccs.geteCO2();
            lastTVOC = ccs.getTVOC();
            /*
            Serial.print("\tCO2: ");
            Serial.print(lastECO2);
            Serial.print("ppm,\t TVOC: ");
            Serial.print(lastTVOC);
            Serial.println("ppb");*/
      }
  }   
}

float SensorsManager::getTemperature ()
{
  return (lastTemperature);
}

float SensorsManager::getHumidity()
{
  return (lastHumidity);
}

float SensorsManager::getECO2()
{  
 return (lastECO2);  
}

float SensorsManager::getTVOC()
{  
 return (lastTVOC);  
}



