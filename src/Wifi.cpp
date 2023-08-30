#include "Wifi.hpp"

static const char *TAG = "Sunton_ESP32-8048S070c_board_wifi";

Wifi::Wifi(const char *wifi_ssid, const char *wifi_password)
    : ssid(wifi_ssid), password(wifi_password) {
  nvs_flash_init();
  connect();
}

Wifi::~Wifi() {}

void Wifi::eventHandler(void *event_handler_arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data) {
  if (event_id == WIFI_EVENT_STA_START) {
    ESP_LOGI(TAG, "WIFI CONNECTING....");
  } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
    // s_retry_num = 0;
    ESP_LOGI(TAG, "WiFi CONNECTED");
  } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGI(TAG, "WiFi lost connection");
    if (s_retry_num < 5) {
      esp_wifi_connect();
      s_retry_num++;
      ESP_LOGI(TAG, "Retrying to Connect...");
    }
  } else if (event_id == IP_EVENT_STA_GOT_IP) {
    ESP_LOGI(TAG, "WiFi got IP...");
  }
}

void Wifi::connect() {
  esp_netif_init();
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();
  wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&wifi_initiation);
  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, eventHandler, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, eventHandler, NULL);
  wifi_config_t wifi_configuration = {.sta =
                                          {
                                              .ssid = "",
                                              .password = "",

                                          }

  };
  strcpy((char *)wifi_configuration.sta.ssid, ssid);
  strcpy((char *)wifi_configuration.sta.password, password);
  esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);
  esp_wifi_start();
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_connect();
  ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s  password:%s", ssid,
           password);
}