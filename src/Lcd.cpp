#include "Lcd.hpp"

static const char *TAG = "Sunton_ESP32-8048S070c_board_lcd";

Lcd::Lcd() {
  ESP_LOGI(TAG, "Turn off LCD backlight");

  back_light_gpio->set_low();

  ESP_LOGI(TAG, "Install RGB LCD panel driver");
  esp_lcd_rgb_panel_config_t panel_config =
  {.clk_src = LCD_CLK_SRC_DEFAULT,
   .timings =
       {
           .pclk_hz = pixel_clock_frequency,
           .h_res = h_resolution,
           .v_res = v_resolution,
           .hsync_pulse_width = hsync_pulse_width,
           .hsync_back_porch = hsync_back_porch,
           .hsync_front_porch = hsync_front_porch,
           .vsync_pulse_width = vsync_pulse_width,
           .vsync_back_porch = vsync_back_porch,
           .vsync_front_porch = vsync_front_porch,
           .flags =
               {
                   .hsync_idle_low = (uint32_t)NULL,
                   .vsync_idle_low = (uint32_t)NULL,
                   .de_idle_high = (uint32_t)NULL,
                   .pclk_active_neg = pclk_active_neg,
                   .pclk_idle_high = (uint32_t)NULL,
               },
       },
   .data_width = data_width, // RGB565 in parallel mode, thus 16bit in width
   .bits_per_pixel = (uint8_t)NULL,
   .num_fbs = frame_buffers_number,
#if CONFIG_EXAMPLE_USE_BOUNCE_BUFFER
   .bounce_buffer_size_px = 10 * h_resolution,
#else
   .bounce_buffer_size_px = (size_t)NULL,
#endif
   .sram_trans_align = (size_t)NULL,
   .psram_trans_align = psram_trans_align,
   .hsync_gpio_num = hsync_pin,
   .vsync_gpio_num = vsync_pin,
   .de_gpio_num = de_pin,
   .pclk_gpio_num = pclk_pin,
   .disp_gpio_num = en_pin,

   .data_gpio_nums =
       {
           b0_pin,
           b1_pin,
           b2_pin,
           b3_pin,
           b4_pin,
           g0_pin,
           g1_pin,
           g2_pin,
           g3_pin,
           g4_pin,
           g5_pin,
           r0_pin,
           r1_pin,
           r2_pin,
           r3_pin,
           r4_pin,
       },
   .flags = {
       .disp_active_low = (uint32_t)NULL,
       .refresh_on_demand = (uint32_t)NULL,
       .fb_in_psram = frame_buffer_psram,
       .double_fb = (uint32_t)NULL,
       .no_fb = (uint32_t)NULL,
       .bb_invalidate_cache = (uint32_t)NULL,
   } };
  ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
}

void Lcd::init(void *user_display_driver) {
  ESP_LOGI(TAG, "Register event callbacks");
  esp_lcd_rgb_panel_event_callbacks_t cbs = {
      .on_vsync = onVsyncEvent,
      .on_bounce_empty = NULL,
  };
  ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(
      panel_handle, &cbs, user_display_driver));

  ESP_LOGI(TAG, "Initialize RGB LCD panel");
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

  ESP_LOGI(TAG, "Turn on LCD backlight");
  back_light_gpio->set_high();
}

Lcd::~Lcd() {}

bool Lcd::onVsyncEvent(esp_lcd_panel_handle_t panel,
                       const esp_lcd_rgb_panel_event_data_t *event_data,
                       void *user_data) {
  BaseType_t high_task_awoken = pdFALSE;
#if CONFIG_EXAMPLE_AVOID_TEAR_EFFECT_WITH_SEM
  if (xSemaphoreTakeFromISR(sem_gui_ready, &high_task_awoken) == pdTRUE) {
    xSemaphoreGiveFromISR(sem_vsync_end, &high_task_awoken);
  }
#endif
  return high_task_awoken == pdTRUE;
}

esp_lcd_panel_handle_t *Lcd::getPanelHandle() { return &panel_handle; }
