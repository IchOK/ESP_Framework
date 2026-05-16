import os
import re
import subprocess
from collections import defaultdict
from SCons.Script import Import

Import("env")

# OTA app slot size from ota_dual_app.csv (0x180000)
JCA_FLASH_OTA_SLOT_BYTES = 0x180000
# Target: leave ~150 KB headroom for OTA updates
JCA_FLASH_TARGET_BYTES = 0x15C000  # 1_416_960 bytes (~1.35 MiB)

NM_GROUP_PREFIXES = (
    ("JCA::FNC::", "JCA_FNC"),
    ("JCA::IOT::", "JCA_IOT"),
    ("JCA::LNK::", "JCA_LNK"),
    ("JCA::SYS::", "JCA_SYS"),
    ("JCA::TAG::", "JCA_TAG"),
    ("AsyncWeb", "AsyncWebServer"),
    ("ArduinoJson", "ArduinoJson"),
    ("Update", "Update"),
    ("WiFi", "WiFi"),
)


def _parse_size(value):
    if isinstance(value, int):
        return value
    text = str(value).strip()
    if text.startswith("0x"):
        return int(text, 16)
    return int(text)


def _read_elf_sizes(env, elf_file):
    sizes = {"flash": None, "ram": None}
    build_dir = env.subst("$BUILD_DIR")
    bin_file = os.path.join(build_dir, f"{env.subst('$PROGNAME')}.bin")
    if os.path.isfile(bin_file):
        sizes["flash"] = os.path.getsize(bin_file)

    pioenv = env["PIOENV"]
    try:
        result = subprocess.run(
            ["pio", "run", "-e", pioenv, "-t", "size"],
            capture_output=True,
            text=True,
            check=False,
            cwd=env["PROJECT_DIR"],
        )
        for line in result.stdout.splitlines():
            if "Flash:" in line and "RAM:" in line:
                m = re.search(r"RAM:\s+\[[^\]]+\]\s+[\d.]+%\s+\(used\s+(\d+)\s+bytes", line)
                if m:
                    sizes["ram"] = int(m.group(1))
                m = re.search(r"Flash:\s+\[[^\]]+\]\s+[\d.]+%\s+\(used\s+(\d+)\s+bytes", line)
                if m:
                    sizes["flash"] = int(m.group(1))
    except OSError:
        pass
    return sizes


def _summarize_nm(nm_path):
    groups = defaultdict(int)
    total = 0
    if not nm_path or not os.path.isfile(nm_path):
        return groups, total
    line_re = re.compile(r"^\s*([0-9a-fA-Fx]+)\s+([0-9a-fA-Fx]+)\s+([Tt])\s+(.+)$")
    with open(nm_path, "r", encoding="utf-8", errors="replace") as handle:
        for line in handle:
            match = line_re.match(line)
            if not match:
                continue
            size = _parse_size(match.group(2))
            symbol = match.group(4)
            total += size
            matched = False
            for prefix, name in NM_GROUP_PREFIXES:
                if prefix in symbol:
                    groups[name] += size
                    matched = True
                    break
            if not matched:
                groups["other"] += size
    return groups, total


def _write_build_report(env, nm_path):
    build_dir = env.subst("$BUILD_DIR")
    pioenv = env["PIOENV"]
    elf_file = os.path.join(build_dir, f"{env.subst('$PROGNAME')}.elf")
    report_path = os.path.join(env["PROJECT_DIR"], f"build_size_{pioenv}.txt")

    sizes = _read_elf_sizes(env, elf_file)
    groups, nm_total = _summarize_nm(nm_path)

    flash = sizes.get("flash")
    ram = sizes.get("ram")
    lines = [
        f"Environment: {pioenv}",
        f"ELF: {elf_file}",
        "",
        "Memory (pio size):",
        f"  Flash used: {flash if flash is not None else 'n/a'} bytes",
        f"  RAM used:   {ram if ram is not None else 'n/a'} bytes",
        "",
        f"OTA slot limit: {JCA_FLASH_OTA_SLOT_BYTES} bytes ({JCA_FLASH_OTA_SLOT_BYTES // 1024} KiB)",
        f"Target max:     {JCA_FLASH_TARGET_BYTES} bytes ({JCA_FLASH_TARGET_BYTES // 1024} KiB)",
    ]
    if flash is not None:
        ota_headroom = JCA_FLASH_OTA_SLOT_BYTES - flash
        target_ok = flash <= JCA_FLASH_TARGET_BYTES
        lines.append(f"  OTA headroom:   {ota_headroom} bytes")
        lines.append(f"  Target OK:      {'yes' if target_ok else 'NO'}")
        if flash > JCA_FLASH_OTA_SLOT_BYTES:
            lines.append("  WARNING: firmware exceeds OTA slot size")

    lines.extend(["", "nm symbol groups (text section, bytes):"])
    for name, size in sorted(groups.items(), key=lambda item: -item[1]):
        lines.append(f"  {name:16} {size:8}")
    lines.append(f"  {'TOTAL (approx)':16} {nm_total:8}")

    with open(report_path, "w", encoding="utf-8") as report:
        report.write("\n".join(lines) + "\n")
    print(f"Build size report: {report_path}")


def run_nm_analysis(source, target, env):
    build_dir = env.subst("$BUILD_DIR")
    elf_file = os.path.join(build_dir, f"{env.subst('$PROGNAME')}.elf")
    nm_tool = env.subst("$NM")

    if not nm_tool or nm_tool == "$NM":
        print("Skipping nm analysis: NM tool is not configured")
        _write_build_report(env, "")
        return

    nm_path = nm_tool if os.path.isabs(nm_tool) else env.WhereIs(nm_tool)
    if not nm_path:
        print(f"Skipping nm analysis: NM tool not found ({nm_tool})")
        _write_build_report(env, "")
        return

    if not os.path.isfile(elf_file):
        print(f"Skipping nm analysis: ELF file not found ({elf_file})")
        _write_build_report(env, "")
        return

    output_file = os.path.join(build_dir, f"nm_output_{env['PIOENV']}.txt")
    print(f"Running: {nm_path} -S -td {elf_file} > {output_file}")

    with open(output_file, "w", encoding="utf-8") as output:
        subprocess.run([nm_path, "-S", "-td", elf_file], stdout=output, check=False)

    project_nm = os.path.join(env["PROJECT_DIR"], f"nm_output_{env['PIOENV']}.txt")
    try:
        with open(output_file, "r", encoding="utf-8", errors="replace") as src:
            with open(project_nm, "w", encoding="utf-8") as dst:
                dst.write(src.read())
    except OSError as err:
        print(f"Could not copy nm output to project root: {err}")

    _write_build_report(env, output_file)


env.AddPostAction("buildprog", run_nm_analysis)
