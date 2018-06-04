#include "LEDsManager.h"




Adafruit_NeoPixel strip = Adafruit_NeoPixel(DEFAULT_NUM_LEDS, DEFAULT_LED_PIN, NEO_GRB + NEO_KHZ800); 

LEDsManager::LEDsManager( uint8_t aNum_LEDs, uint8_t aLEDs_PIN ) 
{  
    LEDs_PIN = aLEDs_PIN;        
}

void LEDsManager::init( uint8_t aInitialBrightness)
{     
    strip.begin();   
    strip.setBrightness(10);
    strip.show();
}


void LEDsManager::setColor( uint8_t aR, uint8_t aG, uint8_t aB )
{   
    uint32_t currentColor = (( uint32_t ) aR << 16 ) | (( uint32_t ) aG << 8) | aB; 
    setColor( currentColor);
}

void LEDsManager::setColor( uint32_t aColor )
{    
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, aColor);
        strip.show();       
    }
}

void LEDsManager::setBrightness( uint8_t aBrightness )
{
    strip.setBrightness( aBrightness ); 
}


void LEDsManager::setEffect(uint8_t aEffect)
{
  currentEffect = aEffect; 



}
void LEDsManager::runEffect()
{
  switch(currentEffect)
  {
    case EFFECT_RAINBOW:      
      effectRainbow();       
    break;
    
    case EFFECT_BREATHING:
      effectBreathing();
    break;

    case EFFECT_CHASER:
      effectChaser(); 
    break;

    default: 
    break;
  }
}

void LEDsManager::effectRainbow() 
{

   uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(DELAY_EFFECTS);
  }
  
 
}

void LEDsManager::effectBreathing() 
{
  
}

void LEDsManager::effectChaser()
{ 

}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

uint32_t LEDsManager::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}





