# SIMB Research — Beginning

A prototype environmental sensing system for a **Seasonal Ice Mass Balance (SIMB)** research buoy. The system reads distance and weather data from two sensors and logs the measurements to a file on a computer.

## Overview

This project uses an **Adafruit Feather M0** microcontroller to read two sensors, then sends the data over USB to a computer where a Python script timestamps and saves it to a CSV file.

**The basic cycle:**
1. The sensors power on and take a measurement.
2. The system waits (2 minutes in the final version; a few seconds in testing mode).
3. It takes a second measurement.
4. Data is saved to `sensor_data.csv`.

## Hardware

| Component | Purpose |
|-----------|---------|
| Adafruit Feather M0 | Microcontroller (reads sensors, sends data over USB) |
| MaxBotix MB7374-101 | Ultrasonic distance sensor (e.g. snow/ice surface height) |
| Bosch BME280 | Temperature, barometric pressure, and humidity sensor |

### Wiring

**BME280 (I2C):**
| BME280 pin | Feather M0 pin |
|------------|----------------|
| VIN / VCC  | 3V |
| GND        | GND |
| SCL        | SCL |
| SDA        | SDA |

**MaxBotix MB7374:**
| MaxBotix pin | Feather M0 pin |
|--------------|----------------|
| GND (black)  | GND |
| V+ (red)     | 3V or USB (3.3–5.5V) |
| PW (orange)  | Pin 5 |

## Software

### Files
- `sensor_logger.ino` — Arduino code (final version: 2 measurements, then idle)
- `sensor_logger_TESTING.ino` — Arduino code (testing version: loops every few seconds)
- `logger.py` — Python script that reads the serial data and saves it to a CSV file
- `sensor_data.csv` — Output data file (created automatically)

### Requirements

**Arduino IDE libraries:**
- Adafruit BME280
- Adafruit Unified Sensor

**Python (version 3):**


## How to Run

1. Upload the Arduino sketch to the Feather M0 using the Arduino IDE.
2. **Close the Arduino Serial Monitor** (only one program can use the port at a time).
3. In `logger.py`, set the `PORT` variable to match your board:
   - Mac: something like `/dev/cu.usbmodem101`
   - Windows: something like `COM3`
   - Linux: something like `/dev/ttyACM0`
   - (The script prints a list of available ports when it runs.)
4. Run the logger: logger.py
5. Data will appear in the terminal and be saved to `sensor_data.csv`.
6. To stop the testing version, press **Control + C**.

## Data Format

Each row in `sensor_data.csv` contains:
timestamp, reading, distance_cm, temperature_C, pressure_hPa, humidity_pct

## Project Status

This is an early-stage prototype. See `PROGRESS.md` for a detailed breakdown of what is complete, in progress, and planned next.

**Completed:** Git repo, sensor selection, sensor firmware, breadboard circuit, and basic continuous logging.

**Next up:** Final MCU/comms decision, MOSFET power switching, longer bench tests, watchdog timer, and deep-sleep power savings.

## Author

Savannah Byron
