#include <SoftwareSerial.h>

#define enableA 9
#define in1 6
#define in2 7
#define buttonDown 4
#define buttonUp 5

# define BAUD

# define USE_WIFI true


const byte pinRx = 2;
const byte pinTx = 3;

// set up a new serial object
SoftwareSerial serial1 (rxPin, txPin);


int rotDirection = 0;
int pressed = false;

union Message {
    char buffer[4];
    struct {
        float f;
        int direction;
    } data;
} message;

char buffer[128];



void wifi_read_message() {
    
    wifi_read();
    
    // +IPD,n:xxxxxxxxxx
    
    if(strcmp(buffer, "+IPD,") != 0) {
        Serial.write("ERROR");
        return;
    }
    
    char * p = strstr(buffer, ":");
    
    if(p == null) {
        Serial.write("ERROR");
        return;
    }
    
    *p = '\0';
    
    int n = atoi(buffer + 5);
    
    sprintf(serial0_buffer, "wifi received %i bytes\n", n);
    Serial.write(serial0_buffer);
    
    if(n != (int)sizeof(Message)) {
        Serial.write("ERROR");
        return;
    }
    
    memcpy(message.buffer, p, n);
}

void wifi_read() {
    
    int i = 0;
    
    while(true) {
        char c = serial1.read();
        if(c == -1) return;
        buffer[i] = c;
        Serial.write(c);
    }
    
}

void wifi_setup() {
    
    serial1.write("AT+CWMODE=3");
    
    wifi_read();
    
    serial1.write("AT+CWJAP=\"CHARJESS\",\"espresso\"");
    
    wifi_read();
    
    serial1.write("AT+CIPSTART=\"TCP\",\"192.168.1.101\",30000");
    
    wifi_read();
}

void setup() {
    Serial.begin(BAUD);
    
    if(USE_WIFI) {
        wifi_setup();
    }
    
    pinMode(enableA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(buttonDown, INPUT);
    pinMode(buttonUp, INPUT);
    // Set initial rotation direction
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
}

void doMotor(float pwmOutput, int direction) {
    
    // Send PWM signal to L298N Enable pin
    analogWrite(enableA, pwmOutput);     
    
    if(direction == 0) {
        // off
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    } else if(direction < 0) {
        // down
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    } else if(direction > 0) {
        // up
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
}

int directionFromButtons() {
    
    // Read buttons
    bool bUp = digitalRead(buttonUp);
    bool bDown = digitalRead(buttonDown);
    
    if(bUp && bDown) {
        // off
        return 0;
    } else if(bUp) {
        // up
        return 1;
    } else if(bDown) {
        // down
        return -1;
    } else {
        // off
        return 0;
    }
}

void loop() {
    

    int direction = 0;
    
    if(USE_WIFI) {
        wifi_read_message();
        
        doMotor(message.data.f, message.data.direction);
    } else {
        int potValue = analogRead(A0); // Read potentiometer value
        int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
        
        doMotor(pwmOutput, directionFromButtons());
    }
    
    
}
