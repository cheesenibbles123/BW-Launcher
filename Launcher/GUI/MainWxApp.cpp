#include "./MainWxApp.h"
#include "./MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(MainWxApp);

bool MainWxApp::OnInit() {
	mainFrame = new MainFrame("BW Mod Launcher");
	mainFrame->CenterOnScreen();
	mainFrame->Show();
	return true;
}

int MainWxApp::OnExit() {
	mainFrame->DestroyFrame();
	return true;
}