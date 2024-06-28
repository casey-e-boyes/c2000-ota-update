import os, subprocess, sys, time
from DSSClient import DSSClient

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
debugger.command({"name":"load" , "program":"/home/runner/projects/c2000-ota-update/build/firmware.elf"})

debugger.command({"name": "runAsynch"})
time.sleep(1)
debugger.command({"name": "halt"})

message_address = (debugger.command({"name": "getVariableAddress" , "variable":"debug_message"}, True))

debugger.command({
	"name": "readDataArray",
	"page": 0,
	"address": message_address,
	"numValues": 18,	
	"typeSize": 16,
	"signed": 0,
})

# DSSServer.terminate()