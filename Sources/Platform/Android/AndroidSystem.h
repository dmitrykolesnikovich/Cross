/*	Copyright � 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#pragma once

#include "System.h"
#include <jni.h>
#include <android/asset_manager.h>
#include "android/log.h"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "Cross++", __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "CrossException", __VA_ARGS__)

namespace cross{

class AndroidSystem : public System{
public:
    AndroidSystem(JNIEnv* env, jobject crossActivity, AAssetManager* assManager, string dataPath);
	~AndroidSystem();

	void Log(const char* msg) override;
	string AssetsPath() override;
	string DataPath() override;
	U64 GetTime() override;
	float GetScreenDPI() override;

	File* LoadAssetFile(const string& filename) override;

	void PromtToExit();
	void RequestOrientation(Orientation orientation);
	void Messagebox(const string& title, const string& message) override;
	bool IsMobile() override;
	void Exit();
	void Sleep(float milis);
	void CallActivityVoidMethod(const string& methodName);
	void CallActivityVoidMethod(const string& methodName, const string& parameter);
	
public:
	void DetachFromJVM();
	JNIEnv* GetJNIEnv();
private:
	JavaVM* jvm;
	jobject cross_activity;
	AAssetManager* asset_manager;
	string data_path;
};

}
