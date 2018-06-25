#include "GestureHandler.h"
#include "LEDsManager.h"


#include <SparkFun_APDS9960.h>


SparkFun_APDS9960 apds = SparkFun_APDS9960();

volatile uint8_t APDS_INT_flag;
LEDsManager lm;

void APDS9960_ISR() 
{
  APDS_INT_flag = 1;
}

GestureHandler::GestureHandler( uint8_t aPinAPDS9960_INT ) 
{    
    APDS_INT_flag = 0;
    PinAPDS9960_INT = aPinAPDS9960_INT;
}

void GestureHandler::init()
{
    pinMode(PinAPDS9960_INT, INPUT);
    
    // Initialize interrupt service routine
    attachInterrupt(PinAPDS9960_INT, APDS9960_ISR, FALLING);

    // Initialize APDS-9960 (configure I2C and initial values)
    if ( apds.init() ) 
    {
        Serial.println(F("APDS-9960 initialization complete"));
    } 
    else 
    {
        Serial.println(F("Something went wrong during APDS-9960 init!"));
    }

    // Start running the APDS-9960 gesture sensor engine
    if ( apds.enableGestureSensor(true) ) 
    {
        Serial.println(F("Gesture sensor is now running"));
    } 
    else 
    {
        Serial.println(F("Something went wrong during gesture sensor init!"));
    }
}

void GestureHandler::checkForGestures( void )
{    
    /*if( APDS_INT_flag == 1 ) 
    {
        detachInterrupt(0);
        handleGesture();
        APDS_INT_flag = 0;
        attachInterrupt(PinAPDS9960_INT, APDS9960_ISR, FALLING);
    }*/

    if (digitalRead(PinAPDS9960_INT))
    {
      handleGesture();
    }
}

void GestureHandler::handleGesture( void )
{
    if ( apds.isGestureAvailable() ) 
    {
        switch ( apds.readGesture() ) 
        {
          case DIR_UP:
            Serial.println("DOWN");
            break;
          case DIR_DOWN:
            Serial.println("UP");
            break;
          case DIR_LEFT:
            Serial.println("RIGHT");
            lm.nextColor();
            break;
          case DIR_RIGHT:
            Serial.println("LEFT");
            lm.nextBrightness();
            break;
          case DIR_NEAR:
            Serial.println("NEAR");
            break;
          case DIR_FAR:
            Serial.println("FAR");
            lm.switchLamp();
            break;
          default:
            Serial.println("NONE");
        }
    }
}


