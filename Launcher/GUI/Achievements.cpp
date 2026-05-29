#include "Achievements.h"
#include "wx/wrapsizer.h"
#include "../resource.h"
#include "../Lib.h"
#include <wx/mstream.h> 

#define ACHIEVEMENT_ICON_SIZE 100

wxImage LoadPngFromResource(int resourceId) {
	// Locate the resource
	HRSRC hRes = FindResource(nullptr, MAKEINTRESOURCE(resourceId), L"PNG");
	HGLOBAL hData = LoadResource(nullptr, hRes);
	DWORD size = SizeofResource(nullptr, hRes);
	void* pData = LockResource(hData);

	// Wrap the raw bytes in a wx stream
	wxMemoryInputStream stream(pData, size);

	// Load into wxImage
	wxImage img(stream, wxBITMAP_TYPE_PNG);
	return img;
}

AchievementsWindow::AchievementsWindow(const wxString& title): wxFrame(nullptr, wxID_ANY, title)
{
	mainSizer = new wxBoxSizer(wxVERTICAL);
	SetBackgroundColour(MAIN_WINDOW_BACKGROUND_COLOUR);
	SetSizer(mainSizer);

	wxImage fallbackImg = LoadPngFromResource(BW_CSHARP);
	fallbackImg = fallbackImg.Scale(ACHIEVEMENT_ICON_SIZE, ACHIEVEMENT_ICON_SIZE, wxIMAGE_QUALITY_HIGH);;
	fallbackImage = wxBitmap(fallbackImg);
}

void AchievementsWindow::SetMainApp(MainApp* inMainApp)
{
	mainApp = inMainApp;
}

void AchievementsWindow::Populate()
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

void AchievementsWindow::SetAchievementManager(AchievementManager* inAchievementManager)
{
	achievementManager = inAchievementManager;
}

void AchievementsWindow::CreateAchievementSection(ModConfig mod)
{
	wxPanel* modPanel = new wxPanel(this, wxID_ANY);
	modPanel->SetBackgroundColour(MAIN_WINDOW_CONTENT_BACKGROUND_COLOUR);
	wxBoxSizer* modContainer = new wxBoxSizer(wxVERTICAL);


	/* MOD HEADER START */
	wxStaticText* modName = new wxStaticText(modPanel, wxID_ANY, mod.ModName);
	modName->SetForegroundColour(MAIN_BUTTON_TEXT_COLOUR);
	wxFont font = modName->GetFont();
	font.Bold();
	font.SetPointSize(20);
	modName->SetFont(font);
	modContainer->Add(modName, 0, wxALL, 5);

	wxStaticLine* line = new wxStaticLine(modPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	modContainer->Add(line, 0, wxEXPAND | wxALL, 5);
	/* MOD HEADER END */
	modContainer->Add(0, 10, 0, wxEXPAND, 5);

	wxScrolledWindow* scrollBox = new wxScrolledWindow(modPanel, wxID_ANY, wxDefaultPosition, wxSize(500, 300), wxVSCROLL);
	scrollBox->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_DEFAULT);
	scrollBox->SetScrollRate(5, 5);

	wxWrapSizer* achievementShowcase = new wxWrapSizer(wxHORIZONTAL, wxEXTEND_LAST_ON_EACH_LINE);
	const size_t numAchievements = mod.Achievements.size();
	for (size_t i = 0; i < numAchievements; i++)
	{
		AchievementInfo* achievement = mod.Achievements.at(i);
		wxBoxSizer* achieveContainer = new wxBoxSizer(wxVERTICAL);
		wxImage image;

		std::string achievementImagesFolder = Lib::ModsPath() + mod.ModName + "/Assets/AchievementIcons/";
		std::string imageToUse = achievement->currentScore >= achievement->goalScore ? achievement->completeImageUrl : achievement->incompleteImageUrl;
		wxBitmap bitmap;

		if (imageCache.contains(achievementImagesFolder + imageToUse)) {
			bitmap = imageCache.at(achievementImagesFolder + imageToUse);
		}
		else {
			if (imageToUse == "" || !image.LoadFile(achievementImagesFolder + imageToUse, wxBITMAP_TYPE_ANY)) {
				mainApp->logger->log("No image found for achievement " + achievement->ID);
				bitmap = fallbackImage;
			}
			else {
				image = image.Scale(ACHIEVEMENT_ICON_SIZE, ACHIEVEMENT_ICON_SIZE, wxIMAGE_QUALITY_HIGH);
				bitmap = wxBitmap(image);
			}

			imageCache.emplace(achievementImagesFolder + imageToUse, bitmap);
		}
		
		wxStaticBitmap* modIcon = new wxStaticBitmap(scrollBox, wxID_ANY, bitmap);
		achieveContainer->Add(modIcon, 1, wxALIGN_CENTER_HORIZONTAL | wxALL);

		wxPanel* namePanel = new wxPanel(scrollBox, wxID_ANY, wxDefaultPosition, wxSize(-1, 30));
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(0, 0, 1, wxEXPAND, 5);

		wxStaticText* achievementName = new wxStaticText(namePanel, wxID_ANY, achievement->DisplayText, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
		achievementName->Wrap(50);
		achievementName->SetForegroundColour(MAIN_TEXT_COLOUR);
		achievementName->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
		sizer->Add(achievementName, 0, wxALIGN_CENTER_HORIZONTAL);

		sizer->Add(0, 0, 1, wxEXPAND, 5);

		namePanel->SetSizer(sizer);
		namePanel->Layout();
		achieveContainer->Add(namePanel, 1, wxEXPAND);

		StyledGauge* guage = new StyledGauge(scrollBox, wxID_ANY, achievement->goalScore, true, wxDefaultPosition, wxSize(120, 36), wxGA_HORIZONTAL);
		guage->SetValue(achievement->currentScore);

		achieveContainer->Add(guage, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
		achievementShowcase->Add(achieveContainer, 0, wxALIGN_LEFT | wxALL, 5);
	}

	achievementShowcase->AddSpacer(1);
	scrollBox->SetSizer(achievementShowcase);
	scrollBox->Layout();
	achievementShowcase->Fit(scrollBox);

	modContainer->Add(scrollBox, 0, wxALL | wxEXPAND, 5);

	modPanel->SetSizer(modContainer);
	modPanel->Layout();
	modContainer->Fit(modPanel);
	mainSizer->Add(modPanel, 0, wxALL | wxEXPAND, 10);
}
