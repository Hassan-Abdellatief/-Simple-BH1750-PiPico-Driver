#include <Wire.h>

// BH1750 I2C address
#define BH1750_ADDRESS 0x23

// BH1750 Commands
#define BH1750_POWER_DOWN                  0x00
#define BH1750_POWER_ON                    0x01
#define BH1750_RESET                       0x07
#define BH1750_CONTINUOUS_HIGH_RES_MODE    0x10
#define BH1750_CONTINUOUS_HIGH_RES_MODE_2  0x11
#define BH1750_ONE_TIME_HIGH_RES_MODE      0x20
#define BH1750_ONE_TIME_HIGH_RES_MODE_2    0x21

// Timings for BH1750 measurements (in milliseconds)
#define BH1750_POWER_ON_DELAY  10
#define BH1750_MEASUREMENT_DELAY 180

/**
 * @brief Sends a command byte to the BH1750 sensor.
 * 
 * @param command The command byte to send (e.g., BH1750_POWER_ON).
 */
void sendBH1750Command(byte command) {
    Wire.beginTransmission(BH1750_ADDRESS);
    Wire.write(command);
    Wire.endTransmission();
    delay(BH1750_POWER_ON_DELAY); // Allow sensor time to process the command
}

/**
 * @brief Triggers a one-time high-resolution measurement and reads the result in lux.
 * 
 * @return A float representing the light level in lux. Returns -1 if an error occurs.
 */
float getBH1750LightLevel() {
    // Trigger a new measurement in one-time high-resolution mode
    Wire.beginTransmission(BH1750_ADDRESS);
    Wire.write(BH1750_ONE_TIME_HIGH_RES_MODE);
    Wire.endTransmission();
    
    // Wait for the measurement to complete
    delay(BH1750_MEASUREMENT_DELAY);
    
    // Request 2 bytes from the BH1750 sensor
    Wire.requestFrom(BH1750_ADDRESS, (uint8_t)2);
    if (Wire.available() != 2) {
        Serial.println("Error: Invalid number of bytes received from BH1750");
        return -1.0f; // Indicate an error
    }
    
    // Read the two bytes
    byte highByte = Wire.read();
    byte lowByte  = Wire.read();
    
    // Combine bytes into a 16-bit value and convert to lux
    uint16_t rawValue = (static_cast<uint16_t>(highByte) << 8) | lowByte;
    float lightLevel = rawValue / 1.2f; // BH1750 formula for one-time high-res mode
    return lightLevel;
}

void setup() {
    Serial.begin(9600);
    Wire.begin();  // Initialize I2C communication
    
    // Power on the BH1750 sensor
    sendBH1750Command(BH1750_POWER_ON);
    
    Serial.println("Starting BH1750 Test...");
}

void loop() {
    float lightLevel = getBH1750LightLevel();
    
    if (lightLevel >= 0.0f) {
        Serial.print("Light Level: ");
        Serial.print(lightLevel);
        Serial.println(" lx");
    } else {
        Serial.println("Error reading light level from BH1750");
    }
    
    delay(1000); // Delay one second between measurements
}
