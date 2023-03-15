
#include <JCA_SYS_PwmOutput.h>

namespace JCA {
  namespace SYS {
#ifdef ESP32
    const uint8_t PwmOutput::ChannelCount = JCA_SYS_PWM_CHANNELS;
#endif

    /**
     * @brief Construct a new PwmOutput::PwmOutput object
     *
     */
    PwmOutput::PwmOutput () {
#ifdef ESP8266
      ;
#elif ESP32
      for (uint8_t i = 0; i < ChannelCount; i++) {
        Channels[i].Pin = 0;
        Channels[i].Resolution = 0;
      }
#endif
    }

    /**
     * @brief Setup a PWM-Pin if a Channel is available
     *
     * @param _Pin GPIO for PWM output
     * @param _Freq Frequence of the PWM Signal
     * @param _BitResolution Resolution of the PWM-Timer in Bit
     * @return true Setup ended successful
     * @return false Setup failed, Debug Print in Flag ERROR
     */
    bool PwmOutput::setupPin (uint8_t _Pin, uint32_t _Freq, uint8_t _BitResolution) {
#ifdef ESP8266
      Resolution = _BitResolution;
      analogWriteResolution (_BitResolution);
      analogWriteFreq (_Freq);
      analogWrite (_Pin, 0);
      return true;
#elif ESP32
      uint8_t ActChannel = 255;
      uint8_t NewChannel = 255;
      uint8_t PinChannel;

      if (_BitResolution > JCA_SYS_PWM_MAX_BIT_WIDTH) {
        if (Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, "Bit resolution too big. Pin:")) {
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, _Pin);
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, " Resolution:");
          Debug.println (FLAG_ERROR, true, "PwmOutput", __func__, _BitResolution);
        }
        return false;
      }

      // Check if Pin already linked to an Channel or get first unused Channel
      for (uint8_t i = 0; i < ChannelCount; i++) {
        if (Channels[i].Pin == _Pin) {
          ActChannel = i;
        }
        if (Channels[i].Pin == 0 && NewChannel == 255) {
          NewChannel = i;
        }
      }

      // Select PWM Channel
      if (ActChannel != 255) {
        PinChannel = ActChannel;
      } else if (NewChannel != 255) {
        PinChannel = NewChannel;
      } else {
        if (Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, "No more PWM Channel available. Pin:")) {
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, _Pin);
        }
        return false;
      }

      if (!JCA_SYS_PWM_Setup (PinChannel, _Freq, _BitResolution)) {
        if (Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, "Setup LEDC Timer failed. Pin:")) {
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, _Pin);
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, " Channel:");
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, PinChannel);
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, " Frequence:");
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, _Freq);
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, " Resolution:");
          Debug.println (FLAG_ERROR, true, "PwmOutput", __func__, _BitResolution);
        }
        return false;
      }

      if (!JCA_SYS_PWM_Attache (PinChannel, _Pin)) {
        if (Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, "Setup LEDC Channel failed. Pin:")) {
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, _Pin);
          Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, " Channel:");
          Debug.println (FLAG_ERROR, true, "PwmOutput", __func__, PinChannel);
        }
        return false;
      }

      // PWM-Resolution set an Pin is linked to Channel
      Channels[PinChannel].Resolution = _BitResolution;
      Channels[PinChannel].Pin = _Pin;
      return true;
#endif
    }

    /**
     * @brief Write a PWM-Duty to the Output-Pin
     *
     * @param _Pin GPIO for Output
     * @param _Value Duty in % [0-100]
     * @return true Write ended successful
     * @return false Write failed, Debug Print on Flag ERROR
     */
    bool PwmOutput::writePin (uint8_t _Pin, float _Value) {
#ifdef ESP8266
      uint32_t Duty;
      if (_Value <= 0.0) {
        Duty = 0;
      } else if (_Value >= 100.0) {
        Duty = (1 << Resolution);
      } else {
        Duty = (uint32_t)((float)((1 << Resolution) - 1) * _Value / 100.0);
      }
      analogWrite (_Pin, Duty);
      return true;

#elif ESP32
      uint8_t PinChannel = 255;
      uint32_t Duty;

      // Check if Pin already linked to an Channel or get first unused Channel
      for (uint8_t i = 0; i < ChannelCount; i++) {
        if (Channels[i].Pin == _Pin) {
          PinChannel = i;
          break;
        }
      }
      if (PinChannel == 255) {
        if (Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, "No Channel Setup found. Pin:")) {
          Debug.println (FLAG_ERROR, true, "PwmOutput", __func__, _Pin);
        }
        return false;
      }

      // Set PWM duty
      if (_Value <= 0.0) {
        Duty = 0;
      } else if (_Value >= 100.0) {
        Duty = (1 << Channels[PinChannel].Resolution);
      } else {
        Duty = (uint32_t)((float)((1 << Channels[PinChannel].Resolution) - 1) * _Value / 100.0);
      }

      if (!JCA_SYS_PWM_Write (PinChannel, Duty)) {
        if (Debug.print (FLAG_ERROR, true, "PwmOutput", __func__, "Set Channel-Duty failed. Pin:")) {
          Debug.println (FLAG_ERROR, true, "PwmOutput", __func__, _Pin);
        }
        return false;
      }
      return true;
#endif
    }
  }
}