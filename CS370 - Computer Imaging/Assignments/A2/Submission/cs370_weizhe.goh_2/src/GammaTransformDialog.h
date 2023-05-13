#pragma once
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class GammaTransformDialog : public wxDialog
{
public:

    GammaTransformDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr);

    virtual ~GammaTransformDialog();

    float getC() const { return c; }
    float getGamma() const { return gamma; }

private:

    void onUpdateOkButton(wxUpdateUIEvent& event);

    wxTextCtrl* constantlabel = nullptr;
    wxTextCtrl* gammalabel = nullptr;

    float c = 1.0f;
    float gamma = 1.0f;

    DECLARE_EVENT_TABLE()
};