# LCD4004 PCF8574 ESPHome External Component

External component for ESPHome that drives HD44780-compatible `40x4` LCD modules
through a `PCF8574` I2C backpack.

## Status

This component has been tested on a `40x4` LCD setup with a dual-controller
layout (top 2 rows + bottom 2 rows).

This component now enforces `dimensions: 40x4` in schema validation.
Other LCD sizes are not supported.

## Repository Layout

- `components/lcd4004_pcf8574/` component source
- `examples/` sanitized ESPHome example configurations

## License

This repository is published as `GPL-3.0-or-later`.

Reason: parts of this component are adapted from ESPHome's existing LCD/PCF8574
component implementation. See `NOTICE.md` for attribution.

Additional metadata:

- License text: `LICENSE`
- Copyright/summary: `COPYRIGHT`
- Third-party attribution: `NOTICE.md`

## Install (GitHub Source)

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/<your-user>/<your-repo>.git
      ref: main
    components: [lcd4004_pcf8574]
```

## Minimal Usage

```yaml
display:
  - platform: lcd4004_pcf8574
    id: lcd
    dimensions: 40x4
    address: 0x27
    lambda: |-
      it.print(0, 0, "LCD4004 + PCF8574");
      it.print(0, 1, "ESPHome external");
      it.print(0, 2, "component");
      it.print(0, 3, "ready.");
```

For full configuration details, see:
`components/lcd4004_pcf8574/README.md`

## Development Note

Parts of this repository were developed with AI assistance and then reviewed,
tested, and curated by a human maintainer before publication.
