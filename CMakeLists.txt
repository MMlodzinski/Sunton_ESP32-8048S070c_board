cmake_minimum_required(VERSION 3.16)

idf_build_get_property(target IDF_TARGET)

set(srcs "src/Lcd.cpp" "src/Touch.cpp" "src/Wifi.cpp")
set(requires "esp_timer" "freertos" "driver" "esp_wifi" "nvs_flash")

# if(NOT ${target} STREQUAL "linux")
#     list(APPEND srcs
#         "esp_event_api.cpp"
#         "esp_event_cxx.cpp")
#     list(APPEND requires "esp_event" "pthread")
# endif()

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "inc"
                    #PRIV_INCLUDE_DIRS "private_include"
                    #PRIV_REQUIRES freertos driver
                    REQUIRES ${requires})