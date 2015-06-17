package com.cross;
import org.fmod.FMOD;

import android.app.NativeActivity;
import android.util.Log;

public class CrossActivity extends NativeActivity 
							implements Runnable{
	
	static{
		System.loadLibrary("c++_shared");
		System.loadLibrary("fmodL");
		System.loadLibrary("cross");
	}
	
	private Thread mThread;
	
	private native void InitAudio();
	private native void ReleaseAudio();
	
	@Override
	protected void onResume() {
		Log.d("Cross++", "Java onResume()");
		FMOD.init(this);
        mThread = new Thread(this, "Audio thread");
        mThread.start();
		super.onResume();
	}
	
	@Override
	protected void onPause() {
		Log.d("Cross++", "Java onPause()");
		super.onPause();
	}
	
	@Override
	protected void onStop(){
		Log.d("Cross++", "Java onStop()");
		//ReleaseAudio();
		//FMOD.close();
		super.onStop();
	}
	
	@Override
	public void run() {
		InitAudio();
	}
}