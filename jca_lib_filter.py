"""
Pre-build: set lib_ignore from JCA_IGNORE_* build flags (merged env build_flags).
"""
from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

FNC_LIB_IGNORE = {
    "JCA_IGNORE_ACDIMMERS": "JCA_FNC_AcDimmers",
    "JCA_IGNORE_CHARGER": "JCA_FNC_Charger",
    "JCA_IGNORE_CLOCKVALUES": "JCA_FNC_ClockValues",
    "JCA_IGNORE_DAYSELECT": "JCA_FNC_DaySelect",
    "JCA_IGNORE_DIGITALIN": "JCA_FNC_DigitalIn",
    "JCA_IGNORE_DIGITALOUT": "JCA_FNC_DigitalOut",
    "JCA_IGNORE_DS18B20": "JCA_FNC_DS18B20",
    "JCA_IGNORE_FEEDER": "JCA_FNC_Feeder",
    "JCA_IGNORE_INA219": "JCA_FNC_INA219",
    "JCA_IGNORE_LEDSTRIP": "JCA_FNC_LedStrip",
    "JCA_IGNORE_LEVEL": "JCA_FNC_Level",
    "JCA_IGNORE_MULTISWITCHCONTROLLER": "JCA_FNC_MultiSwitchController",
    "JCA_IGNORE_OLED128X64": "JCA_FNC_OLED128x64",
    "JCA_IGNORE_PIDCONTROLLER": "JCA_FNC_PIDController",
    "JCA_IGNORE_SERVERLINK": "JCA_FNC_ServerLink",
    "JCA_IGNORE_VALUEANALOG": "JCA_FNC_ValueAnalog",
    "JCA_IGNORE_VALUEDIGITAL": "JCA_FNC_ValueDigital",
    "JCA_IGNORE_VALVE2DPOSIMP": "JCA_FNC_Valve2DPosImp",
    "JCA_IGNORE_BUTTONVALUE": "JCA_FNC_ButtonValue",
}


def _collect_defines():
    names = set()
    for item in env.get("CPPDEFINES", []):
        if isinstance(item, (list, tuple)):
            names.add(str(item[0]))
        else:
            names.add(str(item))
    try:
        for flag in env.ParseFlagsExtended(env.get("BUILD_FLAGS", []))[0]["CPPDEFINES"]:
            if isinstance(flag, (list, tuple)):
                names.add(str(flag[0]))
            else:
                names.add(str(flag))
    except (KeyError, TypeError):
        pass
    for flag in env.get("BUILD_FLAGS", []):
        text = str(flag)
        if text.startswith("-D"):
            names.add(text[2:].split("=", 1)[0])
    return names


defines = _collect_defines()
lib_ignore = []

for define_name, lib_name in FNC_LIB_IGNORE.items():
    if define_name in defines:
        lib_ignore.append(lib_name)

if lib_ignore:
    env.Append(LIB_IGNORE=lib_ignore)
    print(f"jca_lib_filter: lib_ignore += {lib_ignore}")
