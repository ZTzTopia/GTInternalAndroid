# Growtopia Android ImGUI
Growtopia android modmenu with ImGUI.
![](https://cdn.discordapp.com/attachments/488978346072604682/896775860550533200/unknown.png)

## 📜 Features
- Built with [ImGUI](https://github.com/ocornut/imgui).

## 😋 Coming Soon
- None

## 💻 Requirements
The following dependencies are required to build the library from source.
- [NDK Android](https://developer.android.com/ndk).
- [APK Easy Tools](https://forum.xda-developers.com/android/software-hacking/tool-apk-easy-tool-v1-02-windows-gui-t3333960).
- Any text editor. We use [Notepad++](https://notepad-plus-plus.org/).

For AIDE users (I'm not sure if this works 100%).
- [AIDE CMods](https://secufiles.com/nE9J/AIDE_CMODs_3.2.200108.apk).
- [NDK AIDE for arm32](https://www.mediafire.com/file/62l4gcxqsqtxy20/ndk_arm.tar.gz/file).
- [NDK AIDE for arm64](https://www.mediafire.com/file/w56tbt9pmqxzmpu/ndk_arm64.tar.gz/file).
- [APK Tool M](https://maximoff.su/apktool)

There are many tutorials for installing on youtube, Attention if your android doesn't support arm then it won't work. one of them [AIDE CMods tutorial](https://youtu.be/VYvKEFHSgkg)

## 🔨 Building the source
`ndk-build`:
```shell
git clone https://github.com/ZTzTopia/GTInternalAndroid
cd GrowtopiaImGui
ndk-build
```

## 📲 Inject to growtopia
<details>
  <summary markdown="span">Android</summary>
  
  I highly recommend using [APK Tool M](https://maximoff.su/apktool) to decompile the growtopia apk. Or you can get growtopia with edited: [Growtopia Folder](https://bit.ly/2ZanMBy), so you don't need to do the tutorial below. 
  
  #### 1. Antisplit the apk
  If you see a split sign like the picture below then follow these steps:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901407788067069962/unknown.png)
  
  Click the growtopia apk and press antisplit as shown below:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901408001313894480/unknown.png)
  
  Select the apk signature as shown below:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901408621211029534/unknown.png)
  
  And install the growtopia apk.
  
  #### 2. Decompile growtopia apk
  Click the growtopia apk and press decompile as shown below:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901409650090573914/unknown.png)
  
  Do what the image below does:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901409767019409408/unknown.png)
  
  And press go to:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901409975052673054/unknown.png)
  
  #### 3. Add your library to apk
  Copy the library file (.so) from `libs` to the target game. Watch out the folder names, we don't want to mix them up, so make sure to copy our lib file to the correct architecture. Like our armeabi-v7a lib goes to the target games armeabi-v7a folder, arm64-v8a lib goes to the arm64-v8a folder...

  PUTTING THE LIB FILE ON A WRONG ARCHITECTURE LIKE PUTTING ARM64 LIB TO ARMV7 WILL RESULT A CRASH!
  
  Now go to `lib/{arch}`:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901410620933541898/unknown.png)
  
  You can use second tab to copy library file to target game lib:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901410854174613524/unknown.png)
  
  And paste library file to target game lib:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901410915893772328/unknown.png)
  
  #### 4. Load your library
  Now go to `smali/com/rtsoft/growtopia`:
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901412534756724756/unknown.png)
  
  and open the `Main.smali` file, Add this code before `const-string v0, "growtopia"`.
  ```smali
  const-string v0, "ModMenu"

  invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
  ```
  
  Like the picture below:
  
  ![](https://cdn.discordapp.com/attachments/773831752271527946/896769601218691142/unknown.png)
  
  #### 5. Compiling game apk
  Now compile and sign the apk, and install it on your device. Congrats. You have successfully implemented a mod menu.
  
  ![](https://cdn.discordapp.com/attachments/488978346697424907/901411013793021952/unknown.png)
</details>

<details>
  <summary markdown="span">Windows</summary>
  
  Later ok?
</details>

Some parts of this content from [LGLTeam Mod Menu](https://github.com/LGLTeam/Android-Mod-Menu)
