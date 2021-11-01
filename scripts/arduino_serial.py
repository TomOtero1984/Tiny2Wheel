import sys
import os
import argparse
import serial
import yaml
import time
from enum import Enum


# Use defaults if yaml is missing or arg not given
DEFAULT_PORT = "COM5"
DEFAULT_BAUD = 115200
args = {"port": DEFAULT_PORT, "baud": DEFAULT_BAUD}

# Yaml args
if os.getcwd().endswith("scripts"):
    config_path = "arduino_serial.yaml"
else:
    config_path = "scripts/arduino_serial.yaml"
try:
    with open(config_path, "r") as stream:
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
    serial_read_timeout = 0
    serial_write_timeout = 0
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
        msg = input("WRITE> ")
        msg = msg + "\n"
        ser.write(msg.encode("UTF-8"))
    
    @staticmethod
    def read():
        try:
            return ser.readline().decode("UTF-8")
        except Exception as e:
            print(e)

    def read_all():
        start_time = time.time()
        time_limit = 5
        end_flag = False
        timeout_flag = False
        while(not end_flag and not timeout_flag):
            msg = CommandHandler.read()
            if msg == "\n" or msg == '':
                end_flag = True
            else:
                start_time = time.time()
            if time.time() - start_time == time_limit:
                timeout_flag = True
            print(msg)

    @staticmethod
    def test():
        print("WRITE> test")
        ser.write("test\n".encode("UTF-8"))
        time.sleep(1)
        CommandHandler.read_all()

class ValidCommands(Enum):
    QUIT = 0
    WRITE = 1
    READ = 2
    TEST = 3

    @staticmethod
    def help():
        print("\n~~~ Help Menu ~~~")
        print("1. READ \t-->\tRead from serial")
        print("2. WRITE \t-->\tWrite to serial")
        print("3. QUIT \t-->\tQuit program")
    
CommandHandler.switch = {
    "WRITE": CommandHandler.write,
    "READ": CommandHandler.read_all,
    "TEST": CommandHandler.test,
    "QUIT" : safe_quit
}

def command_handler_script(usr_input):
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
        command_handler_script(usr_input)



if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("KeyboardInterrupt...")
        safe_quit()
