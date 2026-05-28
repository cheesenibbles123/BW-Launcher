#include "MainWindow.h"
#include "WindowManager.h"
#include "Custom Components/StyledButton.h"
#include "../resource.h"

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

void MainWindow::OpenAchievementsWindow(wxCommandEvent& event)
{
	windowManager->ShowAchievementsWindow();
}

void MainWindow::Populate()
{
	wxPanel* panel = new wxPanel(this);
	panel->SetBackgroundColour(MAIN_WINDOW_BACKGROUND_COLOUR);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// HEADER START
	wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* headerText = new wxStaticText(panel, wxID_ANY, "BW Custom Launcher");
	headerText->SetForegroundColour(MAIN_BUTTON_TEXT_COLOUR);
	headerSizer->Add(headerText, 1, wxALIGN_CENTER_VERTICAL);

	//headerSizer->AddStretchSpacer();

	StyledButton* settingsButton = new StyledButton(panel, wxID_ANY, "Settings");
	headerSizer->Add(settingsButton);

	mainSizer->Add(headerSizer, 0, wxEXPAND);
	// HEADER END

	// NAVIGATION BAR START
	wxBoxSizer* navigationBarSizer = new wxBoxSizer(wxHORIZONTAL);

	StyledButton* achievementsButton = new StyledButton(panel, wxID_ANY, "Achievements");
	achievementsButton->Bind(wxEVT_BUTTON, &MainWindow::OpenAchievementsWindow, this);
	navigationBarSizer->Add(achievementsButton);

	StyledButton* statsButton = new StyledButton(panel, wxID_ANY, "Stats");
	navigationBarSizer->Add(statsButton);

	navigationBarSizer->AddStretchSpacer(1);

	StyledButton* launchGameButton = new StyledButton(panel, wxID_ANY, "Launch Game");
	launchGameButton->Bind(wxEVT_BUTTON, &MainWindow::OnLaunchGameSelected, this);
	navigationBarSizer->Add(launchGameButton);

	mainSizer->Add(navigationBarSizer, 0, wxEXPAND);
	// NAVIGATION BAR END

	// MOD LIST START
	wxBoxSizer* modListSizer = new wxBoxSizer(wxVERTICAL);
	std::vector<ModConfig> mods = mainApp->GetMods();

	for (int i = 0; i < mods.size(); i++) {
		wxPanel* modListEntryPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
		wxBoxSizer* modListEntry = new wxBoxSizer(wxHORIZONTAL);

		if (i % 2 == 1) {
			modListEntryPanel->SetBackgroundColour(ENTRY_ALT_BACKGROUND_COLOUR);
		}
		else {
			modListEntryPanel->SetBackgroundColour(ENTRY_BACKGROUND_COLOUR);
		}

		wxCheckBox* isEnabledCheckbox = new wxCheckBox(modListEntryPanel, wxID_ANY, "");
		isEnabledCheckbox->SetValue(true);
		isEnabledCheckbox->SetBackgroundColour(CHECKBOX_BACKGROUND_COLOUR);
		modListEntry->Add(isEnabledCheckbox, 0, wxALIGN_CENTER_VERTICAL);
		
		wxStaticBitmap* modIcon = new wxStaticBitmap();
		modListEntry->Add(modIcon, 0, wxALIGN_CENTER_VERTICAL);

		wxStaticText* modNameBtn = new wxStaticText(modListEntryPanel, wxID_ANY, mods.at(i).ModName);
		modNameBtn->SetForegroundColour(MAIN_TEXT_COLOUR);
		modListEntry->Add(modNameBtn, 1, wxALIGN_CENTER_VERTICAL);

		wxStaticText* modAuthorBtn = new wxStaticText(modListEntryPanel, wxID_ANY, mods.at(i).ModAuthor);
		modAuthorBtn->SetForegroundColour(MAIN_TEXT_COLOUR);
		modListEntry->Add(modAuthorBtn, 1, wxALIGN_CENTER_VERTICAL);

		modListEntry->AddStretchSpacer(1);

		StyledButton* modAchievesDeeplinkBtn = new StyledButton(modListEntryPanel, i, "Achievements");
		modListEntry->Add(modAchievesDeeplinkBtn, 0, wxALL);

		StyledButton* modStatsDeeplinkBtn = new StyledButton(modListEntryPanel, wxID_ANY, "Stats");
		modListEntry->Add(modStatsDeeplinkBtn, 0, wxALL);
		
		modListEntryPanel->SetSizer(modListEntry);
		modListEntryPanel->Layout();
		modListSizer->Add(modListEntryPanel, 0, wxEXPAND);
	}

	mainSizer->Add(modListSizer, 0, wxEXPAND);
	// MOD LIST END

	panel->SetAutoLayout(true);
	panel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	CreateStatusBar();
	SetClientSize(mainApp->configManager->windowWidth, mainApp->configManager->windowHeight);
}

void MainWindow::SetMainApp(MainApp* inMainApp)
{
	mainApp = inMainApp;
}

//void GenerateTestPanel(wxPanel* panel) {
//	wxButton* button = new wxButton(panel, wxID_ANY, "Launch Game", wxPoint(150, 50), wxSize(100, 35), wxBU_LEFT);
//
//	wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "Checkbox", wxPoint(550, 55));
//
//	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Static text - NOT EDITABLE", wxPoint(0, 150), wxSize(400, -1), wxALIGN_CENTER_HORIZONTAL);
//	staticText->SetBackgroundColour(*wxLIGHT_GREY);
//
//	wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "TextCtrl - editable", wxPoint(500, 145), wxSize(200, -1), wxTE_PASSWORD);
//
//	wxSlider* slider = new wxSlider(panel, wxID_ANY, 25, 0, 100, wxPoint(100, 250), wxSize(200, -1), wxSL_VALUE_LABEL);
//
//	wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(590, 205), wxSize(-1, 125), wxGA_VERTICAL);
//	gauge->SetValue(50);
//
//	wxArrayString choices;
//	choices.Add("Item C");
//	choices.Add("Item A");
//	choices.Add("Item B");
//
//	wxChoice* choice = new wxChoice(panel, wxID_ANY, wxPoint(150, 375), wxSize(100, -1), choices, wxCB_SORT);
//	choice->Select(0);
//
//	//wxSpinCtrl* spinCtrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(550, 375), wxSize(100, -1), wxSP_WRAP);
//
//	wxListBox* listBox = new wxListBox(panel, wxID_ANY, wxPoint(150, 475), wxSize(100, -1), choices, wxLB_MULTIPLE);
//
//	wxRadioBox* radioBox = new wxRadioBox(panel, wxID_ANY, "RadioBox", wxPoint(555, 450), wxDefaultSize, choices, 3, wxRA_SPECIFY_ROWS);
//}
