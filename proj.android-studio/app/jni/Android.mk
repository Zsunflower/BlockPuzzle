LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/Tile.cpp \
                   $(LOCAL_PATH)/../../../Classes/Block.cpp \
                   $(LOCAL_PATH)/../../../Classes/TileBlock.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/BaseDialog.cpp \
                   $(LOCAL_PATH)/../../../Classes/SettingLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/PauseLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/QuitLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameOverLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/Menu.cpp \
                   $(LOCAL_PATH)/../../../Classes/AdmobHelper.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
$(LOCAL_PATH)/../../../cocos2d/cocos/platform/android/jni

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END




