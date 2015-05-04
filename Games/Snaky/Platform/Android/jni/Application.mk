# use this to select gcc instead of clang
NDK_TOOLCHAIN_VERSION := 4.8
# OR use this to select the latest clang version:
#NDK_TOOLCHAIN_VERSION := clang

APP_CPPFLAGS += -std=c++11
APP_STL := c++_shared
#APP_STL := c++_static
#APP_STL := gnustl_static
#APP_STL := stlport_shared

APP_PLATFORM := android-9
APP_ABI := armeabi