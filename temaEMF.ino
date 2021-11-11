#define NUMREADINGS 15 // raise this number to increase data smoothing
#include <Servo.h>
Servo servo;
float startingPoint = 0; //used to track the time since we started the current stage
float beepFreq = 0; //used to track the time since we played the last beep
//stuff to make the button behave how it should:
bool buttonState = 0; 
bool reading = LOW;
bool lastReading = HIGH;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;
const int buttonPin = 2; //pin connected to our button

const int latchPin = 10;
// SH_CP pin 11
const int clockPin = 11;
// DS pin 14
const int dataPin = 12;
 
int senseLimit = 15; // raise this number to decrease sensitivity (up to 1023 max)
int probePin = A5; // analog 5
int buzzerPin = 3;

int val = 0; // reading from probePin
int datArray[11] = {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110,B11110111};

int readings[NUMREADINGS];                // the readings from the analog input
int index = 0;                            // the index of the current reading
int total = 0;                            // the running total
int average = 0;                          // final average of the probe reading
int state=0;

//CHANGE THIS TO affect the speed of the updates for numbers. Lower the number the faster it updates.
int updateTime = 40;

void setup() {
  startingPoint=millis();
  beepFreq = millis();
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  servo.attach(9);
  servo.write(0);
  //Serial.begin(9600);  // initiate serial connection for debugging/etc

  for (int i = 0; i < NUMREADINGS; i++)
    readings[i] = 0;                      // initialize all the readings to 0


}

void loop() {

reading = digitalRead(buttonPin);
if (reading != lastReading){
  lastDebounceTime = millis();
  }
if(millis() - lastDebounceTime > debounceDelay){
  if(reading != buttonState){
    buttonState = reading;
    if(buttonState == LOW){
    if(state==0){ //Check if the stage is 0 when pressed, otherwise the sequence shouldn't start
      state=1; //start stage 1
      startingPoint=millis();  //we use startingpoint to measure the time since the sequence started, so we know when to move on
      }
     else {
      state=0;
     }

      }
    }
  }
  if(state==1){
if((millis() - startingPoint)>=updateTime){
  
  val = analogRead(probePin);  // take a reading from the probe
  if(val >= 1){                // if the reading isn't zero, proceed
    val = constrain(val, 1, senseLimit);  // turn any reading higher than the senseLimit value into the senseLimit value
    val = map(val, 1, senseLimit, 1, 1023);  // remap the constrained value within a 1 to 1023 range

    total -= readings[index];               // subtract the last reading
    readings[index] = val; // read from the sensor
    total += readings[index];               // add the reading to the total
    index = (index + 1);                    // advance to the next index

    if (index >= NUMREADINGS)               // if we're at the end of the array...
      index = 0;                            // ...wrap around to the beginning

    average = total / NUMREADINGS;          // calculate the average

    
    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, datArray[average/100]);
    digitalWrite(latchPin, HIGH);

   if(average>100 && average<500){

        if((millis() - beepFreq) >= 1000){ //if the time we wanted passed since the last beep, we can play another
    tone(buzzerPin, 1000, 150); //play a sound
    beepFreq = millis();
  }

   }
    
   if(average>500 && average<800){

        if((millis() - beepFreq) >= 300){ //if the time we wanted passed since the last beep, we can play another
    tone(buzzerPin, 1500, 150); //play a sound
    beepFreq = millis();
  }

   }
   if(average>800){
        if((millis() - beepFreq) >= 200){ //if the time we wanted passed since the last beep, we can play another
    tone(buzzerPin, 2500, 150); //play a sound
    beepFreq = millis();
  }
   }
   
   digitalWrite(latchPin, HIGH);
   servo.write(180-(average/6));
   // Serial.println(average); // use output to aid in calibrating
   
   startingPoint=millis();
  }
}}
}
