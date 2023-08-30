#pragma once

#include <string>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

class Wifi {
private:
  const char *ssid;
  const char *password;
  inline static int max_retry = 5;

  inline static EventGroupHandle_t s_wifi_event_group;
  inline static int s_retry_num = 0;

public:
  Wifi(const char *wifi_ssid, const char *wifi_password);
  ~Wifi();

  static void eventHandler(void *event_handler_arg, esp_event_base_t event_base,
                           int32_t event_id, void *event_data);
  void connect();
};
