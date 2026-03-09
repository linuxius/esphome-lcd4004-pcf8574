#pragma once

#include "esphome/core/component.h"
#include "esphome/components/lcd_base/lcd_display.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/display/display.h"

namespace esphome {
namespace lcd4004_pcf8574 {

// PCF8574 bit mapping for 40x4 LCD (manufacturer layout)
static const uint8_t LCD_RS_BIT   = 0x01;  // Register Select
static const uint8_t LCD_EN1_BIT  = 0x04;  // Enable for rows 0–1
static const uint8_t LCD_EN2_BIT  = 0x02;  // Enable for rows 2–3

class PCF8574LCDDisplay;

using pcf8574_lcd_writer_t = display::DisplayWriter<PCF8574LCDDisplay>;

class PCF8574LCDDisplay : public lcd_base::LCDDisplay, public i2c::I2CDevice {
 public:
  void set_writer(pcf8574_lcd_writer_t &&writer) { this->writer_ = std::move(writer); }
  void setup() override;
  void dump_config() override;
  void update() override;
  void backlight();
  void no_backlight();

 protected:
  bool is_four_bit_mode() override { return true; }
  void write_n_bits(uint8_t value, uint8_t n) override;
  void send(uint8_t value, bool rs) override;
  uint8_t active_enable_bit_{LCD_EN1_BIT};
  bool broadcast_all_{false};   // used only during setup init, optional but recommended

  void call_writer() override { this->writer_(*this); }

  // Stores the current state of the backlight.
  uint8_t backlight_value_;
  pcf8574_lcd_writer_t writer_;
};

}  // namespace lcd4004_pcf8574
}  // namespace esphome
