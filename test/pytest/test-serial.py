import pytest, time
from DSSClient import DSSClient
from serial import Serial


@pytest.fixture
def debugger():
    return DSSClient("127.0.0.1", 4444)

@pytest.fixture
def tty():
    return Serial('/dev/ttyACM0', 9600, timeout = 1)

@pytest.mark.parametrize("message", ["message from pytest!", "MESSAGE FROM PYTEST!", "Message from pytest!"])
def test_serial_rx(debugger, tty, message: str):
    debugger.__enter__()
    debugger.command({"name":"load" , "program":"/home/runner/projects/c2000-ota-update/build/firmware.elf"})
    debugger.command({"name": "runAsynch"})

    time.sleep(0.5)
    tty.write(message.encode('UTF-8'))
    time.sleep(0.5)

    debugger.command({"name": "halt"})

    rx_buffer_address = (debugger.command({"name": "getVariableAddress" , "variable":"rx_buf"}, True))
    rx_buffer_contents = debugger.command({
        "name": "readDataArray",
        "page": 0,
        "address": rx_buffer_address,
        "numValues": 10,	
        "typeSize": 16,
        "signed": 0,
    }, True)

    rx_crc_address = (debugger.command({"name": "getVariableAddress" , "variable":"serial_rx_buf"}, True))
    print("{}:{}".format(type(rx_crc_address), rx_crc_address))
    rx_crc_value = debugger.command({
        "name": "readData",
        "page": 1,
        "address": int(rx_crc_address) + 5,
        "typeSize": 32,
        "signed": 0,
    }, True)

    # Raw Message Buffer
    # rx_message = "".join([str(chr((int(x) >> 8 & 0xFF))) + str(chr(int(x) & 0xFF)) for x in (rx_buffer_contents.split(','))])

    # CRC Processed Message Buffer
    rx_message = ""
    for raw_char in rx_buffer_contents.split(','):
        rx_message += (str(chr(int( '{:08b}'.format((int(raw_char) & 0xFF))[::-1], 2  ))))
        rx_message += (str(chr(int( '{:08b}'.format(((int(raw_char) >> 8) & 0xFF))[::-1], 2  ))))

    print("\x1b[1;34m{:>{length}}\x1b[0m".format(rx_message, length = len(rx_message) + 25))

    print((rx_crc_value))

    debugger.__exit__()