conan install conanfile.py --build=missing
conan build . -s build_type=Debug --build missing
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build/ --config Debug
