
#include <TaskScheduler.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "GestureHandler.h"
#include "SensorsManager.h"
#include "LEDsManager.h"

#define SAMPLING_PERIOD 5000


const char* ssid     = "RED_TUTOELECTRO";
const char* password = "tutoelectro123";

//const char* ssid     = "FISIO_WF";
//const char* password = "fisfosFUS";
const uint16_t port = 8266;

WiFiServer TCPServer(port);
WiFiClient TCPClient; 


bool clientConnected = false;

SensorsManager sensorManager = (true, false, true);
GestureHandler gestureHandler;
LEDsManager ledsManager;



void decodeSET (String aPacket);
void decodeCommand(String aPacket);

void checkConnectionStatus();
void sampleSensors();
void checkForGestures();
void runEffects();

uint32_t currentColor = 0;

Task task_ConnectionStatus(100, TASK_FOREVER, &checkConnectionStatus);
Task task_GestureHandler(20, TASK_FOREVER, &checkForGestures);
Task task_LedsManager(100, TASK_FOREVER, &runEffects);
Task task_SensorManager(SAMPLING_PERIOD, TASK_FOREVER, &sampleSensors);

Scheduler scheduler; 
   
void setup() 
{  

  Serial.begin(115200);
  
  ledsManager.init(30);   
  sensorManager.init();
  gestureHandler.init();
  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  TCPServer.begin();

  scheduler.addTask(task_ConnectionStatus);
  scheduler.addTask(task_GestureHandler);
  scheduler.addTask(task_SensorManager);
  scheduler.addTask(task_LedsManager);

  task_ConnectionStatus.enable();
  task_GestureHandler.enable();
  task_SensorManager.enable();
  task_LedsManager.enable(); 
}

void loop() 
{

  scheduler.execute();
  

}

void checkConnectionStatus()
{

  if (!TCPClient.connected()) {
        // try to connect to a new client
        TCPClient = TCPServer.available();
        clientConnected = false;
  }    

  else 
  {
        clientConnected = true;  
        if (TCPClient.available() > 0)    //Check for new data
        { 
            String packet = TCPClient.readStringUntil('\r'); 

            Serial.print ("Received: ");
            Serial.println(packet);
                        
            decodeCommand(packet);
        }
  }
  
}

void decodeCommand(String aPacket)
{  
  String command = aPacket.substring(0,3);

  if (command.equals("GET"))
  {
    //To be implemented
  }
  else if ( command.equals ("SET"))
  {
    decodeSET(aPacket);  
  } 
}

void decodeSET (String aPacket)
{  
  String command = aPacket.substring(4,7);  
  
  if (command.equals("COL"))  // S E T   C O L   2 5  5     2  5  5     2  5  5     1  0  0 \r
                              // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  {
    uint8_t r = aPacket.substring(8,11).toInt(); 
    uint8_t g = aPacket.substring(12,15).toInt(); 
    uint8_t b = aPacket.substring(16,19).toInt(); 
    uint8_t i = aPacket.substring(20,23).toInt();   
    currentColor = (( uint32_t ) r << 16 ) | (( uint32_t )g << 8) | b;
    ledsManager.setColor(currentColor);
    ledsManager.setBrightness(i);
  } 
  else if (command.equals("EFE"))
  {    
    String effect = aPacket.substring(8,11);

    if (effect.equals("RAI"))
    {      
      ledsManager.setEffect(EFFECT_RAINBOW);
    }
    else if( effect.equals("BRE"))
    {
      ledsManager.setEffect(EFFECT_BREATHING);
    }
    else if (effect.equals("CHA"))
    {
      ledsManager.setEffect(EFFECT_CHASER);
    }
    else if (effect.equals("NON"))
    {
      ledsManager.setEffect(EFFECT_NONE);      
    }    
  }  
}

void sampleSensors()
{
  sensorManager.sampleSensors();

  if (TCPClient.connected())
  {
     String sensorsDataString = "SEN TEMP:" + String(sensorManager.getTemperature());
     sensorsDataString += " HUM:" + String (sensorManager.getHumidity());
     sensorsDataString += " eCO2:" + String (sensorManager.getECO2());
     sensorsDataString += " TVOC:" + String (sensorManager.getTVOC());
     Serial.print("Sent: ");
     Serial.println(sensorsDataString);
     TCPClient.write( sensorsDataString.c_str() );
  }
}

void checkForGestures()
{  
  gestureHandler.checkForGestures();
}

void runEffects()
{
  ledsManager.runEffect();
}




