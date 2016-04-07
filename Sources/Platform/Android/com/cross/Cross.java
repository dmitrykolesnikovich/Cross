package com.cross;

import android.content.res.AssetManager;
import android.view.Surface;

public class Cross {
	public static final int	KEY_BACK		= 0x08;

	public native void OnCreate(CrossActivity activity, AssetManager assManager, String dataPath);
	public native void SurfaceChanged(Surface surface, int width, int height);
	public native void SurfaceDestroyed();
	public native void OnResume();
	public native void OnSuspend();
	public native void OnExit();
	
	public native void ActionDown(float x, float y, int actionID);
	public native void ActionUp(float x, float y, int actionID);
	public native void ActionMove(float x, float y, int actionID);
	public native void PressKey(int key);
	public native void ReleaseKey(int key);
	
	public native void InitialCommercial(Commercial comm);
	public native void CommertialResult(int event);
}
