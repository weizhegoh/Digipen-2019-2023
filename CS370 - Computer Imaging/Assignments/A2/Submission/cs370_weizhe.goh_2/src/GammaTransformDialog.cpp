#include "GammaTransformDialog.h"

#include <wx/valnum.h>

BEGIN_EVENT_TABLE(GammaTransformDialog, wxDialog)
EVT_UPDATE_UI(wxID_OK, GammaTransformDialog::onUpdateOkButton)
END_EVENT_TABLE()

GammaTransformDialog::GammaTransformDialog(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    //Create Constant Field 
    wxBoxSizer* constantSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create Constant Label 
    wxStaticText* text = new wxStaticText(this, wxID_ANY, _("Constant factor (c): "), wxPoint(10, 10));
    text->SetMinSize(wxSize(120, text->GetMinSize().y));
    constantSizer->Add(text);

    wxFloatingPointValidator< float > _val(2, &c);
    _val.SetRange(0.01f, 100000.0f);

    constantlabel = new wxTextCtrl(this, wxID_ANY, "", wxPoint(250, 50), wxDefaultSize, wxTE_PROCESS_ENTER, _val);
    constantSizer->Add(constantlabel, 1);
    mainSizer->Add(constantSizer, 0, wxEXPAND | wxALL, 5);

    //Create Gamma Field 
    wxBoxSizer* gammaSizer = new wxBoxSizer(wxHORIZONTAL);

    //Create Gamma Label 
    wxStaticText* gammatext = new wxStaticText(this, wxID_ANY, _("Gamma Value: "), wxPoint(10, 10));
    gammatext->SetMinSize(wxSize(120, gammatext->GetMinSize().y));
    gammaSizer->Add(gammatext);

    wxFloatingPointValidator< float > _val2(2, &gamma);
    _val.SetRange(0.01f, 100000.0f);

    gammalabel = new wxTextCtrl(this, wxID_ANY, "", wxPoint(250, 50), wxDefaultSize, wxTE_PROCESS_ENTER, _val2);
    gammaSizer->Add(gammalabel, 1);
    mainSizer->Add(gammaSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    //Create Button Field 
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(150, 120));
    Fit();
}

void GammaTransformDialog::onUpdateOkButton(wxUpdateUIEvent& event)
{
    event.Enable(false);

    if (!constantlabel->GetValue().empty() && !gammalabel->GetValue().empty())
    {
        event.Enable(true);
    }
}

GammaTransformDialog:: ~GammaTransformDialog(){}