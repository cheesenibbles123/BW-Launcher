#pragma once
#include <wx/wx.h>

const wxColor PROGRESS_BAR_COLOUR = wxColor(122, 2, 2);

class StyledGauge : public wxControl {
private:
    int range;
    int value = 0;
    int borderWidth = 4;
    wxColour backgroundColour = *wxBLACK;
    wxColour barColour = PROGRESS_BAR_COLOUR;
    bool showProgressText = false;

public:
    StyledGauge(
        wxWindow* parent,
        wxWindowID id = wxID_ANY,
        int inRange = 100,
        bool inShowProgressText = false,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxGA_HORIZONTAL
    ) : wxControl(parent, id, pos, size, wxBORDER_NONE)
    {
        range = inRange;
        showProgressText = inShowProgressText;
        Bind(wxEVT_PAINT, &StyledGauge::OnPaint, this);
    }

    void SetValue(int newValue)
    {
        value = std::clamp(newValue, 0, range);
        Refresh();
    }

    void SetBorderWidth(int newWidth)
    {
        borderWidth = newWidth;
        Refresh();
    }

    void SetBarBackgroundColour(const wxColour& col)
    {
        backgroundColour = col;
        Refresh();
    }

    void SetBarColour(const wxColour& col)
    {
        barColour = col;
        Refresh();
    }

private:
    std::string GetProgressText()
    {
        return std::to_string(value) + " / " + std::to_string(range);
    }

    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(this);
        wxSize size = GetSize();

        // Draw background
        dc.SetBrush(wxBrush(backgroundColour));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(0, 0, size.x, size.y);

        // Draw filled bar
        int barWidth = (value * size.x) / range;
        dc.SetBrush(wxBrush(barColour));
        dc.DrawRectangle(borderWidth, borderWidth, std::clamp(barWidth, 0, size.x - borderWidth), size.y - borderWidth * 2);

        if (showProgressText) {
            dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
            dc.SetTextForeground(MAIN_TEXT_COLOUR);
            dc.DrawLabel(GetProgressText(), wxRect(0, 0, size.x, size.y), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
        }
    }
};