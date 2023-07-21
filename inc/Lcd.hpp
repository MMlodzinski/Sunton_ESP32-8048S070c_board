#pragma once

#include "esp_err.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "gpio_cxx.hpp"
#include <memory>

extern "C" {
extern SemaphoreHandle_t sem_vsync_end;
extern SemaphoreHandle_t sem_gui_ready;
}

class Lcd {
private:
  const uint32_t pixel_clock_frequency = 17000000; // Hz

  const int back_light_on_level = 1;
  const int back_light_off_level = 0;

  const int back_light_pin = 2;
  const int hsync_pin = 39;
  const int vsync_pin = 40;
  const int de_pin = 41;
  const int pclk_pin = 42;
  const int en_pin = -1;

  const int b0_pin = 15;
  const int b1_pin = 7;
  const int b2_pin = 6;
  const int b3_pin = 5;
  const int b4_pin = 4;
  const int g0_pin = 9;
  const int g1_pin = 46;
  const int g2_pin = 3;
  const int g3_pin = 8;
  const int g4_pin = 16;
  const int g5_pin = 1;
  const int r0_pin = 14;
  const int r1_pin = 21;
  const int r2_pin = 47;
  const int r3_pin = 48;
  const int r4_pin = 45;

  const uint32_t h_resolution = 800;
  const uint32_t v_resolution = 480;

  const size_t data_width = 16; // bits
  const size_t psram_trans_align = 64;

  const uint32_t hsync_back_porch = 16;
  const uint32_t hsync_front_porch = 210;
  const uint32_t hsync_pulse_width = 30;
  const uint32_t vsync_back_porch = 10;
  const uint32_t vsync_front_porch = 22;
  const uint32_t vsync_pulse_width = 13;

  const bool pclk_active_neg = true;
  const bool frame_buffer_psram = true;

#if CONFIG_EXAMPLE_DOUBLE_FB
  const size_t frame_buffers_number = 2;
  const bool double_frame_buffer = true;
#else
  const size_t frame_buffers_number = 1;
  const bool double_frame_buffer = false;
#endif // CONFIG_EXAMPLE_DOUBLE_FB

  static bool onVsyncEvent(esp_lcd_panel_handle_t panel,
                           const esp_lcd_rgb_panel_event_data_t *event_data,
                           void *user_data);

  std::shared_ptr<idf::GPIO_Output> back_light_gpio =
      std::make_shared<idf::GPIO_Output>(idf::GPIONum(back_light_pin));
  esp_lcd_panel_handle_t panel_handle = NULL;

public:
  Lcd();
  ~Lcd();
  void init(void *user_display_driver);
  esp_lcd_panel_handle_t *getPanelHandle();
};
