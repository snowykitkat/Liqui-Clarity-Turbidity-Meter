/* PHC435 Design Project: Liqui-Clarity Turbidity Meter
* Filename: PHC435_turbidity_meter.ino
* Author: Jiarong (Jane) Li
* Date: 05/Dec/2023
* IDE Version: Arduino IDE v. 2.2.1
* Purpose of Program: Convert voltage to NTU units to test turbidity.
* Library used: 
  - QuickStats by David Dubins, v. 1.2.4
  - LiquidCrystal I2C by Frank de Brabander, v. 1.1.2
*/

#include <LiquidCrystal_I2C.h>       // Import the LiquidCrystal I2C library for display on the LCD screen
#include "QuickStats.h"              // Import the QuickStats library for filtering
QuickStats stats;                    // Initialize an instance of th Quickstats class
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Creates the LCD object to print to

#define NUMREPEATS 100           // Number of readings to take per datapoint
float readings[NUMREPEATS];      // Create an array to hold analog readings
float smoothed;                  // Create a variable to hold final filtered value
String date = "";                // Create a variable to hold the date
unsigned long timer = millis();  // To hold start time

const byte buttonPin = 7;  // Set button pin as pin 7
const byte ledPin = 6;     // Set LED pin as pin 6
const byte signal = A0;    // Set op-amp output pin as pin A0
int measurement = 1;       // Start measurement number as 1

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);                                        // Set button as input_pullup state
  pinMode(ledPin, OUTPUT);                                                 // Set LED Pin to output mode
  Serial.begin(9600);                                                      // Set up the Serial monitor
  Serial.println();                                                        // Print a new line
  Serial.println("Enter the date for today in the format DD/MM/YYYY:> ");  // Prompts the user to enter the date for the day
  while (Serial.available() == 0) {}                                       // Wait for user to input the date
  date = Serial.readString();                                              // Save the user input into variable <date>
  Serial.println("Date: " + date);                                         // Print the date onto the Serial Monitor
  Serial.print("Measurement: ");                                           // Print "Measurement: " header
  Serial.print(", ");                                                      // Print a comma
  Serial.print("Turbidity (NTU): ");                                       // Print "Turbidity (NTU): "" header
  Serial.println();                                                        // Print a new line
  lcd.init();                                                              // Initialize the LCD screen
  lcd.backlight();                                                         // Turn on the backlight
  lcd.clear();                                                             // Clear LCD screen
  lcd.setCursor(0, 0);                                                     // Set Cursor to first row, col
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {             // If button switch is pushed
    lcd.clear();                                   // Clear LCD screen
    lcd.setCursor(0, 0);                           // Set cursor to first row, col
    unsigned long startTime = millis();            // Record start time
    while (digitalRead(buttonPin) == LOW) {}       // Wait until button is not pushed
    unsigned long timeDur = millis() - startTime;  // Record duration
    if (timeDur > 10) {                            // Ignore short pushes (debounce)
      digitalWrite(ledPin, HIGH);                  // Switch on the IR LED
      for (int i = 0; i < NUMREPEATS; i++) {
        int divs = analogRead(signal);  // Read the divisions from Analog Pin A2
        readings[i] = divs;             // Fill array with division readings
      }
      smoothed = stats.average(readings, NUMREPEATS);  // Mean filter
      float voltage = divs_to_volts(smoothed);         // Convert divisions to voltage
      int ntu_units = volts_to_ntu(voltage);           // Convert voltage to NTU units
      Serial.print(measurement);                       // Print measurement number to Serial monitor
      Serial.print(", ");                              // Print a comma
      Serial.print(ntu_units);                         // Print NTU units
      Serial.println();                                // Print a new line
      lcd.print(ntu_units);                            // Print NTU units to the lcd display
      measurement += 1;                                // Increment measurements by 1
    }
  }
  digitalWrite(ledPin, LOW);  // Switch off the IR LED
}

float divs_to_volts(int divs) {  // Function to convert divisions to voltage
  return divs * 5.0 / 1024.0;    // Formula to convert divisions to voltage
}

float volts_to_ntu(float volts) {                              // Function to convert voltage to NTU units
  if (volts < 2.56) {                                          // Check if voltage is less than 2.56V
    return 3004.7;                                             // If voltage is less than 2.56V, return 3004.7 NTU
  } else if (volts > 4.2) {                                    // Check if voltage is more than 4.2V
    return 0.0;                                                // If voltage is more than 4.2V, return 0.0 NTU
  } else {                                                     // Check if voltage is in between 2.56V to 4.2V
    return -1120.4 * square(volts) + 5742.3 * volts - 4352.9;  // Calcalate the NTU to return if voltage is in between 2.56V to 4.2V
  }
}