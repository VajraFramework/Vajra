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

VAJRA_ROOT := ../Vajra
LIBRARIES_ROOT := ../Libraries
VAJRA_DIRS := $(addprefix .., ${shell find ${VAJRA_ROOT} -type d -print})

define all-cpp-files-under
$(patsubst ./%, %, \
  $(shell cd $(LOCAL_PATH) ; \
  		  echo "printing dollar1" ; echo $(1) ; echo "@" ; \
          find $(1) -name "*.cpp" -and -not -name ".*") \
 )
endef

include $(CLEAR_VARS)

LOCAL_MODULE    := libVajra
LOCAL_CFLAGS    := -Werror -DPLATFORM_ANDROID -DDEBUG -g -rdynamic
LOCAL_CPPFLAGS  := -std=c++11
LOCAL_CPP_FEATURES += rtti

# Additional include directories:
LOCAL_C_INCLUDES += $(VAJRA_ROOT)/..

LOCAL_FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
# LOCAL_SRC_FILES := $(LOCAL_FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(call all-cpp-files-under, ../$(LOCAL_PATH))
# LOCAL_SRC_FILES := gl_code.cpp
# LOCAL_SRC_FILES := $(wildcard *.cpp)
# LOCAL_SRC_FILES := *.cpp 
LOCAL_SRC_FILES += $(call all-cpp-files-under, ../$(VAJRA_ROOT))
# DON'T INCLUDE LIBRARIES UNDER SRC FILES
# LOCAL_SRC_FILES += $(call all-cpp-files-under, ../$(LIBRARIES_ROOT))

# LOCAL_SRC_FILES += ../$(VAJRA_ROOT)/test/test.cpp ../$(VAJRA_ROOT)/Core/Engine.cpp
# LOCAL_SRC_FILES += ../$(VAJRA_ROOT)/Core/Engine.cpp
# LOCAL_SRC_FILES += ../$(VAJRA_ROOT)/*/*.cpp
# LOCAL_SRC_FILES += $(wildcard $(addsuffix /*.cpp, $(VAJRA_DIRS)))

LOCAL_LDLIBS    := -llog -lGLESv2 -g -rdynamic

# Include libpng, zlib, and OpenAL libraries:
LOCAL_LDLIBS    += $(LIBRARIES_ROOT)/libpng-android-master/built/libpng.a -lz
LOCAL_C_INCLUDES += $(LIBRARIES_ROOT)/libpng/headers/
LOCAL_LDLIBS    += $(LIBRARIES_ROOT)/openal-android/built/libopenal.a
LOCAL_C_INCLUDES += $(LIBRARIES_ROOT)/openal/headers/

include $(BUILD_SHARED_LIBRARY)
