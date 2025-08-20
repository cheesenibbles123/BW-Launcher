#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

class MainWxApp: public wxApp
{
private:
	MainFrame* mainFrame;
public:
	bool OnInit();
	int OnExit();
};

