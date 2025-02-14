#include "LiquidCrystal.h"

LiquidCrystal lcd( 8,  9,  4,  5,  6,  7);

char currentCommand = 'P';


void setup() {

 // Honestly, could get rid of this 
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("  Hi Tova!  ");
 lcd.setCursor(0,1);
 lcd.print("Move Stick");


  Serial.begin(9600);
}

void loop() {

  recvOneChar();

  lcd.setCursor(0,0);
  lcd.print("Hi Tova & Char!");
  lcd.setCursor(0,1);

  // lcd.print(" ");

  if (currentCommand == 'W') {
    lcd.print("Front Left        ");
  } else if (currentCommand == 'X') {
    lcd.print("Front Right       ");
  } else if (currentCommand == 'Y') {
    lcd.print("Back Right        ");
  } else if (currentCommand == 'Z') {
    lcd.print("Back Left               ");
  } else if (currentCommand == 'L') {
    lcd.print("Left               ");
  } else if (currentCommand == 'F') {
    lcd.print("Front               ");
  } else if (currentCommand == 'B') {
    lcd.print("Backwards               ");
  } else if (currentCommand == 'R') {
    lcd.print("Right               ");
  } else {
    lcd.print(":)                  ");
  }

  Serial.println(currentCommand);
} 

void recvOneChar() {
    if (Serial.available() > 0) {
        currentCommand = Serial.read();
    }
}