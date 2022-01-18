LOCAL_PATH := $(call my-dir)

# ============================================================================
# Dobby static libraries.

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := $(LOCAL_PATH)/libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

# ============================================================================
# Build the ImGui static library.

include $(CLEAR_VARS)
LOCAL_MODULE := libImGui
LOCAL_ARM_MODE := arm

LOCAL_CPPFLAGS := -fvisibility=hidden

LOCAL_C_INCLUDES := $(LOCAL_PATH)/vendor/imgui

FILE_LIST := $(wildcard $(LOCAL_PATH)/vendor/imgui/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/imgui/backends/*.c*)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
include $(BUILD_STATIC_LIBRARY)

# ============================================================================
# Build the enet static library.

include $(CLEAR_VARS)
LOCAL_MODULE := libenet
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS := -fvisibility=hidden

LOCAL_C_INCLUDES := $(LOCAL_PATH)/vendor/enet/include
FILE_LIST := $(wildcard $(LOCAL_PATH)/vendor/enet/*.c*)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
include $(BUILD_STATIC_LIBRARY)

# ============================================================================
# Build the lua static library.

include $(CLEAR_VARS)
LOCAL_MODULE := liblua
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS := -fvisibility=hidden

LOCAL_C_INCLUDES := $(LOCAL_PATH)/vendor/lua
FILE_LIST := $(wildcard $(LOCAL_PATH)/vendor/lua/*.c*)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
include $(BUILD_STATIC_LIBRARY)

# ============================================================================
# Build the ModMenu library.

include $(CLEAR_VARS)
LOCAL_MODULE := ModMenu
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS := -fvisibility=hidden
LOCAL_CPPFLAGS := -w -s -fvisibility=hidden -pthread -Wall -O3 -std=c++17 -fexceptions
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2

LOCAL_STATIC_LIBRARIES := libImGui libenet libdobby liblua

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/vendor/imgui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/vendor/enet/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/vendor/lua
LOCAL_C_INCLUDES += $(LOCAL_PATH)/vendor/sol2/include

FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/client/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/game/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/gui/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/include/KittyMemory/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/packet/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/proton/shared/manager/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/proton/shared/util/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/src/utilities/*.c*)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
include $(BUILD_SHARED_LIBRARY)
