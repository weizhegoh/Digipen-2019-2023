#include "GaussianFilterDialog.h"
#include <wx/valnum.h>

BEGIN_EVENT_TABLE(GaussianFilterDialog, wxDialog)
EVT_UPDATE_UI(wxID_OK, GaussianFilterDialog::onUpdateOkButton)
END_EVENT_TABLE()

GaussianFilterDialog::GaussianFilterDialog(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    //Create Kernel Field 
    wxBoxSizer* kernelSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create Kernel Label 
    wxStaticText* text = new wxStaticText(this, wxID_ANY, _("Kernel: "), wxPoint(10, 10));
    text->SetMinSize(wxSize(200, text->GetMinSize().y));
    kernelSizer->Add(text);

    wxIntegerValidator<int>_val(&k);
    _val.SetRange(1, 100000);

    kernelLabel = new wxTextCtrl(this, wxID_ANY, "", wxPoint(250, 50), wxDefaultSize, wxTE_PROCESS_ENTER, _val);
    kernelSizer->Add(kernelLabel, 1);
    mainSizer->Add(kernelSizer, 0, wxEXPAND | wxALL, 5);

    //Create Gamma Field 
    wxBoxSizer* SDSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create Standard Deviation Label 
    wxStaticText* SDtext = new wxStaticText(this, wxID_ANY, _("Standard Deviation (Sigma): "), wxPoint(10, 10));
    SDtext->SetMinSize(wxSize(200, SDtext->GetMinSize().y));
    SDSizer->Add(SDtext);

    wxFloatingPointValidator< float > _val2(2, &standard_deviation);
    _val.SetRange(0.01f, 100000.0f);

    SD_Label = new wxTextCtrl(this, wxID_ANY, "", wxPoint(250, 50), wxDefaultSize, wxTE_PROCESS_ENTER, _val2);
    SDSizer->Add(SD_Label, 1);
    mainSizer->Add(SDSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    //Create Button Field 
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(150, 120));
    Fit();
}

void GaussianFilterDialog::onUpdateOkButton(wxUpdateUIEvent& event)
{
    event.Enable(false);

    if (!kernelLabel->GetValue().empty() && !SD_Label->GetValue().empty())
    {
        event.Enable(true);
    }
}

GaussianFilterDialog:: ~GaussianFilterDialog() {}