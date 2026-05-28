#pragma once
#include <wx/wx.h>
#include "MainWindow.h"
#include "Custom Components/StyledButton.h"
#include "../AchievementManager.h"
#include "../MainApp.h"
#include <map>

class AchievementsWindow: public wxFrame
{
private:
	MainApp* mainApp;
	AchievementManager* achievementManager;
	wxBoxSizer* mainSizer;
	wxPanel* mainPanel;
	wxBitmap fallbackImage;

	std::map<std::string, wxBitmap> imageCache = {};
public:
	AchievementsWindow(const wxString& title);
	void Populate();
	void SetAchievementManager(AchievementManager* inAchievementManager);
	void SetMainApp(MainApp* inMainApp);
private:
	void CreateAchievementSection(ModConfig mod);
};