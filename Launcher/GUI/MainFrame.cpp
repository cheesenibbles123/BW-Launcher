#include "./MainFrame.h"
#include "../wxButtonWithIcon.h"
#include "../resource.h"

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title)
{
	mainApp = new MainApp();
	SetIcons(wxICON(APP_ICON));

	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// HEADER START
	wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText* headerText = new wxStaticText(panel, wxID_ANY, "BW Custom Launcher");
	headerSizer->Add(headerText, 1, wxEXPAND);

	//headerSizer->AddStretchSpacer();

	// wxButtonWithIcon* settingsButton = new wxButtonWithIcon(panel, "Settings", NULL);
	wxButton* settingsButton = new wxButton(panel, wxID_ANY, "Settings");
	headerSizer->Add(settingsButton);

	mainSizer->Add(headerSizer, 0, wxEXPAND);
	// HEADER END

	// NAVIGATION BAR START
	wxBoxSizer* navigationBarSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* modListButton = new wxButton(panel, wxID_ANY, "Mod List");
	navigationBarSizer->Add(modListButton);

	wxButton* achievementsButton = new wxButton(panel, wxID_ANY, "Achievements");
	navigationBarSizer->Add(achievementsButton);

	wxButton* statsButton = new wxButton(panel, wxID_ANY, "Stats");
	navigationBarSizer->Add(statsButton);

	navigationBarSizer->AddStretchSpacer(1);

	wxButton* launchGameButton = new wxButton(panel, wxID_ANY, "Launch Game");
	launchGameButton->Bind(wxEVT_BUTTON, &MainFrame::OnLaunchGameSelected, this);
	navigationBarSizer->Add(launchGameButton);

	mainSizer->Add(navigationBarSizer, 0, wxEXPAND);
	// NAVIGATION BAR END

	// MOD LIST START
	wxBoxSizer* modListSizer = new wxBoxSizer(wxVERTICAL);
	std::vector<ModConfig> mods = mainApp->loadMods();

	for (int i = 0; i < mods.size(); i++) {
		wxBoxSizer* modListEntry = new wxBoxSizer(wxHORIZONTAL);

		wxCheckBox* isEnabledCheckbox = new wxCheckBox(panel, wxID_ANY, "");
		modListEntry->Add(isEnabledCheckbox, wxALL);

		wxStaticBitmap* modIcon = new wxStaticBitmap();
		modListEntry->Add(modIcon, wxALL);

		wxStaticText* modNameBtn = new wxStaticText(panel, wxID_ANY, mods.at(i).ModName);
		modListEntry->Add(modNameBtn, wxALL);

		wxStaticText* modAuthorBtn = new wxStaticText(panel, wxID_ANY, mods.at(i).ModAuthor);
		modListEntry->Add(modAuthorBtn, wxALL);

		modListEntry->AddStretchSpacer(1);

		wxButton* modAchievesDeeplinkBtn = new wxButton(panel, wxID_ANY, "Achievements");
		modListEntry->Add(modAchievesDeeplinkBtn, wxALL);

		wxButton* modStatsDeeplinkBtn = new wxButton(panel, wxID_ANY, "Stats");
		modListEntry->Add(modStatsDeeplinkBtn, wxALL);

		modListSizer->Add(modListEntry, 0, wxEXPAND);
	}

	mainSizer->Add(modListSizer, 0, wxEXPAND);
	// MOD LIST END

	panel->SetAutoLayout(true);
	panel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	CreateStatusBar();
	SetClientSize(mainApp->configManager->windowWidth, mainApp->configManager->windowHeight);
}

void MainFrame::OnLaunchGameSelected(wxCommandEvent& evt)
{
	wxLogStatus("Launching Game");
	mainApp->LaunchGame();
	wxLogStatus("Game Launched!");
}

void MainFrame::DestroyFrame()
{
	mainApp->Destroy();
}

void GenerateTestPanel(wxPanel* panel) {
	wxButton* button = new wxButton(panel, wxID_ANY, "Launch Game", wxPoint(150, 50), wxSize(100, 35), wxBU_LEFT);

	wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "Checkbox", wxPoint(550, 55));

	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Static text - NOT EDITABLE", wxPoint(0, 150), wxSize(400, -1), wxALIGN_CENTER_HORIZONTAL);
	staticText->SetBackgroundColour(*wxLIGHT_GREY);

	wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "TextCtrl - editable", wxPoint(500, 145), wxSize(200, -1), wxTE_PASSWORD);

	wxSlider* slider = new wxSlider(panel, wxID_ANY, 25, 0, 100, wxPoint(100, 250), wxSize(200, -1), wxSL_VALUE_LABEL);

	wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(590, 205), wxSize(-1, 125), wxGA_VERTICAL);
	gauge->SetValue(50);

	wxArrayString choices;
	choices.Add("Item C");
	choices.Add("Item A");
	choices.Add("Item B");

	wxChoice* choice = new wxChoice(panel, wxID_ANY, wxPoint(150, 375), wxSize(100, -1), choices, wxCB_SORT);
	choice->Select(0);

	//wxSpinCtrl* spinCtrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(550, 375), wxSize(100, -1), wxSP_WRAP);

	wxListBox* listBox = new wxListBox(panel, wxID_ANY, wxPoint(150, 475), wxSize(100, -1), choices, wxLB_MULTIPLE);

	wxRadioBox* radioBox = new wxRadioBox(panel, wxID_ANY, "RadioBox", wxPoint(555, 450), wxDefaultSize, choices, 3, wxRA_SPECIFY_ROWS);
}
