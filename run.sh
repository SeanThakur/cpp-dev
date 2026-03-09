#!/bin/bash

# Quick run script - compile and execute in one command
# Usage: ./run.sh

cmake --build build 2>&1 | grep -E "error:" || true
if [ -f "./build/practice" ]; then
    ./build/practice
else
    echo "❌ Build failed"
    exit 1
fi
