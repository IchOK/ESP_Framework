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

namespace JCA {
  namespace FNC {
    /* #region(collapsed) Datapoint description */
    const char *INA219::ReadInterval_Name = "ReadInterval";
    const char *INA219::ReadInterval_Text = "Leseintervall";
    const char *INA219::ReadInterval_Unit = "s";
    const char *INA219::ReadInterval_Comment = nullptr;
    const char *INA219::PowerPlus_Name = "PowerPlus";
    const char *INA219::PowerPlus_Text = "Leistung Eingang";
    const char *INA219::PowerPlus_Unit = "W";
    const char *INA219::PowerPlus_Comment = nullptr;
    const char *INA219::VoltagePlus_Name = "VoltagePlus";
    const char *INA219::VoltagePlus_Text = "Spannung Eingang";
    const char *INA219::VoltagePlus_Unit = "V";
    const char *INA219::VoltagePlus_Comment = nullptr;
    const char *INA219::PowerMinus_Name = "PowerMinus";
    const char *INA219::PowerMinus_Text = "Leistung Ausgang";
    const char *INA219::PowerMinus_Unit = "W";
    const char *INA219::PowerMinus_Comment = nullptr;
    const char *INA219::VoltageMinus_Name = "VoltageMinus";
    const char *INA219::VoltageMinus_Text = "Spannung Ausgang";
    const char *INA219::VoltageMinus_Unit = "C";
    const char *INA219::VoltageMinus_Comment = nullptr;
    const char *INA219::Current_Name = "Current";
    const char *INA219::Current_Text = "Strom";
    const char *INA219::Current_Unit = "A";
    const char *INA219::Current_Comment = nullptr;
    /* #endregion */

    /**
     * @brief Construct a new INA219::INA219 object
     *
     * @param _Wire Pointer to the TwoWire Interface
     * @param _Addr Address of the INA219 Sensor
     * @param _Name Element Name inside the Communication
     */
    INA219::INA219 (TwoWire *_Wire, const uint8_t _Addr, const char *_Name)
        : Parent (_Name), Sensor (_Wire, _Addr) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      ReadInterval = 1;
    }
    /**
     * @brief Construct a new INA219::INA219 object
     *  Use default TwoWire-Interface
     * @param _Addr Address of the INA219 Sensor
     * @param _Name Element Name inside the Communication
     */
    INA219::INA219 (const uint8_t _Addr, const char *_Name)
        : Parent (_Name), Sensor (_Addr) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      ReadInterval = 1;
    }
    /**
     * @brief Construct a new INA219::INA219 object
     *  Use default TwoWire-Interface
     *  Use default Sensor-Address 0x40
     * @param _Name Element Name inside the Communication
     */
    INA219::INA219 (const char *_Name)
        : Parent (_Name), Sensor () {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      ReadInterval = 1;
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void INA219::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[ReadInterval_Name] = ReadInterval;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void INA219::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[PowerPlus_Name] = PowerPlus;
      _Values[VoltagePlus_Name] = VoltagePlus;
      _Values[PowerMinus_Name] = PowerMinus;
      _Values[VoltageMinus_Name] = VoltageMinus;
      _Values[Current_Name] = Current;
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void INA219::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == ReadInterval_Name) {
          ReadInterval = Tag[JsonTagValue].as<uint16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ReadInterval_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ReadInterval);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void INA219::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Execute the Commands
     * currently not used
     * @param _Tags Array of Commands ("cmd": [])
     */
    void INA219::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void INA219::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (ReadInterval_Name, ReadInterval_Text, ReadInterval_Comment, false, ReadInterval_Unit, ReadInterval) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void INA219::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (PowerPlus_Name, PowerPlus_Text, PowerPlus_Comment, true, PowerPlus_Unit, PowerPlus) + "}");
      _SetupFile.println (",{" + createSetupTag (VoltagePlus_Name, VoltagePlus_Text, VoltagePlus_Comment, true, VoltagePlus_Unit, VoltagePlus) + "}");
      _SetupFile.println (",{" + createSetupTag (PowerMinus_Name, PowerMinus_Text, PowerMinus_Comment, true, PowerMinus_Unit, PowerMinus) + "}");
      _SetupFile.println (",{" + createSetupTag (VoltageMinus_Name, VoltageMinus_Text, VoltageMinus_Comment, true, VoltageMinus_Unit, VoltageMinus) + "}");
      _SetupFile.println (",{" + createSetupTag (Current_Name, Current_Text, Current_Comment, true, Current_Unit, Current) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void INA219::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Init the Sensor
     */
    bool INA219::init () {
      LastMillis = millis ();
      UpdateMillis = 0;
      if (Sensor.init ()) {
        Sensor.setADCMode (SAMPLE_MODE_64);
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
      uint32_t ActMillis = millis ();
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
