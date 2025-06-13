# ChessLink Version 1

ChessLink is an interactive chess project that bridges physical chess play with digital capabilities through embedded sensors and LED feedback. This repository contains the full open-source code used for the demonstration prototype.

For more inforamtion, view the following links
- YouTube page with video of our project in action: [youtube.com/@ChessLink](https://www.youtube.com/@ChessLink)
- Website with additional documentation: [linkchess.org](https://linkchess.org/)

## Project Overview

ChessLink integrates hardware sensors with a software application to:
- Detect chess pieces on a physical board using optical sensors
- Provide visual feedback through RGB LEDs
- Process and transmit board state in real-time
- Display the game state digitally

## Hardware Components

- **Microcontrollers**: Multiple Arduino Nano boards (regions A, B, C, D)
- **Sensors**: Phototransistors for piece detection
- **Visual Feedback**: RGB LEDs (WS2812B) on the edge of each square
- **Communication**: Serial and I2C interfaces between components

## Software Architecture

- **Firmware**: Arduino code for sensor reading and LED control
- **Libraries**: Custom StickerReader and LEDUtils for hardware interfacing
- **Backend**: Python application for processing and integrating data from multiple controllers
- **Communication**: Serial protocol for data transfer

## Directory Structure

- `/src` - Main source code for the Arduino firmware
- `/lib` - Custom libraries for sensor reading and LED control
- `/app` - Python application for data processing from board serial
- `/calibration_files` - Sensor calibration data for different boards
- `/examples` - Example code for different hardware components
- `/visualize` - Web visualization components

## Development Tools

- **PlatformIO**: Used for building and uploading firmware
- **Arduino Framework**: Base framework for microcontroller code
- **FastLED**: Library for controlling RGB LEDs
- **ArduinoJSON**: For structured data handling

## Getting Started

### Prerequisites
- PlatformIO CLI or PlatformIO IDE extension for VS Code
- Python 3.x with required packages (see app/requirements.txt)
- Arduino Nano boards

### Setup
1. Clone this repository
2. Install required dependencies
3. Connect Arduino boards to your computer
4. Upload firmware to each board using appropriate environment

## Useful Commands 

### Debugging
- Monitor serial output: `pio device monitor --port /dev/cu.usbserial-130 --baud 9600`
- Upload firmware to region D board: `pio run -t upload -e nanoD`
- Upload firmware to other regions: `pio run -t upload -e nano[A|B|C]`

### Environment Configuration
Each region of the chessboard (A-D) is controlled by a separate Arduino Nano, configured through different PlatformIO environments in `platformio.ini`.

## Hardware Design

The ChessLink hardware system consists of:
1. **Sensor Array**: Phototransistors detect chess pieces through light reflection
2. **LED Matrix**: RGB LEDs provide visual feedback for each square
3. **Control System**: Multiple Arduino Nanos, each responsible for a 4x4 section of the board
4. **Communication Layer**: I2C and serial interfaces connect the components

## Firmware Operation

The firmware in each Arduino Nano:
1. Initializes the FastLED library to control the RGB LEDs
2. Sets up the StickerReader to detect chess pieces
3. Reads analog values from phototransistors
4. Processes readings to determine if pieces are present
5. Updates the board state and communicates via serial/I2C
6. Controls LEDs to provide visual feedback

## Python Backend

The Python application in the `app` directory:
- Establishes serial connections to all Arduino boards
- Maps regional coordinates to standard chess notation
- Integrates data from all board regions
- Updates the current FEN representation of the board
- Provides an interface for external chess applications

## Working Example Video

Watch our demonstration video:

https://github.com/user-attachments/assets/0f5ba1e5-fd48-4c44-ab49-8f6b43367be4

## Future Development

Planned improvements include:
- Improved piece recognition algorithms
- Wireless connectivity options
- Integration with popular chess engines
- Enhanced LED feedback patterns
- Mobile application support
