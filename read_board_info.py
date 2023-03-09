import os
from pathlib import Path
from datetime import date

userhome = str(Path.home())
localpath = os.path.dirname(__file__)
today = date.today()
dateact = today.strftime("%Y-%m-%d")

fileESP8266 = userhome + r'\.platformio\packages\framework-arduinoespressif8266\boards.txt'
fileESP32 = userhome + r'\.platformio\packages\framework-arduinoespressif32\boards.txt'
fileOut = localpath + r'\lib\JCA_IOT_Webserver\JCA_IOT_Webserver_Boardinfo.h'

ident = "build.board"


def getBoardInfo(prefix, name):
  file = open(name, "r")
  variant = ""
  mcu = ""
  for sub_index, sub_line in enumerate(file, start=1):
    if (prefix + "build.variant") in sub_line:
      variant = sub_line[sub_line.find("=")+1:-1]
    if (prefix + "build.mcu") in sub_line:
      mcu = sub_line[sub_line.find("=")+1:-1]
  file.close()
  return variant, mcu

def findBoardInfo(name, count):
  counter = count
  file = open(name, "r")
  for index, line in enumerate(file, start=1):
    if ident in line:
      boardPrefix = line[0:line.find(ident)]
      boardDefine = line[line.find("=")+1:-1].replace("-", "_")
      boardVariant, boardMcu = getBoardInfo(boardPrefix, name)
      if (boardVariant != "" and boardMcu != ""):
        if (counter == 0):
          outfile.write(f"#if defined(ARDUINO_{boardDefine})\n")
        else:
          outfile.write(f"#elif defined(ARDUINO_{boardDefine})\n")
        outfile.write(f"  #define BOARD_VARIANT \"{boardVariant}\"\n")
        outfile.write(f"  #define BOARD_MCU \"{boardMcu}\"\n")
        counter += 1
  file.close()
  return counter


counter = 0

outfile = open(fileOut, "w")

outfile.write("/**\n")
outfile.write(" * @file JCA_IOT_Webserver_Boardinfo.h\n")
outfile.write(" * @author JCA(https: // github.com/ichok)\n")
outfile.write(" * @brief Selection of Boardtypes\n")
outfile.write(" * Contains Variant and MCU Type.\n")
outfile.write(" * @version 0.1\n")
outfile.write(f" * @date {dateact}\n")
outfile.write(" *\n")
outfile.write(" * Copyright Jochen Cabrera 2022\n")
outfile.write(" * Apache License\n")
outfile.write(" *\n")
outfile.write(" */\n")
outfile.write("\n")
outfile.write("#ifndef _JCA_IOT_WEBSERVER_BOARDINFO_\n")
outfile.write("#define _JCA_IOT_WEBSERVER_BOARDINFO_\n")

counter = findBoardInfo(fileESP8266, counter)
counter = findBoardInfo(fileESP32, counter)

outfile.write(f"#else\n")
outfile.write(f"  #define BOARD_VARIANT \"undef\"\n")
outfile.write(f"  #define BOARD_MCU \"undef\"\n")
outfile.write(f"#endif\n")
outfile.write(f"#endif\n")
outfile.close()

print("Generation completed.")
