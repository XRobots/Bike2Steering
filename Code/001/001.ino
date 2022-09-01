#include <Servo.h>
Servo ESC1;   //   throttle ESC

#include <PID_v1.h>

double Pk1 = 1.5; 
double Ik1 = 0.5;
double Dk1 = 0.05;

double Setpoint1, Input1, Output1, Output1a;    // PID variables
PID PID1(&Input1, &Output1, &Setpoint1, Pk1, Ik1 , Dk1, DIRECT);    // PID Setup

int pot1;   // throttle
int pot2;
int pot3;

int sw1;    //left switch
int sw2;    //right switch
int sw3;    //reverse lever

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long interval = 5;        // time constant for timer

void setup() {

    pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    pinMode(4,INPUT_PULLUP);

    Serial.begin(115200);

    pinMode(5,OUTPUT);      // wiper PWMs
    pinMode(6,OUTPUT);

    PID1.SetMode(AUTOMATIC);              
    PID1.SetOutputLimits(-127, 127);
    PID1.SetSampleTime(10);

    ESC1.attach(7);
    ESC1.writeMicroseconds(1300);

}

void loop() {


        currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {  // start timed event
          
            previousMillis = currentMillis;

            sw1 = digitalRead(2);
            sw2 = digitalRead(3);
            sw3 = digitalRead(4);   

            pot1 = analogRead(A0);      // throttle
            pot2 = analogRead(A2)-570;      // front pot (-trim value)
            pot3 = analogRead(A1)-550;      // rear pot (-trim value)

            Serial.print(pot2);
            Serial.print(" , ");
            Serial.println(pot3);
            
            pot1 = (pot1-176)*1.5;
            pot1 = constrain(pot1,0,1000);

            if (sw3 == 1) {
                ESC1.writeMicroseconds(1300+pot1);    // forward
            }
            else if (sw3 == 0) {
                ESC1.writeMicroseconds(1300-pot1);    // reverse
            }

            if (sw1 == 1 & sw2 == 1) {
                Setpoint1 = 40;       //trim
            }
            else if (sw1 == 0 && sw2 == 1) {
                Setpoint1 = pot2 + 35; 
            }
            else if (sw2 == 0 && sw1 == 1) {
                Setpoint1 = (pot2 + -65) *-1; 
            }  

            else if (sw2 == 0 && sw1 == 0) {
                Setpoint1 = 130; 
            } 
                        
            Input1 = pot3;
            
            PID1.Compute();

            if (Output1 > 0) {
              analogWrite(5, Output1);
              analogWrite(6, 0);
            }
            else if (Output1 < 0) {
              Output1a = abs(Output1);
              analogWrite(6, Output1a);
              analogWrite(5, 0);
            }
            else {
              analogWrite(5, 0);
              analogWrite(6, 0);
            }

          


        } // end of timed loop





}
