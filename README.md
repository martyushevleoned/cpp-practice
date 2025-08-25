# cpp-practice

установка библиотек
```shell
conan install . --build=missing --output-folder=build -s build_type=Debug
```

Сборка проекта
```shell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```