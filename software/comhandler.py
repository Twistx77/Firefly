
from kivy.app import App

from sockethandler import SocketHandler

import logging
import pdb

class ComHandler:

    def __init__(self, mainWindow):

        self.PORT = 8266 # Socket Port

        self.mainWindow = mainWindow
        
        self.FRAME_DECODDER = {
            'STA' 	: self.statusReceived,
	        'SEN' 	: self.sensorsReceived
        } 
	      

        self.socketHandler = SocketHandler()
    
    def isConnected(self):
        return self.socketHandler.isConnected()
    
    def connect(self, ip):
        self.socketHandler.connect(ip, self.PORT)        

    def disconnect(self):
        self.socketHandler.disconnect()        

    def checkForData(self):
        while (self.socketHandler.isConnected()):
            data  = self.socketHandler.receive()
            self.dataReceived(data)    

    def dataReceived (self, data): 
        dataString = data.decode('utf-8')   
        logging.info('Data Received: ' + dataString)
        command = dataString.split()[0]        
        self.FRAME_DECODDER [command](dataString)
        
    def statusReceived(self, frame):
        subFrames = frame.split()      
        logging.info('Data Received: ' + subFrames[1] )
        pass


    def sensorsReceived(self, frame):
        subFrames = frame.split()        
        temp = subFrames[1][5:]
        hum = subFrames[2][4:]
        eCO2 = subFrames[3][5:]
        TVOC = subFrames[4][5:]
        
        self.mainWindow.updateSensors(temp ,hum, eCO2, TVOC)
        
    def getStatus(self):    
        data = 'GET STA\r'
        self.socketHandler.send(data)  
        
    def setColor(self, r, g, b, i):
        data = 'SET COL ' + r + ' ' + g +' ' + b + ' ' + i +'\r'
        print(data)
        self.socketHandler.send(data)
    
    def setEffect(self, effect):
        data = 'SET EFE ' + effect + '\r'
        print(data)
        self.socketHandler.send(data)
    
         
