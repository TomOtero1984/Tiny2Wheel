import argparse
import serial
from enum import Enum


# Argparse Setup
parser = argparse.ArgumentParser()
parser.add_argument('--port',
                    type=str,
                    default="COM13",
                    help='The connecting port')
parser.add_argument('--baud',
                    type=int,
                    default=115200,
                    help="The baudrate")
args = parser.parse_args()

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
    @staticmethod
    def write():
        msg = input("WRITE>")
        ser.write(msg.encode())
    
    @staticmethod
    def read():
        msg = []
        msg.append(ser.readline().decode("UTF-8"))
        print(msg)
    
CommandHandler.switch = {
    "WRITE": CommandHandler.write,
    "READ": CommandHandler.read,
    "QUIT" : exit
}

def command_handler_script(ser, usr_input):
    usr_input = usr_input.upper()
    if not hasattr(ValidCommands, usr_input):
        print("\n[ERROR] Invalid input!")
        ValidCommands.help()
        return
    CommandHandler.switch[usr_input]()
    


def main():
    print("### Arduino Serial Tool ###")
    print(f"Starting serial connection on port: {args.port}"
          f" with baudrate: {args.baud}")
    global ser
    ser = ser_connect(port=args.port, baudrate=args.baud)
    usr_input = ""
    while(True):
        usr_input = input("> ")
        command_handler_script(ser, usr_input)




if __name__ == "__main__":
    main()
