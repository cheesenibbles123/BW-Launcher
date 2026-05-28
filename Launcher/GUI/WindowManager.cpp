#include "WindowManager.h"

wxIMPLEMENT_APP(WindowManager);

bool WindowManager::OnInit() {
	wxInitAllImageHandlers();
	mainApp = new MainApp();

	mainWindow = new MainWindow("BW Mod Launcher", this);
	mainWindow->CenterOnScreen();
	mainWindow->SetMainApp(mainApp);
	mainWindow->Populate();
	mainWindow->Show();

	return true;
}

int WindowManager::OnExit() {
	mainWindow->DestroyFrame();
	return true;
}

void WindowManager::ShowAchievementsWindow()
{
	achievementsWindow = new AchievementsWindow("Achievements");
	achievementsWindow->CenterOnScreen();
	achievementsWindow->SetMainApp(mainApp);
	achievementsWindow->SetAchievementManager(mainApp->achievementManager);
	achievementsWindow->Populate();
	achievementsWindow->Show();
}

