# Script is tested on OS X 10.12
# YOUR MILEAGE MAY VARY

import sys
import shutil
import fnmatch
import subprocess
from pathlib import Path

source_paths = [
    "LICENSE",
    "src/",
]

# clone the repository
print("Cloning TinyUSB repository...")
if not Path("tinyusb_src").exists():
    subprocess.run("git clone --jobs=8 --recurse-submodules https://github.com/hathach/tinyusb.git tinyusb_src", shell=True)

# remove the sources in this repo
if Path("src").exists():
    shutil.rmtree("src")

print("Copying TinyUSB sources...")
for pattern in source_paths:
    for path in Path("tinyusb_src").glob(pattern):
        dest = path.relative_to("tinyusb_src")
        dest.parent.mkdir(parents=True, exist_ok=True)
        if path.is_dir():
            shutil.copytree(path, dest)
        else:
            shutil.copy2(path, dest)

print("Normalizing TinyUSB newlines and whitespace...")
subprocess.run("sh ./post_script.sh > /dev/null 2>&1", shell=True)

# print("Apply patch...")
# subprocess.run("git apply -v --ignore-whitespace modm.patch", shell=True)
