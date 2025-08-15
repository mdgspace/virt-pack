#!/bin/bash

# CONFIG
TOOL_NAME="virt-pack"
BIN_NAME="virt-pack"
INSTALL_BIN_DIR="/usr/local/bin"
INSTALL_SHARE_DIR="/usr/local/share/$TOOL_NAME"
SCRIPTS_DIR="scripts"
PYTHON_SCRIPT="update_db.py"

# Build the binary
echo "[*] Building $TOOL_NAME..."
make
if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed."
    exit 1
fi

# Ensure installation directories exist
echo "[*] Creating install directories..."
sudo mkdir -p "$INSTALL_BIN_DIR"
sudo mkdir -p "$INSTALL_SHARE_DIR/scripts"

# Install binary
echo "[*] Installing binary to $INSTALL_BIN_DIR..."
sudo cp "$BIN_NAME" "$INSTALL_BIN_DIR/$TOOL_NAME"
sudo chmod +x "$INSTALL_BIN_DIR/$TOOL_NAME"

# Install supporting scripts
echo "[*] Installing Python scripts to $INSTALL_SHARE_DIR/scripts..."
sudo cp "$SCRIPTS_DIR/$PYTHON_SCRIPT" "$INSTALL_SHARE_DIR/scripts/"
sudo chmod +x "$INSTALL_SHARE_DIR/scripts/$PYTHON_SCRIPT"

echo "[✓] Installation complete. You can now run '$TOOL_NAME' from anywhere."