$target="ftxui-starter.exe"

cd build

taskkill /IM $pwd/$target /F

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .