/*
 * ============================================================================
 *  Project:  SIMB Research - Dual Sensor Logger  [TESTING VERSION]
 *  Author:   Savannah Byron
 *  Sensors:  MaxBotix MB7374-101 (ultrasonic distance)
 *            Bosch BME280 (temperature, pressure, humidity)
 *
 *  Description:
 *    TESTING MODE - Takes a measurement every few seconds, over and over,
 *    so you can quickly confirm both sensors and logging are working.
 *    (Switch back to the 2-minute version for real data collection.)
 * ============================================================================
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// ---------------------------------------------------------------------------
//  MaxBotix MB7374 (ultrasonic rangefinder) settings
// ---------------------------------------------------------------------------
const int pwPin = 5;        // Pin 5 reads the pulse-width (orange wire)
long pulseDuration;         // Raw pulse length in microseconds
long distanceCm;            // Calculated distance in centimeters

// ---------------------------------------------------------------------------
//  BME280 (temperature / pressure / humidity) settings
// ---------------------------------------------------------------------------
Adafruit_BME280 bme;        // BME280 object (communicates over I2C)
bool bmeReady = false;      // Tracks whether the sensor was found

// ---------------------------------------------------------------------------
//  TESTING SETTINGS
// ---------------------------------------------------------------------------
const long WAIT_TIME = 5000;   // Time between measurements in milliseconds
                               // 5000 = 5 seconds (change this for testing)
int readingNumber = 0;         // Counts how many measurements we've taken

// ---------------------------------------------------------------------------
//  setup() runs ONCE when the board powers on or resets.
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // Wait until a computer opens the serial port before continuing.
  while (!Serial) {
    delay(10);
  }

  pinMode(pwPin, INPUT);    // Configure pin 5 to listen to the MaxBotix pulse

  // Try to start the BME280 at its two common I2C addresses.
  if (bme.begin(0x76) || bme.begin(0x77)) {
    bmeReady = true;
    Serial.println("# BME280 found");
  } else {
    Serial.println("# WARNING: BME280 not found - check wiring/address");
  }

  Serial.println("# Sensors starting... [TESTING MODE]");

  // Print the CSV column headers once at the start.
  Serial.println("reading,distance_cm,temperature_C,pressure_hPa,humidity_pct");
}

// ---------------------------------------------------------------------------
//  loop() runs OVER AND OVER. In testing mode we take a measurement,
//  wait a few seconds, and repeat forever.
// ---------------------------------------------------------------------------
void loop() {
  readingNumber++;              // Increase the counter each time
  takeMeasurement(readingNumber);
  delay(WAIT_TIME);             // Wait before the next measurement
}

// ---------------------------------------------------------------------------
//  takeMeasurement()
//  Reads both sensors and prints one line of CSV data.
// ---------------------------------------------------------------------------
void takeMeasurement(int readingNumber) {
  // ----- Read the MaxBotix ultrasonic sensor -----
  long sum = 0;
  int samples = 5;
  for (int i = 0; i < samples; i++) {
    pulseDuration = pulseIn(pwPin, HIGH);  // Measure pulse length
    sum += (pulseDuration / 58);           // 58 microseconds = 1 cm
    delay(100);                            // Small pause between samples
  }
  distanceCm = sum / samples;              // Average of all samples

  // ----- Read the BME280 environmental sensor -----
  float temp = NAN, pressure = NAN, humidity = NAN;
  if (bmeReady) {
    temp = bme.readTemperature();          // Temperature in Celsius
    pressure = bme.readPressure() / 100.0; // Pressure in hPa
    humidity = bme.readHumidity();         // Relative humidity in %
  }

  // ----- Print all values as one CSV row -----
  Serial.print(readingNumber);
  Serial.print(",");
  Serial.print(distanceCm);
  Serial.print(",");
  Serial.print(temp, 2);
  Serial.print(",");
  Serial.print(pressure, 2);
  Serial.print(",");
  Serial.println(humidity, 2);
}