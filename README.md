## BH1750 Ambient Light Sensor Driver for Pi Pico (Arduino Mbed Environment)

### Overview
This repository contains a simple driver for the BH1750 ambient light sensor, created when I was unable to find an existing library for my project. It was tested on a **Raspberry Pi Pico** using the **Arduino Mbed extension board** at my university. The code demonstrates how to configure the BH1750 via I2C, trigger a measurement, and read the resulting lux values.

### How It Works
1. **I2C Initialization**: The code uses the standard Arduino `Wire.h` library to set up I2C communication.  
2. **Powering On**: At startup, the sensor is sent the `BH1750_POWER_ON` command using the `sendBH1750Command()` function.  
3. **Measurement**: Each loop triggers a new measurement by sending the `BH1750_ONE_TIME_HIGH_RES_MODE` command via `getBH1750LightLevel()`.  
4. **Data Retrieval**: After a `BH1750_MEASUREMENT_DELAY` (180 ms), two bytes are requested from the sensor. These are converted into a lux reading using the BH1750’s conversion factor (`/ 1.2`).  

### Features
- Straightforward, single-file sketch in C++ that’s easy to integrate as a `.ino` file for Arduino IDE.  
- Uses named constants (`BH1750_POWER_ON_DELAY`, `BH1750_MEASUREMENT_DELAY`) instead of "magic numbers" for better clarity.  
- Basic error checking (verifies that two bytes are read).  
- Demonstrates how to communicate with an I2C device on the Pi Pico using the Arduino Mbed environment.  

### Limitations
- Only one BH1750 mode is demonstrated: **one-time high-resolution mode**. Other modes exist but are not shown here.  
- Uses fixed delays rather than acknowledging data-ready signals.  
- No adjustable measurement timing register (MTreg) control.  

### Usage
1. **Wiring**:  
   - Connect the BH1750’s SDA to the Pi Pico’s SDA pin.  
   - Connect the BH1750’s SCL to the Pi Pico’s SCL pin.  
   - Provide power (3.3 V) to the BH1750 and common ground.  
2. **Upload**:  
   - Copy the code into a sketch in your Arduino IDE (with the Pi Pico and Mbed core installed) or another environment supporting Arduino on the Pico.  
3. **Monitor**:  
   - Open the Serial Monitor set to 9600 baud. You should see lux readings if the BH1750 is connected properly.  

### Testing
- This code was originally tested on a Pi Pico within a group project at my university, and it performed correctly for our needs at the time.  
- We no longer have access to the exact sensor hardware setup for further verification, so you may wish to test and adjust if any issues arise.  

### Further Development
- **Extend Mode Support**: Add continuous modes (`BH1750_CONTINUOUS_HIGH_RES_MODE`) or lower-resolution modes.  
- **Customize Sensitivity**: Implement measurement time register (MTreg) adjustments for more accurate readings or different lighting conditions.  
- **Robust Error Handling**: Check acknowledgements from the sensor before proceeding.  

### License
You may use or modify the code freely for personal or commercial purposes. If you improve upon it, feel free to share your changes.

---

**Note:** This simple driver proved sufficient for my group’s requirements at the time, and it highlights my ability to handle I2C communication directly. If you have any questions or recommendations, please open an issue or submit a pull request.
