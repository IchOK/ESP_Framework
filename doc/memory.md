# JCA Framework – Speicher / Build-Profile

## Flash-Ziel

| Parameter | Wert |
|-----------|------|
| OTA App-Slot (`ota_dual_app.csv`) | 1 572 864 Bytes (0x180000) |
| Empfohlenes Maximum Firmware | 1 416 960 Bytes (~1,35 MiB) |
| Ziel-OTA-Reserve | ~150 KB |

Nach jedem Build: `build_size_<env>.txt` im Projektroot (via `create_build_size.py`).

## Module ausschließen (`JCA_IGNORE_*`)

| Build-Flag | Bibliothek (lib_ignore) | Typische externe lib_deps |
|------------|-------------------------|---------------------------|
| `JCA_IGNORE_ACDIMMERS` | JCA_FNC_AcDimmers | — |
| `JCA_IGNORE_CHARGER` | JCA_FNC_Charger | MAX6675, LiquidCrystal |
| `JCA_IGNORE_CLOCKVALUES` | JCA_FNC_ClockValues | — |
| `JCA_IGNORE_DAYSELECT` | JCA_FNC_DaySelect | — |
| `JCA_IGNORE_DIGITALIN` | JCA_FNC_DigitalIn | — |
| `JCA_IGNORE_DIGITALOUT` | JCA_FNC_DigitalOut | — |
| `JCA_IGNORE_DS18B20` | JCA_FNC_DS18B20 | OneWire |
| `JCA_IGNORE_FEEDER` | JCA_FNC_Feeder | AccelStepper |
| `JCA_IGNORE_INA219` | JCA_FNC_INA219 | INA219_WE |
| `JCA_IGNORE_LEDSTRIP` | JCA_FNC_LedStrip | NeoPixel |
| `JCA_IGNORE_LEVEL` | JCA_FNC_Level | — |
| `JCA_IGNORE_MULTISWITCHCONTROLLER` | JCA_FNC_MultiSwitchController | — |
| `JCA_IGNORE_OLED128X64` | JCA_FNC_OLED128x64 | Adafruit SSD1306 |
| `JCA_IGNORE_PIDCONTROLLER` | JCA_FNC_PIDController | — |
| `JCA_IGNORE_SERVERLINK` | JCA_FNC_ServerLink | — |
| `JCA_IGNORE_VALUEANALOG` | JCA_FNC_ValueAnalog | — |
| `JCA_IGNORE_VALUEDIGITAL` | JCA_FNC_ValueDigital | — |
| `JCA_IGNORE_VALVE2DPOSIMP` | JCA_FNC_Valve2DPosImp | AccelStepper |
| `JCA_IGNORE_BUTTONVALUE` | JCA_FNC_ButtonValue | — |

## Link-Typen ausschließen (`JCA_IGNORE_LINK_*`)

| Build-Flag | Quelle |
|------------|--------|
| `JCA_IGNORE_LINK_ADD` | JCA_LNK_LinkAdd.cpp |
| `JCA_IGNORE_LINK_AND` | JCA_LNK_LinkAnd.cpp |
| `JCA_IGNORE_LINK_DIRECT` | JCA_LNK_LinkDirect.cpp |
| `JCA_IGNORE_LINK_MOVE` | JCA_LNK_LinkMove.cpp |
| `JCA_IGNORE_LINK_NOT` | JCA_LNK_LinkNot.cpp |
| `JCA_IGNORE_LINK_OR` | JCA_LNK_LinkOr.cpp |

## Release-Flags

| Flag | Wirkung |
|------|---------|
| `-Os` | Größen-Optimierung |
| `-DJCA_DEBUG_DISABLED` | DebugOut-Stubs (kein Serial-Log-Code) |
| `-DJCA_NO_SETUP_LOG` | Kein `usrLog.json`, kein zweites Setup-Json |
| `-DARDUINOJSON_POOL_SIZE=2048` | Kleinerer ArduinoJson-Pool |

## Beispiel: Cooler ESP32-C3

```bash
pio run -e ESP32c3_Cooler
```

Enthält nur: DS18B20, MultiSwitchController, OLED, ButtonValue, DigitalIn/Out, Link `direct`.

## Vollbuild

```bash
pio run -e WemosMini32
pio run -e ESP32c3_SuperMini
```
