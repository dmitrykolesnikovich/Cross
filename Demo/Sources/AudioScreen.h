#pragma once
#include "Screen.h"
#include "MenuBar.h"

using namespace cross;

class AudioScreen :	public Screen,
					public MenuBar {
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Sound* bck_music		= NULL;
	Sound* jaguar			= NULL;
	Sound* truck			= NULL;
	bool is_bck_playing		= false;
};

