#include "Touch.hpp"

static const char *TAG = "Sunton_ESP32-8048S070c_board_touch";

Touch::Touch() {
  i2c_handler = std::make_shared<idf::I2CMaster>(
      idf::I2CNumber::I2C0(), idf::SCL_GPIO(scl_pin), idf::SDA_GPIO(sda_pin),
      idf::Frequency(i2c_frequency), true, true);

  esp_lcd_panel_io_handle_t tp_io_handle = NULL;
  esp_lcd_panel_io_i2c_config_t tp_io_config =
      ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
  // Attach the TOUCH to the I2C bus
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)I2C_NUM_0,
                                           &tp_io_config, &tp_io_handle));

  esp_lcd_touch_config_t tp_cfg = {
      .x_max = lcd_h_resolution,
      .y_max = lcd_v_resolution,
      .rst_gpio_num = rst_pin,
      .int_gpio_num = int_pin,
      .levels =
          {
              .reset = NULL,
              .interrupt = NULL,
          },
      .flags =
          {
              .swap_xy = 0,
              .mirror_x = 0,
              .mirror_y = 0,
          },
      .process_coordinates = NULL,
      .interrupt_callback = NULL,
  };

  ESP_LOGI(TAG, "Initialize GT911 touch controller");
  ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &tp));
}

Touch::~Touch() {}

esp_lcd_touch_handle_t *Touch::getTouchHandler() { return &tp; }
