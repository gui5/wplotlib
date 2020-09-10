#pragma once
#include <wx/wx.h>
#include "../wplotlib/PlotWidget.h"
#include <cmath>

class MainFrame : public wxFrame {

public:
  MainFrame();
  ~MainFrame() = default;

private:
  wxSizer *_mainsizer;
  PlotWidget *_plot;

  PDataSet createSineWave(double amplitute, double frequency, double phase, int samplesCount);
};
