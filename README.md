### STM32 Firmware
- Reads data from TSL2591 light sensor.
- Controls LEDs based on ambient light.
- Sends data over WiFi to the Flask server.

### Flask Server
- Receives lux data from STM32.
- Displays current lux readings on a web page.
- Includes an "About Me" section and project description.
- Can be extended to use PostgreSQL for data storage.

## Getting Started

### STM32
- Build and flash the firmware using STM32CubeIDE or Makefile.
- Ensure WiFi credentials are configured in `SECRETS_H.h`.

### Flask Server
```bash
cd flask-server
python -m venv venv
source venv/bin/activate   # Linux/Mac
venv\Scripts\activate      # Windows
pip install -r requirements.txt
python run.py
