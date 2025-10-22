#!/bin/bash

# Script to run bear intercept and capture events to events.json
# Usage: bear-intercept.sh <build_command> [args...]

if [ $# -eq 0 ]; then
    echo "Error: No build command provided"
    echo "Usage: $0 <build_command> [args...]"
    exit 1
fi

# Run bear intercept with the provided command
# Bear outputs events in NDJSON format (newline-delimited JSON)
# We capture this to events.json
bear intercept --output events.json -- "$@"

if [ $? -ne 0 ]; then
    echo "Error: bear intercept failed"
    exit 1
fi

echo "Events captured to events.json"
