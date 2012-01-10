LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := sockets

LOCAL_SRC_FILES := Exception.cpp \
				   socket_include.cpp \
				   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
                   
LOCAL_LDLIBS := -llog

            
include $(BUILD_SHARED_LIBRARY)