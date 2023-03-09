/**
 * @file JCA_SYS_DebugOut.cpp
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

#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace SYS {
    /**
     * @brief Construct a new DebugOut::DebugOut object
     *
     * @param _Serial The Serial-Interface to use for Debug output
     */
    DebugOut::DebugOut (const HardwareSerial &_Serial) : DebugSerial (_Serial) {
      NewLine = true;
    }

    /**
     * @brief Destroy the DebugOut::DebugOut object
     *
     */
    DebugOut::~DebugOut () {
    }

    String DebugOut::getPrefix (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function) {
      String RetVal = "";
      if (NewLine) {
        switch (_Flag) {
        case FLAG_ERROR:
          RetVal += "[ERROR] ";
          break;

        case FLAG_SETUP:
          RetVal += "[SETUP] ";
          break;

        case FLAG_CONFIG:
          RetVal += "[CONFIG] ";
          break;

        case FLAG_TRAFFIC:
          RetVal += "[TRAFFIC] ";
          break;

        case FLAG_LOOP:
          RetVal += "[LOOP] ";
          break;

        default:
          RetVal += "[---]";
          break;
        }
        if (_Framework) {
          RetVal += "JCA::";
        }
        RetVal += String (_ElementName) + String ("::") + String (_Function) + String (" - ");
        NewLine = false;
      }
      return RetVal;
    }

    /**
     * @brief Initalises the Debug-Level and Debug-Interface
     *
     * @param _Flags Select the diffrent Levels of Debug-Output
     * @param _Baud Baud rate of Serial interface
     */
    void DebugOut::init (uint16_t _Flags, unsigned long _Baud) {
      Flags = _Flags;
      if (Flags > 0) {
        DebugSerial.begin (_Baud);
        println (FLAG_SETUP, true, ObjectName, __func__, "Serial stated");
      }
    }

    /**
     * @brief Initalises the Debug-Level with Default Baud rate
     *
     * @param _Flags Select the diffrent Levels of Debug-Output
     */
    void DebugOut::init (uint16_t _Flags) {
      init (_Flags, JCA_SYS_DebugOut_DEFAULT_BAUD);
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const Printable &_Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const Printable &_Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const String &_Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const String &_Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const char *_Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, const char *_Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, double _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, double _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long long _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long long _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long long _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long long _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned long _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, long _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned int _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned int _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, int _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, int _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned char _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, unsigned char _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, without line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::print (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, char _Message) {
      if (_Flag & Flags) {
        DebugSerial.print (getPrefix (_Flag, _Framework, _ElementName, _Function));
        DebugSerial.print (_Message);
        return true;
      } else {
        return false;
      }
    }
    /**
     * @brief Generate a Debug-Output depends on the selected Falgs on init, with line break.
     * "[JCA::IOT::]Object::Function - Message"
     *
     * @param _Flag Flag for Debug Message, only Output if Flag set on init
     * @param _Framework use from Framework, add the Namespace tp Prefix
     * @param _ElementName Name of the Object or Function-Group that calls the Function
     * @param _Function Name of the Function
     * @param _Message Message Text
     * @return true Message was output to Debug-Interface
     * @return false Message not output to Debug-Interface
     */
    bool DebugOut::println (DEBUGOUT_FLAGS _Flag, bool _Framework, String _ElementName, const char *_Function, char _Message) {
      if (print (_Flag, _Framework, _ElementName, _Function, _Message)) {
        DebugSerial.println ();
        NewLine = true;
        return true;
      } else {
        return false;
      }
    }

    DebugOut Debug (Serial);
  }
}
