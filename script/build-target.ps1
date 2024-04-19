$target="ftxui-starter.exe"

cd build

taskkill /IM $target /F

cmake ..
cmake --build .