#include "Wifi.hpp"

static const char *TAG = "Sunton_ESP32-8048S070c_board_wifi";

Wifi::Wifi() { init(); }

Wifi::~Wifi() {}

void Wifi::init() {
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();
  wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&config));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());

  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, eventHandler, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, eventHandler, NULL);

  wifi_config_t wifi_configuration;
  esp_wifi_get_config(WIFI_IF_STA, &wifi_configuration);
  if (strlen((char *)wifi_configuration.sta.ssid) != 0) {
    ESP_LOGI(TAG, "Connect with remembered wifi");
    esp_wifi_connect();
  }
}

void Wifi::printAuthMode(int authmode) {
  switch (authmode) {
  case WIFI_AUTH_OPEN:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OPEN");
    break;
  case WIFI_AUTH_OWE:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OWE");
    break;
  case WIFI_AUTH_WEP:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WEP");
    break;
  case WIFI_AUTH_WPA_PSK:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_PSK");
    break;
  case WIFI_AUTH_WPA2_PSK:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_PSK");
    break;
  case WIFI_AUTH_WPA_WPA2_PSK:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
    break;
  case WIFI_AUTH_WPA2_ENTERPRISE:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_ENTERPRISE");
    break;
  case WIFI_AUTH_WPA3_PSK:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_PSK");
    break;
  case WIFI_AUTH_WPA2_WPA3_PSK:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
    break;
  default:
    ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_UNKNOWN");
    break;
  }
}

std::vector<wifi_ap_record_t> Wifi::scan() {
  uint16_t max_ap_number = 20;
  wifi_ap_record_t ap_info[max_ap_number];
  uint16_t ap_count = 0;

  memset(ap_info, 0, sizeof(ap_info));

  esp_wifi_scan_start(NULL, true);

  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&max_ap_number, ap_info));

  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

  std::vector<wifi_ap_record_t> wifi_records(ap_info, ap_info + ap_count);

  // ESP_LOGI(TAG, "Number of scanned AP's: %u", wifi_records.size());
  // for (uint8_t i = 0; i < wifi_records.size(); i++) {
  //   ESP_LOGI(TAG, "SSID \t\t%s", wifi_records[i].ssid);
  //   ESP_LOGI(TAG, "RSSi \t\t%d", wifi_records[i].rssi);
  //   printAuthMode(wifi_records[i].authmode);
  //   ESP_LOGI(TAG, "Channel \t\t%d\n", wifi_records[i].primary);
  // }

  return wifi_records;
}

void Wifi::eventHandler(void *event_handler_arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data) {
  if (event_id == WIFI_EVENT_STA_START) {
    ESP_LOGI(TAG, "WiFi connecting....");
  } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
    connection_flag = true;
    ESP_LOGI(TAG, "WiFi connected");
  } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {

    if (connection_flag) {
      connection_flag = false;
      ESP_LOGE(TAG, "WiFi lost connection");
    } else {
      ESP_LOGE(TAG, "WiFi connecting failed");
    }

    wifi_event_sta_disconnected_t *event_info =
        static_cast<wifi_event_sta_disconnected_t *>(event_data);

    if (event_info->reason == WIFI_REASON_NO_AP_FOUND ||
        event_info->reason ==
            WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT) { // assuming the ssid comes
                                                  // from scan method and is
                                                  // always valid
      ESP_LOGI(TAG, "Retrying to connect...");
      esp_wifi_connect();
    } else {
      ESP_LOGE(TAG, "WiFi password wrong");
    }
  } else if (event_id == IP_EVENT_STA_GOT_IP) {
    ESP_LOGI(TAG, "WiFi got IP...");
  }
}

void Wifi::connect(const char *ssid, const char *password) {
  wifi_config_t wifi_configuration = {.sta =
                                          {
                                              .ssid = "",
                                              .password = "",

                                          }

  };
  strcpy((char *)wifi_configuration.sta.ssid, ssid);
  strcpy((char *)wifi_configuration.sta.password, password);
  esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);
  esp_wifi_connect();
  ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s  password:%s", ssid,
           password);
}

bool Wifi::getConnectionStatus() { return connection_flag; }