# YOUNG_Serial_Interface

Arduino library for interfacing with the R.M. Young Model 32400 Serial Interface Box.

## Description

This library provides easy communication with the YOUNG 32400 Serial Interface Box using ASCII polled serial format via RS485. The library supports reading wind speed, wind direction, and four voltage input channels. It includes built-in conversion for the YOUNG 41342VC temperature probe.

## Features

- **ASCII Polled Protocol**: Send poll commands and receive formatted data
- **Configurable Device Address**: Support for multi-device RS485 networks (addresses 0-9, A-F)
- **19200 Baud Rate**: Standard configuration for YOUNG 32400
- **Multiple Data Channels**:
  - Wind speed (m/s)
  - Wind direction (degrees)
  - 4 voltage input channels (VIN1-VIN4)
- **Temperature Conversion**: Built-in support for YOUNG 41342VC temperature probe (0-1V = -50°C to +50°C)
- **Error Handling**: Timeout detection and error reporting

## Hardware Requirements

- R.M. Young Model 32400 Serial Interface Box configured for ASCII polled format
- RS485 to UART/TTL converter module (e.g., MAX485)
- Arduino board with at least one hardware serial port
- YOUNG 41342VC temperature probe (or other voltage output sensors)

## Wiring

```
YOUNG 32400 (RS485) <---> RS485 Converter <---> Arduino
                           A (Data+)              
                           B (Data-)              
                           TX/RX    <--->  Serial1 RX/TX
```

## Installation

### Arduino IDE
1. Download this repository as a ZIP file
2. In Arduino IDE, go to Sketch > Include Library > Add .ZIP Library
3. Select the downloaded ZIP file
4. Restart Arduino IDE

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    https://github.com/dkaulukukui/YOUNG_Serial_Interface.git
```

## Usage

### Basic Example

```cpp
#include <YOUNG_32400.h>

// Create sensor object (Serial port, device address)
YOUNG_32400 sensor(Serial1, '0');

void setup() {
  Serial.begin(115200);
  Serial1.begin(19200);
  sensor.begin();
}

void loop() {
  if (sensor.poll()) {
    float temp = sensor.getTemperatureVIN1();
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
  }
  delay(2000);
}
```

## API Reference

### Constructor

```cpp
YOUNG_32400(Stream &serial, char address = '0')
```
- `serial`: Reference to hardware serial port (Serial1, Serial2, etc.)
- `address`: Device address (0-9, A-F). Default is '0'

### Methods

#### Initialization
- `void begin()` - Initialize the sensor (call after Serial.begin())

#### Data Acquisition
- `bool poll()` - Poll the device and read response. Returns true if successful.

#### Wind Data
- `float getWindSpeed()` - Get wind speed in m/s
- `float getWindDirection()` - Get wind direction in degrees (0-360)

#### Voltage Channels (Raw)
- `uint16_t getVIN1Raw()` - Get raw VIN1 value (0-4000)
- `uint16_t getVIN2Raw()` - Get raw VIN2 value (0-4000)
- `uint16_t getVIN3Raw()` - Get raw VIN3 value (0-4000)
- `uint16_t getVIN4Raw()` - Get raw VIN4 value (0-4000)

#### Voltage Channels (Millivolts)
- `float getVIN1_mV()` - Get VIN1 in mV (0-1000 mV range)
- `float getVIN2_mV()` - Get VIN2 in mV (0-1000 mV range)
- `float getVIN3_mV()` - Get VIN3 in mV (0-5000 mV range)
- `float getVIN4_mV()` - Get VIN4 in mV (0-5000 mV range)

#### Temperature Conversion (YOUNG 41342VC)
- `float convertToTemperature(uint16_t rawValue)` - Convert raw value to temperature (°C)
- `float getTemperatureVIN1()` - Get temperature from VIN1 channel (°C)
- `float getTemperatureVIN2()` - Get temperature from VIN2 channel (°C)

#### Configuration & Error Handling
- `void setTimeout(unsigned long timeout)` - Set timeout in milliseconds (default: 1000)
- `const char* getLastError()` - Get last error message

## Data Format

The YOUNG 32400 responds to poll commands with ASCII data in the following format:

```
32400!XXXX,YYYY,ZZZZ,AAAA,BBBB,CCCC<CR><LF>
```

Where:
- `XXXX` = Wind speed (tenths of m/s)
- `YYYY` = Wind direction (tenths of degrees)
- `ZZZZ` = VIN1 (0-4000 scale for 0-1000 mV)
- `AAAA` = VIN2 (0-4000 scale for 0-1000 mV)
- `BBBB` = VIN3 (0-4000 scale for 0-5000 mV)
- `CCCC` = VIN4 (0-4000 scale for 0-5000 mV)

## YOUNG 41342VC Temperature Probe

The YOUNG 41342VC temperature probe outputs 0-1V for -50°C to +50°C:
- 0 mV = -50°C
- 1000 mV = +50°C
- Linear relationship: Temperature (°C) = -50 + (voltage_mV / 10)

Connect the probe to VIN1 or VIN2 channel of the YOUNG 32400 (0-1000 mV range).

## Examples

See the `examples` folder for complete sketches:
- **BasicReading**: Simple temperature reading example
- **AllChannels**: Read all available data channels

## Technical Notes

- **Baud Rate**: 19200, 8N1 (8 data bits, no parity, 1 stop bit)
- **Protocol**: ASCII polled format
- **Poll Command**: `M<address>!` (e.g., `M0!` for address 0)
- **Timeout**: Default 1000 ms, configurable
- **RS485**: Half-duplex communication with 2 ms turnaround time

## References

- [YOUNG 32400 Manual (PDF)](https://www.fondriest.com/pdf/rm_young_32400_manual.pdf)
- [YOUNG 41342VC Specifications](https://www.youngusa.com/)

## License

This library is released into the public domain.

## Author

Created by dkaulukukui, 2026
