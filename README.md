# GTInternalAndroid

[![Github Action](https://img.shields.io/github/actions/workflow/status/ZTzTopia/GTInternalAndroid/android_ci.yml?branch=develop&logo=github&logoColor=white)](https://github.com/ZTzTopia/GTInternalAndroid/actions?query=workflow%3AAndroid%20CI)
[![GitHub Release](https://img.shields.io/github/v/release/ZTzTopia/GTInternalAndroid.svg?color=orange&logo=docusign&logoColor=orange)](https://github.com/ZTzTopia/GTInternalAndroid/releases/latest)

**GTInternalAndroid** is a [Growtopia](https://www.growtopiagame.com/) mod menu with [ImGui](https://github.com/ocornut/imgui).

## Supported Version
Android 5.0 Lollipop (SDK 21) ~ 13 Tiramisu (SDK 33)

## Features
- Free and open source.

## Screenshot
|                  ImGui Demo                  |
|:--------------------------------------------:|
| ![ImGui Demo](https://i.imgur.com/GD2Jydt.png) |

## Build
The following dependencies are required to build from source.
- [CMake](https://cmake.org/)
- [Android NDK](https://developer.android.com/ndk)

The following steps are for building from source.
1. First you need to clone the source code of this project. `git clone --recurse-submodules https://github.com/ZTzTopia/GLauncherAndroid.git`
2. Then create a build folder for cmake to use cmake when configuring and building the project.
```bash
mkdir build
cd build
```
3. Configure the cmake project the way you want, for example to build the arm64 library with api above 21:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a -DCMAKE_ANDROID_NDK="/path/to/android/ndk" -DCMAKE_SYSTEM_VERSION=21 -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang
```
4. After configuring the project you can immediately build it.
```bash
cmake --build .
```

> **Note**: Building this source from android will most likely work but I haven't tried it so feel free to experiment yourself :)
>
> But you can try this tutorial: [Build for Android](https://github.com/ZTzTopia/GTInternalAndroid/wiki#build-for-android)

## Credits
- [Dobby](https://github.com/jmpews/Dobby): A lightweight, multi-platform, multi-architecture hook framework
- [frida-gum](https://github.com/frida/frida-gum): Retrieve some memory code
- [ImGui](https://github.com/ocornut/imgui): Bloat-free Graphical User interface
- [KittyMemory](https://github.com/MJx0/KittyMemory): Runtime code patching

## License
This project is licensed under the MIT License. See the [LICENSE](https://github.com/ZTzTopia/GTInternalAndroid/blob/main/LICENSE) file for details.
