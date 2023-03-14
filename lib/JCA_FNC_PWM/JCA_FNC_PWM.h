#if (defined(CONFIG_IDF_TARGET_ESP32))
const uint64_t pinMask = 0X18777F81F; // pwm pins
const uint8_t chMax = 16;
const uint8_t widthMax = 20;
#elif (defined(CONFIG_IDF_TARGET_ESP32S2))
const uint64_t pinMask = 0xF; // ANY GPIO
const uint8_t chMax = 8;
const uint8_t widthMax = 14;
#elif (defined(CONFIG_IDF_TARGET_ESP32S3))
const uint64_t pinMask = 0xF; // pwm pins
const uint8_t chMax = 8;
const uint8_t widthMax = 14;
#elif (defined(CONFIG_IDF_TARGET_ESP32C2)) //ESP8684
const uint64_t pinMask = 0xC03FF; // pwm pins
const uint8_t chMax = 6;
const uint8_t widthMax = 18;
#elif (defined(CONFIG_IDF_TARGET_ESP32C3)) //ESP8685
const uint64_t pinMask = 0xC03FF; // pwm pins
const uint8_t chMax = 6;
const uint8_t widthMax = 18;
#elif (defined(CONFIG_IDF_TARGET_ESP32C6))
const uint64_t pinMask = 0xC03FF; // pwm pins
const uint8_t chMax = 6;
const uint8_t widthMax = 20;
#endif
