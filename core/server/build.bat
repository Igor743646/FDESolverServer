@ECHO OFF

setlocal

if "%1" equ "all" (
	set build_dependencies=true
	set build_debug=true
	set build_release=true
)
if "%1" equ "allbuild" (
	set build_debug=true
	set build_release=true
)
if "%1" equ "dependencies" (
	set build_dependencies=true
)
if "%1" equ "debug" (
	set build_debug=true
)
if "%1" equ "release" (
	set build_release=true
)

if not exist build\ (
	mkdir build
)

if defined build_dependencies (
	conan install conanfile.py --build=missing
	conan build . -s build_type=Release --build missing
	conan build . -s build_type=Debug --build missing
)

if defined build_debug (
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build ./build/ --config Debug
)

if defined build_release (
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build ./build/ --config Release
)

endlocal
