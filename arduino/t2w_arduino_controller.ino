#include "Arduino.h"

#define MOTOR_PIN 5
#define LEFT 2000
#define MIDDLE 1500
#define RIGHT 1000


void setup();
void loop();
void serial_delay();
void serial_read();
void serial_write();
void check_message();
void clear_message();
void motor_position(char pos, int drive_time);
void motor_drive(int drive_time, int time_high, int time_low);
void motor_test();
void clean_message();

bool WRITE_FLAG = false;
const int MAX_SERIAL_READ = 64;
char message[MAX_SERIAL_READ] = {'\0'};
int message_index = 0;

// Arduino 
void setup() {
    pinMode(MOTOR_PIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    if(!Serial){
        while(!Serial);
        Serial.println("Arduino Connected");
        delay(2000);
    }
    serial_read();
    if(WRITE_FLAG) {
        serial_write();
        check_message();
        clear_message();
    }
}

// Serial functions
void serial_delay() {
    while(!Serial.available());
}

void serial_read() {
    char ser_in;
    if(Serial.available()){
        while(true){
            ser_in = Serial.read();
            if (ser_in == '\n' || ser_in == '\0' || ser_in == '\r') break;
            message[message_index] = ser_in;
            message_index++;
            serial_delay();
        }
        WRITE_FLAG = true;
    }
}

void serial_write() {
    char ser_out;
    for (int i=0; i < message_index; i++){
        ser_out = message[i];
        Serial.print(ser_out);
        if (ser_out == '\n' || ser_out == '\0' || ser_out == '\r') break;
    }
    Serial.print('\n');
    WRITE_FLAG = false;
}

// Message Functions
void clean_message() {
    char c;
    for (int i=0; i < message_index; i++) {
        c = message[i];
        if (c == '\n' || c == '\0' || c == '\r') {
            message[i] = '\0';
        }
    }
}

void check_message(){
    int drive_time = 100;
    clean_message();
    Serial.println(message);
    if (strcmp(message, "stop") == 0) {
        // TODO: add stop function
    }
    else if (strcmp(message, "start") == 0) {
        // TODO: add stop function
    }
    else if (strcmp(message, "right") == 0) {
        Serial.print("Moving Right");
        motor_position('r', drive_time);
    }
    else if (strcmp(message, "middle") == 0) {
        Serial.print("Moving middle");
        motor_position('m', drive_time);
    }
    else if (strcmp(message, "left") == 0) {
        Serial.print("Moving Left");
        motor_position('l', drive_time);
    }
    else if (strcmp(message, "test") == 0) {
        Serial.print("Motor Test Script\n");
        motor_test();
    }
    else {
        char msg[64];
        sprintf(msg, "[ERROR] Unknown command: %s", message);
        Serial.println(msg);
    }
}

void clear_message() {
    for (int i = 0; i < message_index; i++)
    {
        message[i] = '\0';
    }
    message_index = 0;
}


// Motor Functions
void motor_position(char pos, int drive_time) {
    int time_total = 20000;
    int time_high = 0;
    if (pos == 'r') {
        time_high = RIGHT;
    }
    else if (pos == 'm') {
        time_high = MIDDLE;
    }
    else if (pos == 'l') {
        time_high = LEFT;
    }
    else {
        char msg[64];
        sprintf(msg, "[ERROR] Unknown command: %s", message);
        Serial.println(msg);
    }
    int time_low = time_total - time_high;
    motor_drive(drive_time, time_high, time_low);
}

void motor_drive(int drive_time, int time_high, int time_low) {
    int time_start = millis();
    while (millis() - time_start < drive_time) {
        digitalWrite(MOTOR_PIN, HIGH);
        delayMicroseconds(time_high);
        digitalWrite(MOTOR_PIN, LOW);
        delayMicroseconds(time_low);
    }
}

void motor_test(){
    motor_position('l', 1000);
    motor_position('r', 500);
    motor_position('m', 500);
}