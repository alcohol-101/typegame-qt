@echo off
chcp 65001 > nul

echo ========================================
echo   运行 TypeGame 单元测试
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

if not exist "build" (
    echo 错误：找不到 build 目录，请先运行 build_win.bat 构建项目
    pause
    exit /b 1
)

set "PATH=%VCPKG_ROOT%\installed\x64-windows\bin;%PATH%"

cd "build"

echo.
echo 运行单元测试...
echo ========================================

ctest -C Release --output-on-failure

echo ========================================
echo 单元测试完成！
echo ========================================

cd ..
pause
