import sys
import argparse
import serial
import yaml
from enum import Enum


# Use defaults if yaml is missing or arg not given
DEFAULT_PORT = "COM5"
DEFAULT_BAUD = 115200
args = {"port": DEFAULT_PORT, "baud": DEFAULT_BAUD}

# Yaml args
try:
    with open("arduino_serial.yaml", "r") as stream:
        args = yaml.safe_load(stream)
except Exception as e:
    print(f"[DEBUG] Exception: {e}")
    print("Missing arduino_serial.yaml\n")

# Argparse Setup iff additional args given at command
if len(sys.argv) > 1:
    parser = argparse.ArgumentParser()
    parser.add_argument('--port',
                        type=str,
                        default=DEFAULT_PORT,
                        help='The connecting port')
    parser.add_argument('--baud',
                        type=int,
                        default=DEFAULT_BAUD,
                        help="The baudrate")
    args = parser.parse_args()
    args = vars(args)


# Program functions
def safe_quit():
    print(f"### Exitting {__file__} ###")
    exit()


# Serial Functions
def ser_connect(port, baudrate):
    serial_read_timeout = 5
    serial_write_timeout = 10
    ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            timeout=serial_read_timeout,
            write_timeout=serial_write_timeout,
        )
    return ser


### CommandHandler class and related functions ###
class CommandHandler():
    @staticmethod
    def write():
        msg = input("WRITE>")
        ser.write(msg.encode())
    
    @staticmethod
    def read():
        msg = []
        msg.append(ser.readline().decode("UTF-8"))
        print(msg)

class ValidCommands(Enum):
    QUIT = 0
    WRITE = 1
    READ = 2

    @staticmethod
    def help():
        print("\n~~~ Help Menu ~~~")
        print("1. READ \t-->\tRead from serial")
        print("2. WRITE \t-->\tWrite to serial")
        print("3. QUIT \t-->\tQuit program")
    
CommandHandler.switch = {
    "WRITE": CommandHandler.write,
    "READ": CommandHandler.read,
    "QUIT" : safe_quit
}

def command_handler_script(ser, usr_input):
    usr_input = usr_input.upper()
    if not hasattr(ValidCommands, usr_input):
        print("\n[ERROR] Invalid input!")
        ValidCommands.help()
        return
    CommandHandler.switch[usr_input]()
### END of CommandHandler ###


def main():
    print("### Arduino Serial Tool ###")
    print(f"Starting serial connection on port: {args['port']}"
          f" with baudrate: {args['baud']}")
    global ser
    ser = ser_connect(port=args['port'], baudrate=args['baud'])
    usr_input = ""
    while(True):
        usr_input = input("> ")
        command_handler_script(ser, usr_input)



if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("KeyboardInterrupt...")
        safe_quit()
