import sys, socket, json

class DSSClient:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.connected = False

    def __enter__(self):
        self.open()
        self.command({"name": "connect"})
    
    def __exit__(self):
        if self.connected:
            self.command({"name": "removeAllBreakpoints"})
            self.command({"name": "disconnect"})
            self.close()

    def __del__(self):
        if self.connected:
            self.__exit__()

    def open(self):
        if not self.connected:
            print("\n")
            print("    [DSS][Connecting to {}:{}]".format(self.host, self.port))
            skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            skt.settimeout(5.0)
            try: 
                skt.connect((self.host,self.port))
            except Exception as Err:
                self.connection = None
                self.connected = False
                print("    [DSS]\x1b[1;31m[Connection to host failed]\x1b[0m")
                sys.exit(0)
            else:
                self.connection = skt
                self.connected = True
                print("    [DSS]\x1b[1;32m[Connection successful]\x1b[0m")
        else:
            print("    [DSS]\x1b[1;33m[Connection already established]\x1b[0m")
        
    def close(self):
        if self.connected:
            self.connection.close()
            self.connection = None
            self.connected = False
        else:
            print("    [DSS]\x1b[1;33m[Connection already closed]\x1b[0m")
            
    def execute(self, cmd: json):
        if self.connected:
            temp = json.dumps(cmd) + "\n"
            cmd_json = temp.encode('UTF-8')
            self.connection.sendall(cmd_json)
            result = self.connection.recv(1024)
            return json.loads(result)
        else:
            print("    [DSS]\x1b[1;31m[Connection is not established]\x1b[0m")
            # sys.exit(0)
            return None

    def command(self, command: json, expect_value: bool = False):
        result = self.execute(command)
        if result:
            if str(result['status']) == "OK":
                print("    [DSS]\x1b[1;32m[{:<4}]\x1b[0m COMMAND: {}".format(str(result['status']), str(command['name'])))
            else:
                print("    [DSS]\x1b[1;31m[{}]\x1b[0m COMMAND: {}".format(str(result['status']), str(command['name'])))

            # If there is a message, print it
            if ('message' in result.keys()):           
                print("         \x1b[1;31m      \x1b[0m MESSAGE: {}".format(str(result['message'])))
            # If a value was returned, print it or return it
            if ('value' in result.keys()):           
                if (expect_value):
                    return result['value']
                else:
                    print("         \x1b[1;31m        \x1b[0m VALUE: {}".format(str(result['value'])))
        else:
            print("    [DSS]\x1b[1;31m[FAILED COMMAND: {}]\x1b[0m".format(str(command['name'])))