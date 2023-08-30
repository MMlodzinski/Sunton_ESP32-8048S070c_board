#pragma once

#include <vector>

#include "esp_event.h"
#include "esp_event_loop.h"
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
  Wifi();
  ~Wifi();

  static void eventHandler(void *event_handler_arg, esp_event_base_t event_base,
                           int32_t event_id, void *event_data);
  void connect(const char *ssid, const char *password);
  static void printAuthMode(int authmode);
  void init();
  std::vector<wifi_ap_record_t> scan();
};
