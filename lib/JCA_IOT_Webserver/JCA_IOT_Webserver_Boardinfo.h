/**
 * @file JCA_IOT_Webserver_Boardinfo.h
 * @author JCA (https://github.com/ichok)
 * @brief Selection of Boardtypes
 * Contains Variant and MCU Type.
 * Compatible with Boards.txt Revision 3.0.1
 * @version 0.1
 * @date 2022-09-04
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
  #define BOARD_VARIANT "generic"
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
#elif defined(ARDUINO_ESP8266_OAK)
  #define BOARD_VARIANT "oak"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_ESP13)
  #define BOARD_VARIANT "ESPDuinoeric"
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
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINILITE)
  #define BOARD_VARIANT "d1_mini"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINIPRO)
  #define BOARD_VARIANT "d1_mini"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_WEMOS_D1R1)
  #define BOARD_VARIANT "di"
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
#elif defined(ARDUINO_WIFIDUINO_ESP8266)
  #define BOARD_VARIANT "wifiduino"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_WIFINFO)
  #define BOARD_VARIANT "wifinfo"
  #define BOARD_MCU "esp8266"
#elif defined(ARDUINO_ESP8266_XINABOX_CW01)
  #define BOARD_VARIANT "xinabox"
  #define BOARD_MCU "esp8266"
#else
  #define BOARD_VARIANT "undef"
  #define BOARD_MCU "undef"
#endif
#endif