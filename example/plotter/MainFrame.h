#pragma once
#include "../wplotlib/PlotWidget.h"
#include <atomic>
#include <cmath>
#include <wx/wx.h>

enum class MenuItemId {
  ID_VIEW_GRID = 1000,
  ID_VIEW_CROSSHAIR,
  ID_STYLE_PLOT,
  ID_STYLE_WINDOW,
  ID_STYLE_GRID,
  ID_STYLE_LABELS,
  ID_MENU_LOAD,
  ID_MENU_SAVE
};

class MainFrame : public wxFrame {

public:
  MainFrame();
  ~MainFrame() = default;

private:
  wxSizer *_mainsizer;
  PlotWidget *_plot;

  // menu variables
  wxMenuBar *_menuBar;
  wxMenu *_menuFile;
  wxMenu *_menuView;
  wxMenu *_menuStyle;

  // event handlers
  void OnMenuSelected(wxCommandEvent &event) noexcept;
  void OnPlotMouseHover(MouseHoverEvent &event) noexcept;

  PDataSet createSineWave(double amplitute, double frequency, double phase,
                          int samplesCount);

  PDataSet CreatePulseDcWave(double amplitute, double frequency, double phase,
                             int samplesCount);

  void buildMenu() noexcept;
};
