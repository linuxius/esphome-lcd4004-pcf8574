#include "pcf8574_display.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace lcd4004_pcf8574 {

static const char *const TAG = "lcd4004_pcf8574";

static const uint8_t LCD_DISPLAY_BACKLIGHT_ON = 0x08;
static const uint8_t LCD_DISPLAY_BACKLIGHT_OFF = 0x00;

void PCF8574LCDDisplay::setup() {
  this->backlight_value_ = LCD_DISPLAY_BACKLIGHT_ON;
  if (!this->write_bytes(this->backlight_value_, nullptr, 0)) {
    this->mark_failed();
    return;
  }

  this->active_enable_bit_ = LCD_EN1_BIT;

  // During base LCDDisplay::setup(), mirror everything to both controllers:
  // - 4-bit init sequence
  // - function set, display control, entry mode
  // - clear/home
  // - and user-defined chars (CGRAM) if you use them
  this->broadcast_all_ = true;
  LCDDisplay::setup();
  this->broadcast_all_ = false;
}
void PCF8574LCDDisplay::dump_config() {
  ESP_LOGCONFIG(TAG,
                "PCF8574 LCD Display:\n"
                "  Columns: %u, Rows: %u",
                this->columns_, this->rows_);
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
}
void PCF8574LCDDisplay::write_n_bits(uint8_t value, uint8_t n) {
  if (n == 4) {
    // ESPHome init passes 0x03/0x02 as nibbles, needs to go to D7..D4
    value <<= 4;
  }

  uint8_t data = value | this->backlight_value_;
  this->write_bytes(data, nullptr, 0);

  auto pulse = [&](uint8_t en_bit) {
    this->write_bytes(data | en_bit, nullptr, 0);
    delayMicroseconds(1);
    this->write_bytes(data, nullptr, 0);
    delayMicroseconds(100);
  };

  if (this->broadcast_all_) {
    // Initialize BOTH controllers (and also load CGRAM to both if desired)
    pulse(LCD_EN1_BIT);
    pulse(LCD_EN2_BIT);
  } else {
    pulse(this->active_enable_bit_);
  }
}
void PCF8574LCDDisplay::send(uint8_t value, bool rs) {
  const uint8_t rs_mask = rs ? LCD_RS_BIT : 0x00;
  this->write_n_bits((value & 0xF0) | rs_mask, 0);
  this->write_n_bits(((value << 4) & 0xF0) | rs_mask, 0);
}
void PCF8574LCDDisplay::backlight() {
  this->backlight_value_ = LCD_DISPLAY_BACKLIGHT_ON;
  this->write_bytes(this->backlight_value_, nullptr, 0);
}
void PCF8574LCDDisplay::no_backlight() {
  this->backlight_value_ = LCD_DISPLAY_BACKLIGHT_OFF;
  this->write_bytes(this->backlight_value_, nullptr, 0);
}
void PCF8574LCDDisplay::update() {
  this->clear();
  this->call_writer();

  // Controller 1 (top): rows 0,1
  this->active_enable_bit_ = LCD_EN1_BIT;
  this->send(0x80 | 0x00, false);
  for (uint8_t i = 0; i < this->columns_; i++)
    this->send(this->buffer_[i], true);

  if (this->rows_ >= 2) {
    this->send(0x80 | 0x40, false);
    for (uint8_t i = 0; i < this->columns_; i++)
      this->send(this->buffer_[this->columns_ + i], true);
  }

  // Controller 2 (bottom): rows 2,3
  if (this->rows_ >= 3) {
    this->active_enable_bit_ = LCD_EN2_BIT;

    this->send(0x80 | 0x00, false);
    for (uint8_t i = 0; i < this->columns_; i++)
      this->send(this->buffer_[this->columns_ * 2 + i], true);

    if (this->rows_ >= 4) {
      this->send(0x80 | 0x40, false);
      for (uint8_t i = 0; i < this->columns_; i++)
        this->send(this->buffer_[this->columns_ * 3 + i], true);
    }
  }
}


}  // namespace lcd4004_pcf8574
}  // namespace esphome
