#include "pitches.h"
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };
// Pin buzzer
const int BZZ = 4;
//Los pines 0 y 1 Digital se conectan directos al bluetooth
//const int BPW = 3; //PWM reinicio de bluetooth
//Motor A - Avanzar y backward
const int ENA = 11; //PWM
const int IN1 = 12;
const int IN2 = 13;
//Motor B - Derecha e Izquierda 
const int ENB = 6; //PWM
const int IN3 = 7;
const int IN4 = 8;
//Luces
const int LTR = 9; //Luz trasera PWM
const int LDR = 10; //Luz delantera PWM Der
const int LDL = 5; //Luz delantera PWM Izq

//movimientos
const char FORWARD = 'F';
const char BACK = 'B';
const char LEFT = 'L';
const char RIGHT = 'R';
const char FORWARDLEFT = 'G';
const char FORWARDRIGHT = 'I';
const char BACKLEFT = 'H';
const char BACKRIGHT = 'J';
const char STOP = 'S';
const char FRONTLIGHTSON = 'W';
const char FRONTLIGHTSOFF = 'w';
const char BACKLIGHTSON = 'U';
const char BACKLIGHTSOFF = 'u';
const char HORNON = 'V';
const char HORNOFF = 'v';
const char EXTRAON = 'X';
const char EXTRAOFF = 'x';
const char STOPAll = 'D';


int PWA = 0; //Velocidad Motor A
int PWB = 255; //Velocidad Motor A
int minLight = 0;
boolean light = false;
boolean higth = false;
boolean extra = false;
boolean lined = true;
boolean rig = false;
boolean lef = false;
boolean horn = false;
boolean forward = true;
char command = 'z';

void setup()
{
  Serial.begin(9600);
  //Motor A
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  //Motar B
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);
  //Luces
  pinMode(LDR, OUTPUT); //Luz delantera
  pinMode(LDL, OUTPUT); //Luz delantera
  pinMode(LTR, OUTPUT); //Luz trasera
  
  //Inicializando pines
  //Motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  //Motar B
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  //Luces
  digitalWrite (LDR, LOW); //Luz delantera
  digitalWrite (LDL, LOW); //Luz delantera
  digitalWrite(LTR, LOW); //Luz trasera
  //anunciar inicio
  toneMelody();
}

void loop()
{
      serialRead(); //Recibir dato bluetooth    
      if (command == FORWARD)
      {
        center();
        avanzar();
      }
      else if (command == BACK)
      {
        center();
        backward();
      }
      else if (command == RIGHT)
      {
        right();
      }
      else if (command == LEFT)
      {
        left();
      }
      else if (command == FORWARDLEFT)
      {
        avanzar();
        left();
      }
      else if (command == FORWARDRIGHT)
      {
        avanzar();
        right();
      }
      else if (command == BACKLEFT)
      {
        backward();
        left();
      }
      else if (command == BACKRIGHT)
      {
        backward();
        right();
      }
      else if (command == STOPAll || command == STOP)
      {
        stop();
      }else if (command == '0') {
        PWA = 0;
      }
      else if (command == '1') {
        PWA = 60;
      }
      else if (command == '2') {
        PWA = 80;
      }
      else if (command == '3') {
        PWA = 100;
      }
      else if (command == '4') {
        PWA = 120;
      }
      else if (command == '5') {
        PWA = 130;
      }
      else if (command == '6') {
        PWA = 155;
      }
      else if (command == '7') {
        PWA = 175;
      }
      else if (command == '8') {
        PWA = 200;
      }
      else if (command == '9') {
        PWA = 225;
      }
      else if (command == 'q') {
        PWA = 255;
      }
      else if (command == FRONTLIGHTSON) {
        higth = true;
        highLights();
      }
      else if (command == FRONTLIGHTSOFF) {
        higth = false;
        highLights();
      }      
      else if (command == BACKLIGHTSON) {
        light = true;
        statusLights();
      }
      else if (command == BACKLIGHTSOFF) {
        light = false;
        statusLights();
      }
      else if (command == HORNON) {
         toneMelody();
         horn = true;
      }
      else if (command == HORNOFF) {
         horn = false;
      }
      else if (command == EXTRAON) {
         extra = true;
      }
      else if (command == EXTRAOFF) {
         extra = false;
      }
      luzEstrobo();

}

void serialRead()
{
  command = 'z';
  while(Serial.available())
  {//while there is data available on the serial monitor
    command= char(Serial.read());//store string from serial command
  }
  
}


void stop() // Detiene y centra la direccion
{
  //Detener Motor A
  traction();
  analogWrite(ENA, 0);
  //Detener Motor b
  center();
  analogWrite(LTR, 255);
}

void center() //Centra direccion
{
  if (!lined) {
    //Detener motor B
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW);
    analogWrite(ENB, 0);
    delay(100);
  }
  lined = true;
  rig = false;
  lef = false;

}

void right() // Giro a la derecha
{
  if (!lined && !rig)
  {
    center();
  }
  if (!rig)
  {
    //Activar Motor B
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    analogWrite(ENB, PWB);
    rig = true;
    lined = false;
  }
}

void left() // Giro a la left
{
  if (!lined && !lef)
  {
    center();
  }
  if (!lef) {
    //Activar Motor B
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    analogWrite(ENB, PWB);
    lef = true;
    lined = false;
  }

}

void traction() //Centra detiene Motor A
{
  //Detener Motor A
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, LOW);
  analogWrite (ENA, 0);
  delay(100);
  if (horn)
  {
    toneMelody();
  }
}


void avanzar()
{
    if (!forward) {
       traction(); //Detener Motor A    
    }
    statusLights();
    //Activar motor A
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    analogWrite (ENA, PWA);
    forward = true;

}

void backward(){
    if (forward) {
       traction(); //Detener Motor A    
    }
    statusLights();
    //Activar Motor A
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    analogWrite (ENA, 70);
    forward = false;
    
}


void statusLights()
{
  if (light)
  {
    minLight = 100;
  }
  else
  {
    minLight = 0;
  }
  analogWrite(LTR, minLight);
  highLights();
  
}

void highLights()
{
  if (higth)
  {
    analogWrite(LDR, 255);
    analogWrite(LDL, 255);
  }
  else
  {
    analogWrite(LDR, minLight);
    analogWrite(LDL, minLight);
  }
}

void luzEstrobo(){
   int tiempo = 30;
  if (extra){
    digitalWrite(LDR, HIGH);
    delay(tiempo);
    digitalWrite(LDR, LOW);
    delay(tiempo);
    digitalWrite(LDR, HIGH);
    delay(tiempo);
    digitalWrite(LDR, LOW);
    delay(80);

    digitalWrite(LDL, HIGH);
    delay(tiempo);
    digitalWrite(LDL, LOW);
    delay(tiempo);
    digitalWrite(LDL, HIGH);
    delay(tiempo);
    digitalWrite(LDL, LOW);
    delay(80);

    highLights();
  }
  else
  {
    delay(100);
  }
}


void toneMelody()
{
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(BZZ, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // STOP the tone playing:
    noTone(BZZ);
  }
}
