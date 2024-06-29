import pytest, time, zlib
from DSSClient import DSSClient
from serial import Serial

test_messages = [
    ("message from pytest!", zlib.crc32(b'message from pytest!') & 0xFFFFFFFF),
    ("MESSAGE FROM PYTEST!", zlib.crc32(b'MESSAGE FROM PYTEST!') & 0xFFFFFFFF),
    ("Message from pytest!", zlib.crc32(b'Message from pytest!') & 0xFFFFFFFF)
]

print("\n")
print("[TEST MESSAGES & EXPECTED CRC CODES]")
for message in test_messages:
    print("    {}: 0x{:08X}".format(message[0], message[1]))
print("")


@pytest.fixture
def debugger():
    return DSSClient("127.0.0.1", 4444)

@pytest.fixture
def tty():
    return Serial('/dev/ttyACM0', 9600, timeout = 1)

@pytest.mark.parametrize("message, expected_crc", test_messages)
def test_serial_rx(debugger, tty, message: str, expected_crc: int):
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

    # rx_crc_address = (debugger.command({"name": "getVariableAddress" , "variable":"serial_rx_buf"}, True))
    # print("{}:{}".format(type(rx_crc_address), rx_crc_address))
    rx_crc_value = debugger.command({
        "name": "readData",
        "page": 0,
        "address": 0x0000AADE + 6, # Hard-coded struct member address from firmware.map
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

    print("                MESSAGE: \x1b[1;34m{}\x1b[0m".format(rx_message))
    print("                    CRC: \x1b[1;33m0x{:08X}\x1b[0m".format(int(rx_crc_value)))

    assert int(rx_crc_value) == expected_crc
    assert rx_message == message

    debugger.__exit__()