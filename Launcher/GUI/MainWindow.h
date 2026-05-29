#pragma once
#include <wx/wx.h>
#include "./../Types/ModConfig.h"
#include "../MainApp.h"
#include <vector>

#ifndef WindowManager
class WindowManager;
#endif

const wxColor MAIN_WINDOW_BACKGROUND_COLOUR = wxColor(49, 47, 46);
const wxColor MAIN_WINDOW_CONTENT_BACKGROUND_COLOUR = wxColor(34, 32, 32);

const wxColor MAIN_TEXT_COLOUR = wxColor(245, 245, 245);
const wxColor CHECKBOX_BACKGROUND_COLOUR = wxColor(91, 87, 85);

const wxColor ENTRY_BACKGROUND_COLOUR = wxColor(39, 37, 36);
const wxColor ENTRY_ALT_BACKGROUND_COLOUR = wxColor(34, 32, 32);

class MainWindow: public wxFrame
{
private:
	MainApp* mainApp;
	WindowManager* windowManager;
public:
	MainWindow(const wxString& title, WindowManager* inWindowManager);
	void DestroyFrame();
	void Populate();
	void SetMainApp(MainApp* inMainApp);
private:
	void OnLaunchGameSelected(wxCommandEvent& evt);
};

