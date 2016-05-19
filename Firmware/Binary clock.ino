/*
An open-source binary clock for Arduino. 
Adapted from the work of cello62 http://www.instructables.com/id/My-Arduino-Binary-Clock/step11/Upload-the-sketch/
by POTI-POTI.org (www.poti-poti.org)

This is based on a minimalistic design where only minutes and hours are represented in a 6 bit byte.
*/
  /*////////////////////////////////////////////////////////////////////////
  /////////            DEFINE GLOBAL VARIABLES                    ////////
  ////////////////////////////////////////////////////////////////////////*/
  
int Sec=0, Min=0, Hou=0; //start the time on 00:00:00

//Set the pins with PWM in the Arduino UNO so that we can use
//the analogWrite function
int pin1 = 3;   
int pin2 = 5;
int pin4 = 6;
int pin8 = 9;
int pin16 = 10;
int pin32 = 11;

long FirstTick=0;       //Changes every sixty seconds

int brightness=0;       //defines how much the LEDs dim

  /*////////////////////////////////////////////////////////////////////////
  /////////           INITIALIZE THE MICROCONTROLLER               ////////
  ////////////////////////////////////////////////////////////////////////*/
  
void setup() { //set outputs and inputs
//Set LEDs Grounds as outputs
pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);
pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);

//Set the pins to LOW so they have zero voltage
digitalWrite(A0, LOW);
digitalWrite(A1, LOW);
digitalWrite(A2, LOW);
digitalWrite(A3, LOW);
digitalWrite(A4, LOW);
digitalWrite(A5, LOW);

//Set the LEDs positive pins as output
pinMode(pin1, OUTPUT);
pinMode(pin2, OUTPUT);
pinMode(pin4, OUTPUT);
pinMode(pin8, OUTPUT);
pinMode(pin16, OUTPUT);
pinMode(pin32, OUTPUT);

//Set the button as input
pinMode(2, INPUT);

//Set the pin high so we can compare later on
digitalWrite(2, HIGH);

//Prepare the communication with the computer via serial
Serial.begin(9600);

//Set the first tick of the clock
FirstTick=millis();
}

  /*////////////////////////////////////////////////////////////////////////
  /////////                ITERATE FOREVER                          ////////
  ////////////////////////////////////////////////////////////////////////*/
  
void loop() {
 
  //Set the seconds
  Sec=(millis()-FirstTick)/1000;
  
  // move forward one minute every 60 seconds
  if (Sec >= 60) 
  {
    Min++;                // Increase the minutes one unit
    Sec = 0;              // Reset seconds to zero
    FirstTick=millis();   // Reset the first tick to become the current time
  }
  
  // move forward one hour every 60 minutes
  if (Min >=60) 
  {
    Hou++;                // Increase the hour one unit
    Min = 0;              // Reset minutes to zero
  }
  
  if (Hou >=24) 
  {
    Hou=0;                // Reset the hours to zero
    Min = 0;              // Reset minutes to zero
  }

  //Get some feed back in the serial terminal
  Serial.print(Hou);
  Serial.print(":");
  Serial.print(Min);
  Serial.print(":");
  Serial.println(Sec);

    
  /*////////////////////////////////////////////////////////////////////////
  /////////              SETUP HOURS AND MINUTES                    ////////
  ////////////////////////////////////////////////////////////////////////*/
  
  if( digitalRead(2)== LOW) //Add one minute when the button is pressed
  {
     Min++;              // Increase the minutes one unit
     Sec=0;              // Reset seconds to zero
     Serial.println("Setting minutes");    
     delay(250);         //Wait a bit to give time to react to the user 
  }
  else{
      //Dim on the LEDs for the hours
    for (brightness=0; brightness<255;brightness=brightness + 5){
      LightLEDs(Hou, brightness);
      delay(10);
    }
    //Dim off the LEDs for the hours
    for (brightness=255; brightness>0;brightness=brightness - 5){
      LightLEDs(Hou, brightness);
      delay(10);
    } 
    //Dim on the LEDs for the minutes
    for (brightness=0; brightness<255;brightness=brightness + 5){
      LightLEDs(Min, brightness);
      delay(10);
    }
    //Dim on the LEDs for the minutes
    for (brightness=255; brightness>0;brightness=brightness - 5){
      LightLEDs(Min, brightness);
      delay(10);
    } 
  }
  //Switch off all the LEDs 
   LightLEDs(60,0);
}
 
  /*////////////////////////////////////////////////////////////////////////
  /////////            CONVERT DECIMAL VALUES TO BINARY             ////////
  ////////////////////////////////////////////////////////////////////////*/
  
void LightLEDs(int value, int PWM){
  int BitPin=0;                   //Iterates bit by bit 
  int Pin=0;                      //Converts digital bits into digital pins
  int remainder=0;                //Stores the remainder of the division by two
  int Quotient;                   //Stores the quotient
  
   for (int i=6; i>-1; i--)       //Iterate through the six bit "byte"
    {
      remainder=Quotient-(1<<i);  //Shift one bit
      
      if (remainder>=0)           //If the remainder is greater than zero
      {                           //the number is not a multiple of two  
        
        
        Quotient=remainder;       //In the next step we will divide the remainder by two
        Serial.print("/");        //in order to get the value of the next bit
        Serial.print("1");
        
        //Light up the LED corresponding to the bit
        if (BitPin == 1) Pin=11;
        if (BitPin == 2) Pin=10;
        if (BitPin == 3) Pin=9;
        if (BitPin == 4) Pin=6;
        if (BitPin == 5) Pin=5;
        if (BitPin == 6) Pin=3;
        analogWrite(Pin,PWM);     
      }
      else
      {
        Serial.print("/");        //If the remainder is zero then the
        Serial.print("0");        //bit stays zero
      }
      BitPin++;                   //Step one bit in the "byte"
    }
    Serial.println();
}
