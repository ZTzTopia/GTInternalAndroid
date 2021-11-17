LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ModMenu
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS := -Wno-error=format-security -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -fpermissive -w -Werror -s -std=c++14
LOCAL_CPPFLAGS += -fno-rtti -fno-exceptions -fms-extensions -Wno-error=c++11-narrowing

LOCAL_C_INCLUDES := $(LOCAL_PATH)/vendor/imgui

FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/game/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/gui/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/include/Substrate/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/include/And64InlineHook/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/include/KittyMemory/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/utilities/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/libmem/libmem/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/imgui/*.c*)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
