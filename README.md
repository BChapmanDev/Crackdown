# Crackdown Recompilation
***Crackdown Recompilation is unfinished and it should be expected to encounter issues.***
This project uses [ReXGlue SDK 0.2.2](https://github.com/rexglue/rexglue-sdk/releases/tag/v0.2.2) (subject to change) to recompile Crackdown TU0.

## Requirements
- CMake
- ReXGlue SDK ([installation guide](https://github.com/rexglue/rexglue-sdk/wiki/Guide-%E2%80%90-Getting-Started))
- A legally acquired copy of Crackdown (2007) for the Xbox 360

## Steps To Recompile
Before attempting to recompile Crackdown, run the following command to ensure ReXGlue is installed and accessible in your build environment:
```
rexglue
```
Once you are sure ReXGlue is installed, you can continue with the rest of the steps.
```
git clone https://github.com/SkiddyToast/Crackdown.git
cd Crackdown
cmake configure
```
Copy the contents of the Crackdown disc into the `assets` directory.
```
cmake --build . --target crackdown_codegen
cmake --build .
```
Then, launch the executable with the path to the assets folder:
```
crackdown --assets ../../../assets
```

## Legal Stuff
This project is only inteded for use with legally acquired copies of Crackdown.
This project is not affiliated with Microsoft, Microsoft Game Studios, or the now defunct Realtime Worlds.