#include "wxButtonWithIcon.h"

BEGIN_EVENT_TABLE(wxButtonWithIcon, wxPanel)

EVT_MOTION(wxButtonWithIcon::mouseMoved)
EVT_LEFT_DOWN(wxButtonWithIcon::mouseDown)
EVT_LEFT_UP(wxButtonWithIcon::mouseReleased)
EVT_RIGHT_DOWN(wxButtonWithIcon::rightClick)
EVT_LEAVE_WINDOW(wxButtonWithIcon::mouseLeftWindow)
EVT_KEY_DOWN(wxButtonWithIcon::keyPressed)
EVT_KEY_UP(wxButtonWithIcon::keyReleased)
EVT_MOUSEWHEEL(wxButtonWithIcon::mouseWheelMoved)

// catch paint events
EVT_PAINT(wxButtonWithIcon::paintEvent)

END_EVENT_TABLE()



wxButtonWithIcon::wxButtonWithIcon(wxPanel* parent, wxString text, wxImage* inImage) :
    wxWindow(parent, wxID_ANY)
{
    image = inImage;
    imageBitmap = wxBitmap(*image, -1);
    SetMinSize(wxSize(buttonWidth, buttonHeight));
    this->text = text;
    pressedDown = false;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void wxButtonWithIcon::paintEvent(wxPaintEvent& evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void wxButtonWithIcon::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void wxButtonWithIcon::render(wxDC& dc)
{
    if (pressedDown)
        dc.SetBrush(*wxRED_BRUSH);
    else
        dc.SetBrush(*wxGREY_BRUSH);

    dc.DrawRectangle(0, 0, buttonWidth, buttonHeight);
    dc.DrawText(text, 20, 15);
    if (image->IsOk()) {
        dc.DrawBitmap(imageBitmap, 10, 10);
    }
}

void wxButtonWithIcon::mouseDown(wxMouseEvent& event)
{
    pressedDown = true;
    paintNow();
}
void wxButtonWithIcon::mouseReleased(wxMouseEvent& event)
{
    pressedDown = false;
    paintNow();

    wxMessageBox(wxT("You pressed a custom button"));
}
void wxButtonWithIcon::mouseLeftWindow(wxMouseEvent& event)
{
    if (pressedDown)
    {
        pressedDown = false;
        paintNow();
    }
}

// currently unused events
void wxButtonWithIcon::mouseMoved(wxMouseEvent& event) {}
void wxButtonWithIcon::mouseWheelMoved(wxMouseEvent& event) {}
void wxButtonWithIcon::rightClick(wxMouseEvent& event) {}
void wxButtonWithIcon::keyPressed(wxKeyEvent& event) {}
void wxButtonWithIcon::keyReleased(wxKeyEvent& event) {}