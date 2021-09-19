# Arduino Design Documentation

### Table of contents
1. [Overview](#overview)
2. [Serial](#serial)

### Overview <a name="overview"></a>
The Arduino used in this project is running a motor control script with serial communication enabled to recieve commands from an external computer. 

### Serial <a name="serial"></a>
Serial communication uses baudrate of 115200.

Messages recieved by the Arduino must terminate with a newline character.

Outbound messages are currently unstructured, but will be JSON.

