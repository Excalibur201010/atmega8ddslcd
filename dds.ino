#include "AD9850.h"
#include "ClickEncoder.h"
// include the library code:
#include <LiquidCrystal.h>



#include "TimerOne.h"


const int W_CLK_PIN = 8;
const int FQ_UD_PIN = 7;
const int DATA_PIN = 6;
const int RESET_PIN = 5;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);


double freq = 101;
double trimFreq = 124999500;

int phase = 0;
ClickEncoder *encoder;
long last, value;
long diff=1;
int tmp;

unsigned long now=0;

void timerIsr() {
  now++;
  encoder->service(now);
}

void setup(){
  //Serial.begin(9600);
  //Serial.println("Start");
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(1);
  digitalWrite(13,LOW);

  encoder = new ClickEncoder(11,10,9,4);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 

  last = 1;
  
  DDS.begin(W_CLK_PIN, FQ_UD_PIN, DATA_PIN, RESET_PIN);
  DDS.calibrate(trimFreq);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  debug();
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  DDS.setfreq(freq, 0);
}
void debug(){
    /*Serial.println("Diff ");
    Serial.println(diff);
    Serial.println("Freq");
    Serial.println(freq);*/
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print((long)freq);
    lcd.print(" Hz ");

    lcd.setCursor(0, 1);
    switch(diff){
      case 1: lcd.print("1 Hz ");break;
      case 100: lcd.print("100 Hz ");break;
      case 1000: lcd.print("1 kHz ");break;
      case 10000: lcd.print("10 kHz ");break;
      case 100000: lcd.print("100 kHz ");break;
      case 1000000: lcd.print("1 MHz ");break;
    }

    lcd.print((freq/1e6),4);
    lcd.print("M");

}
void loop(){
value += encoder->getValue();
 tmp= abs(last-value);
  
    if (value > last) {
   
    last = value;
    
    
    freq=freq+diff*tmp;
    if (freq>40e6) freq=40e6;
    debug();
    DDS.setfreq(freq,0);
    delay(10);
  }

   if (value < last ) {
    
    last = value;

    freq=freq-diff*tmp;
    if (freq<0) freq=0;
    debug();
    DDS.setfreq(freq,0);
    delay(10);
   }
   // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):


 //Serial.println(digitalRead(9));
  ClickEncoder::Button b = encoder->getButton();
  //if (b==ClickEncoder::Open) Serial.println("Open");
  if (b != ClickEncoder::Open ) {
    if(b==ClickEncoder::Clicked){
         
          if (diff==1) {diff=100;debug();return;}
          if (diff==100) {diff=1000;debug();return;}
          if (diff==1000) {diff=10000;debug();return;}
          if (diff==10000) {diff=100000;debug();return;}
          if (diff==100000) {diff=1000000;debug();return;}
          if (diff==1000000) {diff=1;debug();return;}
    }
          
  }

}

