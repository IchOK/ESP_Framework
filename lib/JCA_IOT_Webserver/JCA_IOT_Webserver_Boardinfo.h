/**
 * @file JCA_IOT_Webserver_Boardinfo.h
 * @author JCA(https: // github.com/ichok)
 * @brief Selection of Boardtypes
 * Contains Variant and MCU Type.
 * @version 0.1
 * @date 2023-03-09
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_IOT_WEBSERVER_BOARDINFO_
#define _JCA_IOT_WEBSERVER_BOARDINFO_
#if defined(ARDUINO_ESP8266_GENERIC)
  #define BOARD_VARIANT "generic"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESP01)
  #define BOARD_VARIANT "esp8285"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_GEN4_IOD)
  #define BOARD_VARIANT "generic"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ADAFRUIT_HUZZAH)
  #define BOARD_VARIANT "adafruit"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_AMPERKA_WIFI_SLOT)
  #define BOARD_VARIANT "wifi_slot"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ARDUINO)
  #define BOARD_VARIANT "generic"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESP01)
  #define BOARD_VARIANT "esp8285"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_OAK)
  #define BOARD_VARIANT "oak"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESP13)
  #define BOARD_VARIANT "ESPDuino"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESPECTRO_CORE)
  #define BOARD_VARIANT "espectro"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESPINO_ESP12)
  #define BOARD_VARIANT "espino"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESPRESSO_LITE_V1)
  #define BOARD_VARIANT "espresso_lite_v1"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESPRESSO_LITE_V2)
  #define BOARD_VARIANT "espresso_lite_v2"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_SONOFF_SV)
  #define BOARD_VARIANT "itead"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_INVENT_ONE)
  #define BOARD_VARIANT "inventone"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  #define BOARD_VARIANT "d1_mini"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  #define BOARD_VARIANT "d1_mini"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINILITE)
  #define BOARD_VARIANT "d1_mini"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINIPRO)
  #define BOARD_VARIANT "d1_mini"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WEMOS_D1R1)
  #define BOARD_VARIANT "d1"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_AGRUMINO_LEMON_V4)
  #define BOARD_VARIANT "agruminolemonv4"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_NODEMCU_ESP12)
  #define BOARD_VARIANT "nodemcu"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_NODEMCU_ESP12E)
  #define BOARD_VARIANT "nodemcu"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_MOD_WIFI_ESP8266)
  #define BOARD_VARIANT "modwifi"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_PHOENIX_V1)
  #define BOARD_VARIANT "phoenix_v1"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_PHOENIX_V2)
  #define BOARD_VARIANT "phoenix_v2"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_SCHIRMILABS_EDUINO_WIFI)
  #define BOARD_VARIANT "eduinowifi"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WIO_LINK)
  #define BOARD_VARIANT "wiolink"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_THING)
  #define BOARD_VARIANT "thing"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_THING)
  #define BOARD_VARIANT "thing"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_THING_DEV)
  #define BOARD_VARIANT "thing"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESP210)
  #define BOARD_VARIANT "generic"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESPINO_ESP13)
  #define BOARD_VARIANT "espinotee"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_wifi_kit_8)
  #define BOARD_VARIANT "wifi_kit_8"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_WIFIDUINO_ESP8266)
  #define BOARD_VARIANT "wifiduino"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_WIFINFO)
  #define BOARD_VARIANT "wifinfo"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_XINABOX_CW01)
  #define BOARD_VARIANT "xinabox"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP32S3_DEV)
  #define BOARD_VARIANT "redpill_esp32s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ESP32C3_DEV)
  #define BOARD_VARIANT "adafruit_qtpy_esp32c3"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_ESP32S2_DEV)
  #define BOARD_VARIANT "department_of_alchemy_minimain_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "dpu_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_WROOM_DA)
  #define BOARD_VARIANT "esp32da"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_PICO)
  #define BOARD_VARIANT "pico32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_S3_BOX)
  #define BOARD_VARIANT "esp32s3box"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ESP32_S3_USB_OTG)
  #define BOARD_VARIANT "esp32s3usbotg"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ESP32S3_CAM_LCD)
  #define BOARD_VARIANT "esp32s3camlcd"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ESP32S2_USB)
  #define BOARD_VARIANT "esp32s2usb"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ESP32_WROVER_KIT)
  #define BOARD_VARIANT "esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_TINYPICO)
  #define BOARD_VARIANT "um_tinypico"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_FEATHERS2)
  #define BOARD_VARIANT "um_feathers2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_FEATHERS2NEO)
  #define BOARD_VARIANT "um_feathers2neo"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_TINYS2)
  #define BOARD_VARIANT "um_tinys2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_RMP)
  #define BOARD_VARIANT "um_rmp"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_TINYS3)
  #define BOARD_VARIANT "um_tinys3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_PROS3)
  #define BOARD_VARIANT "um_pros3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_FEATHERS3)
  #define BOARD_VARIANT "um_feathers3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "S_ODI_Ultra_v1"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_MICROS2)
  #define BOARD_VARIANT "micro_s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "magicbit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_PICO)
  #define BOARD_VARIANT "pico32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_TTGO_T1)
  #define BOARD_VARIANT "ttgo-t1"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_TTGO_T7_V13_Mini32)
  #define BOARD_VARIANT "ttgo-t7-v13-mini32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_TTGO_T7_V14_Mini32)
  #define BOARD_VARIANT "ttgo-t7-v14-mini32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_TTGO_T_OI_PLUS_DEV)
  #define BOARD_VARIANT "ttgo-t-oi-plus"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "xinabox"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_THING)
  #define BOARD_VARIANT "esp32thing"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_THING_PLUS)
  #define BOARD_VARIANT "esp32thing_plus"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_THING_PLUS_C)
  #define BOARD_VARIANT "esp32thing_plus_c"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32S2_THING_PLUS)
  #define BOARD_VARIANT "esp32s2thing_plus"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ESP32_MICROMOD)
  #define BOARD_VARIANT "esp32micromod"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "sparkfun_lora_gateway_1-channel"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_IOT_REDBOARD)
  #define BOARD_VARIANT "sparkfun_esp32_iot_redboard"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_UBLOX_NINA_W10)
  #define BOARD_VARIANT "nina_w10"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_UBLOX_NORA_W10)
  #define BOARD_VARIANT "nora_w10"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_WIDORA_AIR)
  #define BOARD_VARIANT "widora-air"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP320)
  #define BOARD_VARIANT "esp320"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_NANO32)
  #define BOARD_VARIANT "nano32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_LOLIN_D32)
  #define BOARD_VARIANT "d32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_LOLIN_D32_PRO)
  #define BOARD_VARIANT "d32_pro"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_LOLIN_C3_MINI)
  #define BOARD_VARIANT "lolin_c3_mini"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_LOLIN_S2_MINI)
  #define BOARD_VARIANT "lolin_s2_mini"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_LOLIN_S2_PICO)
  #define BOARD_VARIANT "lolin_s2_pico"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_LOLIN_S3)
  #define BOARD_VARIANT "lolin_s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_LOLIN32)
  #define BOARD_VARIANT "lolin32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_LOLIN32_LITE)
  #define BOARD_VARIANT "lolin32-lite"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Pocket32)
  #define BOARD_VARIANT "pocket_32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Pocket32)
  #define BOARD_VARIANT "pocket_32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESPea32)
  #define BOARD_VARIANT "espea32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_QUANTUM)
  #define BOARD_VARIANT "quantum"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Node32s)
  #define BOARD_VARIANT "node32s"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_HORNBILL_ESP32_DEV)
  #define BOARD_VARIANT "hornbill32dev"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_HORNBILL_ESP32_MINIMA)
  #define BOARD_VARIANT "hornbill32minima"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32C3_DEV)
  #define BOARD_VARIANT "dfrobot_beetle_esp32c3"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_ESP32S3_DEV)
  #define BOARD_VARIANT "dfrobot_firebeetle2_esp32s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "firebeetle32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_INTOROBOT_ESP32_DEV)
  #define BOARD_VARIANT "intorobot-fig"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ONEHORSE_ESP32_DEV)
  #define BOARD_VARIANT "onehorse32dev"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_FEATHER_ESP32)
  #define BOARD_VARIANT "feather_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_METRO_ESP32S2)
  #define BOARD_VARIANT "adafruit_metro_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_MAGTAG29_ESP32S2)
  #define BOARD_VARIANT "adafruit_magtag29_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_FUNHOUSE_ESP32S2)
  #define BOARD_VARIANT "adafruit_funhouse_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  #define BOARD_VARIANT "adafruit_feather_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2_TFT)
  #define BOARD_VARIANT "adafruit_feather_esp32s2_tft"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ADAFRUIT_QTPY_ESP32S2)
  #define BOARD_VARIANT "adafruit_qtpy_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_ADAFRUIT_QTPY_ESP32C3)
  #define BOARD_VARIANT "adafruit_qtpy_esp32c3"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_ADAFRUIT_QTPY_ESP32_PICO)
  #define BOARD_VARIANT "adafruit_qtpy_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32_V2)
  #define BOARD_VARIANT "adafruit_feather_esp32_v2"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3)
  #define BOARD_VARIANT "adafruit_feather_esp32s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3_NOPSRAM)
  #define BOARD_VARIANT "adafruit_feather_esp32s3_nopsram"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3_TFT)
  #define BOARD_VARIANT "adafruit_feather_esp32s3_tft"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ADAFRUIT_QTPY_ESP32S3_NOPSRAM)
  #define BOARD_VARIANT "adafruit_qtpy_esp32s3_nopsram"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ADAFRUIT_ITSYBITSY_ESP32)
  #define BOARD_VARIANT "adafruit_itsybitsy_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_NodeMCU_32S)
  #define BOARD_VARIANT "nodemcu-32s"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_MH_ET_LIVE_ESP32DEVKIT)
  #define BOARD_VARIANT "mhetesp32devkit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_MH_ET_LIVE_ESP32MINIKIT)
  #define BOARD_VARIANT "mhetesp32minikit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_esp32vn_iot_uno)
  #define BOARD_VARIANT "esp32vn-iot-uno"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "doitESP32devkitV1"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "doitESPduino32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_EVB)
  #define BOARD_VARIANT "esp32-evb"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_GATEWAY)
  #define BOARD_VARIANT "esp32-gateway"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_POE)
  #define BOARD_VARIANT "esp32-poe"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_POE_ISO)
  #define BOARD_VARIANT "esp32-poe-iso"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEVKIT_LIPO)
  #define BOARD_VARIANT "esp32-devkit-lipo"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESPino32)
  #define BOARD_VARIANT "espino32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5Stack_Core_ESP32)
  #define BOARD_VARIANT "m5stack_core_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5STACK_FIRE)
  #define BOARD_VARIANT "m5stack_fire"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5Stack_Station)
  #define BOARD_VARIANT "m5stack_station"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5Stick_C)
  #define BOARD_VARIANT "m5stick_c"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5Stack_ATOM)
  #define BOARD_VARIANT "m5stack_atom"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5STACK_Core2)
  #define BOARD_VARIANT "m5stack_core2"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5Stack_Timer_CAM)
  #define BOARD_VARIANT "m5stack_timer_cam"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_M5Stack_CoreInk)
  #define BOARD_VARIANT "m5stack_coreink"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ODROID_ESP32)
  #define BOARD_VARIANT "odroid_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_heltec_wifi_kit_32)
  #define BOARD_VARIANT "heltec_wifi_kit_32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_heltec_wifi_kit_32_V3)
  #define BOARD_VARIANT "heltec_wifi_kit_32_V3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_heltec_wifi_lora_32)
  #define BOARD_VARIANT "heltec_wifi_lora_32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_heltec_wifi_lora_32_V2)
  #define BOARD_VARIANT "heltec_wifi_lora_32_V2"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_heltec_heltec_wifi_32_lora_V3)
  #define BOARD_VARIANT "heltec_heltec_wifi_lora_32_V3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_heltec_wireless_stick)
  #define BOARD_VARIANT "heltec_wireless_stick"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_heltec_wireless_stick_LITE)
  #define BOARD_VARIANT "heltec_wireless_stick_lite"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESPECTRO32)
  #define BOARD_VARIANT "espectro32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_CoreESP32)
  #define BOARD_VARIANT "Microduino-esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ALKS)
  #define BOARD_VARIANT "alksesp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_WIPY3)
  #define BOARD_VARIANT "wipy3"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_WT32_ETH01)
  #define BOARD_VARIANT "wt32-eth01"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_BPI_BIT)
  #define BOARD_VARIANT "bpi-bit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_BPI_LEAF_S3)
  #define BOARD_VARIANT "bpi_leaf_s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_WESP32)
  #define BOARD_VARIANT "wesp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_TBeam)
  #define BOARD_VARIANT "tbeam"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_D_Duino_32)
  #define BOARD_VARIANT "d-duino-32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_LoPy)
  #define BOARD_VARIANT "lopy"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_LoPy4)
  #define BOARD_VARIANT "lopy4"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_OROCA_EDUBOT)
  #define BOARD_VARIANT "oroca_edubot"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_fm_devkit)
  #define BOARD_VARIANT "fm-devkit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_FROG_ESP32)
  #define BOARD_VARIANT "frog32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_TWatch)
  #define BOARD_VARIANT "twatch"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_D1_MINI32)
  #define BOARD_VARIANT "d1_mini32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_D1_UNO32)
  #define BOARD_VARIANT "d1_uno32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_PYCOM_GPY)
  #define BOARD_VARIANT "gpy"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "vintlabsdevkitv1"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_HONEYLEMON)
  #define BOARD_VARIANT "honeylemon"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_MGBOT_IOTIK32A)
  #define BOARD_VARIANT "mgbot-iotik32a"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_MGBOT_IOTIK32B)
  #define BOARD_VARIANT "mgbot-iotik32b"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Piranha)
  #define BOARD_VARIANT "piranha_esp-32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Metro)
  #define BOARD_VARIANT "metro_esp-32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_sensesiot_weizen)
  #define BOARD_VARIANT "esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_PICO)
  #define BOARD_VARIANT "pico32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "mpython"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_openkb)
  #define BOARD_VARIANT "openkb"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Wifiduino32)
  #define BOARD_VARIANT "wifiduino32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_WiFiduinoV2)
  #define BOARD_VARIANT "wifiduinov2"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_WiFiduino32S3)
  #define BOARD_VARIANT "wifiduino32s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_IMBRIOS_LOGSENS_V1P1)
  #define BOARD_VARIANT "imbrios-logsens-v1p1"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_HEALTHYPI_4)
  #define BOARD_VARIANT "healthypi4"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ET_Board)
  #define BOARD_VARIANT "ET-Board"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_DENKY)
  #define BOARD_VARIANT "ch_denky"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_uPesy_WROVER)
  #define BOARD_VARIANT "uPesy_esp32_wrover_devkit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_uPesy_WROOM)
  #define BOARD_VARIANT "uPesy_esp32_wroom_devkit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_DEV)
  #define BOARD_VARIANT "esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_DYDK)
  #define BOARD_VARIANT "deneyapkart"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_DYDK1A)
  #define BOARD_VARIANT "deneyapkart1A"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_DYDK1Av2)
  #define BOARD_VARIANT "deneyapkart1Av2"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_DYM)
  #define BOARD_VARIANT "deneyapmini"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_DYMv2)
  #define BOARD_VARIANT "deneyapminiv2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_DYG)
  #define BOARD_VARIANT "deneyapkartg"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_Trueverit_ESP32_Universal_IoT_Driver)
  #define BOARD_VARIANT "esp32-trueverit-iot-driver"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Trueverit_ESP32_Universal_IoT_Driver_MK_II)
  #define BOARD_VARIANT "esp32-trueverit-iot-driver-mkii"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_atmegazero_esp32s2)
  #define BOARD_VARIANT "atmegazero_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_FRANZININHO_WIFI)
  #define BOARD_VARIANT "franzininho_wifi_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_FRANZININHO_WIFI_MSC)
  #define BOARD_VARIANT "franzininho_wifi_msc_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_TAMC_TERMOD_S3)
  #define BOARD_VARIANT "tamc_termod_s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_DPU_ESP32)
  #define BOARD_VARIANT "dpu_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_SONOFF_DUALR3)
  #define BOARD_VARIANT "esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_Lion \
              : Bit_Dev_Board)
  #define BOARD_VARIANT "lionbit"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_WATCHY)
  #define BOARD_VARIANT "watchy"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_AirM2M_CORE_ESP32C3)
  #define BOARD_VARIANT "AirM2M_CORE_ESP32C3"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_XIAO_ESP32C3)
  #define BOARD_VARIANT "XIAO_ESP32C3"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_connaxio_espoir)
  #define BOARD_VARIANT "connaxio_espoir"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_ESP32_PICO)
  #define BOARD_VARIANT "cnrs_aw2eth"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_DEPARTMENT_OF_ALCHEMY_MINIMAIN_ESP32S2)
  #define BOARD_VARIANT "department_of_alchemy_minimain_esp32s2"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_BeeMotionS3)
  #define BOARD_VARIANT "Bee_Motion_S3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_Bee_Motion)
  #define BOARD_VARIANT "Bee_Motion"
  #define BOARD_MCU "esp32s2"
#elif defined(ARDUINO_Bee_Motion_Mini)
  #define BOARD_VARIANT "Bee_Motion_Mini"
  #define BOARD_MCU "esp32c3"
#elif defined(ARDUINO_Bee_S3)
  #define BOARD_VARIANT "Bee_S3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_FEATHER_ESP32)
  #define BOARD_VARIANT "feather_esp32"
  #define BOARD_MCU "esp32"
#elif defined(ARDUINO_unphone8)
  #define BOARD_VARIANT "unphone8"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_unphone9)
  #define BOARD_VARIANT "unphone9"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_CYTRON_MAKER_FEATHER_AIOT_S3)
  #define BOARD_VARIANT "cytron_maker_feather_aiot_s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_REDPILL_ESP32S3)
  #define BOARD_VARIANT "redpill_esp32s3"
  #define BOARD_MCU "esp32s3"
#elif defined(ARDUINO_ESP32C3_M1_I_KIT)
  #define BOARD_VARIANT "esp_c3_m1_i_kit"
  #define BOARD_MCU "esp32c3"
#else
  #define BOARD_VARIANT "undef"
  #define BOARD_MCU "undef"
#endif
#endif
