#pragma once
#include "Cross.h"
#include "Screen.h"
#include "Input.h"
#include "Button.h"

using namespace cross;

class TestNaPidoraScreen : public Screen
{
public:
	//void Start();
	void Update(float sec);
	TestNaPidoraScreen();
	~TestNaPidoraScreen();

private:
	void OnKeyPressed(Key key);
	Button* fooButton;

};



