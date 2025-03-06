cd "$(dirname "$0")"
mkdir -p build-posix
cd build-posix
cmake ..
cmake --build .