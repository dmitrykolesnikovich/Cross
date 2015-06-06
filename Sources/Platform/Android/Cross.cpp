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

#include "Cross.h"
#include "Graphics.h"
#include "Audio.h"
#include "Input.h"

#include <EGL/egl.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>

#include <unistd.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Cross++", __VA_ARGS__))

android_app* app;

EGLDisplay display;
EGLSurface surface;
EGLContext context;

LauncherAndroid* launcher;
Game* mGame;

bool onPause = false;
bool focusLost = true;

extern "C"{
	void Java_com_cross_CrossActivity_InitAudio(JNIEnv *env, jobject thiz)
	{
		Audio::Init(NULL);
		//onPause = false;
		/*
		if(app->window != NULL){
			init_display();
		}else{
			LOGI("Error window not initialized");
		}*/
	}
	void Java_com_cross_CrossActivity_ReleaseAudio(JNIEnv* env, jobject thiz){
		//onPause = true;
		Audio::Release();
		//launcher = NULL;
	}
}

static void init_display(){
	LOGI("init_display()");
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGI("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    launcher = new LauncherAndroid(app, w, h);
}

static int32_t handle_input(android_app* appl, AInputEvent* event){
    int32_t eventType = AInputEvent_getType(event);
    switch(eventType){
        case AINPUT_EVENT_TYPE_MOTION:
            switch(AInputEvent_getSource(event)){
                case AINPUT_SOURCE_TOUCHSCREEN:
                    int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                    switch(action){
                        case AMOTION_EVENT_ACTION_DOWN:
                        case AMOTION_EVENT_ACTION_MOVE:
                        	mGame->input->input_state = true;
                        	mGame->input->input_loc.x = AMotionEvent_getX(event, 0);
                        	mGame->input->input_loc.y = AMotionEvent_getY(event, 0);
                        return 1;
                        case AMOTION_EVENT_ACTION_UP:
                        	mGame->input->input_state = false;
                        return 1;
                    }
                break;
            } // end switch
        break;
        case AINPUT_EVENT_TYPE_KEY:
        	int action = AKeyEvent_getAction(event);
				if(action == AKEY_EVENT_ACTION_DOWN){
				mGame->input->key_state = true;
				switch(AKeyEvent_getKeyCode(event)){
				case AKEYCODE_BACK:
					mGame->input->key_key = Key::PAUSE;
					return 1;
				case AKEYCODE_MENU:
					mGame->input->key_key = Key::PAUSE;
					return 1;
				}
        	}
        break;
    } // end switch
	return 0;
}

static void on_exit(){
	//Audio::Release();
	LOGI("on_exit()");
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    delete launcher;
    launcher = NULL;
    if(mGame != NULL){
    	mGame->Exit();
    }
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
	switch(cmd){
	case APP_CMD_SAVE_STATE:
		LOGI("APP_CMD_SAVE_STATE");
		break;
	case APP_CMD_INIT_WINDOW:
		LOGI("APP_CMD_INIT_WINDOW");
		if(app->window != NULL){
			init_display();
			onPause = false;
		}
		break;
	case APP_CMD_TERM_WINDOW:
		LOGI("APP_CMD_TERM_WINDOW");
		onPause = true;
		on_exit();
		break;
	case APP_CMD_START:
		LOGI("APP_CMD_START");
		break;
	case APP_CMD_RESUME:
		LOGI("APP_CMD_RESUME");
		break;
	case APP_CMD_PAUSE:
		LOGI("APP_CMD_PAUSE");
		break;
	case APP_CMD_STOP:
		LOGI("APP_CMD_STOP");
		break;
	case APP_CMD_GAINED_FOCUS:
		LOGI("APP_CMD_GAINED_FOCUS");
		focusLost = false;
		break;
	case APP_CMD_LOST_FOCUS:
		LOGI("APP_CMD_LOST_FOCUS");
		focusLost = true;
		break;
	}
}

LauncherAndroid* CrossInit(android_app* application){
	LOGI("CrossInit");
	app = application;
	app_dummy();
	app->onAppCmd = handle_cmd;
	app->onInputEvent = handle_input;
#ifdef CROSSDEBUG
	//sleep(5);
#endif
	while(!launcher){
		android_poll_source* source;
		int events;
		int eventID = ALooper_pollOnce(0, NULL, &events, (void**)&source);
		if(eventID >= 0){
			if(source != NULL)
				source->process(app, source);
			//if(app->destroyRequested != 0){
				//on_exit();
				//return 0;
			//}
		}
	}
	return launcher;
}

void CrossMain(Game* game){
	LOGI("CrossMain");
	mGame = game;
	game->graphics = new Graphics(game);
	game->Start();
	while(true){
		android_poll_source* source;
		int events;
		int eventID = ALooper_pollOnce(0, NULL, &events, (void**)&source);
		if(eventID >= 0){
			if(source != NULL)
				source->process(app, source);
			if(app->destroyRequested != 0){
				launcher->LogIt("destroyRequested");
				return;
			}
		}
		if(game != NULL && launcher != NULL && !focusLost){
			game->Update();
			mGame->input->key_state = false;
		    eglSwapBuffers(display, surface);
		}
	}
}
