//I realise that i could have saved this number as a vector, each digit separated so i wouldn't have to break down the number every time i display it or go trough overcomplicated math to raise/lower
//the digits individually without raising the whole number when i pass 9 or go below 0. Also would have been an easier solution to the not being able to display a number that starts with 0 problem.
//Why haven't i done that? întreabă-mă ca să te-ntreb...i knew how to do it and i was aware it's easier, i just didn't bother i guess...too late to change it now, it works for this homework but i wouldn't
//code this the same way if i needed my display to do more things.

#include <EEPROM.h>
int VRx = A0; //joystick axis
int VRy = A2;
const byte interruptPin = 2; //button joystick
const int dataPin = 12; //shift register pins
const int latchPin = 11;
const int clockPin = 10;


const int seg01 = 7; //4 digit 7 segment display pins
const int seg02 = 6;
const int seg03 = 5;
const int seg04 = 4;
float timp; //keep track of time
int blinky = 0; //variable used to switch between the digit that is lit up (even if that digit doesn't exist) at any time to create the illusion of blinking
int chosen = 4; //used to keep track of where the "cursor" is
int xPosition = 500; //joystick x,y positions
int yPosition = 500;
int xylock = 0; //used to lock in place the x/y commands on the joystick so we don't spam up/down by holding the joystick down
int state = 5; //used to keep track of the state between blinking light where you choose your digit and the fixed light where you increase/decrease the digit chosen
int zeroProtection = 10000; //we just add this to the number so we can display numbers with '0' in front of them
int numar = 0; //the number we are displaying
float stateDelay; //keep track of when we changed the last state so we can't spam the joystick button by holding it down
const int segmentDigits[] = { //our digits
  seg01, seg02, seg03, seg04

};
const int segmentsCount = 4; //number of segments



int digitArray[20] = { //the array of digits and digits followed by numbers translated from bytes
  //A B C D E F G DP
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11111101, // 0.
  B01100001, // 1.
  B11011011, // 2.
  B11110011, // 3.
  B01100111, // 4.
  B10110111, // 5.
  B10111111, // 6.
  B11100001, // 7.
  B11111111, // 8.
  B11110111, // 9.
};




void setup() {
  for (int i = 0; i <= 3; i++) { //we pass trough the first 4 numbers in the eeprom, each one represents one of our digits.
    numar = numar + EEPROM.read(i) * pow(10, i); //we use those digits to form our number
  }
  numar = numar + zeroProtection;//we add the "1" in front of our number so we can display numbers that start with a 0
  chosen = EEPROM.read(chosen); //we get the number we had selected
  blinky = chosen; // blinky is linked to the chosen number, we only use the dot on the number selected
  state = EEPROM.read(state); //we get the state we were in

  //obvious:
  timp = millis();
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  for (int i = 0; i < segmentsCount; i++) {
    pinMode(segmentDigits[i], OUTPUT);
    digitalWrite(segmentDigits[i], LOW); //turn on the segments.
    attachInterrupt(digitalPinToInterrupt(interruptPin), stateChange, CHANGE); //button press with interupt
  }

  Serial.begin(9600);

}



void loop() {
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  if (state == 0) { //blinking and move digits

    if (millis() - timp >= 300) {//blinks every 0.3 seconds
      if (blinky >= 5) { //set back the dot on the chosen digit
        blinky = chosen;
      }
      else {
        blinky = 5; //take away the dot from the chosen digit and give it to nothing (we don't have a digit # 5)
      }
      timp = millis();
    }

    if (xPosition >= 750 && xylock == 0) { //if i pass the treshold
      xylock = 1; //make sure i'm not sending more imput by holding it down
      if (chosen < 3) chosen++; //if i didn't reach the last digit, go to the next 
    }
    if (xPosition <= 250 && xylock == 0) { //same as above, just go in reverse and check the first digit
      xylock = 1;
      if (chosen > 0) chosen--;
    }
    if (xPosition > 250 && xPosition < 750 && xylock == 1) { //if i bring back the joystick to the neutral unlock it again for another user input
      xylock = 0;
    }
  }


  else { //not blinking and add/substract
    if (blinky >= 5) {
      blinky = chosen; //set the dot on the chosen digit in case it's stuck on some other one
    }


    if (yPosition <= 250 && xylock == 0) {//treshold and lock, same principle as above but a different axis

      xylock = 1;
      int x = (numar + pow(10, chosen)) / pow(10, chosen + 1); //we check to see if adding one to the digit we are currently in changes the rest of the number trough some math
      int y = numar / pow(10, chosen + 1);
      if (x > y) {
        numar = numar - 9 * pow(10, chosen); //as i was writing this i just realised i could simply check if the digit we are in is 9 and change it to 0 by doing :
        /*x=numar%pow(10,chosen+1);
         *x=x/pow(10,chosen)
         *if(x==9)x=0;
         *else{ numar = numar + pow(10, chosen);}
         */
      }
      else {
        numar = numar + pow(10, chosen);
      }

    }

    if (yPosition >= 750 && xylock == 0) { //treshold and lock, same principle as above but a different axis
      xylock = 1;
      int x = (numar - pow(10, chosen)) / pow(10, chosen + 1); //same as above but we decrease the digit instead of increasing it
      int y = numar / pow(10, chosen + 1); //
      if (x < y) {
        numar = numar + 9 * pow(10, chosen);
      }
      else {
        numar = numar - pow(10, chosen);
      }

    }



    if (yPosition > 250 && yPosition < 750 && xylock == 1) { //unlock the joystick controls after we reach neutral again
      xylock = 0;
    }


  }




  writeNumber(numar);
}



void writeNumber(int number) {
  int currentNumber = number;
  int lastDigit;
  int displayDigit = 0;
  while (displayDigit <= 3) {//we take apart the number
    lastDigit = currentNumber % 10;
    showDigit(displayDigit);
    EEPROM.update(displayDigit, lastDigit);//save the number in eeprom
    if (displayDigit == blinky) {
      writeReg(digitArray[lastDigit + 10]);
    }
    else {
      writeReg(digitArray[lastDigit]);
    }
    displayDigit++;
    currentNumber /= 10;
    delay(5);
  }
  EEPROM.update(4, chosen); //save the chosen number in the eeprom
  EEPROM.update(5, state); //save the state in the eeprom
  
}



void writeReg(int digit) {//write in the shift register
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}



void showDigit(int digitNumber) {//display the digit
  for (int i = 0; i <= segmentsCount; i++) {
    digitalWrite(segmentDigits[i], HIGH);
  }
  digitalWrite(segmentDigits[digitNumber], LOW);

}

void stateChange() { //change the state when we press the button
  if (millis() - stateDelay >= 300) {
    if (state == 0) {
      state = 1;
    }
    else {
      state = 0;
    }
    xylock = 0;
    stateDelay = millis();
  }
}
