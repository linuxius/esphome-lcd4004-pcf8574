## Third-Party Attribution

This project includes code adapted from:

- ESPHome LCD/PCF8574 component implementation
  - Project: https://github.com/esphome/esphome
  - License: GPL-3.0-or-later for C++ runtime components

The adapted code is primarily in:

- `components/lcd4004_pcf8574/display.py`
- `components/lcd4004_pcf8574/pcf8574_display.cpp`
- `components/lcd4004_pcf8574/pcf8574_display.h`

Modifications include dual-enable handling for 40x4 controller splits and
component-specific update behavior for four-line output.
