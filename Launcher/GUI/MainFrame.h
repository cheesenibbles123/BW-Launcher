#pragma once
#include <wx/wx.h>
#include "./../MainApp.h"
#include "./../Types/ModConfig.h"
#include <vector>

class MainFrame: public wxFrame
{
private:
	MainApp* mainApp;
public:
	MainFrame(const wxString& title);
	void DestroyFrame();
private:
	void OnLaunchGameSelected(wxCommandEvent& evt);
};

