/////////////////////////////////////////////////////////////////////////////
// Name:        MyApp.cpp
// Purpose:     Assignment 3
// Author:      Goh Wei Zhe
// Modified by:	Goh Wei Zhe
// Created:     27/11/22
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
#include <wx/math.h>      
#include <wx/valnum.h>
#include <array>
#include <algorithm>
#include <vector>
#include <complex>

#include "LogTransformDialog.h"
#include "GammaTransformDialog.h"
#include "GaussianFilterDialog.h"
#include "FourierTransformDialog.h"

class MyApp : public wxApp
{
	virtual bool OnInit();
};

enum
{
	QUIT = 1,
	ABOUT,
	OPEN,
	CLEAR,
	SAVE,
	SAVEAS,
	NEARESTNEIGHBOUR,
	BILINEAR,
	BICUBIC,
	ADDITION,
	SUBTRACTION,
	MULTIPLY,
	DIVISION,
	NEGATIVE,
	LOG,
	GAMMA,
	HISTOGRAM,
	GAUSSIAN,
	SOBEL,
	DFT_BRUTEFORCE,
	DFT_SEPARABLE,
	FFT,
};

struct Pixel
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
};

class MyCanvas : public wxPanel
{
public:
	MyCanvas(wxWindow* parent, wxWindowID, const wxPoint& pos, const wxSize& size);
	~MyCanvas();
	void LoadImage(wxString fileName);
	void SaveImage(wxString fileName);
	void OnAddition();
	void OnSubtraction();
	void OnMultiply();
	void OnDivision();
	void OnImageNegative();
	void OnLogTransform();
	void OnGammaTransform();
	void OnHistogramEqualization();
	void OnGaussianSmootheringFilter();
	void OnSobelEdgeDetection();
	void OnDiscreteFourierTransform(std::vector <std::complex<double>>& c);
	void OnInverseDiscreteFourierTransform(std::vector <std::complex<double>>& c);
	void OnSeparableFourierTransform(std::vector<std::complex<double>>& c);
	void OnInverseSeparableFourierTransform(std::vector <std::complex<double>>& c);
	void FFT_1D(std::vector< std::complex< double > >& _input, const double _sign);
	void OnFastFourierTransform(std::vector <std::complex<double>>& c);
	void OnInverseFastFourierTransform(std::vector <std::complex<double>>& c);
	void onClamp(int& r, int& g, int& b);

	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int imageIndex = 0;
	float log_c = 0.0f;

	bool interpolate, negative, histogramEqualization;

	wxImage* m_imageRGB;				
	wxVector <Pixel*> m_myImage = NULL;	
	wxImageResizeQuality m_resizeQuality;

private:

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	int w, h;
	float input = 0.0f;

	wxBitmap m_imageBitmap;

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
	negative = false;
	histogramEqualization = false;

	w = -1;
	h = -1;

	m_resizeQuality = wxIMAGE_QUALITY_NORMAL;
}

MyCanvas::~MyCanvas()
{
	for (int i = 0; i < m_myImage.size(); i++)
	{
		free(m_myImage[i]);
	}

	if (m_imageRGB)
		delete m_imageRGB;
}

void MyCanvas::LoadImage(wxString fileName)
{
	if (m_imageRGB)
		delete m_imageRGB;

	// open image dialog box
	m_imageRGB = new wxImage(fileName, wxBITMAP_TYPE_ANY, -1);
	m_imageRGB->Rescale(wxWindow::GetSize().GetWidth(), wxWindow::GetSize().GetHeight());
	m_imageBitmap = wxBitmap(*m_imageRGB, -1);

	m_imageWidth = m_imageRGB->GetWidth();
	m_imageHeight = m_imageRGB->GetHeight();

	imageIndex = m_myImage.size();
	m_myImage.push_back((Pixel*)malloc(m_imageWidth * m_imageHeight * sizeof(unsigned char) * 3));
	memcpy(m_myImage[imageIndex], m_imageRGB->GetData(), m_imageWidth * m_imageHeight * sizeof(unsigned char) * 3);

	m_resizeQuality = wxIMAGE_QUALITY_NORMAL;
	SetSize(wxWindow::GetSize().GetWidth(), wxWindow::GetSize().GetHeight());
	GetParent()->SetClientSize(GetSize());
	Refresh(false);
}

