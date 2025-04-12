/**
 * @file JCA_SYS_DebugOut.h
 * @author JCA (https://github.com/ichok)
 * @brief The DebugOut Class is used to handle Debug-Prints,
 * Serial prints for the selected Debug-Level.
 * It's declerated as `extern DebugOut Debug(Serial)` to use in all other Parts of the JCA::IOT Namespace
 * @version 0.1
 * @date 2022-09-04
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_IO_DEBUGOUT_
#define _JCA_IO_DEBUGOUT_
#include <Arduino.h>
/**
 * @brief
 * Default Baud rate if not defined on init.
 */
#define JCA_SYS_DebugOut_DEFAULT_BAUD 74880

namespace JCA {
  namespace SYS {
    /**
     * @brief
     * Flags for define Debug-Message Level
     */
    enum DEBUGOUT_FLAGS : uint16_t {
      FLAG_NONE = 0x0,      ///< Don't send any Messages
      FLAG_ERROR = 0x1,     ///< Error Message
      FLAG_SETUP = 0x2,     ///< From Setup or Init Functions
      FLAG_CONFIG = 0x4,    ///< Informations about (Re-)Configuraton
      FLAG_TRAFFIC = 0x8,   ///< Informations about Network traffic or data
      FLAG_LOOP = 0x10,     ///< Loop Informations, like readen Values or Counter (not recomended)
      FLAG_PROTOCOL = 0x20, ///< Protocoll Informations from Parent Element (not recomended)
      FLAG_DATA = 0x40      ///< Data, readen from Sensors or calculated
    };
    /**
     * @brief
     * Generate and output Debug-Messages to Serial interface
     */
    class DebugOut {
    private:
      const char *ObjectName = "DebugOut";
      uint16_t Flags;
      HardwareSerial DebugSerial;
      bool NewLine;
      String getPrefix (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function);

    public:
      DebugOut (const HardwareSerial &_Serial);
      ~DebugOut ();

      void init (uint16_t _Flags, unsigned long _Baud);
      void init (uint16_t _Flags);

      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const Printable &_Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const Printable &_Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const String &_Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const String &_Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const char *_Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const char *_Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, double _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, double _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long long _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long long _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long long _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long long _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned int _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned int _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, int _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, int _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned char _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned char _Message);
      bool println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, char _Message);
      bool print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, char _Message);
    };

    extern DebugOut Debug;
  }
}

#endif