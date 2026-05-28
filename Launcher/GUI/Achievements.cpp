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

	Layout();
	mainSizer->SetSizeHints(this);
	SetClientSize(mainApp->configManager->windowWidth, mainApp->configManager->windowHeight);
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
	wxBoxSizer* titleContainer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* modName = new wxStaticText(modPanel, wxID_ANY, mod.ModName);
	modName->SetForegroundColour(MAIN_BUTTON_TEXT_COLOUR);
	modName->SetFont(modName->GetFont().Bold());
	titleContainer->Add(modName);

	modContainer->Add(titleContainer, 0, wxLEFT | wxTOP | wxBOTTOM, 5);
	/* MOD HEADER END */

	wxWrapSizer* achievementShowcase = new wxWrapSizer(wxHORIZONTAL);
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
		
		wxStaticBitmap* modIcon = new wxStaticBitmap(modPanel, wxID_ANY, bitmap);
		achieveContainer->Add(modIcon, 0, wxALIGN_CENTER_HORIZONTAL, 5);

		wxStaticText* achievementName = new wxStaticText(modPanel, wxID_ANY, achievement->DisplayText);
		achievementName->SetForegroundColour(MAIN_TEXT_COLOUR);
		achieveContainer->Add(achievementName, 0, wxALIGN_CENTER_HORIZONTAL);

		wxGauge* guage = new wxGauge(modPanel, wxID_ANY, achievement->goalScore);
		guage->SetValue(achievement->currentScore);
		const wxSize size = guage->GetSize();
		guage->SetSize(size.GetWidth() / 2, size.GetHeight());
		achieveContainer->Add(guage, 0, wxALIGN_CENTER_HORIZONTAL);

		std::string progressText = std::to_string(achievement->currentScore) + " / " + std::to_string(achievement->goalScore);
		wxStaticText* achievementProgress = new wxStaticText(modPanel, wxID_ANY, progressText);
		achievementProgress->SetForegroundColour(MAIN_TEXT_COLOUR);
		achieveContainer->Add(achievementProgress, 0, wxALIGN_CENTER_HORIZONTAL);

		achievementShowcase->Add(achieveContainer, 0, 0, 10);
	}

	modContainer->Add(achievementShowcase, 0, 0, 10);
	modPanel->SetAutoLayout(true);
	modPanel->SetSizer(modContainer);
	
	mainSizer->Add(modPanel, 0, wxEXPAND, 10);
}
