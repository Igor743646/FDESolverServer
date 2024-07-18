@ECHO OFF

setlocal

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
	conan install conanfile.py
)

if defined build_debug (
	conan build conanfile.py -s build_type=Debug
)

if defined build_release (
	conan build conanfile.py -s build_type=Release
)

endlocal
