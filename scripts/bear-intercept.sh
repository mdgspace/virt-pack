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

# Parse events.json to extract pkg-config packages
# This extracts all pkg-config arguments that don't start with "--"
echo "Parsing pkg-config dependencies..."

# Extract packages and write to packages.txt (one per line)
jq -r 'select(.started.execution.executable? // "" | contains("pkg-config")) | .started.execution.arguments[1:][] | select(startswith("--") | not)' events.json > packages.txt

if [ $? -ne 0 ]; then
    echo "Error: Failed to parse events.json with jq"
    exit 1
fi

# Count the number of packages found
pkg_count=$(wc -l < packages.txt | tr -d ' ')

if [ "$pkg_count" -eq 0 ]; then
    echo "Warning: No pkg-config packages found in build events"
else
    echo "Found $pkg_count pkg-config package(s):"
    cat packages.txt
fi

echo "Package list written to packages.txt"
