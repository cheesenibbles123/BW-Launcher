#include "WindowManager.h"

wxIMPLEMENT_APP(WindowManager);

void SetInitialWindowSize(wxFrame* frame) {
	int screenX = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
	int targetX = std::round(screenX / 3.0);

	int screenY = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
	int targetY = std::round(screenY / 3.0);

	frame->SetInitialSize(wxSize(targetX * 2, targetY * 2));
	frame->SetPosition(wxPoint(targetX / 2, targetY / 2));
}

bool WindowManager::OnInit() {
	wxInitAllImageHandlers();
	mainApp = new MainApp();

	mainWindow = new MainWindow("BW Mod Launcher", this);
	mainWindow->CenterOnScreen();
	mainWindow->SetMainApp(mainApp);
	mainWindow->Populate();
	SetInitialWindowSize(mainWindow);
	mainWindow->Show();

	return true;
}

int WindowManager::OnExit() {
	if (mainWindow) {
		mainWindow->DestroyFrame();
	}

	if (achievementsWindow) {
		achievementsWindow->Close();
	}

	return true;
}

void WindowManager::ShowAchievementsWindow()
{
	achievementsWindow = new AchievementsWindow("Achievements");
	achievementsWindow->CenterOnScreen();
	achievementsWindow->SetMainApp(mainApp);
	achievementsWindow->SetAchievementManager(mainApp->achievementManager);
	achievementsWindow->Populate();
	SetInitialWindowSize(achievementsWindow);
	achievementsWindow->Show();
}