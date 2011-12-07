LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libClipper
LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := \
Clipper/JNIClipper.cpp
				   
				   
LOCAL_CFLAGS := -DFIXED_POINT -ffast-math -O3 -Wall -I$(LOCAL_PATH) -D_ARM_ASSEM_ -DANDROID
LOCAL_CPPFLAGS := -DFIXED_POINT -I$LOCAL_PATH/libvorbis/ -D_ARM_ASSEM_
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)