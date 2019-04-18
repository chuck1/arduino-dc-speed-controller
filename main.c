
#define enableA 9
#define in1 6
#define in2 7
#define buttonDown 5
#define buttonUp 4


int rotDirection = 0;
int pressed = false;

void setup() {
    pinMode(enableA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(buttonDown, INPUT);
    pinMode(buttonUp, INPUT);
    // Set initial rotation direction
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
}

void loop() {
    int potValue = analogRead(A0); // Read potentiometer value
    int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
    analogWrite(enableA, pwmOutput); // Send PWM signal to L298N Enable pin

    // Read buttons
    bool bUp = digitalRead(buttonUp);
    bool bDown = digitalRead(buttonDown);
    
    if(bUp && bDown) {
        // off
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    } else if(bUp) {
        // up
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    } else if(bDown) {
        // down
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    } else {
        // off
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    }
}
