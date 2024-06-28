// Import the DSSServer code
load("/home/runner/projects/c2000-ota-update/test/debug-server-scripting/DSSServer.js");

/* Details of the board configuration.  */
board_configuration = {
  configFile: "/home/runner/projects/c2000-ota-update/test/debug-server-scripting/tms320f280049c.ccxml",
     timeout: 150000,
     logFile: "/home/runner/projects/c2000-ota-update/test/test_server.xml",
    sessions: [
        {
            name:	"Texas Instruments XDS110 USB Debug Probe_0/C28xx_CPU1"
        }
    ]
};

/* Start the server, for the sessions start using TCP/IP ports from 4444.  */
var server = new DSSServer(board_configuration, 4444);

var commands = {
	"getVariableAddress"    :getVariableAddress,
    "getConnectionStatus"   :getConnectionStatus
};

server.addHandlers(commands);
server.run();

function getVariableAddress(session, command) {
    try {     
        // var address = session.symbol.getAddress(command.variable);
        var address = session.expression.evaluate(command.variable);
    } catch (err) {
        return {"status":"FAIL","message":""+err};                  
    }      
    return {"status": "OK","value":address};
}

function getConnectionStatus(session) {
    if (session.target.isConnected()) {
        return {"status":"OK"};
    } else {
        return {"status":"FAIL"};
    }
}