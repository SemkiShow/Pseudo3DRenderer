#!/bin/bash
# Example
# cmake -B build -DCMAKE_BUILD_TYPE=Release
# cmake --build build --config Release
# ./build/bin/main

# Release
cd build
cmake .. && make && ./bin/main
cd ..

# Release
# cd build
# g++ -c -g ../src/*.cpp && g++ *.o -g -o main -lsfml-graphics -lsfml-window -lsfml-system && ./main
# cd ..
