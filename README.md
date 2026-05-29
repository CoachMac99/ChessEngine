# ChessEngine

## Prerequisites
- [VS Code](https://code.visualstudio.com/)
- [CMake Tools extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [MinGW (msys64/ucrt64)](https://www.msys2.org/) — install, then add `C:\msys64\ucrt64\bin` to PATH
- [vcpkg](https://github.com/microsoft/vcpkg)

## Setup vcpkg
```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install glad:x64-mingw-dynamic glfw3:x64-mingw-dynamic
```

## Setup VS Code
Create `.vscode/settings.json` in the project root:
```json
{
    "cmake.configureArgs": [
        "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "-DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic"
    ]
}
```
## IntelliSense Setup
1. `Ctrl+Shift+P` → `C/C++: Edit Configurations (JSON)`
2. Add to the `includePath` array: "C:/vcpkg/installed/x64-mingw-dynamic/include"
3. Set `"configurationProvider": "ms-vscode.cmake-tools"`

## Build & Run
1. Open the `ChessEngine` folder in VS Code
2. `Ctrl+Shift+P` → `CMake: Configure` — select the **GCC (ucrt64)** kit
3. `F7` to build
4. `Ctrl+Shift+P` → `CMake: Run Without Debugging`
