/////////////////////////////////////////////////////////////////////////////
// Name:        MyApp.cpp
// Purpose:     Minimal wxWidgets sample
// Author:      Goh Wei Zhe
// Modified by:	Goh Wei Zhe
// Created:     01/09/22
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <wx/wfstream.h>
#include <wx/filename.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>

//***************************************************************************
// Application class
//***************************************************************************
class MyApp : public wxApp
{
	virtual bool OnInit();
};

enum
{
	QUIT = 1,
	ABOUT,
	OPEN,
	SAVE,
	SAVEAS,
	NEARESTNEIGHBOUR,
	BILINEAR,
	BICUBIC,
};

//***************************************************************************
// Canvas class (where we display the image)
//***************************************************************************

class MyCanvas : public wxPanel
{
public:
	MyCanvas(wxWindow* parent, wxWindowID, const wxPoint& pos, const wxSize& size);
	~MyCanvas();
	void LoadImage(wxString fileName);
	void SaveImage(wxString fileName);

	bool interpolate;
	wxImageResizeQuality m_resizeQuality;

private:
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	wxBitmap m_imageBitmap;				// used to display the image
	wxImage* m_imageRGB;				// used to load the image
	unsigned char* m_myImage = NULL;	// used to process the image

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	int w, h;

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyCanvas, wxPanel)
EVT_PAINT(MyCanvas::OnPaint)
EVT_SIZE(MyCanvas::OnSize)
END_EVENT_TABLE()


MyCanvas::MyCanvas(wxWindow* parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size)
	: wxPanel(parent, id, pos, size, wxSUNKEN_BORDER)
{
	m_myImage = NULL;
	m_imageRGB = NULL;
	interpolate = false;

	w = -1;
	h = -1;

	m_resizeQuality = wxIMAGE_QUALITY_NORMAL;
}

MyCanvas::~MyCanvas()
{
	if (m_myImage)
		free(m_myImage);
	if (m_imageRGB)
		delete m_imageRGB;
}

void MyCanvas::LoadImage(wxString fileName)
{
	if (m_myImage)
		free(m_myImage);
	if (m_imageRGB)
		delete m_imageRGB;

	// open image dialog box
	m_imageRGB = new wxImage(fileName, wxBITMAP_TYPE_ANY, -1); 
	m_imageBitmap = wxBitmap(*m_imageRGB, -1);

	m_imageWidth = m_imageRGB->GetWidth();
	m_imageHeight = m_imageRGB->GetHeight();

	m_myImage = (unsigned char*)malloc(m_imageWidth * m_imageHeight * 3);
	memcpy(m_myImage, m_imageRGB->GetData(), m_imageWidth * m_imageHeight * 3);

	w = -1;
	h = -1;

	m_resizeQuality = wxIMAGE_QUALITY_NORMAL;

	Refresh();
}

void MyCanvas::SaveImage(wxString fileName)
{
	bool b;

	wxImage* tempImage = new wxImage(m_imageWidth, m_imageHeight, m_myImage, true); // lend my image buffer...
	b = tempImage->SaveFile(fileName);
	delete(tempImage);		

	if (!b)
		wxMessageBox(wxT("A problem occured during saving"));
}

void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
// update the main window content
{
	wxImage* tempImage;  // the bridge between my image buffer and the bitmap to display

	wxPaintDC dc(this);

	if (m_myImage)
	{
		tempImage = new wxImage(m_imageWidth, m_imageHeight, m_myImage, true); // lend my image buffer...
		m_imageBitmap = wxBitmap(*tempImage, -1);

		int neww, newh;
		dc.GetSize(&neww, &newh);

		if (neww != w || newh != h || interpolate)
		{
			m_imageBitmap = wxBitmap(tempImage->Scale(neww, newh, m_resizeQuality));
			w = neww;
			h = newh;
			delete(tempImage);
			dc.DrawBitmap(m_imageBitmap, 0, 0, false);
		}
		else
		{
			m_imageBitmap = wxBitmap(tempImage->Scale(w, h, m_resizeQuality));

			delete(tempImage);
			dc.DrawBitmap(m_imageBitmap, 0, 0, false);
		}
	}
}

void MyCanvas::OnSize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

//************************************************************************
// Frame class (the main window)
//************************************************************************

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title);

protected:
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpenImage(wxCommandEvent& WXUNUSED(event));
	void OnSaveImage(wxCommandEvent& WXUNUSED(event));
	void OnSaveAsImage(wxCommandEvent& WXUNUSED(event));
	void OnNearestNeighbourInterpolation(wxCommandEvent& WXUNUSED(event));
	void OnBilinearInterpolation(wxCommandEvent& WXUNUSED(event));
	void OnBicubicInterpolation(wxCommandEvent& WXUNUSED(event));
	void OnClose(wxCloseEvent& event);

	MyCanvas* m_canvas; // the canvas inside the main frame
	wxBitmap m_imageBitmap;	// used to display the image
	bool m_imageLoaded;
	bool saved = false;

	wxTextCtrl* textctrl = new wxTextCtrl();
	wxSize frameSize;
	wxString filename;

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(OPEN, MyFrame::OnOpenImage)
EVT_MENU(SAVE, MyFrame::OnSaveImage)
EVT_MENU(SAVEAS, MyFrame::OnSaveAsImage)
EVT_MENU(QUIT, MyFrame::OnQuit)
EVT_MENU(ABOUT, MyFrame::OnAbout)
EVT_MENU(NEARESTNEIGHBOUR, MyFrame::OnNearestNeighbourInterpolation)
EVT_MENU(BILINEAR, MyFrame::OnBilinearInterpolation)
EVT_MENU(BICUBIC, MyFrame::OnBicubicInterpolation)
EVT_CLOSE(MyFrame::OnClose)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp) // macro that contains the main() function

