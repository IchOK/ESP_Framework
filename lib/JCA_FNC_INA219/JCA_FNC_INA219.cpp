/**
 * @file JCA_FNC_INA219.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to get Data from INA219 Sensor
 * @version 0.1
 * @date 2023-03-07
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#include <JCA_FNC_INA219.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    /**
     * @brief Construct a new INA219::INA219 object
     *
     * @param _Wire Pointer to the TwoWire Interface
     * @param _Addr Address of the INA219 Sensor
     * @param _Name Element Name inside the Communication
     */
    INA219::INA219 (TwoWire *_Wire, const uint8_t _Addr, const char *_Name)
        : FuncParent (_Name), Sensor (_Wire, _Addr) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      createTags();

      ReadInterval = 1;
    }
    /**
     * @brief Construct a new INA219::INA219 object
     *  Use default TwoWire-Interface
     * @param _Addr Address of the INA219 Sensor
     * @param _Name Element Name inside the Communication
     */
    INA219::INA219 (const uint8_t _Addr, const char *_Name)
        : FuncParent (_Name), Sensor (_Addr) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      createTags();

      ReadInterval = 1;
    }
    /**
     * @brief Construct a new INA219::INA219 object
     *  Use default TwoWire-Interface
     *  Use default Sensor-Address 0x40
     * @param _Name Element Name inside the Communication
     */
    INA219::INA219 (const char *_Name)
        : FuncParent (_Name), Sensor () {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      createTags();

      ReadInterval = 1;
    }

    void INA219::createTags() {
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("ReadInterval", "Leseintervall", "", false, TagUsage_T::UseConfig, &ReadInterval, "s"));

      Tags.push_back (new TagFloat ("PowerPlus", "Leistung Eingang", "", true, TagUsage_T::UseConfig, &PowerPlus, "W"));
      Tags.push_back (new TagFloat ("VoltagePlus", "Spannung Eingang", "", true, TagUsage_T::UseConfig, &VoltagePlus, "V"));
      Tags.push_back (new TagFloat ("PowerMinus", "Leistung Ausgang", "", true, TagUsage_T::UseConfig, &PowerMinus, "W"));
      Tags.push_back (new TagFloat ("VoltageMinus", "Spannung Ausgang", "", true, TagUsage_T::UseConfig, &VoltageMinus, "V"));
      Tags.push_back (new TagFloat ("Current", "Strom", "", true, TagUsage_T::UseConfig, &Current, "A"));
    }

    /**
     * @brief Init the Sensor
     */
    bool INA219::init () {
      LastMillis = millis ();
      UpdateMillis = 0;
      if (Sensor.init ()) {
        Sensor.setADCMode(SAMPLE_MODE_64);
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Handling INA219-Sensor
     * Read and Skale the Temperature
     * @param time Current Time to check the Samplerate
     */
    void INA219::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      // Get Update Intervall
      unsigned long ActMillis = millis ();
      UpdateMillis += (ActMillis - LastMillis);
      LastMillis = ActMillis;

      if (UpdateMillis >= ReadInterval) {
        // Read Values from Sensor
        ShuntVoltage_mV = Sensor.getShuntVoltage_mV ();
        BusVoltage_V = Sensor.getBusVoltage_V ();
        Current_mA = Sensor.getCurrent_mA ();

        // Calc internal Values
        VoltagePlus = BusVoltage_V + ShuntVoltage_mV / 1000.0;
        VoltageMinus = BusVoltage_V;
        Current = Current_mA / 1000.0;
        PowerPlus = VoltagePlus * Current;
        PowerMinus = VoltageMinus * Current;
        if (Debug.print (FLAG_DATA, false, Name, __func__, "Values:")) {
          Debug.println (FLAG_DATA, false, Name, __func__, Sensor.getOverflow ());
          Debug.print (FLAG_DATA, false, Name, __func__, " - Shunt Voltage[mV]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, ShuntVoltage_mV);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Bus Voltage   [V]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, BusVoltage_V);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Current      [mA]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, Current_mA);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Voltage Plus  [V]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, VoltagePlus);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Voltage Minus [V]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, VoltageMinus);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Current       [A]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, Current);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Power Plus    [W]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, PowerPlus);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Power Minus   [W]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, PowerMinus);
        }
        UpdateMillis = 0;
      }
    }

    /**
     * @brief Set the Update Interval to sync with other Elements
     * 
     * @param _ReadInterval new Update Interval
     */
    void INA219::setInterval (uint16_t _ReadInterval) {
      ReadInterval = _ReadInterval;
    }

    /**
     * @brief Get the scaled INA219 PowerPlus
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current INA219 PowerPlus [W]
     */
    float INA219::getPowerPlus () {
      return PowerPlus;
    }

    /**
     * @brief Get the scaled INA219 VoltagePlus
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current INA219 VoltagePlus [V]
     */
    float INA219::getVoltagePlus () {
      return VoltagePlus;
    }

    /**
     * @brief Get the scaled INA219 PowerMinus
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current INA219 PowerMinus [W]
     */
    float INA219::getPowerMinus () {
      return PowerMinus;
    }

    /**
     * @brief Get the scaled INA219 VoltageMinus
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current INA219 VoltageMinus [V]
     */
    float INA219::getVoltageMinus () {
      return VoltageMinus;
    }

    /**
     * @brief Get the scaled INA219 Current
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current INA219 Current [A]
     */
    float INA219::getCurrent () {
      return Current;
    }
  }
}
