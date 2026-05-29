#include "MainWindow.h"
#include "WindowManager.h"
#include "Custom Components/StyledButton.h"
#include "Custom Components/DynamicFont.h"
#include "../resource.h"

void CreateModListEntry(wxPanel* panel, wxBoxSizer* parentSizer, ModConfig mod, wxColor background)
{
	wxPanel* modListEntryPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
		modListEntryPanel->SetBackgroundColour(background);
	wxBoxSizer* modListEntry = new wxBoxSizer(wxHORIZONTAL);

	wxCheckBox* isEnabledCheckbox = new wxCheckBox(modListEntryPanel, wxID_ANY, "");
	isEnabledCheckbox->SetValue(true);
	isEnabledCheckbox->SetBackgroundColour(CHECKBOX_BACKGROUND_COLOUR);
	modListEntry->Add(isEnabledCheckbox, 0, wxALIGN_CENTER_VERTICAL);

	wxStaticBitmap* modIcon = new wxStaticBitmap();
	modListEntry->Add(modIcon, 0, wxALIGN_CENTER_VERTICAL);

	wxStaticText* modNameBtn = new wxStaticText(modListEntryPanel, wxID_ANY, mod.ModName);
	modNameBtn->SetForegroundColour(MAIN_TEXT_COLOUR);
	modNameBtn->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	modListEntry->Add(modNameBtn, 1, wxALIGN_CENTER_VERTICAL);

	wxStaticText* modAuthorBtn = new wxStaticText(modListEntryPanel, wxID_ANY, mod.ModAuthor);
	modAuthorBtn->SetForegroundColour(MAIN_TEXT_COLOUR);
	modAuthorBtn->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	modListEntry->Add(modAuthorBtn, 1, wxALIGN_CENTER_VERTICAL);

	modListEntry->AddStretchSpacer(1);
	/*
	StyledButton* modAchievesDeeplinkBtn = new StyledButton(modListEntryPanel, wxID_ANY, "Achievements");
	modAchievesDeeplinkBtn->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	modListEntry->Add(modAchievesDeeplinkBtn, 0, wxALL);

	StyledButton* modStatsDeeplinkBtn = new StyledButton(modListEntryPanel, wxID_ANY, "Stats");
	modStatsDeeplinkBtn->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	modListEntry->Add(modStatsDeeplinkBtn, 0, wxALL);
	*/
	modListEntryPanel->SetSizer(modListEntry);
	modListEntryPanel->Layout();
	parentSizer->Add(modListEntryPanel, 0, wxEXPAND);
}

MainWindow::MainWindow(const wxString& title, WindowManager* inWindowManager) : wxFrame(nullptr, wxID_ANY, title)
{
	windowManager = inWindowManager;
	SetIcons(wxICON(APP_ICON));
}

void MainWindow::OnLaunchGameSelected(wxCommandEvent& evt)
{
	wxLogStatus("Launching Game");
	mainApp->LaunchGame();
	wxLogStatus("Game Launched!");
}

void MainWindow::DestroyFrame()
{
	mainApp->Destroy();
}

void MainWindow::Populate()
{
	wxPanel* panel = new wxPanel(this);
	panel->SetBackgroundColour(MAIN_WINDOW_BACKGROUND_COLOUR);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// HEADER START
	wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* headerText = new wxStaticText(panel, wxID_ANY, "BW Custom Launcher");
	headerText->SetFont(DynamicFont(DYNAMIC_HEADER_FONT_SIZE));
	headerText->SetForegroundColour(MAIN_BUTTON_TEXT_COLOUR);
	headerSizer->Add(headerText, 1, wxALIGN_CENTER_VERTICAL);

	//headerSizer->AddStretchSpacer();

	StyledButton* settingsButton = new StyledButton(panel, wxID_ANY, "Settings");
	settingsButton->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	headerSizer->Add(settingsButton, 0, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(headerSizer, 0, wxEXPAND);
	// HEADER END

	// NAVIGATION BAR START
	wxBoxSizer* navigationBarSizer = new wxBoxSizer(wxHORIZONTAL);

	StyledButton* achievementsButton = new StyledButton(panel, wxID_ANY, "Achievements");
	achievementsButton->Bind(wxEVT_BUTTON, &WindowManager::ShowAchievementsWindow, this->windowManager);
	achievementsButton->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	navigationBarSizer->Add(achievementsButton);

	StyledButton* statsButton = new StyledButton(panel, wxID_ANY, "Stats");
	statsButton->Bind(wxEVT_BUTTON, &WindowManager::ShowStatsWindow, this->windowManager);
	statsButton->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	navigationBarSizer->Add(statsButton);

	navigationBarSizer->AddStretchSpacer(1);

	StyledButton* launchGameButton = new StyledButton(panel, wxID_ANY, "Launch Game");
	launchGameButton->SetFont(DynamicFont(DYNAMIC_DEFAULT_FONT_SIZE));
	launchGameButton->Bind(wxEVT_BUTTON, &MainWindow::OnLaunchGameSelected, this);
	navigationBarSizer->Add(launchGameButton);

	mainSizer->Add(navigationBarSizer, 0, wxEXPAND);
	// NAVIGATION BAR END

	// MOD LIST START
	wxStaticLine* spacer = new wxStaticLine(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 5));
	spacer->SetBackgroundColour(MAIN_BUTTON_TEXT_COLOUR);

	mainSizer->Add(spacer, 0, wxEXPAND | wxALL, 0);

	wxBoxSizer* modListSizer = new wxBoxSizer(wxVERTICAL);
	std::vector<ModConfig> mods = mainApp->GetMods();

	for (int i = 0; i < mods.size(); i++) {
		wxColor background;

		if (i % 2 == 1) {
			background = ENTRY_ALT_BACKGROUND_COLOUR;
		}
		else {
			background = ENTRY_BACKGROUND_COLOUR;
		}

		CreateModListEntry(panel, mainSizer, mods.at(i), background);
	}

	mainSizer->Add(modListSizer, 0, wxEXPAND);
	// MOD LIST END

	panel->SetAutoLayout(true);
	panel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	CreateStatusBar();
}

void MainWindow::SetMainApp(MainApp* inMainApp)
{
	mainApp = inMainApp;
}
