/* Activity 4-12 Thermistor + LED Indicator Circuit Program
* Filename: thermistorled.ino
* Author: Jiarong (Jane) Li
* Date: 16-Oct-2023
* IDE Version: Arduino IDE v. 2.2.1
* Purpose of Program: Calibrate a thermistor that will report temperature to the Serial Monitor. 
Incorporate a lightbulb that lights up when the temperature drops lower than 24.0 degrees Celcius.
* Library required: LiquidCrystal I2C by Frank de Brabander, v. 1.1.2
*/

const byte ledPin = 6;  // set LED pin to pin 6

void setup() {
  Serial.begin(9600);         // start serial monitor 9600 bps
  analogReference(EXTERNAL);  // use 3.3V = 1023 divs (external reference)
  pinMode(ledPin, OUTPUT);    // set pin 6 to output mode to light up the LED
}

void loop() {
  int divRead = analogRead(A1);  // Takes an analog reading from Pin A1
  delay(1000);                                                                               // Delay the reading for 1 second
  float voltage = divRead * 3.3 / 1023.0;                                                    // Convert divRead to voltage
  float resistance = 10000.0 * (voltage / (3.3 - voltage));                                  // Convert voltage to resistance
  float temperature = 1 / (1.0 / 273.15 + 1.0 / 3873.4 * log(resistance / 32200)) - 273.15;  // Convert voltage to temperature
  Serial.println(temperature);                                                               // Print the temperature to the Serial monitor
  delay(1000);                                                                               // Delay the reading for 1 second
  if (temperature < 24.0) {                                                                  // Check if temperature is less than 24.0 degrees celsius
    digitalWrite(ledPin, HIGH);                                                              // Turn on the LED if temperature is less than 24.0 degrees celsius
  } else {                                                                                   // if temperature is more than 24.0 degrees celsius
    digitalWrite(ledPin, LOW);                                                               // Turn off the LED
  }
}
