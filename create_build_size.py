import os
from SCons.Script import Import

Import("env")

def run_nm_analysis(source, target, env):
    elf_file = os.path.join(".pio", "build", env["PIOENV"], f"{env['PROGNAME']}.elf")

    # Controller-Typ bestimmen
    platform = env["PIOPLATFORM"]

    # Passende nm-Executable auswählen
    if platform == "espressif32":
        nm_tool = "xtensa-esp32-elf-nm.exe"
    elif platform == "espressif32s2":
        nm_tool = "xtensa-esp32s2-elf-nm.exe"
    elif platform == "espressif32s3":
        nm_tool = "xtensa-esp32s3-elf-nm.exe"
    elif platform == "espressif8266":
        nm_tool = "xtensa-lx106-elf-nm.exe"
    else:
        print(f"Unsupported platform: {platform}")
        return

    # Vollständigen Pfad zur nm-Executable ermitteln
    toolchain_dir = os.path.join(os.environ["USERPROFILE"], ".platformio", "packages", f"toolchain-{nm_tool.split('-elf')[0]}")
    nm_path = os.path.join(toolchain_dir, "bin", nm_tool)

    # Überprüfen, ob die nm-Executable existiert
    if not os.path.isfile(nm_path):
        print(f"Error: nm tool not found at {nm_path}")
        return

    # Befehl für nm-Analyse
    nm_command = f"{nm_path} -S -td {elf_file}"

    # Ausgabe in eine Datei umleiten
    output_file = os.path.join(".pio", "build", env["PIOENV"], f"nm_output_{platform}.txt")
    full_command = f"{nm_command} > {output_file}"

    print(f"Running: {full_command}")

    # Befehl ausführen
    os.system(full_command)

# Post-Build-Aktion hinzufügen
env.AddPostAction("buildprog", run_nm_analysis)