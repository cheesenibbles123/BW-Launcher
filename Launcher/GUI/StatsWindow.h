#pragma once
#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/statline.h>
#include "../MainApp.h"
#include "MainWindow.h"
#include "Custom Components/StyledButton.h"
#include "Custom Components/DynamicFont.h"

class StatsWindow : public wxFrame
{
private:
	MainApp* mainApp;
	wxBoxSizer* mainSizer;
	wxPanel* mainPanel;

public:
	StatsWindow(const wxString& title);
	void Populate();
	void SetMainApp(MainApp* inMainApp);
private:
	void CreateAchievementSection(ModConfig mod);
};