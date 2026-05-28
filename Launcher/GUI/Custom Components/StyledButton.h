#pragma once
#include <wx/wx.h>

const wxColor MAIN_BUTTON_TEXT_COLOUR = wxColor(185, 181, 174);
const wxColor MAIN_BUTTON_BACKGROUND_COLOUR = wxColor(66, 62, 61);

class StyledButton : public wxButton {
public:
    StyledButton(
        wxWindow* parent,
        wxWindowID id,
        const wxString& label = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0
    ) : wxButton(parent, id, label, pos, size, style)
    {
        SetBackgroundColour(MAIN_BUTTON_BACKGROUND_COLOUR);
        SetForegroundColour(MAIN_BUTTON_TEXT_COLOUR);
    }
};