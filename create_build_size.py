import os
import subprocess
from SCons.Script import Import

Import("env")

def run_nm_analysis(source, target, env):
    build_dir = env.subst("$BUILD_DIR")
    elf_file = os.path.join(build_dir, f"{env.subst('$PROGNAME')}.elf")
    nm_tool = env.subst("$NM")

    if not nm_tool or nm_tool == "$NM":
        print("Skipping nm analysis: NM tool is not configured")
        return

    nm_path = nm_tool if os.path.isabs(nm_tool) else env.WhereIs(nm_tool)
    if not nm_path:
        print(f"Skipping nm analysis: NM tool not found ({nm_tool})")
        return

    if not os.path.isfile(elf_file):
        print(f"Skipping nm analysis: ELF file not found ({elf_file})")
        return

    output_file = os.path.join(build_dir, f"nm_output_{env['PIOENV']}.txt")
    print(f"Running: {nm_path} -S -td {elf_file} > {output_file}")

    with open(output_file, "w", encoding="utf-8") as output:
        subprocess.run([nm_path, "-S", "-td", elf_file], stdout=output, check=False)

# Post-Build-Aktion hinzufügen
env.AddPostAction("buildprog", run_nm_analysis)