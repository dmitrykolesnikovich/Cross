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

#include "Graphics.h"
#include "IMP/LauncherIMP.h"

LauncherIMP* launcher;
Graphics* gfx;

static void Start(JNIEnv* env){
    launcher = new LauncherIMP(env);
	gfx = new Graphics();
	gfx->Init(launcher);
}

static void Update(){
	gfx->Test();
}

static JNINativeMethod methods[] = {
		{ "Start", 	"()V", (void*)Start },
		{ "Update", "()V", (void*)Update }
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    jclass clazz = env->FindClass("com/cross/android/IMP");
    if(clazz){
    	env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(JNINativeMethod));
    	env->DeleteLocalRef(clazz);
    }
    return JNI_VERSION_1_6;
}
