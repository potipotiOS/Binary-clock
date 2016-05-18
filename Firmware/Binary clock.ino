/*
An open-source binary clock for Arduino. 
*/

int Sec=0, Min=1, Hou=2; //start the time on 00:00:00
int munit = 0;
int hunit = 0;
int valm=0;
int ledstats = 0;
int i= 0;
boolean light = 1;
int remainder=0;
int Quotient;
int pin1 = 3;
int pin2 = 5;
int pin4 = 6;
int pin8 = 9;
int pin16 = 10;
int pin32 = 11;

int BitPin=1;
int Pin=0;

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

void setup() { //set outputs and inputs
//LEDs
pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);
pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);
pinMode(pin1, OUTPUT);
pinMode(pin2, OUTPUT);
pinMode(pin4, OUTPUT);
pinMode(pin8, OUTPUT);
pinMode(pin16, OUTPUT);
pinMode(pin32, OUTPUT);
//Buttons
pinMode(2, INPUT);

digitalWrite(A0, LOW);
digitalWrite(A1, LOW);
digitalWrite(A2, LOW);
digitalWrite(A3, LOW);
digitalWrite(A4, LOW);
digitalWrite(A5, LOW);

Serial.begin(9600);
}


void loop() {
  static unsigned long lastTick = 0; // set up a local variable to hold the last time we moved forward one second
  // (static variables are initialized once and keep their values between function calls)
  // move forward one second every 1000 milliseconds
  
  if (millis() - lastTick >= 1000) 
  {
    lastTick = millis();
    Sec++;
  }
  
  // move forward one minute every 60 seconds
  if (Sec >= 60) 
  {
    Min++;
    Sec = 0; // reset seconds to zero
  }
  
  // move forward one hour every 60 minutes
  if (Min >=60) 
  {
    Hou++;
    Min = 0; // reset minutes to zero
  }
  
  if (Hou >=24) 
  {
    Hou=0;
    Min = 0; // reset minutes to zero
  }

  Serial.print(Hou);
  Serial.print(":");
  Serial.print(Min);
  Serial.print(":");
  Serial.println(Sec);
  
  LightLEDs(Min);
 
}

void LightLEDs(int value){
  
 BitPin=0;
  Quotient=value;
  // set the brightness of the pins:
   for (int i=6; i>-1; i--)
    {
      remainder=Quotient-(1<<i);
      
      if (remainder>=0)
      {
        if (BitPin == 1) Pin=11;
        if (BitPin == 2) Pin=10;
        if (BitPin == 3) Pin=9;
        if (BitPin == 4) Pin=6;
        if (BitPin == 5) Pin=5;
        if (BitPin == 6) Pin=3;
        
        analogWrite(Pin,brightness);
        Quotient=remainder;
        Serial.print("1");
        Serial.print("/");
      }
      else
      {
        Serial.print("0");
        Serial.print("/");
      }  
      BitPin++;
    }
  

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(10);
  Serial.println();
}


