#ifndef _GESTURE_HANDLER_H
#define _GESTURE_HANDLER_H


#include "Arduino.h"

class GestureHandler 
{
  public:
   GestureHandler( uint8_t aPin_APD9960 = D5 );
   void init ( void );
   void checkForGestures( void ) ;
   void handleGesture( void );   

 private:
  uint8_t PinAPDS9960_INT;
    

};

#endif
