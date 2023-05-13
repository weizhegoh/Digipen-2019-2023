#pragma once
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class LogTransformDialog : public wxDialog
{
public:

    LogTransformDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr);

    virtual ~LogTransformDialog();

    float getC() const { return c; }

private:

    void onUpdateOkButton(wxUpdateUIEvent& event);

    wxTextCtrl* constantlabel = nullptr;
    float c = 1.0f;

    DECLARE_EVENT_TABLE()
};