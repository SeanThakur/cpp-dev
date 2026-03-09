#!/bin/bash

# Auto-compile and run C++ on file changes
# Usage: ./watch.sh

echo "👀 Watching src/ directory for changes..."
echo "💡 Edit your code and save - it will auto-compile and run!"
echo "🛑 Press Ctrl+C to stop"
echo ""

# Initial build and run
echo "🔨 Initial build..."
cmake -S . -B build > /dev/null 2>&1
cmake --build build 2>&1 | grep -E "error|warning|Building" || true
if [ -f "./build/practice" ]; then
    echo "─────────────────────────────────────────"
    ./build/practice
    echo "─────────────────────────────────────────"
else
    echo "❌ Build failed"
fi
echo ""

# Check if fswatch is available
if command -v fswatch &> /dev/null; then
    # Use fswatch (better)
    fswatch -o src/ | while read -r _; do
        clear
        echo "🔄 File changed - recompiling..."
        cmake --build build 2>&1 | grep -E "error|warning|Building" || true
        if [ -f "./build/practice" ]; then
            echo "─────────────────────────────────────────"
            ./build/practice
            echo "─────────────────────────────────────────"
        else
            echo "❌ Build failed"
        fi
        echo ""
        echo "👀 Watching for changes... (Ctrl+C to stop)"
    done
else
    # Fallback to simple polling
    echo "💡 Tip: Install fswatch for better performance: brew install fswatch"
    LAST_CHANGE=$(date +%s)
    while true; do
        CURRENT_CHANGE=$(stat -f %m src/main.cpp 2>/dev/null || echo "0")
        if [ "$CURRENT_CHANGE" != "$LAST_CHANGE" ]; then
            LAST_CHANGE=$CURRENT_CHANGE
            clear
            echo "🔄 File changed - recompiling..."
            cmake --build build 2>&1 | grep -E "error|warning|Building" || true
            if [ -f "./build/practice" ]; then
                echo "─────────────────────────────────────────"
                ./build/practice
                echo "─────────────────────────────────────────"
            else
                echo "❌ Build failed"
            fi
            echo ""
            echo "👀 Watching for changes... (Ctrl+C to stop)"
        fi
        sleep 1
    done
fi
