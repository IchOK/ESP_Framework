
#ifdef ESP32
#include <JCA_SYS_PWM_C.h>
#include "driver/ledc.h"
#include "esp32-hal.h"
#include "soc/soc_caps.h"

bool JCA_SYS_PWM_Setup (uint8_t _Channel, uint32_t _Freq, uint8_t _BitResolution) {
  uint8_t Group = (_Channel / 8);
  uint8_t Timer = ((_Channel / 2) % 4);

  // Setup PWM Channel
  ledc_timer_config_t ledc_timer = {
    .speed_mode = Group,
    .duty_resolution = (ledc_timer_bit_t)_BitResolution,
    .timer_num = Timer,
    .freq_hz = _Freq,
    .clk_cfg = JCA_SYS_PWM_DEFAULT_CLK
  };
  if (ledc_timer_config (&ledc_timer) == ESP_OK) {
    return true;
  } else {
    return false;
  }
}

bool JCA_SYS_PWM_Attache (uint8_t _Channel, uint8_t _Pin) {
  uint8_t Group = (_Channel / 8);
  uint8_t Channel = (_Channel % 8);
  uint8_t Timer = ((_Channel / 2) % 4);

  // Attach PWm Pin to Channel
  ledc_channel_config_t ledc_channel = {
    .gpio_num = (int)_Pin,
    .speed_mode = Group,
    .channel = Channel,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = Timer,
    .duty = 0,
    .hpoint = 0
  };
  if (ledc_channel_config (&ledc_channel) == ESP_OK) {
    return true;
  } else {
    return false;
  }
}

bool JCA_SYS_PWM_Write (uint8_t _Channel, uint32_t Duty) {
  uint8_t Group = (_Channel / 8);
  uint8_t Channel = (_Channel % 8);
  if (ledc_set_duty (Group, Channel, Duty) != ESP_OK) {
    return false;
  }
  if (ledc_update_duty (Group, Channel) != ESP_OK) {
    return false;
  }
  return true;
}

#endif