import argparse
import serial
from enum import Enum


# Argparse Setup
parser = argparse.ArgumentParser()
parser.add_argument('--port',
                    type=str,
                    default="COM5",
                    help='The connecting port')
parser.add_argument('--baud',
                    type=int,
                    default=115200,
                    help="The baudrate")

# Serial Functions
def ser_connect(port, baudrate):
    ser = serial.Serial(
            port=port,
            baudrate=baudrate
        )
    return ser


# Valid Commands
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


class CommandHandler():

    @classmethod
    def write(cls):
        msg = input("WRITE>")
        print(f"{msg}")
    
    @classmethod
    def read(cls):
        print("read test")
    
    options = {
        "WRITE": write,
        "READ" : read,
        "QUIT" : exit
    }

def command_handler_script(usr_input):
    usr_input = usr_input.upper()
    if not hasattr(ValidCommands, usr_input):
        print("\n[ERROR] Invalid input!")
        ValidCommands.help()
        return
    CommandHandler.options[usr_input]()



def main():
    print("### Arduino Serial Tool ###")
    usr_input = ""
    while(True):
        usr_input = input("> ")
        command_handler_script(usr_input)




if __name__ == "__main__":
    main()