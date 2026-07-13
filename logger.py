# ============================================================================
#  Project:  SIMB Research - Dual Sensor Logger (computer side)
#  Author:   Savannah Byron
#
#  Description:
#    Reads CSV data coming from the Feather M0 over USB Serial and saves
#    each line (with a timestamp) into a CSV file on your computer.
# ============================================================================

import serial                      # Talks to the Arduino over the serial port
import serial.tools.list_ports     # Lets us list available serial ports
from datetime import datetime      # Used to timestamp each reading

# ---- CONFIGURE THESE FOR YOUR SETUP ----
PORT = "/dev/cu.usbmodem101"   # Feather M0 on this Mac 
BAUD = 9600                  # Must match Serial.begin(9600) in the Arduino code
OUTPUT_FILE = "sensor_data.csv"   # File where the data will be saved
# -----------------------------------------


def find_port_hint():
    # Print all serial ports so you can identify which one is the Feather.
    print("Available ports:")
    for p in serial.tools.list_ports.comports():
        print(f"  {p.device} - {p.description}")


# Show the user what ports are available (helpful if PORT is wrong)
find_port_hint()

print(f"\nConnecting to {PORT} at {BAUD} baud...")

# Open the serial connection. timeout is set above 2 minutes so we don't
# give up while the board is waiting between measurements.
ser = serial.Serial(PORT, BAUD, timeout=130)

# Open the output file in "append" mode so old data is not erased.
with open(OUTPUT_FILE, "a") as f:
    print(f"Logging to {OUTPUT_FILE}. Waiting for data...\n")

    while True:
        # Read one line from the Arduino and clean it up.
        line = ser.readline().decode("utf-8", errors="ignore").strip()

        if not line:
            continue  # Skip empty lines

        # Create a timestamp for when we received this line.
        timestamp = datetime.now().isoformat()
        print(f"{timestamp}  |  {line}")

        # Save the timestamp + the sensor line to the file.
        f.write(f"{timestamp},{line}\n")
        f.flush()  # Force the data to be written to disk right away

        # The Arduino prints "# Done" when finished, so we stop then.
        if line.startswith("# Done"):
            print("\nMeasurements complete. Closing.")
            break

# Close the serial connection cleanly.
ser.close()