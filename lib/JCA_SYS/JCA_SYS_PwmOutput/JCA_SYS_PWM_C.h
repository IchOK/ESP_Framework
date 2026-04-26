
#ifndef _JCA_SYS_PWM_C_
#define _JCA_SYS_PWM_C_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef ESP32
#include "driver/ledc.h"
#include "soc/soc_caps.h"
#endif

#ifndef SOC_LEDC_CHANNEL_NUM
  #define SOC_LEDC_CHANNEL_NUM (8)
#endif
#ifndef SOC_LEDC_TIMER_BIT_WIDE_NUM
  #define SOC_LEDC_TIMER_BIT_WIDE_NUM (20)
#endif

#ifdef SOC_LEDC_SUPPORT_HS_MODE
  #define JCA_SYS_PWM_CHANNELS (SOC_LEDC_CHANNEL_NUM << 1)
#else
  #define JCA_SYS_PWM_CHANNELS (SOC_LEDC_CHANNEL_NUM)
#endif
#ifdef SOC_LEDC_SUPPORT_XTAL_CLOCK
  #define JCA_SYS_PWM_DEFAULT_CLK LEDC_USE_XTAL_CLK
#elif defined(LEDC_AUTO_CLK)
  #define JCA_SYS_PWM_DEFAULT_CLK LEDC_AUTO_CLK
#else
  #define JCA_SYS_PWM_DEFAULT_CLK (0)
#endif
#define JCA_SYS_PWM_MAX_BIT_WIDTH SOC_LEDC_TIMER_BIT_WIDE_NUM

bool JCA_SYS_PWM_Setup (uint8_t _Channel, uint32_t _Freq, uint8_t _BitResolution);
bool JCA_SYS_PWM_Attache (uint8_t _Channel, uint8_t _Pin);
bool JCA_SYS_PWM_Write (uint8_t _Channel, uint32_t Duty);

#ifdef __cplusplus
}
#endif

#endif
