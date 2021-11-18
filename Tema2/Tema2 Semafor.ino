const int buttonPin = 2; //pin connected to our button
const int ledRedC = 12; //pins for each leds ("C" for car leds, "P" for pedestrian leds)
const int ledGreenC = 11;
const int ledYellowC = 10;
const int ledRedP = 9;
const int ledGreenP = 8;
bool ledState = HIGH; //led state for the blinking light
//stuff to make the button behave how it should:
bool buttonState = 0; 
bool reading = LOW;
bool lastReading = HIGH;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;
const int buzzerPin = 13; //pin connected to the buzzer
int stage = 0; //used to track the stage we are in
float startingPoint = 0; //used to track the time since we started the current stage
float beepFreq = 0; //used to track the time since we played the last beep
int beepFreqA = 900; //first beeping frequency
int beepFreqB = 400; //second beeping frequency
int buzzerToneA = 1000; //first beep tone
int buzzerToneB = 1300; //second beep tone
int buzzerLenghtA = 400; //first beep lenght
int buzzerLenghtB = 200; //second beep lenght
//The lenght of each stage:
int stageTime1 = 4000;
int stageTime2 = 6000;
int stageTime3 = 4000;

void setup() {
  // put your setup code here, to run once:
pinMode(ledRedC,OUTPUT);
pinMode(ledGreenC, OUTPUT);
pinMode(ledYellowC, OUTPUT);
pinMode(ledRedP,OUTPUT);
pinMode(ledGreenP, OUTPUT);
pinMode(buttonPin,INPUT_PULLUP);
digitalWrite(ledGreenC, HIGH);
digitalWrite(ledRedP, HIGH);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
reading = digitalRead(buttonPin);
if (reading != lastReading){
  lastDebounceTime = millis();
  }
if(millis() - lastDebounceTime > debounceDelay){
  if(reading != buttonState){
    buttonState = reading;
    if(buttonState == LOW){
    if(stage==0){ //Check if the stage is 0 when pressed, otherwise the sequence shouldn't start
      stage=1; //start stage 1
      startingPoint=millis();  //we use startingpoint to measure the time since the sequence started, so we know when to move on
      digitalWrite(ledYellowC, HIGH); //switch led states to fit the first stage
      digitalWrite(ledGreenC, LOW);
      }

      }
    }
  }
  
if(stage==1){
  if((millis() - startingPoint)>=stageTime1){ //if the the time we wanted passed since we started the stage, we can proceed to the next stage
    stage = 2; //start stage 2
    startingPoint = millis(); //we use startingpoint to measure the time since the sequence started, so we know when to move on
    beepFreq = millis(); //we use beepFreq to space out beepings without delay
    digitalWrite(ledYellowC, LOW); //switch led states to fit the second stage
    digitalWrite(ledRedC, HIGH);
    digitalWrite(ledRedP, LOW);
    digitalWrite(ledGreenP, HIGH);
    }
}

if(stage==2){
    if((millis() - beepFreq) >= beepFreqA){ //if the time we wanted passed since the last beep, we can play another
    tone(buzzerPin, buzzerToneA, buzzerLenghtA); //play a sound
    beepFreq = millis();
  }
  
   if((millis() - startingPoint) >= stageTime2){ //if the the time we wanted passed since we started the stage, we can proceed to the next stage
      stage = 3; //start stage 3
      startingPoint = millis(); //we use startingpoint to measure the time since the sequence started, so we know when to move on
      beepFreq = millis(); //we use beepFreq to space out beepings without delay
    }
}

if(stage==3){
    if((millis() - beepFreq) >= beepFreqB){ //if the time we wanted passed since the last beep, we can play another
    tone(buzzerPin, buzzerToneB, buzzerLenghtB); //play a sound
    digitalWrite(ledGreenP, ledState); //switch the led on and off every time we play a beep:
    ledState=!ledState;
    beepFreq = millis();
  }
  
   if((millis() - startingPoint)>=stageTime3){ //if the the time we wanted passed since we started the stage, we can stop and reset to stage 0
      stage = 0; //start stage 0
      digitalWrite(ledRedC, LOW); //switch led states to fit the default stage
      digitalWrite(ledGreenC, HIGH);
      digitalWrite(ledGreenP, LOW);
      digitalWrite(ledRedP, HIGH);
      ledState=HIGH;
    }
}
  
lastReading = reading;
Serial.println(startingPoint);
Serial.println(stage);
}
