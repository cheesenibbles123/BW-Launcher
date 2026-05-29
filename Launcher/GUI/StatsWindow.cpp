#include "StatsWindow.h"
#include "wx/wrapsizer.h"
#include "../resource.h"
#include "../Lib.h"
#include <wx/mstream.h> 

StatsWindow::StatsWindow(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	mainSizer = new wxBoxSizer(wxVERTICAL);
	SetBackgroundColour(MAIN_WINDOW_BACKGROUND_COLOUR);
	SetSizer(mainSizer);
}

void StatsWindow::SetMainApp(MainApp* inMainApp)
{
	mainApp = inMainApp;
}

void StatsWindow::Populate()
{
	mainSizer->Clear(true);
	std::vector<ModConfig> mods = mainApp->GetMods();

	for (ModConfig& mod : mods)
	{
		CreateAchievementSection(mod);
	}

	mainSizer->Layout();
	mainSizer->SetSizeHints(this);
	Layout();
}

void StatsWindow::CreateAchievementSection(ModConfig mod)
{
	wxPanel* modPanel = new wxPanel(this, wxID_ANY);
	modPanel->SetBackgroundColour(MAIN_WINDOW_CONTENT_BACKGROUND_COLOUR);
	wxBoxSizer* modContainer = new wxBoxSizer(wxVERTICAL);


	/* MOD HEADER START */
	wxStaticText* modName = new wxStaticText(modPanel, wxID_ANY, mod.ModName);
	modName->SetForegroundColour(MAIN_BUTTON_TEXT_COLOUR);
	DynamicFont font = DynamicFont(20);
	font.Bold();
	modName->SetFont(font);
	modContainer->Add(modName, 0, wxALL, 5);

	wxStaticLine* line = new wxStaticLine(modPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	modContainer->Add(line, 0, wxEXPAND | wxALL, 5);
	/* MOD HEADER END */
	modContainer->Add(0, 10, 0, wxEXPAND, 5);

	wxScrolledWindow* scrollBox = new wxScrolledWindow(modPanel, wxID_ANY, wxDefaultPosition, wxSize(500, 280), wxVSCROLL);
	scrollBox->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_DEFAULT);
	scrollBox->SetScrollRate(5, 5);

	wxBoxSizer* statShowcase = new wxBoxSizer(wxVERTICAL);
	const size_t numAchievements = mod.Achievements.size();
	for (size_t i = 0; i < numAchievements; i++)
	{
		AchievementInfo* achievement = mod.Achievements.at(i);
		wxBoxSizer* statContainer = new wxBoxSizer(wxHORIZONTAL);

		wxStaticText* statName = new wxStaticText(scrollBox, wxID_ANY, achievement->DisplayText);
		statName->SetForegroundColour(MAIN_TEXT_COLOUR);
		statContainer->Add(statName, 0, wxALL, 5);

		wxStaticText* statValue = new wxStaticText(scrollBox, wxID_ANY, std::to_string(achievement->currentScore));
		statValue->SetForegroundColour(MAIN_TEXT_COLOUR);
		statContainer->Add(statValue, 0, wxALL, 5);

		statShowcase->Add(statContainer, 0, wxALIGN_LEFT | wxALL, 5);
	}

	statShowcase->AddSpacer(1);
	scrollBox->SetSizer(statShowcase);
	scrollBox->Layout();
	statShowcase->Fit(scrollBox);

	modContainer->Add(scrollBox, 0, wxALL | wxEXPAND, 5);

	modPanel->SetSizer(modContainer);
	modPanel->Layout();
	modContainer->Fit(modPanel);
	mainSizer->Add(modPanel, 0, wxALL | wxEXPAND, 10);
}