void MyCanvas::SaveImage(wxString fileName)
{
	bool b;

	wxImage* tempImage = new wxImage(m_imageWidth, m_imageHeight, reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
	b = tempImage->SaveFile(fileName);
	delete(tempImage);

	if (!b)
		wxMessageBox(wxT("A problem occured during saving"));
}

// update the main window content
void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{

	wxPaintDC dc(this);
	int neww, newh;
	dc.GetSize(&neww, &newh);

	if (m_imageRGB != nullptr)
	{
		if (neww != w || newh != h || interpolate)
		{
			m_imageBitmap = wxBitmap(m_imageRGB->Scale(neww, newh, m_resizeQuality));
			w = neww;
			h = newh;
			dc.DrawBitmap(m_imageBitmap, 0, 0, false);
		}
		else
		{
			m_imageBitmap = wxBitmap(m_imageRGB->Scale(w, h, m_resizeQuality));
			dc.DrawBitmap(m_imageBitmap, 0, 0, false);
		}
	}
}

void MyCanvas::OnSize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void MyCanvas::OnAddition()
{
	if (imageIndex < 1)
		return;
	else
	{
		for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
		{
			for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
			{
				int r = 0, g = 0, b = 0;

				r = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r +
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].r;

				g = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g +
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].g;

				b = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b +
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].b;

				onClamp(r, g, b);

				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r = r;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g = g;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b = b;
			}
		}

		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::OnSubtraction()
{
	if (imageIndex < 1)
		return;
	else
	{
		for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
		{
			for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
			{
				int r = 0, g = 0, b = 0;

				r = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r -
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].r;

				g = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g -
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].g;

				b = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b -
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].b;

				onClamp(r, g, b);

				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r = r;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g = g;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b = b;
			}
		}

		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::OnMultiply()
{
	if (imageIndex < 1)
		return;
	else
	{
		for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
		{
			for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
			{
				int r = 0, g = 0, b = 0;

				r = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r *
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].r) / 255;

				g = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g *
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].g) / 255;

				b = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b *
					(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].b) / 255;

				onClamp(r, g, b);

				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r = r;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g = g;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b = b;
			}
		}

		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::OnDivision()
{
	if (imageIndex < 1)
		return;
	else
	{
		for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
		{
			for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
			{
				int r = 0, g = 0, b = 0;

				if (m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].r <= 0)
				{
					r = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r / FLT_EPSILON);
				}
				else
				{
					r = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r /
						(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].r);
				}

				if (m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].g <= 0)
				{
					g = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g / FLT_EPSILON);
				}
				else
				{
					g = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g /
						(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].g);
				}

				if (m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].b <= 0)
				{
					b = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b / FLT_EPSILON);
				}
				else
				{
					b = ((int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b /
						(int)m_myImage[imageIndex - 1][(i * wxWindow::GetSize().GetWidth()) + j].b);
				}

				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r = r;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g = g;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b = b;
			}
		}

		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::OnImageNegative()
{
	if (!m_myImage.empty())
	{
		for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
		{
			for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
			{
				int r = 0, g = 0, b = 0;

				r = 255 - (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r;
				g = 255 - (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g;
				b = 255 - (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b;

				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r = r;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g = g;
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b = b;
			}
		}

		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::OnLogTransform()
{
	if (!m_myImage.empty())
	{
		LogTransformDialog dialog(this, wxID_ANY, _("Log Transform Settings"));

		if (dialog.ShowModal() == wxID_OK)
		{
			for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
			{
				for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
				{
					int r = 0, g = 0, b = 0;

					r = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r;
					m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r = dialog.getC() * log10(1 + r);

					g = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g;
					m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g = dialog.getC() * log10(1 + g);

					b = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b;
					m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b = dialog.getC() * log10(1 + b);
				}
			}

			m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
			Refresh();
		}
	}
}

void MyCanvas::OnGammaTransform()
{
	if (!m_myImage.empty())
	{
		GammaTransformDialog dialog(this, wxID_ANY, _("Gamma Transform Settings"));

		if (dialog.ShowModal() == wxID_OK)
		{
			for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
			{
				for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
				{
					int r = 0, g = 0, b = 0;
					int value_r = 0, value_g, value_b;

					r = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r;
					g = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g;
					b = (int)m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b;

					value_r = dialog.getC() * pow(r + DBL_EPSILON, dialog.getGamma());
					value_g = dialog.getC() * pow(g + DBL_EPSILON, dialog.getGamma());
					value_b = dialog.getC() * pow(b + DBL_EPSILON, dialog.getGamma());

					onClamp(value_r, value_g, value_b);

					m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r = value_r;
					m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g = value_g;
					m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b = value_b;
				}
			}

			m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
			Refresh();
		}
	}
}

void MyCanvas::OnHistogramEqualization()
{
	if (!m_myImage.empty() && !histogramEqualization)
	{
		histogramEqualization = true;

		std::array<int, 128> histogram;
		histogram.fill(0);
		std::array<float, 128> probability;
		probability.fill(0.0f);

		for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
		{
			for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
			{
				int index = (int)((m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j]).r / 2);
				++histogram[index];
			}
		}

		float totalProbability = 0.0f;

		for (int k = 0; k < 128; k++)
		{
			totalProbability += 255.0f * ((float)histogram[k] / (float)(wxWindow::GetSize().GetHeight() * wxWindow::GetSize().GetWidth()));
			probability[k] = totalProbability;
		}

		for (int i = 0; i < wxWindow::GetSize().GetHeight(); i++)
		{
			for (int j = 0; j < wxWindow::GetSize().GetWidth(); j++)
			{
				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].r =
					probability[(int)((m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j]).r / 2)];

				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].g =
					probability[(int)((m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j]).g / 2)];

				m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j].b =
					probability[(int)((m_myImage[imageIndex][(i * wxWindow::GetSize().GetWidth()) + j]).b / 2)];
			}
		}

		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::OnGaussianSmootheringFilter()
{
	if (!m_myImage.empty())
	{
		GaussianFilterDialog dialog(this, wxID_ANY, _("Smooth Settings"));

		if (dialog.ShowModal() == wxID_OK)
		{
			//new img
			int index = m_myImage.size();
			m_myImage.push_back((Pixel*)malloc(m_imageWidth * m_imageHeight * 3));

			float ksum = 0.0;
			int length_N = dialog.getKernel() * 2 + 1;
			int half_kernel = dialog.getKernel();

			float sigma = dialog.getSD();

			std::vector<double> kernel(length_N * length_N, 0);

			//Gaussian Filter
			for (int i = 0; i < length_N; i++)
			{
				for (int j = 0; j < length_N; j++)
				{
					double x = i - half_kernel;
					double y = j - half_kernel;

					kernel[(i * length_N) + j] = exp(-(x * x + y * y) / (2 * sigma * sigma));
					ksum += kernel[(i * length_N) + j];
				}
			}

			for (int i = 0; i < length_N; ++i)
			{
				for (int j = 0; j < length_N; ++j)
				{
					kernel[(i * length_N) + j] /= ksum;
				}
			}

			std::vector<Pixel> output;
			output.resize(m_imageWidth * m_imageHeight);
			float sum = 0.0f;
			int row = 0;
			int col = 0;

			for (int i = 0; i < m_imageHeight; i++)
			{
				for (int j = 0; j < m_imageWidth; j++)
				{
					for (int k = 0; k < length_N; k++)
					{
						int kerRow = k + half_kernel;
						int RowCheck = i + kerRow;

						if (RowCheck < 0)
							row = 0;
						else if (RowCheck >= m_imageHeight)
							row = m_imageHeight - 1;
						else
						{
							row = RowCheck;
						}

						for (int t = 0; t < length_N; t++)
						{
							int kerCol = t + half_kernel;
							int ColCheck = j + kerCol;

							if (ColCheck < 0)
								col = 0;
							else if (ColCheck >= m_imageWidth)
								col = m_imageWidth - 1;
							else
							{
								col = ColCheck;
							}

							sum += kernel[k * dialog.getKernel() + t] * m_myImage[index - 1][row * m_imageWidth + col].r;
						}
					}
					m_myImage[index][i * m_imageWidth + j].r = sum;
					m_myImage[index][i * m_imageWidth + j].g = sum;
					m_myImage[index][i * m_imageWidth + j].b = sum;
					sum = 0.0f;
				}
			}

			m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[index]), true);
			Refresh();
		}
	}
}

