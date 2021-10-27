/*
Tema 2

RGB led controlat cu trei potentiometre.

Acest program preia valoarea senzorilor a 3 potentiometre, le prelucreaza si le trimite inapoi placii arduino prin 3 outputuri diferite pentru a putea
schimba culorile unui led RGB. 

- Buzamat Ionut Alexandru, grupa 333 FMI - Informatica anul 3

*/

int sensorPinR = A0;    // Input pinuri pentru potentiometre
int sensorPinG = A1;
int sensorPinB = A2;
int ledR = 9;           // Pinurile fiecarei culori la care este atasat ledul
int ledG = 10;
int ledB = 11;
float sensorValueR = 0;  // variabile sa salvam valoarea senzorilor pentru fiecare culoare
float sensorValueG = 0;
float sensorValueB = 0;
float multiplu = 4.0117; // 1023/255

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9,10,11 to be an output:
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // Citire potentiometru pin A0 (Rosu):
  sensorValueR = analogRead(sensorPinR);
  // Setare intensitate pin 9(Rosu):
  analogWrite(ledR, sensorValueR / multiplu);
  
  // Citire potentiometru pin A1 (Verde):
    sensorValueG = analogRead(sensorPinG);
  // Setare intensitate pin 10(Verde):
  analogWrite(ledG, sensorValueG / multiplu);
  
  // Citire potentiometru pin A2 (Albastru):
    sensorValueB = analogRead(sensorPinB);
  // Setare intensitate pin 11(Albastru):
  analogWrite(ledB, sensorValueB / multiplu);
  
  delay(30);
}
