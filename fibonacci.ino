/* Fibonacci Sequence Program
* Filename: fibonacci.ino
* Author: D. Dubins and Class
* Date: 04-Oct-2023
* IDE Version: Arduino IDE v. 2.2.1
* Purpose of Program: Display fibonacci sequence and stop before variable type maxes out.
* Library required: LiquidCrystal I2C by Frank de Brabander, v. 1.1.2
*/

#include <LiquidCrystal_I2C.h>       // include the LCD Library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // This creates the LCD object to print to

int fibcount_first = 1;   // to hold the first number
int fibcount_second = 1;  // to hold the second number
int fibcount_third = 1;   // to hold the third number

void setup() {
  lcd.init();           // initialize the LCD screen
  lcd.backlight();      // turn on the backlight
  lcd.clear();          // clear LCD screen
  lcd.setCursor(0, 0);  // set Cursor to first row, col
  lcd.print("1");       // print a 1
  delay(1000);          // wait a second

  do {
    lcd.clear();                                        // clear the screen after addition
    lcd.setCursor(0, 0);                                // go to top of LCD
    lcd.print(fibcount_third);                          // output the answer to the screen
    fibcount_third = fibcount_first + fibcount_second;  // calculate the next value of the fibonacci sequence
    fibcount_first = fibcount_second;                   // change the value of fibcount_first to fibcount_second
    fibcount_second = fibcount_third;                   // change the value of fibcount_second to fibcount_third
    delay(1000);                                        // delay the printout by 1 second
  } while (fibcount_third > fibcount_first);            // our exit condition will not print the last number
}

void loop() {
}
