#!/usr/bin/env python3

import gzip
import urllib.request
import json
import os

def get_local_dir():
    home = os.environ.get("HOME")
    if not home:
        print("[ERROR] HOME not set.")
        return None

    VIRT_PACK_LOCAL_DIR_PATH = ".local/share/virt-pack"

    local_dir = os.path.join(home, VIRT_PACK_LOCAL_DIR_PATH)

    # create the directory if it doesn't exist
    os.makedirs(local_dir, exist_ok=True)

    return local_dir

url = "http://ftp.debian.org/debian/dists/bookworm/main/Contents-amd64.gz"
local_dir = get_local_dir()
dest = f"{local_dir}/contents.gz"

print(f"Downloading archive from {url}")
urllib.request.urlretrieve(url, dest)

mapping = {}

with gzip.open(dest, "rt") as f:
    print("Searching for .pc files")
    for line in f:
        parts = line.strip().split(None, 1)
        if len(parts) != 2:
            continue
        path, package_str = parts
        if "pkgconfig" in path and path.endswith(".pc"):
            name = os.path.basename(path).replace(".pc", "")
            for pkg in package_str.split(","):
                mapping.setdefault(name, []).append(pkg.strip())

print(f"Found {len(mapping)} .pc entries")

with open(f"{local_dir}/virt-pack-db.json", "w") as f:
    json.dump(mapping, f, indent=2)

print("Cleaning up .gz file")
os.remove(dest)
