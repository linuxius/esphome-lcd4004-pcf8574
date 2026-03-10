# LCD4004 PCF8574 Component

ESPHome display platform for HD44780-compatible `40x4` LCDs connected via a
`PCF8574` I2C backpack.

## Highlights

- Integrates as an ESPHome `display:` platform: `lcd4004_pcf8574`.
- Uses `lcd_base` rendering and lambda writer support.
- Supports dual-enable row split used by common 40x4 modules.
- Keeps I2C-only wiring (`SDA`/`SCL`) and standard LCD rendering APIs.

## Requirements

- ESPHome external components support.
- I2C configured in your node.
- `40x4` LCD with PCF8574 backpack.

## Install

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/<your-user>/<your-repo>.git
      ref: main
    components: [lcd4004_pcf8574]
```

## Minimal Configuration

```yaml
i2c:
  sda: 21
  scl: 22
  scan: true

display:
  - platform: lcd4004_pcf8574
    id: lcd
    dimensions: 40x4
    address: 0x27
    update_interval: 1s
    lambda: |-
      it.print(0, 0, "LCD4004 + PCF8574");
      it.print(0, 1, "Line 2");
      it.print(0, 2, "Line 3");
      it.print(0, 3, "Line 4");
```

## Configuration Notes

- Base options come from ESPHome `lcd_base` and `display` schemas.
- I2C options (including `address`) come from ESPHome `i2c_device_schema`.
- `dimensions` is validated and must be exactly `40x4`.
- Default I2C address in this component is `0x3F`; many modules use `0x27`.

## Known Limitations

- Hardware-tested on one 40x4 dual-controller setup.
- Backpack bit mapping is currently hardcoded for a common mapping:
  - `RS=0x01`
  - `EN1=0x04` (top two rows)
  - `EN2=0x02` (bottom two rows)
  - Backlight bit `0x08`
- If your backpack wiring differs, source-level changes may be required.

## Example Configs

See the repository `examples/` folder for sanitized configurations.
