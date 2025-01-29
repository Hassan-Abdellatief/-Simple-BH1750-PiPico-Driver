// For this BH1750 ambient light sensor, I was unable to find any suitable libraries to integrate,
// so I developed my own driver code. This was tested on a Raspberry Pi Pico
// using the Arduino Mbed extension board in my university.

#include <Wire.h>

// BH1750 I2C address
#define BH1750_ADDRESS 0x23

// BH1750 Commands
#define BH1750_POWER_DOWN                 0x00
#define BH1750_POWER_ON                   0x01
#define BH1750_RESET                      0x07
#define BH1750_CONTINUOUS_HIGH_RES_MODE   0x10
#define BH1750_CONTINUOUS_HIGH_RES_MODE_2 0x11
#define BH1750_ONE_TIME_HIGH_RES_MODE     0x20
#define BH1750_ONE_TIME_HIGH_RES_MODE_2   0x21

// Function declarations
void configureBH1750(byte mode);
float readLightLevel();

void setup() {
    Serial.begin(9600);
    Wire.begin(); // Initialise I2C bus

    // Configure BH1750 with the POWER_ON command
    configureBH1750(BH1750_POWER_ON);
    Serial.println("BH1750 Test");
}

void loop() {
    float lightLevel = readLightLevel(); // Read light level in lux
    
    if (lightLevel >= 0) {
        Serial.print("Light: ");
        Serial.print(lightLevel);
        Serial.println(" lx");
    } else {
        // lightLevel < 0 indicates an unsuccessful sensor reading
        Serial.println("Error reading light level");
    }
    
    delay(1000); // Delay for one second between readings
}

void configureBH1750(byte mode) {
    Wire.beginTransmission(BH1750_ADDRESS);
    Wire.write(mode); // Send the desired mode to the sensor
    Wire.endTransmission();
    
    // Allow the sensor time to process the mode change
    delay(10);
}

float readLightLevel() {
    // Trigger a new measurement in one-time high-resolution mode
    Wire.beginTransmission(BH1750_ADDRESS);
    Wire.write(BH1750_ONE_TIME_HIGH_RES_MODE);
    Wire.endTransmission();
    
    // This mode requires a 180 ms delay to complete its measurement
    delay(180);
    
    // Request 2 bytes from the BH1750 sensor
    Wire.requestFrom(BH1750_ADDRESS, 2);
    if (Wire.available() != 2) {
        Serial.println("Error: Received an incorrect number of bytes from the sensor");
        return -1; // Indicate an error
    }
    
    // Read the two bytes
    byte highByte = Wire.read();
    byte lowByte  = Wire.read();
    
    // Combine the two bytes and convert to lux
    float level = (highByte << 8 | lowByte) / 1.2;
    return level;
}
