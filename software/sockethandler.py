import socket
import logging

class SocketHandler:

    def __init__(self, sock=None):
        
        self.connected = False
        
        if sock is None:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock            
            
    def isConnected (self):
        return (self.connected)

    def connect(self, host, port):
        try:
            self.sock.connect((host, port))
            #self.sock.setblocking(0)
            self.connected = True
        except socket.error:
            print("Caught exception socket connect")
            print(socket.error)

    def disconnect(self):
        self.sock.close()
        self.connected = False

    def send(self, data):
        sent = self.sock.send(data.encode('utf-8'))
        logging.info('Sent: ' + data)        
        if sent == 0:
            logging.info("Socket connection broken")
            raise RuntimeError("socket connection broken")

    def receive(self):
        data = self.sock.recv(1024)
        return data
