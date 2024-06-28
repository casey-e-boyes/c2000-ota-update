import socket
import json

class DSSClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.connected = False
        self.open()
        self.command({"name": "connect"})

    def __del__(self):
        self.command({"name": "disconnect"})
        self.close()

    def open(self):
        if not self.connected:
            print("[DSS][Connecting to {}:{}]".format(self.host, self.port))
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((self.host,self.port))
            if(s):
                self.connection = s
                self.connected = True
            else:
                self.connection = None
                self.connected = False
                raise Exception("Remote server at port- "+str(self.port)+" on "+str(self.host)+" seems to be down(or not started)\n")
        else:
            print("Connection already established\n")
        
    def close(self):
        if(self.connected):
            self.connection.close()
            self.connection = None
            self.connected = False
        else:
            print("Connection already closed\n")
            
    def execute(self,cmd):
        if(self.connected):
            temp = json.dumps(cmd)+"\n"
            cmd_json = temp.encode('UTF-8')
            self.connection.sendall(cmd_json)
            result = self.connection.recv(1024)
            return json.loads(result)
        else:
            print("Connection is not established")
            return None

    def command(self, command, expect_value=False):
        result = self.execute(command)
        if (result):

            print(str(command['name'])+": "+str(result['status']))
            # If there is a message, print it
            if ('message' in result.keys()):           
                print ("  message: "+str(result['message'])+"\n")
            # If a value was returned, print it
            if ('value' in result.keys()):           
                print("  value: "+str(result['value'])+"\n")
                if (expect_value):
                    return result['value']
        else:
            print (str(command['name'])+" execution failed\n")