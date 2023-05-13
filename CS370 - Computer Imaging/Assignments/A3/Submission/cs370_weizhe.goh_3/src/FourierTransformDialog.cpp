#include "FourierTransformDialog.h"
#include <wx/valnum.h>

BEGIN_EVENT_TABLE(FourierTransformDialog, wxDialog)
EVT_UPDATE_UI(wxID_OK, FourierTransformDialog::onUpdateOkButton)
END_EVENT_TABLE()

FourierTransformDialog::FourierTransformDialog(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    //Create Constant Field 
    wxBoxSizer* constantSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create Constant Label 
    wxStaticText* text = new wxStaticText(this, wxID_ANY, _("Press OK to generate Inverse Fourier Transform."), wxPoint(10, 10));
    text->SetMinSize(wxSize(250, text->GetMinSize().y));
    constantSizer->Add(text);

    wxFloatingPointValidator< float > _val(2, &c);
    _val.SetRange(0.01f, 100000.0f);

    constantlabel = new wxTextCtrl(this, wxID_ANY, "", wxPoint(250, 50), wxDefaultSize, wxTE_PROCESS_ENTER, _val);
   // constantSizer->Add(constantlabel, 1);
    mainSizer->Add(constantSizer, 0, wxEXPAND | wxALL, 5);
    
    //Create Button Field 
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    
    SetSizer(mainSizer);
    SetMinSize(wxSize(250, 100));
    SetPosition(wxPoint(30, 350));
    Fit();
}

void FourierTransformDialog::onUpdateOkButton(wxUpdateUIEvent& event)
{
    event.Enable(false);

    if (!constantlabel->GetValue().empty())
    {
        event.Enable(true);
    }
}

FourierTransformDialog:: ~FourierTransformDialog()
{

}