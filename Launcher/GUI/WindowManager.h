#pragma once
#include <wx/wx.h>
#include "./MainWindow.h"
#include "Achievements.h"
#include "StatsWindow.h"
#include "./../MainApp.h"

class WindowManager: public wxApp
{
private:
	MainApp* mainApp;
	MainWindow* mainWindow;
	AchievementsWindow* achievementsWindow;
	StatsWindow* statsWindow;
public:
	bool OnInit();
	int OnExit();

	void ShowAchievementsWindow(wxCommandEvent& event);
	void ShowStatsWindow(wxCommandEvent& event);
	void ShowAchievementsWindow(std::string ModID);
};

