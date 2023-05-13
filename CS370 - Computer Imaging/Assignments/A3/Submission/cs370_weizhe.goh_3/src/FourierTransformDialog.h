#pragma once
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class FourierTransformDialog : public wxDialog
{
public:

    FourierTransformDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr);

    virtual ~FourierTransformDialog();

    float getC() const { return c; }

private:

    void onUpdateOkButton(wxUpdateUIEvent& event);

    wxTextCtrl* constantlabel = nullptr;
    float c = 1.0f;

    DECLARE_EVENT_TABLE()
};