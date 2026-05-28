#pragma once
#include <wx/wx.h>
#include "./MainWindow.h"
#include "Achievements.h"
#include "./../MainApp.h"

class WindowManager: public wxApp
{
private:
	MainApp* mainApp;
	MainWindow* mainWindow;
	AchievementsWindow* achievementsWindow;
public:
	bool OnInit();
	int OnExit();

	void ShowAchievementsWindow();
	void ShowAchievementsWindow(std::string ModID);
};

