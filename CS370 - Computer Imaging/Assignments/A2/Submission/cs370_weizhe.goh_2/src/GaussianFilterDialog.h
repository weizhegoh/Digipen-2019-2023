#pragma once
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class GaussianFilterDialog : public wxDialog
{
public:

    GaussianFilterDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr);

    virtual ~GaussianFilterDialog();

    int getKernel() const { return k; }
    float getSD() const { return standard_deviation; }

private:

    void onUpdateOkButton(wxUpdateUIEvent& event);

    wxTextCtrl* kernelLabel = nullptr;
    wxTextCtrl* SD_Label = nullptr;

    int k = 3;
    float standard_deviation = 10.0f;

    DECLARE_EVENT_TABLE()
};