/*
  BasicReading
  
  This example demonstrates basic usage of the YOUNG_32400 library to read
  temperature data from a YOUNG 41342VC temperature probe connected to the
  YOUNG 32400 Serial Interface Box.
  
  Hardware Setup:
  - YOUNG 32400 Serial Interface Box configured for ASCII polled format
  - Baud rate: 19200
  - RS485 to UART converter connected to Arduino Serial1 (or Serial on boards with only one serial port)
  - YOUNG 41342VC temperature probe connected to VIN1 channel of 32400
  
  Circuit:
  - RS485 TX/RX connected to Arduino Serial1 RX/TX
  
  Created by dkaulukukui, 2026
  Released into the public domain.
*/

#include <YOUNG_32400.h>

// Create YOUNG_32400 object
// First parameter: Serial port (Serial1, Serial2, etc.)
// Second parameter: Device address (0-9, A-F). Default is '0'
YOUNG_32400 sensor(Serial1, '0');

void setup() {
  // Initialize debug serial port
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("YOUNG 32400 Basic Reading Example");
  Serial.println("==================================");
  
  // Initialize communication with YOUNG 32400
  // Note: Must call Serial1.begin() with correct baud rate first
  Serial1.begin(19200);
  sensor.begin();
  
  Serial.println("Setup complete. Starting readings...\n");
}

void loop() {
  // Poll the sensor
  if (sensor.poll()) {
    // Successfully read data
    
    // Get wind data
    float windSpeed = sensor.getWindSpeed();
    float windDirection = sensor.getWindDirection();
    
    // Get temperature from VIN1 (where 41342VC probe is connected)
    float temperature = sensor.getTemperatureVIN1();
    
    // Get raw voltage from VIN1 in millivolts
    float voltage = sensor.getVIN1_mV();
    
    // Print the results
    Serial.println("--- Sensor Reading ---");
    Serial.print("Wind Speed: ");
    Serial.print(windSpeed);
    Serial.println(" m/s");
    
    Serial.print("Wind Direction: ");
    Serial.print(windDirection);
    Serial.println(" degrees");
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    Serial.print("Temperature Probe Voltage: ");
    Serial.print(voltage);
    Serial.println(" mV");
    
    Serial.println();
  } else {
    // Error reading data
    Serial.print("Error: ");
    Serial.println(sensor.getLastError());
  }
  
  // Wait 2 seconds before next reading
  delay(2000);
}
