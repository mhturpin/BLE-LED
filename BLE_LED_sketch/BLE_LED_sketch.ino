#include <SoftwareSerial.h>
#include <IRremote.h>

#define LED_PIN 2
#define IR_RECV_PIN 11

#define NBITS 32
#define PRESS_DELAY 800
#define BAUD_RATE 9600
#define COMMAND_COUNT 7


#define INFO                0xE0E0F807
#define POWER               0xE0E040BF
#define MACRO_CHANGETOPC    0x00000001
#define SOURCE              0xE0E0807F
#define TV                  0xE0E0D827
#define UP                  0xE0E006F9
#define DOWN                0xE0E08679
#define LEFT                0xE0E0A659
#define RIGHT               0xE0E046B9

#define CHANNELUP           0xE0E048B7
#define CHANNELDOWN         0xE0E008F7
#define VOLUMEUP            0xE0E0E01F
#define VOLUMEDOWN          0xE0E0D02F


#define REW                 0xE0E0A25D
#define PAUSE               0xE0E052AD
#define FF                  0xE0E012ED
#define REC                 0xE0E0926D
#define PLAY                0xE0E0E21D
#define STOP                0xE0E0629D

#define A                   0xE0E036C9
#define B                   0xE0E028D7
#define C                   0xE0E0A857
#define D                   0xE0E06897

#define ENTER               0xE0E016E9

#define RS     4
#define RW     5
#define ENABLE 6
#define D4     7
#define D5     8
#define D6     9
#define D7     10


IRsend irsend;
IRrecv irrecv(IR_RECV_PIN);

decode_results results;

SoftwareSerial mySerial(7, 8); // RX, TX  
// Connect HM10      Arduino Uno
//     Pin 1/TXD          Pin 7
//     Pin 2/RXD          Pin 8

void setup() {  
    Serial.begin(BAUD_RATE);
    // If the baudrate of the HM-10 module has been updated,
    // you may need to change 9600 by another value
    // Once you have found the correct baudrate,
    // you can update it using AT+BAUDx command 
    // e.g. AT+BAUD0 for 9600 bauds
    mySerial.begin(BAUD_RATE);
    irrecv.enableIRIn(); // Start the receiver
}

void loop() {
    if (irrecv.decode(&results)) {
        long int val = results.value;
        byte b[4];
        intToBytes(val, b);
        Serial.println(val, HEX);
        mySerial.write(b, 4);
        irrecv.resume(); // Receive the next value
    }
    
    int c;
    
    if (mySerial.available()) {
        c = mySerial.read();  
        Serial.println("Got input:");
        if (c == 1) {
            Serial.println("  power");
            irsend.sendSAMSUNG(POWER, NBITS);
        }
        else if (c == 2) {
            Serial.println("  channel up");
            irsend.sendSAMSUNG(CHANNELUP, NBITS);
        }
        else if (c == 3) {
            Serial.println("  channel down");
            irsend.sendSAMSUNG(CHANNELDOWN, NBITS);
        } 
        else {
            Serial.println("  unknown input");
        } 
        irrecv.enableIRIn(); // Start the receiver
    }
}



void intToBytes(long int val, byte b[4]) {
    b[0] = (byte) ((val >> 24) & 0xff);
    b[1] = (byte) ((val >> 16) & 0xff);
    b[2] = (byte) ((val >> 8) & 0xff);
    b[3] = (byte) (val & 0xff);
}
