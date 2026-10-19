#!/bin/bash
set -e
mkdir -p data
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o app
echo "Build OK. Run with: ./app"
