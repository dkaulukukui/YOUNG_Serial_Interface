/*
  YOUNG_32400.h - Library for interfacing with YOUNG 32400 Serial Interface Box
  
  This library provides communication with the R.M. Young Model 32400 Serial Interface
  Box using ASCII polled serial format via RS485 (appears as UART to microcontroller).
  
  Features:
  - ASCII polled protocol with configurable device address
  - 19200 baud rate, 8N1 format
  - Read wind speed, wind direction, and 4 voltage channels
  - Temperature conversion for YOUNG 41342VC probe (0-1V = -50°C to +50°C)
  
  Created by dkaulukukui, 2026
  Released into the public domain.
*/

#ifndef YOUNG_32400_h
#define YOUNG_32400_h

#include "Arduino.h"

class YOUNG_32400
{
  public:
    // Constructor - pass reference to Serial port and device address (0-9, A-F)
    YOUNG_32400(Stream &serial, char address = '0');
    
    // Initialize serial communication (call in setup())
    void begin();
    
    // Poll the device and read response
    // Returns true if successful, false if timeout or parse error
    bool poll();
    
    // Get parsed data values
    float getWindSpeed();        // Returns wind speed in m/s
    float getWindDirection();    // Returns wind direction in degrees (0-360)
    uint16_t getVIN1Raw();       // Returns raw VIN1 value (0-4000)
    uint16_t getVIN2Raw();       // Returns raw VIN2 value (0-4000)
    uint16_t getVIN3Raw();       // Returns raw VIN3 value (0-4000)
    uint16_t getVIN4Raw();       // Returns raw VIN4 value (0-4000)
    
    // Get voltage values in millivolts
    float getVIN1_mV();          // Returns VIN1 in mV (0-1000 mV range)
    float getVIN2_mV();          // Returns VIN2 in mV (0-1000 mV range)
    float getVIN3_mV();          // Returns VIN3 in mV (0-5000 mV range)
    float getVIN4_mV();          // Returns VIN4 in mV (0-5000 mV range)
    
    // Temperature conversion for YOUNG 41342VC probe
    // Call with VIN1 or VIN2 depending on which channel the probe is connected to
    // The probe outputs 0-1V for -50°C to +50°C
    float convertToTemperature(uint16_t rawValue);  // Returns temperature in °C
    float getTemperatureVIN1();  // Convenience method for VIN1
    float getTemperatureVIN2();  // Convenience method for VIN2
    
    // Get last error message
    const char* getLastError();
    
    // Set timeout for reading response (default 1000 ms)
    void setTimeout(unsigned long timeout);
    
  private:
    Stream *_serial;
    char _address;
    unsigned long _timeout;
    
    // Parsed data storage
    uint16_t _windSpeed;      // Wind speed in tenths of m/s
    uint16_t _windDirection;  // Wind direction in tenths of degrees
    uint16_t _vin1;           // VIN1 raw value (0-4000)
    uint16_t _vin2;           // VIN2 raw value (0-4000)
    uint16_t _vin3;           // VIN3 raw value (0-4000)
    uint16_t _vin4;           // VIN4 raw value (0-4000)
    
    bool _dataValid;
    char _lastError[64];
    
    // Constants
    static const int RESPONSE_BUFFER_SIZE = 128;
    
    // Internal methods
    bool parseResponse(char* response);
    void clearError();
    void setError(const char* error);
};

#endif