bool MyApp::OnInit()
{
	//support all available image formats
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame("CS370_GOHWEIZHE_ASSIGNMENT_1");
	frame->SetPosition(wxPoint(0, 0));
	frame->SetSize(1280, 720);

	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

//************************************************************************
// Frame class (the main window)
//************************************************************************

MyFrame::MyFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title)
{
	// set the frame icon
	SetIcon(wxICON(sample));

	filename = "";

#if wxUSE_MENUS
	wxMenuBar* menuBar = new wxMenuBar();
	wxMenu* filemenu = new wxMenu();

	wxMenu* helpMenu = new wxMenu;
	wxMenu* imageRenderingMenu = new wxMenu;
	wxMenu* interpolationMenu = new wxMenu;

	filemenu->Append(SAVE, "&Save", "Save");
	filemenu->Append(SAVEAS, "Save As", "Save new image");
	filemenu->Append(OPEN, "&Open\tAlt-O", "Open image");
	filemenu->Append(QUIT, "E&xit\tAlt-X", "Quit this program");
	helpMenu->Append(ABOUT, "&About\tF1", "Show about dialog");

	interpolationMenu->Append(NEARESTNEIGHBOUR, "Nearest Neighbour");
	interpolationMenu->Append(BILINEAR, "Bilinear");
	interpolationMenu->Append(BICUBIC, "Bicubic");

	imageRenderingMenu->AppendSubMenu(interpolationMenu, ("&Interpolation"));
	//imageRenderingMenu->Append(wxID_ANY, "Scaling");

	menuBar->Append(filemenu, "&File");
	menuBar->Append(imageRenderingMenu, "&ImageRendering");
	menuBar->Append(helpMenu, "&Help");
	SetMenuBar(menuBar);

	// create the canvas that will manage the image
	m_canvas = new MyCanvas(this, -1, wxDefaultPosition, wxSize(1, 2));
	m_canvas->SetBackgroundColour(wxColor(*wxWHITE));
	wxBoxSizer* sizerV = new wxBoxSizer(wxVERTICAL);

	wxPanel* right = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1, 2));
	right->SetBackgroundColour(wxColor(*wxWHITE));

	textctrl = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(3, 1), wxTE_MULTILINE);

	wxBoxSizer* sizerHorizontal = new wxBoxSizer(wxHORIZONTAL);
	sizerHorizontal->Add(m_canvas, 3, wxEXPAND | wxALL, 1);
	sizerHorizontal->Add(right, 1, wxEXPAND | wxALL, 1);

	wxBoxSizer* sizerHorizontal2 = new wxBoxSizer(wxHORIZONTAL);
	sizerHorizontal2->Add(textctrl, 1, wxEXPAND | wxALL, 1);

	sizerV->Add(sizerHorizontal, 8, wxEXPAND | wxALL, 1);
	sizerV->Add(sizerHorizontal2, 1, wxEXPAND | wxALL, 1);

	this->SetSizerAndFit(sizerV);

	m_imageLoaded = false;

	frameSize = m_canvas->GetSize();
	Centre();

#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
	CreateStatusBar(2);
	SetStatusText("Render PPM images using Nearest Neighbour or Bilinear Interpolation");
#endif // wxUSE_STATUSBAR
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MyFrame::OnClose(wxCloseEvent& event)
{
	delete m_canvas;
	event.Skip();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxString::Format
	(
		"Welcome to %s!\n"
		"\n"
		"This is the minimal wxWidgets sample\n"
		"running under %s.",
		wxVERSION_STRING,
		wxGetOsDescription()
	),
		"About wxWidgets minimal sample",
		wxOK | wxICON_INFORMATION,
		this);
}

void MyFrame::OnOpenImage(wxCommandEvent& WXUNUSED(event))
{
	wxBitmap bitmap;

	filename = wxFileSelector(_T("Select PPM file"), _T(""), _T(""), _T(""), _T("PPM files (*.ppm)|*.ppm"));
	if (!filename.empty())
	{
		m_canvas->LoadImage(filename);
		m_imageLoaded = true;
	}
}


void MyFrame::OnSaveImage(wxCommandEvent& WXUNUSED(event))
{
	if (!m_imageLoaded)
		return;

	if (!filename.empty())
		m_canvas->SaveImage(filename);

	wxLogMessage("Image Saved Successfully");
}


void MyFrame::OnSaveAsImage(wxCommandEvent& WXUNUSED(event))
{
	if (!m_imageLoaded)
		return;

	wxString fileName = wxFileSelector("Save Image As",
		wxEmptyString,
		wxEmptyString,
		wxEmptyString,
		"PPM files (*.ppm)|*.ppm|"
		"ICO files (*.ico)|*.ico|"
		"CUR files (*.cur)|*.cur",
		wxFD_SAVE,
		this);

	if (!fileName.empty())
		m_canvas->SaveImage(fileName);

	wxLogMessage("New Image Saved Successfully");
}

void MyFrame::OnNearestNeighbourInterpolation(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->interpolate = true;
	m_canvas->m_resizeQuality = wxIMAGE_QUALITY_NEAREST;
	wxLogMessage(" Nearest Neighbour Interpolation ");
	Refresh();
}

void MyFrame::OnBilinearInterpolation(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->interpolate = true;
	m_canvas->m_resizeQuality = wxIMAGE_QUALITY_BILINEAR;
	wxLogMessage(" Bilinear Interpolation ");
	Refresh();
}

void MyFrame::OnBicubicInterpolation(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->interpolate = true;
	m_canvas->m_resizeQuality = wxIMAGE_QUALITY_BICUBIC;
	wxLogMessage(" Bicubic Interpolation ");
	Refresh();
}