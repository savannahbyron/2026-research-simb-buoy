/*
 * ============================================================================
 *  Project:  SIMB Research - Dual Sensor Logger
 *  Author:   Savannah Byron
 *  Sensors:  MaxBotix MB7374-101 (ultrasonic distance)
 *            Bosch BME280 (temperature, pressure, humidity)
 *  Board:    Adafruit Feather M0
 *
 *  Description:
 *    On startup the board wakes up, takes one measurement from both
 *    sensors, waits 2 minutes, takes a second measurement, then goes idle.
 *    All data is printed over USB Serial as CSV so a computer can log it.
 * ============================================================================
 */

#include <Wire.h>               // Handles I2C communication (used by BME280)
#include <Adafruit_Sensor.h>    // Adafruit's common sensor interface
#include <Adafruit_BME280.h>    // Driver for the BME280 sensor

// ---------------------------------------------------------------------------
//  MaxBotix MB7374 (ultrasonic rangefinder) settings
// ---------------------------------------------------------------------------
const int pwPin = 5;        // Pin 5 reads the pulse-width (orange wire)
long pulseDuration;         // Raw pulse length in microseconds
long distanceCm;            // Calculated distance in centimeters

// ---------------------------------------------------------------------------
//  BME280 (temperature / pressure / humidity) settings
// ---------------------------------------------------------------------------
Adafruit_BME280 bme;        // Create the BME280 object (communicates over I2C)
bool bmeReady = false;      // Tracks whether the sensor was found successfully

// ---------------------------------------------------------------------------
//  setup() runs ONCE when the board powers on or resets.
//  All of our measurement logic lives here.
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);       // Start USB serial connection at 9600 baud

  // Wait until a computer opens the serial port before continuing.
  // (Remove this block if you ever run the board without a computer.)
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

  Serial.println("# Sensors starting...");

  // Print the CSV column headers so the saved file is easy to read.
  Serial.println("reading,distance_cm,temperature_C,pressure_hPa,humidity_pct");

  // ----- Measurement #1 -----
  takeMeasurement(1);

  // ----- Wait 2 minutes (120,000 milliseconds) -----
  delay(120000);

  // ----- Measurement #2 -----
  takeMeasurement(2);

  // Signal that we are finished. The Python logger watches for this line.
  Serial.println("# Done. Sensors idle.");
}

// ---------------------------------------------------------------------------
//  loop() runs continuously after setup(), but we leave it empty because
//  we only want two measurements total, then stop.
// ---------------------------------------------------------------------------
void loop() {
  // Nothing to do here.
}

// ---------------------------------------------------------------------------
//  takeMeasurement()
//  Reads both sensors and prints one line of CSV data.
//  'readingNumber' just labels whether this is reading 1 or 2.
// ---------------------------------------------------------------------------
void takeMeasurement(int readingNumber) {
  // ----- Read the MaxBotix ultrasonic sensor -----
  // Take several samples and average them for a more stable reading.
  long sum = 0;
  int samples = 5;
  for (int i = 0; i < samples; i++) {
    pulseDuration = pulseIn(pwPin, HIGH);  // Measure pulse length
    sum += (pulseDuration / 58);           // 58 microseconds = 1 cm
    delay(100);                            // Small pause between samples
  }
  distanceCm = sum / samples;              // Average of all samples

  // ----- Read the BME280 environmental sensor -----
  float temp = NAN, pressure = NAN, humidity = NAN;  // Default to "not a number"
  if (bmeReady) {
    temp = bme.readTemperature();          // Temperature in Celsius
    pressure = bme.readPressure() / 100.0; // Pressure in hPa (Pa / 100)
    humidity = bme.readHumidity();         // Relative humidity in %
  }

  // ----- Print all values as one CSV row -----
  Serial.print(readingNumber);
  Serial.print(",");
  Serial.print(distanceCm);
  Serial.print(",");
  Serial.print(temp, 2);        // 2 decimal places
  Serial.print(",");
  Serial.print(pressure, 2);
  Serial.print(",");
  Serial.println(humidity, 2);
}