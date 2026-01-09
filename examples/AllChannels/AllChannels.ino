/*
  AllChannels
  
  This example demonstrates reading all available channels from the YOUNG 32400
  Serial Interface Box, including wind speed, wind direction, and all four
  voltage input channels.
  
  Hardware Setup:
  - YOUNG 32400 Serial Interface Box configured for ASCII polled format
  - Baud rate: 19200
  - RS485 to UART converter connected to Arduino Serial1
  - Various sensors connected to VIN1-VIN4 channels
  
  Circuit:
  - RS485 TX/RX connected to Arduino Serial1 RX/TX
  
  Created by dkaulukukui, 2026
  Released into the public domain.
*/

#include <YOUNG_32400.h>

// Create YOUNG_32400 object with device address '0'
YOUNG_32400 sensor(Serial1, '0');

void setup() {
  // Initialize debug serial port
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("YOUNG 32400 All Channels Example");
  Serial.println("=================================");
  
  // Initialize communication with YOUNG 32400
  Serial1.begin(19200);
  sensor.begin();
  
  // Set custom timeout if needed (default is 1000 ms)
  sensor.setTimeout(1500);
  
  Serial.println("Setup complete. Reading all channels...\n");
}

void loop() {
  // Poll the sensor
  if (sensor.poll()) {
    // Successfully read data
    
    Serial.println("======================================");
    Serial.println("           Sensor Reading             ");
    Serial.println("======================================");
    
    // Wind data
    Serial.println("\n--- Wind Data ---");
    Serial.print("Speed:     ");
    Serial.print(sensor.getWindSpeed(), 1);
    Serial.println(" m/s");
    
    Serial.print("Direction: ");
    Serial.print(sensor.getWindDirection(), 1);
    Serial.println(" degrees");
    
    // Voltage channels
    Serial.println("\n--- Voltage Channels ---");
    
    // VIN1 (0-1000 mV range)
    Serial.print("VIN1: ");
    Serial.print(sensor.getVIN1_mV(), 2);
    Serial.print(" mV (raw: ");
    Serial.print(sensor.getVIN1Raw());
    Serial.println(")");
    
    // VIN2 (0-1000 mV range)
    Serial.print("VIN2: ");
    Serial.print(sensor.getVIN2_mV(), 2);
    Serial.print(" mV (raw: ");
    Serial.print(sensor.getVIN2Raw());
    Serial.println(")");
    
    // VIN3 (0-5000 mV range)
    Serial.print("VIN3: ");
    Serial.print(sensor.getVIN3_mV(), 2);
    Serial.print(" mV (raw: ");
    Serial.print(sensor.getVIN3Raw());
    Serial.println(")");
    
    // VIN4 (0-5000 mV range)
    Serial.print("VIN4: ");
    Serial.print(sensor.getVIN4_mV(), 2);
    Serial.print(" mV (raw: ");
    Serial.print(sensor.getVIN4Raw());
    Serial.println(")");
    
    // Temperature conversions
    Serial.println("\n--- Temperature Conversions (41342VC) ---");
    Serial.print("VIN1 as Temperature: ");
    Serial.print(sensor.getTemperatureVIN1(), 2);
    Serial.println(" °C");
    
    Serial.print("VIN2 as Temperature: ");
    Serial.print(sensor.getTemperatureVIN2(), 2);
    Serial.println(" °C");
    
    Serial.println("\n======================================\n");
    
  } else {
    // Error reading data
    Serial.println("ERROR READING SENSOR");
    Serial.print("Error message: ");
    Serial.println(sensor.getLastError());
    Serial.println();
  }
  
  // Wait 3 seconds before next reading
  delay(3000);
}
