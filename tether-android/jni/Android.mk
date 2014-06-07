LOCAL_PATH:= $(call my-dir)

# first lib, which will be built statically
#
include $(CLEAR_VARS)

LOCAL_MODULE    := librickfarmer-proxy
LOCAL_SRC_FILES := proxy.c

include $(BUILD_STATIC_LIBRARY)

# second lib, which will depend on and include the first one
#
include $(CLEAR_VARS)

LOCAL_MODULE    := librickfarmer-tether
LOCAL_SRC_FILES := tether.c

LOCAL_STATIC_LIBRARIES := librickfarmer-proxy

include $(BUILD_SHARED_LIBRARY)
