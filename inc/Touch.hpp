#pragma once

#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_lcd_touch_gt911.h"
#include "esp_log.h"
#include "gpio_cxx.hpp"
#include "i2c_cxx.hpp"
#include <memory>

class Touch {
private:
  const int scl_pin = 20;
  const int sda_pin = 19;
  const gpio_num_t int_pin = GPIO_NUM_NC;
  const gpio_num_t rst_pin = GPIO_NUM_38;
  int i2c_frequency = 100000; // Hz
  const uint16_t lcd_h_resolution = 800;
  const uint16_t lcd_v_resolution = 480;
  std::shared_ptr<idf::I2CMaster> i2c_handler;
  esp_lcd_touch_handle_t tp = NULL;

public:
  Touch(/* args */);
  ~Touch();
  esp_lcd_touch_handle_t *getTouchHandler();
};
