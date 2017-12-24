# Sabre Ambient Lamp

## Test Release v0.1

### Hardware
- Run this code on a Teensy 3.2.
- Connect a wire to pin 23 to use as a capacitive sensor.

### Compiling
- The code should compile from within Visual Studio Code with the PlatformIO plugin enabled. It may also work from the command line with the PlatformIO Core CLI.

### Calibration
- Uncomment line 26 in "main.cpp" to enable capacative pin calibration. Adjust the global `touchTime` variable to change the pin sensitivity.

### Use
- Open a serial monitor to test the system. *Tap* or *Press & Hold* the wire to move through States and Settings. The Arduino `Serial.println()` function sends messages to the serial monitor.
