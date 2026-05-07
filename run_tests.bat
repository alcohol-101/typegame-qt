@echo off
chcp 65001 > nul

echo ========================================
echo    TypeGame Unit Tests (9 cases)
echo ========================================

if defined VCPKG_ROOT (
    echo VCPKG_ROOT = %VCPKG_ROOT%
) else (
    echo VCPKG_ROOT not found
    set /p VCPKG_ROOT="Enter vcpkg root path (e.g. C:\vcpkg): "
    if not defined VCPKG_ROOT (
        echo Error: no vcpkg path provided!
        pause
        exit /b 1
    )
)

if not exist "build" (
    echo Error: build folder not found, run build_win.bat first
    pause
    exit /b 1
)

set "PATH=%VCPKG_ROOT%\installed\x64-windows\bin;%PATH%"

cd "build"

echo.
echo Running tests...
echo ========================================

ctest -C Release --output-on-failure

echo ========================================
echo Tests complete!
echo ========================================

cd ..
pause