void MyCanvas::OnSobelEdgeDetection()
{
	if (!m_myImage.empty())
	{
		int maskX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
		int maskY[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };

		int image_width = wxWindow::GetSize().GetWidth();
		int image_height = wxWindow::GetSize().GetHeight();

		std::vector<float> sobel_sum;
		sobel_sum.resize(image_width * image_height);

		float image_max = 0.0f;
		float image_min = 255.0f;

		for (int i = 0; i < image_height; i++)
		{
			for (int j = 0; j < image_width; j++)
			{
				float gX = 0.0f, gY = 0.0f;
				int xIdx = 0, yIdx = 0;

				for (int a = 0; a < 3; a++)
				{
					for (int b = 0; b < 3; b++)
					{
						if (i + a - 1 >= 0 && i + a - 1 < image_height)
							yIdx = i + a - 1;

						if (i + a - 1 < 0)
							yIdx = 0;

						if (i + a - 1 >= image_height)
							yIdx = image_height - 1;

						if (j + b - 1 >= 0 && j + b - 1 < image_width)
							xIdx = j + b - 1;

						if (j + b - 1 < 0)
							xIdx = 0;

						if (j + b - 1 > image_width)
							xIdx = image_width - 1;

						gX += m_myImage[imageIndex][yIdx * image_width + xIdx].r * maskX[a][b];
						gY += m_myImage[imageIndex][yIdx * image_width + xIdx].r * maskY[a][b];
					}
				}
				sobel_sum[i * image_width + j] = sqrtf(gX * gX + gY * gY);

				if (sobel_sum[i * image_width + j] > image_max)
					image_max = sobel_sum[i * image_width + j];

				if (sobel_sum[i * image_width + j] < image_min)
					image_min = sobel_sum[i * image_width + j];
			}
		}

		for (int i = 0; i < sobel_sum.size(); ++i)
		{
			float temp = sobel_sum[i];
			sobel_sum[i] = ((temp - image_min) * 255.0f) / (image_max - image_min);

			m_myImage[imageIndex][i].r = sobel_sum[i];
			m_myImage[imageIndex][i].g = sobel_sum[i];
			m_myImage[imageIndex][i].b = sobel_sum[i];
		}

		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::onClamp(int& r, int& g, int& b)
{
	if (r > 255 || r < 0)
	{
		if (r > 255)
			r = 255;
		else
			r = 0;
	}

	if (g > 255 || g < 0)
	{
		if (g > 255)
			g = 255;
		else
			g = 0;
	}

	if (b > 255 || b < 0)
	{
		if (b > 255)
			b = 255;
		else
			b = 0;
	}
}

void MyCanvas::OnDiscreteFourierTransform(std::vector <std::complex<double>>& c)
{
	size_t imageSize = m_imageWidth * m_imageHeight;
	double minvalue = std::numeric_limits<double>::max();
	double maxvalue = std::numeric_limits<double>::min();

	std::vector <double> magnitude;
	magnitude.resize(imageSize);

	std::vector<Pixel*> dft;
	dft.push_back((Pixel*)malloc(imageSize * 3));

	for (int i = 0; i < m_imageHeight; i++)
	{
		for (int j = 0; j < m_imageWidth; j++)
		{
			std::complex<double> fx(0, 0);

			for (int m = 0; m < m_imageHeight; m++)
			{
				for (int n = 0; n < m_imageWidth; n++)
				{
					const double& value = m_myImage[imageIndex][m * m_imageWidth + n].r * pow(-1, n + m);
					double exp = (-2 * M_PI) * ((j * n / static_cast<double>(m_imageWidth)) + (i * m / static_cast<double>(m_imageHeight)));
					fx += std::complex<double>(value * cos(exp), value * sin(exp));
				}
			}
			c.push_back(fx);
			double newvalue = std::log10(abs(fx) + 1.0);
			magnitude[i * m_imageWidth + j] = newvalue;

			if (minvalue > newvalue) minvalue = newvalue;
			if (maxvalue < newvalue) maxvalue = newvalue;
		}
	}

	for (size_t i = 0; i < imageSize; i++)
	{
		dft[0][i].r = dft[0][i].g = dft[0][i].b = static_cast<unsigned char> ((magnitude[i] - minvalue) * 255 / (maxvalue - minvalue));
	}

	imageIndex = m_myImage.size();
	m_myImage.push_back(dft[0]);
	m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
	Refresh();

}

void MyCanvas::OnInverseDiscreteFourierTransform(std::vector <std::complex<double>>& c)
{
	FourierTransformDialog dialog(this, wxID_ANY, _("Inverse Fourier Transform Settings"));

	if (dialog.ShowModal() == wxID_OK)
	{
		size_t imgsize = m_imageWidth * m_imageHeight;

		std::vector<Pixel*> idft;
		idft.push_back((Pixel*)malloc(imgsize * 3));

		for (int i = 0; i < m_imageHeight; i++)
		{
			for (int j = 0; j < m_imageWidth; j++)
			{
				std::complex<double> spatial(0, 0);

				for (int m = 0; m < m_imageHeight; m++)
				{
					for (int n = 0; n < m_imageWidth; n++)
					{
						const std::complex<double>& freqval = c[m * m_imageWidth + n];
						const double exp = (2 * M_PI) * ((j * n / static_cast<double>(m_imageWidth)) + (i * m / static_cast<double>(m_imageHeight)));
						spatial += freqval * std::complex<double>(cos(exp), sin(exp));
					}
				}

				Pixel& currPix = idft[0][i * m_imageWidth + j];
				currPix.r = currPix.g = currPix.b = static_cast<unsigned char> (abs(spatial /= imgsize));
			}
		}

		imageIndex = m_myImage.size();
		m_myImage.push_back(idft[0]);
		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::OnSeparableFourierTransform(std::vector <std::complex<double>>& c)
{
	size_t imageSize = m_imageWidth * m_imageHeight;
	double minvalue = std::numeric_limits<double>::max();
	double maxvalue = std::numeric_limits<double>::min();

	std::vector <double> magnitude;
	magnitude.resize(imageSize);

	std::vector<Pixel*> dft;
	dft.push_back((Pixel*)malloc(imageSize * 3));

	std::vector<std::complex<double>> interval;
	interval.resize(imageSize);
	c.resize(imageSize);

	for (int i = 0; i < m_imageHeight; i++)
	{
		for (int m = 0; m < m_imageHeight; m++)
		{
			for (int n = 0; n < m_imageWidth; n++)
			{
				const double& value = m_myImage[imageIndex][m * m_imageWidth + n].r * std::pow(-1, m + n);
				double exp = (-2 * M_PI) * ((i * m / static_cast<double>(m_imageHeight)));
				interval[i * m_imageWidth + n] += std::complex<double>(value * cos(exp), value * sin(exp));
			}

		}

	}
	for (int i = 0; i < m_imageHeight; i++)
	{
		for (int j = 0; j < m_imageWidth; j++)
		{
			for (int n = 0; n < m_imageWidth; n++)
			{

				double exp = (-2 * M_PI) * ((j * n / static_cast<double>(m_imageWidth)));
				c[i * m_imageWidth + j] += interval[i * m_imageWidth + n] * std::complex<double>(cos(exp), sin(exp));
			}

			double newvalue = std::log10(abs(c[i * m_imageWidth + j]) + 1.0);
			magnitude[i * m_imageWidth + j] = newvalue;

			if (minvalue > newvalue) minvalue = newvalue;
			if (maxvalue < newvalue) maxvalue = newvalue;
		}
	}

	for (size_t i = 0; i < imageSize; i++)
	{
		dft[0][i].r = dft[0][i].g = dft[0][i].b =
			static_cast<unsigned char> ((magnitude[i] - minvalue) * 255 / (maxvalue - minvalue));
	}

	imageIndex = m_myImage.size();
	m_myImage.push_back(dft[0]);
	m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
	Refresh();
}

void MyCanvas::OnInverseSeparableFourierTransform(std::vector <std::complex<double>>& c)
{
	FourierTransformDialog dialog(this, wxID_ANY, _("Inverse Fourier Transform Settings"));

	if (dialog.ShowModal() == wxID_OK)
	{
		size_t imageSize = m_imageWidth * m_imageHeight;

		std::vector<Pixel*> idft;
		idft.push_back((Pixel*)malloc(imageSize * 3));

		std::vector<std::complex<double>> interval;
		interval.resize(imageSize);

		for (int i = 0; i < m_imageHeight; i++)
		{
			for (int m = 0; m < m_imageHeight; m++)
			{
				for (int n = 0; n < m_imageWidth; n++)
				{
					const std::complex<double>& freqval = c[m * m_imageWidth + n];
					double exp = (2 * M_PI) * ((i * m / static_cast<double>(m_imageHeight)));
					interval[i * m_imageWidth + n] += freqval * std::complex<double>(cos(exp), sin(exp));
				}
			}
		}

		for (int i = 0; i < m_imageHeight; i++)
		{
			for (int j = 0; j < m_imageWidth; j++)
			{
				std::complex<double> spartial(0, 0);

				for (int n = 0; n < m_imageWidth; n++)
				{
					double exp = (2 * M_PI) * ((j * n / static_cast<double>(m_imageWidth)));
					spartial += interval[i * m_imageWidth + n] * std::complex<double>(cos(exp), sin(exp));
				}

				Pixel& currPix = idft[0][i * m_imageWidth + j];
				currPix.r = currPix.g = currPix.b = static_cast<unsigned char> (abs(spartial /= imageSize));
			}
		}

		imageIndex = m_myImage.size();
		m_myImage.push_back(idft[0]);
		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

void MyCanvas::FFT_1D(std::vector< std::complex< double > >& _input, const double _sign = -1.0)
{
	size_t offset = _input.size();
	const double exponent = _sign * M_PI / _input.size();
	std::complex< double > stageW = std::complex< double >(std::cos(exponent), std::sin(exponent));

	for (size_t i = _input.size(); i > 1; )
	{
		offset = i;
		std::complex< double > W = 1.0;

		i >>= 1;
		stageW *= stageW;

		for (size_t j = 0; j < i; ++j)
		{
			for (size_t idx1 = j; idx1 < _input.size(); idx1 += offset)
			{
				size_t idx2 = idx1 + i;
				std::complex< double > intermediateValue = _input[idx1] - _input[idx2];
				_input[idx1] += _input[idx2];
				_input[idx2] = intermediateValue * W;
			}
			W *= stageW;
		}
	}

	// Swapping the output to the correct index using bit reversal
	// Found this from stackoverflow
	size_t shiftBy = std::log2(_input.size());
	for (size_t i = 0; i < _input.size(); ++i)
	{
		size_t idx = i;

		idx = ((idx & 0xAAAAAAAA) >> 1) | ((idx & 0x55555555) << 1);
		idx = ((idx & 0xCCCCCCCC) >> 2) | ((idx & 0x33333333) << 2);
		idx = ((idx & 0xF0F0F0F0) >> 4) | ((idx & 0x0F0F0F0F) << 4);
		idx = ((idx & 0xFF00FF00) >> 8) | ((idx & 0x00FF00FF) << 8);
		idx = ((idx >> 16) | (idx << 16)) >> (32 - shiftBy);

		// Check if idx is larger than i so that swap only happens once
		if (idx > i)
		{
			std::swap(_input[i], _input[idx]);
		}
	}
}

void MyCanvas::OnFastFourierTransform(std::vector <std::complex<double>>& c)
{
	int paddingT = 0;
	int paddingL = 0;
	int W = m_imageWidth, H = m_imageHeight;

	// If the power of the width is not even
	if (!(m_imageWidth && !(m_imageWidth & (m_imageWidth - 1))))
	{
		W--;
		W |= W >> 1;
		W |= W >> 2;
		W |= W >> 4;
		W |= W >> 8;
		W |= W >> 16;
		W++;
		paddingL = W - m_imageWidth;
		paddingL >>= 1;
	}

	// If the power of the height is not even
	if (!(m_imageHeight && !(m_imageHeight & (m_imageHeight - 1))))
	{
		H--;
		H |= H >> 1;
		H |= H >> 2;
		H |= H >> 4;
		H |= H >> 8;
		H |= H >> 16;
		H++;
		paddingT = H - m_imageHeight;
		paddingT >>= 1;
	}

	std::vector<std::vector<std::complex<double>>> f_xy(H);
	for (int y = 0; y < H; y++)
	{
		f_xy[y].resize(W, std::complex<double>(0, 0));
	}

	for (int y2 = paddingT, y = 0; y < m_imageHeight; y2++, y++)
	{
		for (int x2 = paddingL, x = 0; x < m_imageWidth; x2++, x++)
		{
			f_xy[y2][x2] = std::complex<double>(m_myImage[imageIndex][y * m_imageWidth + x].r, 0);
		}
	}

	int imageSize = W * H;
	double minPixVal = std::numeric_limits< double >::max();
	double maxPixVal = std::numeric_limits< double >::min();

	std::vector< double > magnitude;
	magnitude.resize(imageSize);

	std::vector< std::complex< double > > intermediateValues;
	intermediateValues.resize(H);

	std::vector<Pixel*> fft;
	fft.push_back((Pixel*)malloc(imageSize * 3));

	c.resize(imageSize);

	for (int x = 0; x < W; ++x)
	{
		for (int y = 0; y < H; ++y)
		{
			intermediateValues[y] =
				f_xy[y][x] * std::pow(-1, x + y);
		}

		FFT_1D(intermediateValues);

		for (int y = 0; y < H; ++y)
		{
			c[y * W + x] = intermediateValues[y];
		}
	}

	intermediateValues.clear();
	intermediateValues.resize(W);

	for (int row = 0; row < H; ++row)
	{
		for (int col = 0; col < W; ++col)
		{
			intermediateValues[col] = c[row * W + col];
		}

		FFT_1D(intermediateValues);

		for (int col = 0; col < W; ++col)
		{
			c[row * W + col] = intermediateValues[col];

			double finalPixVal = std::log10(std::abs(c[row * W + col]) + 1.0);
			magnitude[row * W + col] = finalPixVal;

			if (minPixVal > finalPixVal)
				minPixVal = finalPixVal;

			if (maxPixVal < finalPixVal)
				maxPixVal = finalPixVal;
		}
	}

	double minmaxDiff = maxPixVal - minPixVal;
	for (size_t idx = 0; idx < imageSize; ++idx)
	{
		Pixel& currPix = fft[0][idx];

		currPix.r = currPix.g = currPix.b =
			static_cast<unsigned char>((magnitude[idx] - minPixVal) * 255 / minmaxDiff);
	}

	imageIndex = m_myImage.size();
	m_myImage.push_back(fft[0]);
	m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
	Refresh();
}

void MyCanvas::OnInverseFastFourierTransform(std::vector <std::complex<double>>& c)
{
	FourierTransformDialog dialog(this, wxID_ANY, _("Inverse Fourier Transform Settings"));

	if (dialog.ShowModal() == wxID_OK)
	{
		int width = m_imageWidth;
		int height = m_imageHeight;
		int imageSize = width * height;

		std::vector< std::complex< double > >intermediateValues;
		intermediateValues.resize(height);

		std::vector<Pixel*> ifft;
		ifft.push_back((Pixel*)malloc(imageSize * 3));

		for (int col = 0; col < width; ++col)
		{
			for (int row = 0; row < height; ++row)
			{
				intermediateValues[row] = c[row * width + col];
			}

			FFT_1D(intermediateValues, 1.0);

			for (int row = 0; row < height; ++row)
			{
				c[row * width + col] = intermediateValues[row];
			}
		}

		intermediateValues.clear();
		intermediateValues.resize(width);

		for (int row = 0; row < height; ++row)
		{
			for (int col = 0; col < width; ++col)
			{
				intermediateValues[col] = c[row * width + col];
			}

			FFT_1D(intermediateValues, 1.0);

			for (int col = 0; col < width; ++col)
			{
				c[row * width + col] = intermediateValues[col];

				Pixel& currPix = ifft[0][row * width + col];
				currPix.r = currPix.g = currPix.b =
					static_cast<unsigned char>(std::abs(intermediateValues[col] /= imageSize));
			}
		}

		imageIndex = m_myImage.size();
		m_myImage.push_back(ifft[0]);
		m_imageRGB->SetData(reinterpret_cast<unsigned char*>(m_myImage[imageIndex]), true);
		Refresh();
	}
}

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
	void OnAddition(wxCommandEvent& WXUNUSED(event));
	void OnSubtraction(wxCommandEvent& WXUNUSED(event));
	void OnMultiply(wxCommandEvent& WXUNUSED(event));
	void OnDivision(wxCommandEvent& WXUNUSED(event));
	void OnImageNegative(wxCommandEvent& WXUNUSED(event));
	void OnLogTransform(wxCommandEvent& WXUNUSED(event));
	void OnGammaTransform(wxCommandEvent& WXUNUSED(event));
	void OnHistogramEqualization(wxCommandEvent& WXUNUSED(event));
	void OnGaussianSmootheringFilter(wxCommandEvent& WXUNUSED(event));
	void OnSobelEdgeDetection(wxCommandEvent& WXUNUSED(event));
	void OnDiscreteFourierTransform(wxCommandEvent& WXUNUSED(event));
	void OnSeparableFourierTransform(wxCommandEvent& WXUNUSED(event));
	void OnFastFourierTransform(wxCommandEvent& WXUNUSED(event));
	void OnClose(wxCloseEvent& event);

	MyCanvas* m_canvas; // the canvas inside the main frame
	wxBitmap m_imageBitmap;	// used to display the image

	bool m_imageLoaded;
	bool saved = false;

	wxSize frameSize;
	wxString filename;

	wxArrayString   m_arrItems;

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
EVT_MENU(ADDITION, MyFrame::OnAddition)
EVT_MENU(SUBTRACTION, MyFrame::OnSubtraction)
EVT_MENU(MULTIPLY, MyFrame::OnMultiply)
EVT_MENU(DIVISION, MyFrame::OnDivision)
EVT_MENU(NEGATIVE, MyFrame::OnImageNegative)
EVT_MENU(LOG, MyFrame::OnLogTransform)
EVT_MENU(GAMMA, MyFrame::OnGammaTransform)
EVT_MENU(HISTOGRAM, MyFrame::OnHistogramEqualization)
EVT_MENU(GAUSSIAN, MyFrame::OnGaussianSmootheringFilter)
EVT_MENU(SOBEL, MyFrame::OnSobelEdgeDetection)
EVT_MENU(DFT_BRUTEFORCE, MyFrame::OnDiscreteFourierTransform)
EVT_MENU(DFT_SEPARABLE, MyFrame::OnSeparableFourierTransform)
EVT_MENU(FFT, MyFrame::OnFastFourierTransform)
EVT_CLOSE(MyFrame::OnClose)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame("CS370_GOHWEIZHE_A3");
	frame->SetPosition(wxPoint(0, 0));
	frame->SetSize(350, 350);

	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

MyFrame::MyFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title)
{
	// set the frame icon
	SetIcon(wxICON(sample));

	filename = "";

#if wxUSE_MENUS
	wxMenuBar* menuBar = new wxMenuBar();
	wxMenu* filemenu = new wxMenu();

	wxMenu* helpMenu = new wxMenu();
	wxMenu* imageRenderingMenu = new wxMenu();
	wxMenu* interpolationMenu = new wxMenu();

	wxMenu* editMenu = new wxMenu();
	wxMenu* imageOperationMenu = new wxMenu();
	wxMenu* filterMenu = new wxMenu();
	wxMenu* fourierTransformMenu = new wxMenu();

	filemenu->Append(SAVE, "&Save", "Save");
	filemenu->Append(SAVEAS, "Save As", "Save new image");
	filemenu->Append(OPEN, "&Open\tAlt-O", "Open image");
	filemenu->Append(QUIT, "E&xit\tAlt-X", "Quit this program");

	interpolationMenu->Append(NEARESTNEIGHBOUR, "Nearest Neighbour");
	interpolationMenu->Append(BILINEAR, "Bilinear");
	interpolationMenu->Append(BICUBIC, "Bicubic");

	imageRenderingMenu->AppendSubMenu(interpolationMenu, ("&Interpolation"));

	imageOperationMenu->Append(ADDITION, "&Addition", "Perform the addition of the intensities of the two images");
	imageOperationMenu->Append(SUBTRACTION, "&Subtraction", "Subtract second image from first image");
	imageOperationMenu->Append(MULTIPLY, "&Mutiply", "Perform element-wise multiplication of the two images");
	imageOperationMenu->Append(DIVISION, "&Division", "Perform element-wise division of the two images");

	editMenu->Append(NEGATIVE, "&Image Negative", "Performs image negative operation");
	editMenu->Append(LOG, "&Log Transform", "Performs log transform operation");
	editMenu->Append(GAMMA, "&Gamma Transform", "Performs gamma transform operation");
	editMenu->Append(HISTOGRAM, "&Histogram Equalization", "Performs histogram equalization");

	filterMenu->Append(GAUSSIAN, "&Gaussian Blur Filter", "Performs gaussian smoothering filter");
	filterMenu->Append(SOBEL, "&Sobel Edge Detection", "Performs sobel edget detection");

	fourierTransformMenu->Append(DFT_BRUTEFORCE, "&DTF - Brute Force Method", "Performs direct fourier transform (DFT)");
	fourierTransformMenu->Append(DFT_SEPARABLE, "&DTF - Separable Method", "Performs fast fourier transform - Seperable Method");
	fourierTransformMenu->Append(FFT, "&FFT - Fast Fourier Transform", "Performs fast fourier transform - Butterfly Method");

	helpMenu->Append(ABOUT, "&About\tF1", "Show about dialog");

	menuBar->Append(filemenu, "&File");
	menuBar->Append(imageRenderingMenu, "&Image Rendering");
	menuBar->Append(imageOperationMenu, "&Image Operation");
	menuBar->Append(editMenu, "&Edit");
	menuBar->Append(filterMenu, "&Filter");
	menuBar->Append(fourierTransformMenu, "&Fourier Transform");
	menuBar->Append(helpMenu, "&Help");
	SetMenuBar(menuBar);

	// create the canvas that will manage the image
	m_canvas = new MyCanvas(this, -1, wxDefaultPosition, wxSize(1, 2));
	m_canvas->SetBackgroundColour(wxColor(*wxWHITE));

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
}

void MyFrame::OnNearestNeighbourInterpolation(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->interpolate = true;
	m_canvas->m_resizeQuality = wxIMAGE_QUALITY_NEAREST;
	Refresh();
}

void MyFrame::OnBilinearInterpolation(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->interpolate = true;
	m_canvas->m_resizeQuality = wxIMAGE_QUALITY_BILINEAR;
	Refresh();
}

void MyFrame::OnBicubicInterpolation(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->interpolate = true;
	m_canvas->m_resizeQuality = wxIMAGE_QUALITY_BICUBIC;
	//wxLogMessage(" Bicubic Interpolation ");
	Refresh();
}

void MyFrame::OnAddition(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnAddition();
}

void MyFrame::OnSubtraction(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnSubtraction();
}

void MyFrame::OnMultiply(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnMultiply();
}

void MyFrame::OnDivision(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnDivision();
}

void MyFrame::OnImageNegative(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnImageNegative();
}

void MyFrame::OnLogTransform(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnLogTransform();
}

void MyFrame::OnGammaTransform(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnGammaTransform();
}

void MyFrame::OnHistogramEqualization(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnHistogramEqualization();
}

void MyFrame::OnGaussianSmootheringFilter(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnGaussianSmootheringFilter();
}

void MyFrame::OnSobelEdgeDetection(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->OnSobelEdgeDetection();
}

void MyFrame::OnDiscreteFourierTransform(wxCommandEvent& WXUNUSED(event))
{
	if (!m_canvas->m_myImage.empty())
	{
		std::vector <std::complex<double>> c;
		m_canvas->OnDiscreteFourierTransform(c);
		m_canvas->OnInverseDiscreteFourierTransform(c);
	}
}

void MyFrame::OnSeparableFourierTransform(wxCommandEvent& WXUNUSED(event))
{
	if (!m_canvas->m_myImage.empty())
	{
		std::vector <std::complex<double>> c;
		m_canvas->OnSeparableFourierTransform(c);
		m_canvas->OnInverseSeparableFourierTransform(c);
	}
}

void MyFrame::OnFastFourierTransform(wxCommandEvent& WXUNUSED(event))
{
	if (!m_canvas->m_myImage.empty())
	{
		std::vector <std::complex<double>> c;
		m_canvas->OnSeparableFourierTransform(c);
		m_canvas->OnInverseSeparableFourierTransform(c);
	}
}