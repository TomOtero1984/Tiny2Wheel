#include "Arduino.h"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    // Blink, because why not...
    blink();
    // Serial Read
    int ser_in = 0;
    ser_in = Serial.read();
    if (ser_in >= 0){
        Serial.println(ser_in);
        ser_in = 0;
    }
}

// User Functions

void blink() {
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);                     // wait for a second
}

void serial_clear_incoming_buffer(){
    while(Serial.read() >= 0);
}
