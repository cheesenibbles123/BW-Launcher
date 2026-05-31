#pragma once
#include <wx/wx.h>
#include "../MainWindow.h"

class StyledCheckbox : public wxControl {
private:
    bool isEnabled = false;
    int padding = 3;
    wxColour checkColour = MAIN_TEXT_COLOUR;
    wxColour backgroundColour = *wxBLACK;

public:
    StyledCheckbox(
        wxWindow* parent,
        wxWindowID id = wxID_ANY,
        bool inIsEnabled = false,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(15, 15)
    ) : wxControl(parent, id, pos, size, wxBORDER_NONE)
    {
        isEnabled = inIsEnabled;
        Bind(wxEVT_PAINT, &StyledCheckbox::OnPaint, this);
        Bind(wxEVT_LEFT_UP,
            [this](wxMouseEvent& evt) {
                SetValue(!GetValue());
            }
        );
    }

    void SetValue(bool newValue)
    {
        isEnabled = newValue;
        Refresh();
    }

    bool GetValue()
    {
        return isEnabled;
    }

    void SetPadding(int newPadding)
    {
        padding = newPadding;
        Refresh();
    }

    bool SetBackground(const wxColour& col)
    {
        backgroundColour = col;
        Refresh();
        return true;
    }

    void SetCheckColour(const wxColour& col)
    {
        checkColour = col;
        Refresh();
    }

private:
    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(this);
        wxSize size = GetSize();

        // Draw background
        dc.SetBrush(wxBrush(backgroundColour));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(0, 0, size.x, size.y);

        // Draw check
        if (isEnabled) {
            dc.SetBrush(wxBrush(checkColour));
            dc.DrawRectangle(padding, padding, size.x - (padding * 2), size.y - (padding * 2));
        }
    }
};