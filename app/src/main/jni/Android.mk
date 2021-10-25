LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ModMenu
LOCAL_ARM_MODE := arm

ifneq ($(APP_OPTIM), debug)
  LOCAL_LDFLAGS := -Wl,--gc-sections,--strip-all
  LOCAL_CFLAGS := -fvisibility=hidden -fvisibility-inlines-hidden
  LOCAL_CFLAGS += -g0 -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections
  LOCAL_CPPFLAGS := -fvisibility=hidden -fvisibility-inlines-hidden
  LOCAL_CPPFLAGS += -g0 -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections
endif

LOCAL_CFLAGS += -Wno-error=format-security -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS += -Wno-error=format-security -fpermissive -w -Werror -s -std=c++14
LOCAL_CPPFLAGS += -fno-rtti -fno-exceptions -fms-extensions -Wno-error=c++11-narrowing

LOCAL_C_INCLUDES := $(LOCAL_PATH)/vendor/imgui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/vendor/enet/include

FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/game/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/gui/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/include/Substrate/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/include/And64InlineHook/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/include/KittyMemory/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/utilities/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/imgui/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/imgui/backends/*.c*)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
