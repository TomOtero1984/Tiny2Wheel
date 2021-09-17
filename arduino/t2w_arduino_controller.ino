#include "Arduino.h"

// Declarations
void setup();
void loop();
void blink();
void serial_delay();
void serial_read();
void serial_write();
void check_message();


// Globals 
bool WRITE_FLAG = false;
const int MAX_SERIAL_READ = 64;
char message[MAX_SERIAL_READ] = {0};

// Definitions
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}


void loop() {
    serial_read();
    if(WRITE_FLAG) {
        serial_write();
        // check_message();
    }
}

// User Functions
void blink() {
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);                     // wait for a second
}


void serial_delay() {
    while(!Serial.available());
}


void serial_read() {
    char ser_in;
    if (Serial.available() > 0){
        int i = 0;
        for (i; i < MAX_SERIAL_READ; i++)
        {
            ser_in = Serial.read();
            message[i] = ser_in;
            if (ser_in == '\n' || '\0') break;
            serial_delay();
        }
        message[i+1] = '\0';
        WRITE_FLAG = true;
    }
}


void serial_write() {
    char ser_out;
    for (int i=0; i < MAX_SERIAL_READ; i++){
        ser_out = message[i];
        if (ser_out == '\n' || ser_out == '\0') break;
        Serial.print(ser_out);
    }
    WRITE_FLAG = false;
}


void check_message(){
    if (strcmp(message, "test")==0){
        Serial.println("Check Message: test");
    }
    else {
        Serial.println("Check Message: else...");
    }
}

