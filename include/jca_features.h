/**
 * @file jca_features.h
 * @brief JCA framework feature flags (set via platformio build_flags -D...)
 *
 * Module exclusion:  -DJCA_IGNORE_<MODULE>
 * Link exclusion:    -DJCA_IGNORE_LINK_<TYPE>
 * Release trims:     -DJCA_DEBUG_DISABLED, -DJCA_NO_SETUP_LOG
 * Json pool (bytes): -DARDUINOJSON_POOL_SIZE=2048
 */

#ifndef JCA_FEATURES_H
#define JCA_FEATURES_H

/* Function modules */
#ifdef JCA_IGNORE_ACDIMMERS
  #define JCA_FNC_ACDIMMERS 0
#else
  #define JCA_FNC_ACDIMMERS 1
#endif

#ifdef JCA_IGNORE_CHARGER
  #define JCA_FNC_CHARGER 0
#else
  #define JCA_FNC_CHARGER 1
#endif

#ifdef JCA_IGNORE_CLOCKVALUES
  #define JCA_FNC_CLOCKVALUES 0
#else
  #define JCA_FNC_CLOCKVALUES 1
#endif

#ifdef JCA_IGNORE_DAYSELECT
  #define JCA_FNC_DAYSELECT 0
#else
  #define JCA_FNC_DAYSELECT 1
#endif

#ifdef JCA_IGNORE_DIGITALIN
  #define JCA_FNC_DIGITALIN 0
#else
  #define JCA_FNC_DIGITALIN 1
#endif

#ifdef JCA_IGNORE_DIGITALOUT
  #define JCA_FNC_DIGITALOUT 0
#else
  #define JCA_FNC_DIGITALOUT 1
#endif

#ifdef JCA_IGNORE_DS18B20
  #define JCA_FNC_DS18B20 0
#else
  #define JCA_FNC_DS18B20 1
#endif

#ifdef JCA_IGNORE_FEEDER
  #define JCA_FNC_FEEDER 0
#else
  #define JCA_FNC_FEEDER 1
#endif

#ifdef JCA_IGNORE_INA219
  #define JCA_FNC_INA219 0
#else
  #define JCA_FNC_INA219 1
#endif

#ifdef JCA_IGNORE_LEDSTRIP
  #define JCA_FNC_LEDSTRIP 0
#else
  #define JCA_FNC_LEDSTRIP 1
#endif

#ifdef JCA_IGNORE_LEVEL
  #define JCA_FNC_LEVEL 0
#else
  #define JCA_FNC_LEVEL 1
#endif

#ifdef JCA_IGNORE_MULTISWITCHCONTROLLER
  #define JCA_FNC_MULTISWITCHCONTROLLER 0
#else
  #define JCA_FNC_MULTISWITCHCONTROLLER 1
#endif

#ifdef JCA_IGNORE_OLED128X64
  #define JCA_FNC_OLED128X64 0
#else
  #define JCA_FNC_OLED128X64 1
#endif

#ifdef JCA_IGNORE_PIDCONTROLLER
  #define JCA_FNC_PIDCONTROLLER 0
#else
  #define JCA_FNC_PIDCONTROLLER 1
#endif

#ifdef JCA_IGNORE_SERVERLINK
  #define JCA_FNC_SERVERLINK 0
#else
  #define JCA_FNC_SERVERLINK 1
#endif

#ifdef JCA_IGNORE_VALUEANALOG
  #define JCA_FNC_VALUEANALOG 0
#else
  #define JCA_FNC_VALUEANALOG 1
#endif

#ifdef JCA_IGNORE_VALUEDIGITAL
  #define JCA_FNC_VALUEDIGITAL 0
#else
  #define JCA_FNC_VALUEDIGITAL 1
#endif

#ifdef JCA_IGNORE_VALVE2DPOSIMP
  #define JCA_FNC_VALVE2DPOSIMP 0
#else
  #define JCA_FNC_VALVE2DPOSIMP 1
#endif

#ifdef JCA_IGNORE_BUTTONVALUE
  #define JCA_FNC_BUTTONVALUE 0
#else
  #define JCA_FNC_BUTTONVALUE 1
#endif

/* Link types */
#ifdef JCA_IGNORE_LINK_ADD
  #define JCA_LNK_ADD 0
#else
  #define JCA_LNK_ADD 1
#endif

#ifdef JCA_IGNORE_LINK_AND
  #define JCA_LNK_AND 0
#else
  #define JCA_LNK_AND 1
#endif

#ifdef JCA_IGNORE_LINK_DIRECT
  #define JCA_LNK_DIRECT 0
#else
  #define JCA_LNK_DIRECT 1
#endif

#ifdef JCA_IGNORE_LINK_MOVE
  #define JCA_LNK_MOVE 0
#else
  #define JCA_LNK_MOVE 1
#endif

#ifdef JCA_IGNORE_LINK_NOT
  #define JCA_LNK_NOT 0
#else
  #define JCA_LNK_NOT 1
#endif

#ifdef JCA_IGNORE_LINK_OR
  #define JCA_LNK_OR 0
#else
  #define JCA_LNK_OR 1
#endif

#endif
