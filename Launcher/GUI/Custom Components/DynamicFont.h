#pragma once
#include <wx/wx.h>

const float ORIGINAL_SCREEN_SIZE = 1440.0; // Devved on a 1440p screen
const int DYNAMIC_HEADER_FONT_SIZE = 25;
const int DYNAMIC_DEFAULT_FONT_SIZE = 15;

class DynamicFont : public wxFont {
private:
    float sizeMultiplier;
public:
    DynamicFont(
        int size = DYNAMIC_DEFAULT_FONT_SIZE,
        wxFontFamily family = wxFONTFAMILY_DEFAULT,
        wxFontStyle style = wxFONTSTYLE_NORMAL,
        wxFontWeight weight = wxFONTWEIGHT_NORMAL,
        bool underlined = false,
        const wxString& face = wxEmptyString,
        wxFontEncoding encoding = wxFONTENCODING_DEFAULT
    ) : wxFont(size, family, style, weight, underlined, face, encoding) {
        // *Most* monitors aren't constantly changing in size, as such, no need to re-calc this each time
        // Can assume that the smallest value is the most relevant to go off to ensure all fonts are "ok" relative to the proportions of the screen
        //if (!sizeMultiplier) {
            int screenX = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
            int screenY = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

            int smallest = screenX > screenY ? screenY : screenX;
            sizeMultiplier = smallest / ORIGINAL_SCREEN_SIZE;
        //}

            SetPointSize(size);
    }

    void SetPointSize(int pointSize) {
        int convertedSize = std::round(pointSize * sizeMultiplier);
        wxFont::SetPointSize(std::round(convertedSize));
    }
};