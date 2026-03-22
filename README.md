# cpp-dev

CMake setup
Build Command

cmake -S . -B build
cmake --build build
./build/app

Run command
g++ src/yourfile.cpp && ./a.out

Use this for memory tracker app
g++ -fsized-deallocation src/memory_tracker.cpp && ./a.out
-fsized-deallocation --> this is used to tell the os to use our custom delete operator

if you just want to run main.cpp then run in your termainal
./run.sh

if you want to see the result just after saving it then run
./watch.sh
