import subprocess
import os
Import("env")

def get_firmware_specifier_build_flag():    
  ret = subprocess.run(["git", "describe", "--tags"], stdout=subprocess.PIPE, text=True) #Uses only annotated tags
  build_version = ret.stdout.strip()
  build_flag = "-D AUTO_VERSION=\\\"" + build_version + "\\\""
  return (build_flag)

def get_program_name():
  my_flags = env.ParseFlags(env['BUILD_FLAGS'])
  defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
  return ("FW_" + os.path.basename(os.getcwd()) + "_" + defines.get("AUTO_VERSION"))

env.Append(
  BUILD_FLAGS=[get_firmware_specifier_build_flag()]
)

env.Replace(
  PROGNAME=get_program_name()
)