# ============================================================================
#  Project:  SIMB Research - Dual Sensor Logger (computer side)
#  Author:   Savannah Byron
#
#  Description:
#    Reads CSV data coming from the Feather M0 over USB Serial and saves
#    each line (with a timestamp) into a CSV file on your computer.
#
#    Works with BOTH the testing version (runs forever) and the final
#    version (stops after "# Done"). Press Control + C to stop anytime.
# ============================================================================

import serial                      # Talks to the Arduino over the serial port
import serial.tools.list_ports     # Lets us list available serial ports
import time                        # Lets us pause the program
from datetime import datetime      # Used to timestamp each reading

# ---- CONFIGURE THESE FOR YOUR SETUP ----
PORT = "/dev/cu.usbmodem101"       # Feather M0 on this Mac
BAUD = 9600                        # Must match Serial.begin(9600) in the Arduino code
OUTPUT_FILE = "sensor_data.csv"    # File where the data will be saved
# -----------------------------------------


def find_port_hint():
    # Print all serial ports so you can identify which one is the Feather.
    print("Available ports:")
    for p in serial.tools.list_ports.comports():
        print(f"  {p.device} - {p.description}")


# Show the user what ports are available (helpful if PORT is wrong)
find_port_hint()

print(f"\nConnecting to {PORT} at {BAUD} baud...")

# Open the serial connection.
ser = serial.Serial(PORT, BAUD, timeout=130)

# IMPORTANT for Feather M0: give the board a moment to finish waking up
# before we try to read from it. This avoids the "Device not configured" error.
time.sleep(2)
ser.reset_input_buffer()   # Clear out any partial/garbage data
print("Connected! Giving the board a moment...\n")

# Open the output file in "append" mode so old data is not erased.
with open(OUTPUT_FILE, "a") as f:
    print(f"Logging to {OUTPUT_FILE}. Waiting for data...")
    print("(Press Control + C to stop logging.)\n")

    try:
        while True:
            try:
                # Read one line from the Arduino and clean it up.
                line = ser.readline().decode("utf-8", errors="ignore").strip()
            except serial.SerialException:
                # A brief USB hiccup can happen on the M0 - wait and try again.
                print("...waiting for board to be ready...")
                time.sleep(1)
                continue

            if not line:
                continue  # Skip empty lines

            # Create a timestamp for when we received this line.
            timestamp = datetime.now().isoformat()
            print(f"{timestamp}  |  {line}")

            # Save the timestamp + the sensor line to the file.
            f.write(f"{timestamp},{line}\n")
            f.flush()  # Force the data to be written to disk right away

            # The final Arduino version prints "# Done" when finished.
            # (The testing version never sends this, so it runs until Control + C.)
            if line.startswith("# Done"):
                print("\nMeasurements complete. Closing.")
                break

    except KeyboardInterrupt:
        # This runs when you press Control + C
        print("\nStopped by user. Closing.")

# Close the serial connection cleanly.
ser.close()