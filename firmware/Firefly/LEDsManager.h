#ifndef _LEDS_MANAGER_H
#define _LEDS_MANAGER_H


#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define DEFAULT_LED_PIN 2
#define DEFAULT_NUM_LEDS 10

#define MAX_PREDEFINED_COLORS 7
#define MAX_PREDEFINED_BRIGHTNESS_INDEX 10


#define DELAY_EFFECTS 100

#define EFFECT_NONE 0 
#define EFFECT_RAINBOW 1
#define EFFECT_BREATHING 2
#define EFFECT_CHASER 3

class LEDsManager 
{
  public:
   LEDsManager( uint8_t aNum_LEDs = DEFAULT_NUM_LEDS, uint8_t aLEDs_PIN = DEFAULT_LED_PIN );
   void init ( uint8_t aInitialBrightness = 25 );
   void setColor( uint8_t aR, uint8_t aG, uint8_t aB );
   void setColor( uint32_t aColor );
   void setBrightness( uint8_t aBrightness); 
   void switchLamp( void );
   void nextColor( void );
   void nextBrightness( void );
   void setEffect ( uint8_t aEffect );
   void runEffect( void );
   
 private:
  uint8_t LEDs_PIN;  
  uint32_t currentColor;
  uint8_t currentBrightness;
  uint8_t lampON;
  uint8_t currentEffect;
  uint8_t currentColorIndex;
  uint8_t currentBrightnessIndex; 
  
  void effectRainbow(); 
  void effectBreathing();
  void effectChaser(); 
  uint32_t Wheel(byte WheelPos);
 
    

};

#endif
