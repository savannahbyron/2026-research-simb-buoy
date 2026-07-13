# SIMB Research Project — Progress Tracker

Last updated: [add today's date]

============================================================
✅ COMPLETED
============================================================

[x] Set up a Git repository
    HOW: Created the GitHub repo "SIMB_research_beginning" and linked it
    to a local folder using GitHub Desktop. Now able to commit + push
    code changes to save version history online.

[x] Select sensors
    HOW: Chose the MaxBotix MB7374-101 (ultrasonic distance) and the
    Bosch BME280 (temperature/pressure/humidity) for the project.

[x] Write a basic Arduino script to initialize the MCU and communicate
    with the sensors
    HOW: Wrote Arduino code for the Feather M0 that starts serial
    communication, initializes the BME280 over I2C, reads the MaxBotix
    pulse-width on pin 5, and prints all readings as CSV. Confirmed both
    sensors report real data.

[x] Basic continuous data logging (bench prototype)
    HOW: Wrote a Python script (logger.py) using the pyserial library.
    It reads CSV data from the Feather over USB, adds a timestamp to each
    line, and saves everything into sensor_data.csv. Added Feather M0
    timing fixes (2-second startup pause + retry-on-hiccup) and a
    Control+C clean-exit. Made a testing version of the Arduino code that
    loops every 5 seconds for fast debugging.

[x] Assemble the initial circuit on a breadboard
    HOW: Wired the BME280 (I2C: SDA/SCL, 3.3V, GND) and MaxBotix
    (power, GND, pulse-width to pin 5) to the Feather M0 on a solderless
    breadboard. Confirmed stable connections since both sensors return
    valid readings.

[x] Run a bench test for continuous logging
    HOW: Used the testing-loop Arduino code with the Python logger to
    collect repeated timestamped readings into sensor_data.csv, confirming
    the full sensor-to-file pipeline works.


============================================================
🟡 IN PROGRESS
============================================================

[ ] Verify voltages with a multimeter
    STATUS: Need to measure the BME280 power pin (~3.3V) and MaxBotix
    power pin (3.3-5.5V) to confirm correct voltages.

[ ] Attach a battery
    STATUS: Currently powered over USB. Need to connect and test a
    battery so the system can run without a computer.

[ ] Add a README file to GitHub
    STATUS: README.md drafted; needs to be committed and pushed to the repo.


============================================================
⬜ NEXT STEPS (broken into ~1-2 hour chunks)
============================================================

--- CHUNK A: Select MCU (final decision) ---
[ ] Compare Feather M0 vs. ESP32 vs. dedicated comms modules
[ ] Confirm which board supports the buoy's satellite/radio needs
[ ] Document the final choice and reasoning

--- CHUNK B: MOSFET power-switching for sensors (~2 hrs) ---
[ ] Learn what a MOSFET does (low-side switch basics)
[ ] Wire a MOSFET so a digital pin can cut power to a sensor
[ ] Update Arduino code to turn sensor ON, measure, turn OFF
[ ] Test with multimeter that power actually cuts off

--- CHUNK C: Longer bench test (~1 hr setup, runs overnight) ---
[ ] Set testing WAIT_TIME to ~1-5 minutes
[ ] Start logger.py and let it run 24 hours
[ ] Next day: check sensor_data.csv for gaps or crashes

--- CHUNK D: Watchdog Timer (WDT) (~2 hrs) ---
[ ] Learn what a watchdog timer does (auto-reboot on freeze)
[ ] Add WDT library / code to the Feather sketch
[ ] Test it by intentionally freezing the code

--- CHUNK E: Deep-sleep power saving (~2 hrs) ---
[ ] Learn the difference between delay() and true deep-sleep
[ ] Add deep-sleep between measurements using a hardware timer
[ ] Measure current draw to confirm it drops during sleep


============================================================
📋 DOCUMENTATION TO-DO (do alongside technical work)
============================================================

[ ] Write Requirements Document (data rate, data size, power needs)
[ ] Create a high-level system block diagram
[ ] Perform component literature review to justify sensor selection
[ ] Draft Power Budget spreadsheet (active vs. sleep current)
[ ] Sketch mechanical layout in CAD


============================================================
🤔 BIG DECISION TO RESOLVE
============================================================

[ ] Decide final MCU + comms path.
    The buoy must "communicate w/ sat" but the Feather M0 has no built-in
    radio. Options to discuss with advisor:
      - Stick with M0 + add a separate radio (e.g. LoRa or Iridium modem)
      - Switch to an ESP32 (built-in WiFi/BT, but still no satellite)
      - Use a dedicated Iridium modem (e.g. RockBLOCK) for real deployment