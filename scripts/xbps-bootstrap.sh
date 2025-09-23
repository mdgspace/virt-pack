#!/bin/sh

echo "bootstrapping from void-packages"

XBPS_DIR=~/.local/share/virt-pack/xbps
mkdir -p $XBPS_DIR && cd $XBPS_DIR

git clone --depth 1 --filter=blob:none --sparse https://github.com/void-linux/void-packages $XBPS_DIR || echo "[ERROR] git clone failed"
git sparse-checkout set common etc srcpkgs/base-files || echo "[ERROR] git sparse-checkout failed"
./xbps-src binary-bootstrap || echo "[ERROR] binary bootstrap failed"


