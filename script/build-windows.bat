@echo off

set root=%~dp0
set build_arch=%1

if "%1" == "" (
    set proj_root=%root%..\build\win\vs2017_x64
) else (
    set proj_root=%root%..\build\win\vs2017_%1
)

set install_prefix=%root%..\output\win

if exist %proj_root% goto DONE_CHECK_DIR
mkdir %proj_root%
:DONE_CHECK_DIR

if "%1" == "x86" (
    call:x86_build x86 Debug
    call:x86_build x86 release
    cd %root%
) else (
    call:x64_build x64 debug
    call:x64_build x64 release
    cd %root%
)

exit /b 0

:x86_build
cd %proj_root%
cmake %root%proj.cmake -T v141 -G "Visual Studio 15 2017" -DCMAKE_INSTALL_PREFIX=%install_prefix%\%1\%2
cmake --build . --config %2
cmake --install . --config %2


exit /b 0

:x64_build
cd %proj_root%
cmake %root%proj.cmake -T v141 -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=%install_prefix%\%1\%2
cmake --build . --config %2
cmake --install . --config %2
cd %root%
