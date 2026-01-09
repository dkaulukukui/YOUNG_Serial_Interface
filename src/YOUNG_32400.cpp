/*
  YOUNG_32400.cpp - Library for interfacing with YOUNG 32400 Serial Interface Box
  
  Created by dkaulukukui, 2026
  Released into the public domain.
*/

#include "YOUNG_32400.h"

// Protocol constants
const char* RESPONSE_PREFIX = "32400!";

// Constructor
YOUNG_32400::YOUNG_32400(Stream &serial, char address)
{
  _serial = &serial;
  _address = address;
  _timeout = 1000;  // Default 1 second timeout
  _dataValid = false;
  
  // Initialize data to zero
  _windSpeed = 0;
  _windDirection = 0;
  _vin1 = 0;
  _vin2 = 0;
  _vin3 = 0;
  _vin4 = 0;
  
  clearError();
}

// Initialize serial communication
void YOUNG_32400::begin()
{
  // Note: User must call Serial.begin(19200) before calling this
  // This method is here for future expansion if needed
  clearError();
}

// Poll the device and read response
bool YOUNG_32400::poll()
{
  clearError();
  
  // Clear any pending data in the serial buffer
  while (_serial->available()) {
    _serial->read();
  }
  
  // Send poll command: M<address>!
  _serial->print('M');
  _serial->print(_address);
  _serial->print('!');
  
  // Wait for response with timeout
  unsigned long startTime = millis();
  char response[RESPONSE_BUFFER_SIZE];
  int idx = 0;
  bool foundData = false;
  
  while (millis() - startTime < _timeout) {
    if (_serial->available()) {
      char c = _serial->read();
      
      // Look for start of response (digits or '3' from "32400!")
      if (!foundData && (c == '3' || isdigit(c))) {
        foundData = true;
        response[idx++] = c;
      }
      else if (foundData) {
        response[idx++] = c;
        
        // Check for end of line (CR or LF)
        if (c == '\r' || c == '\n') {
          response[idx-1] = '\0';  // Replace CR/LF with null terminator
          
          // Parse the response
          return parseResponse(response);
        }
        
        // Prevent buffer overflow
        if (idx >= RESPONSE_BUFFER_SIZE - 1) {
          response[idx] = '\0';
          setError("Response buffer overflow");
          return false;
        }
      }
    }
  }
  
  setError("Timeout waiting for response");
  return false;
}

// Parse the response string
// Expected format: 32400!XXXX,YYYY,ZZZZ,AAAA,BBBB,CCCC
bool YOUNG_32400::parseResponse(char* response)
{
  // Look for "32400!" prefix
  char *dataStart = strstr(response, RESPONSE_PREFIX);
  if (dataStart == NULL) {
    // Some configurations might not include the prefix, try parsing anyway
    dataStart = response;
  } else {
    dataStart += 6;  // Skip past "32400!"
  }
  
  // Parse comma-separated values
  char *token = strtok(dataStart, ",");
  int fieldCount = 0;
  
  while (token != NULL && fieldCount < 6) {
    uint16_t value = atoi(token);
    
    switch (fieldCount) {
      case 0: _windSpeed = value; break;
      case 1: _windDirection = value; break;
      case 2: _vin1 = value; break;
      case 3: _vin2 = value; break;
      case 4: _vin3 = value; break;
      case 5: _vin4 = value; break;
    }
    
    fieldCount++;
    token = strtok(NULL, ",");
  }
  
  if (fieldCount < 6) {
    setError("Incomplete data received");
    _dataValid = false;
    return false;
  }
  
  _dataValid = true;
  return true;
}

// Get wind speed in m/s
float YOUNG_32400::getWindSpeed()
{
  return _windSpeed / 10.0;  // Convert from tenths to actual value
}

// Get wind direction in degrees
float YOUNG_32400::getWindDirection()
{
  return _windDirection / 10.0;  // Convert from tenths to actual value
}

// Get raw VIN values
uint16_t YOUNG_32400::getVIN1Raw() { return _vin1; }
uint16_t YOUNG_32400::getVIN2Raw() { return _vin2; }
uint16_t YOUNG_32400::getVIN3Raw() { return _vin3; }
uint16_t YOUNG_32400::getVIN4Raw() { return _vin4; }

// Get voltage values in millivolts
// VIN1 and VIN2: 0-1000 mV range, scaled 0-4000
float YOUNG_32400::getVIN1_mV()
{
  return _vin1 / 4.0;
}

float YOUNG_32400::getVIN2_mV()
{
  return _vin2 / 4.0;
}

// VIN3 and VIN4: 0-5000 mV range, scaled 0-4000
float YOUNG_32400::getVIN3_mV()
{
  return _vin3 * 1.25;
}

float YOUNG_32400::getVIN4_mV()
{
  return _vin4 * 1.25;
}

// Convert raw value to temperature for YOUNG 41342VC probe
// Probe outputs 0-1V for -50°C to +50°C (linear)
// Formula: Temperature (°C) = -50 + (voltage_mV / 1000) * 100
//                           = -50 + voltage_mV / 10
float YOUNG_32400::convertToTemperature(uint16_t rawValue)
{
  // First convert raw value to millivolts (VIN1/VIN2 range)
  float voltage_mV = rawValue / 4.0;
  
  // Convert voltage to temperature
  // 0 mV = -50°C, 1000 mV = +50°C
  float temperature = -50.0 + (voltage_mV / 10.0);
  
  return temperature;
}

// Convenience methods for temperature
float YOUNG_32400::getTemperatureVIN1()
{
  return convertToTemperature(_vin1);
}

float YOUNG_32400::getTemperatureVIN2()
{
  return convertToTemperature(_vin2);
}

// Get last error message
const char* YOUNG_32400::getLastError()
{
  return _lastError;
}

// Set timeout
void YOUNG_32400::setTimeout(unsigned long timeout)
{
  _timeout = timeout;
}

// Clear error message
void YOUNG_32400::clearError()
{
  _lastError[0] = '\0';
}

// Set error message
void YOUNG_32400::setError(const char* error)
{
  strncpy(_lastError, error, sizeof(_lastError) - 1);
  _lastError[sizeof(_lastError) - 1] = '\0';
}
