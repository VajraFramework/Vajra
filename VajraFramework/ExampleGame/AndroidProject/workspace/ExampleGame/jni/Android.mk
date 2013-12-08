# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

GAMECODE_ROOT := ../../../Code
VAJRA_INCLUDES_ROOT := ../../../include/
GAME_LIBRARIES_ROOT := ../../../Libraries

PREBUILT_VAJRA_LIBRARIES_PATH := ../../../lib/android/

define all-cpp-files-under
$(patsubst ./%, %, \
  $(shell cd $(LOCAL_PATH) ; \
  		  echo "printing dollar1" ; echo $(1) ; echo "@" ; \
          find $(1) -name "*.cpp" -and -not -name ".*") \
 )
endef

include $(CLEAR_VARS)

LOCAL_MODULE    := libExampleGame
LOCAL_CFLAGS    := -Werror -DPLATFORM_ANDROID -DDEBUG -g -rdynamic
LOCAL_CPPFLAGS  := -std=c++11
LOCAL_CPP_FEATURES += rtti

# Additional include directories:
LOCAL_C_INCLUDES += $(GAMECODE_ROOT)/
LOCAL_C_INCLUDES += $(VAJRA_INCLUDES_ROOT)/

LOCAL_FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
# LOCAL_SRC_FILES := $(LOCAL_FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(call all-cpp-files-under, ../$(LOCAL_PATH))
# LOCAL_SRC_FILES := gl_code.cpp
# LOCAL_SRC_FILES := $(wildcard *.cpp)
# LOCAL_SRC_FILES := *.cpp 
LOCAL_SRC_FILES += $(call all-cpp-files-under, ../$(GAMECODE_ROOT))
# DON'T INCLUDE LIBRARIES UNDER SRC FILES
# LOCAL_SRC_FILES += $(call all-cpp-files-under, ../$(GAME_LIBRARIES_ROOT))
#
LOCAL_LDLIBS += $(PREBUILT_VAJRA_LIBRARIES_PATH)/$(TARGET_ARCH_ABI)/libVajra.so

# LOCAL_SRC_FILES += ../$(GAMECODE_ROOT)/test/test.cpp ../$(GAMECODE_ROOT)/Core/Engine.cpp
# LOCAL_SRC_FILES += ../$(GAMECODE_ROOT)/Core/Engine.cpp
# LOCAL_SRC_FILES += ../$(GAMECODE_ROOT)/*/*.cpp

# LOCAL_LDLIBS    := -llog -lGLESv2 -g -rdynamic	# << used in vajra

# Include libpng, zlib, and OpenAL libraries:
# LOCAL_LDLIBS    += $(GAME_LIBRARIES_ROOT)/libpng-android-master/built/libpng.a -lz	# << used in vajra
# LOCAL_C_INCLUDES += $(GAME_LIBRARIES_ROOT)/libpng/headers/	# << used in vajra
# LOCAL_LDLIBS    += $(GAME_LIBRARIES_ROOT)/openal-android/built/libopenal.a	# << used in vajra
# LOCAL_C_INCLUDES += $(GAME_LIBRARIES_ROOT)/openal/headers/	# << used in vajra

include $(BUILD_SHARED_LIBRARY)
