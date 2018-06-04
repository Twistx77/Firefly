from kivy.app import App
from kivy.uix.boxlayout import BoxLayout

from kivy.core.window import Window
from kivy.uix.colorpicker import ColorPicker
from kivy.utils import get_hex_from_color

import threading
import logging

from comhandler import ComHandler

class FirefLy(BoxLayout):

    CHECK_FOR_DATA_REFRESH_DELAY = 0.1 # 1ms

    def __init__ (self, *args, **kwargs):

        super(BoxLayout, self).__init__(**kwargs)
        self.comHandler = ComHandler (self)
        
        # Create necessary threads
        self.threads = {}
        self.threads['CheckForData'] = threading.Thread(name='CheckForData',target=self.comHandler.checkForData,daemon=False)
                
    def connect(self, state):    
        if state == 'down':            
            if (self.comHandler.isConnected() == False):
                logging.info('Connecting to: ' + self.ids.tiIP.text )
                self.comHandler.connect(self.ids.tiIP.text)
                logging.info('Connected')
                #self.checkForDataHandler = Clock.schedule_interval(self.comHandler.checkForData, self.CHECK_FOR_DATA_REFRESH_DELAY)        
                self.threads['CheckForData'].start() 
                self.comHandler.getStatus()
        else:
            self.disconnect()     
        
    
    def disconnect(self):
        if (self.comHandler.isConnected() == True):
            self.comHandler.disconnect()

    def setLampState (self):
        if (self.ids.btnOnOff.state == 'down'):
            self.ids.btnOnOff.text = 'On'            
            self.setColor() # Send selected color to lamp
        else:
            self.ids.btnOnOff.text = 'Off'            
            if (self.comHandler.isConnected() == True):
                self.comHandler.setColor('0', '0', '0','0'); # Turn lamp off
              
    def setColor(self):
        color = self.ids.colorPicker.color

        intensity = self.ids.intensitySlider.value
        r = str(int(color[0]*255)).zfill(3)
        g = str(int(color[1]*255)).zfill(3)
        b = str(int(color[2]*255)).zfill(3)
        i = str(int(intensity)).zfill(3)
        
          
        print ("Color Selected R:" + r + " G:"+ g +" B:" + b + " P:" + i)
        if (self.comHandler.isConnected() == True):
            self.comHandler.setColor(r, g, b, i)
    
    def setEffect(self, buttonText):
        if (self.comHandler.isConnected() == True):           
            self.comHandler.setEffect(buttonText.upper()[:3]) #Send only the 3 first charaters of the text in uppercase
        
        
            

    def updateSensors(self, temperature, humidity, eCO2, TVOC):
        self.ids.lblTemp.text =  temperature +"ºC"
        self.ids.lblHum.text =  humidity +"%"
        self.ids.lbleCO2.text =  eCO2 +"ppm"
        self.ids.lblTVOC.text =  temperature +"ppb"

class FirefLyApp(App):
        def build(self):
            self.title = 'Firefly'
            self.app = FirefLy()
            return self.app

        def on_stop(self):
            self.disconnect()
            print("Closing Window")
            
if __name__ == "__main__":
    app = FirefLyApp()
    app.run()
