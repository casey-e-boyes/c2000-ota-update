import os, subprocess, sys, time
from DSSClient import DSSClient
from serial import Serial

# cd /home/runner/tooling/ti/ccs1271/ccs/ccs_base/scripting/bin
# clear; ./dss.sh /home/runner/projects/c2000-ota-update/test/debug-server-scripting/DSSServerLaunch.js
# clear; pytest python_client.py -sv -rA


# DSSLauncher     = "/home/runner/tooling/ti/ccs1271/ccs/ccs_base/scripting/bin/dss.sh"
# DSSLaunchScript = "/home/runner/projects/c2000-ota-update/test/debug-server-scripting/DSSServerLaunch.js"

# DSSServer = subprocess.Popen(["sh", DSSLauncher, DSSLaunchScript], stdout=subprocess.PIPE)

# while (1):
#     output = DSSServer.communicate()
#     print(output)

debugger = DSSClient("127.0.0.1", 4444)
tty = Serial('/dev/ttyACM0', 9600, timeout=1)

debugger.command({"name":"load" , "program":"/home/runner/projects/c2000-ota-update/build/firmware.elf"})
# session.breakpoint.add("main.c", 58);
debugger.command({"name": "setBreakPointByLine", "file": "serial.c", "line": "70"})
debugger.command({"name": "runAsynch"})
time.sleep(0.5)

tty.write(b'message from pytest!')

time.sleep(0.5)
debugger.command({"name": "halt"}) # If this fails it has hit the breakpoint

message_address = (debugger.command({"name": "getVariableAddress" , "variable":"rx_buf"}, True))

buffer = debugger.command({
	"name": "readDataArray",
	"page": 0,
	"address": message_address,
	"numValues": 10,	
	"typeSize": 16,
	"signed": 0,
}, True)

# Convert TIC2000's raw packed char16's into printable text
print("".join([str(chr((int(x) >> 8 & 0xFF))) + str(chr(int(x) & 0xFF)) for x in (buffer.split(','))]))


# DSSServer.terminate()