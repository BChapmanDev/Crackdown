# Crackdown Recompilation
***Crackdown Recompilation is unfinished and it should be expected to encounter issues.***
This project uses [ReXGlue SDK 0.10.0](https://github.com/rexglue/rexglue-sdk/releases/tag/v0.10.0) (subject to change) to recompile Crackdown TU0.

## Requirements
- CMake 3.25 or newer
- Ninja and Clang 20 or newer
- ReXGlue SDK 0.10.0 with the Xenos GPU plugin ([installation guide](https://github.com/rexglue/rexglue-sdk/wiki/Guide-%E2%80%90-Getting-Started))
- On Windows, an x64 Visual Studio development environment with a recent MSVC toolset and Windows SDK
- A legally acquired copy of Crackdown (2007) for the Xbox 360

## Steps To Recompile
Before attempting to recompile Crackdown, run the following command to ensure ReXGlue is installed and accessible in your build environment:
```
rexglue
```
Once you are sure ReXGlue is installed, you can continue with the rest of the steps. Add Clang and the SDK's `bin` directory to `PATH`, and set `CMAKE_PREFIX_PATH` to the SDK installation directory.
```
git clone https://github.com/SkiddyToast/Crackdown.git
cd Crackdown
```
Copy the contents of the Crackdown TU0 disc into the `assets` directory, with `default.xex` directly inside it. Generate the sources before configuring the build:
```
rexglue codegen crackdown_manifest.toml
cmake --preset win-amd64-release -DREXSDK_VERSION=0.10.0
cmake --build --preset win-amd64-release --parallel 4
```
For Linux, use the `linux-amd64-release` preset. The 0.10.0 migration has been tested on Windows only. Subsequent builds regenerate code when its inputs change; if the generated source list changes, run codegen and configure again.

Then, launch the executable from the repository root with the path to the assets folder:
```
out\build\win-amd64-release\crackdown.exe --game_data_root assets
```

Keep the SDK runtime and Xenos GPU plugin libraries beside the executable; CMake copies them into the build directory.

## Legal Stuff
This project is only inteded for use with legally acquired copies of Crackdown.
This project is not affiliated with Microsoft, Microsoft Game Studios, or the now defunct Realtime Worlds.