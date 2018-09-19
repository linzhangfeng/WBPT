LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

define all-cpp-files-under
	$(patsubst ./%,%, \
	  $(shell cd $(LOCAL_PATH) ; \
	          find $(1) -name "*.cpp" -or -name "*.cc" -or -name "*.c" -and -not -name ".*" -and -not -name "testxxx.cpp") \
	 )
endef

define all-subdir-cpp-files
	$(call all-cpp-files-under,../../Classes/)
endef

LOCAL_SRC_FILES := hellocpp/main.cpp \
					$(call all-subdir-cpp-files)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_WHOLE_STATIC_LIBRARIES := jsoncpp_static protobuf_static cocos2d_js_static 

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.) 
$(call import-module,jsoncpp)
$(call import-module,protobuf)
$(call import-module, scripting/js-bindings/proj.android)


# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
