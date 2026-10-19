#!/bin/bash
set -e
g++ -std=c++17 -Wall -Wextra -Iinclude $(ls src/*.cpp | grep -v main.cpp) tests/test_all.cpp -o test_runner
./test_runner
