# Growtopia Android ImGUI
Growtopia android modmenu with ImGUI.
![](https://cdn.discordapp.com/attachments/488978346072604682/896775860550533200/unknown.png)

## Features
- Built with [ImGUI](https://github.com/ocornut/imgui).

## Coming Soon
- None

## Requirements
The following dependencies are required to build the library from source.
- [NDK Android](https://developer.android.com/ndk).
- [APK Easy Tools](https://forum.xda-developers.com/android/software-hacking/tool-apk-easy-tool-v1-02-windows-gui-t3333960).
- Any text editor. We use [Notepad++](https://notepad-plus-plus.org/).

For AIDE users (I'm not sure if this works 100%).
- [AIDE CMods](https://www.mediafire.com/file/ihf3so9r9rc6yke/AIDE-_IDE_v3.2.210316__Unlocked.apk/file).
- [NDK AIDE for arm32](https://www.mediafire.com/file/62l4gcxqsqtxy20/ndk_arm.tar.gz/file).
- [NDK AIDE for arm64/aarch64](https://www.mediafire.com/file/w56tbt9pmqxzmpu/ndk_arm64.tar.gz/file).
- APK Editor.

There are many tutorials for installing on youtube, Attention if your android doesn't support arm then it won't work. one of them [AIDE CMods tutorial](https://youtu.be/VYvKEFHSgkg)

## Building the source
`ndk-build`:
```shell
git clone https://github.com/ZTzTopia/GrowtopiaImGui.git
cd GrowtopiaImGui
ndk-build
```

You need [ninja](https://ninja-build.org/) build system to use ninja cmake generator.

`cmake`:
```shell
mkdir build
cd build
cmake -GNinja -DCMAKE_MAKE_PROGRAM=/path/to/ninja.exe -DCMAKE_ANDROID_NDK=/path/to/android-ndk \
  -DCMAKE_TOOLCHAIN_FILE=toolchains/android.cmake ..
cmake --build .
```

## Inject to growtopia
### 1. Decompile growtopia apk

I highly recommend using `APK Easy Tools` but if you can't use it you can use the `APK Editor` on Android. Because `APK Editor` most likely there will be an error when recompiling the smali folder.

Or you can get growtopia with edited: [GrowtopiaLatestEdited](https://bit.ly/3lHn4Vv)

### 2. Add your library to apk

Copy the library file (.so) from `libs/armeabi-v7a` to the target game. Watch out the folder names, we don't want to mix them up, so make sure to copy our lib file to the correct architecture. Like our armeabi-v7a lib goes to the target games armeabi-v7a folder, arm64-v8a lib goes to the arm64-v8a folder...

PUTTING THE LIB FILE ON A WRONG ARCHITECTURE LIKE PUTTING ARM64 LIB TO ARMV7 WILL RESULT A CRASH!

![](https://i.imgur.com/oZq1Wq7.png)

### 1. Load your library

Now go to `smali/com/rtsoft/growtopia` and open the `Main.smali` file, Add this code before `const-string v0, "growtopia"`.
```smali
const-string v0, "ModMenu"

invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

![](https://cdn.discordapp.com/attachments/773831752271527946/896769601218691142/unknown.png)

If you don't see `smali` folder in APK Editor:

![](https://cdn.discordapp.com/attachments/773831752271527946/896772004651356240/unknown.png)

### 4. Compiling game apk
Now compile and sign the apk, and install it on your device. Congrats. You have successfully implemented a mod menu.

#### Some parts of this content from [LGLTeam Mod Menu](https://github.com/LGLTeam/Android-Mod-Menu)
