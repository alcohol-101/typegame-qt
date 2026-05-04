chcp 65001 > nul
@echo off

echo ========================================
echo   开始构建 TypeGame 工程
echo ========================================

if defined VCPKG_ROOT (
    echo 检测到环境变量 VCPKG_ROOT = %VCPKG_ROOT%
) else (
    echo 未检测到环境变量 VCPKG_ROOT
    set /p VCPKG_ROOT="请手动输入 vcpkg 根目录路径（例如 C:\vcpkg）: "
    if not defined VCPKG_ROOT (
        echo 错误：未输入 vcpkg 路径！
        pause
        exit /b 1
    )
)

if not exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    echo 错误：找不到vcpkg工具链文件！
    echo 路径：%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
    pause
    exit /b 1
)

if exist "build" (
    echo 清理旧的build目录...
    rmdir /s /q "build"
)

mkdir "build"
cd "build"

echo.
echo 执行CMake配置...
echo ========================================

cmake .. -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -A x64

if errorlevel 1 (
    echo.
    echo CMake配置失败！
    cd ..
    pause
    exit /b 1
)

echo.
echo CMake配置成功！
echo ========================================
echo.

echo 开始编译...
cmake --build . --config Release

if errorlevel 1 (
    echo.
    echo 编译失败！
    cd ..
    pause
    exit /b 1
)

echo.
echo 编译成功！
echo ========================================

echo.
echo 可执行文件：build\bin\Release\typegame.exe
echo ========================================

cd ..
pause
