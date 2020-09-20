#include "MainFrame.h"
#include <fmt/format.h>

#pragma comment(lib, "wplotlib")

MainFrame::MainFrame()
    : wxFrame(nullptr, -1, "Plot Example", wxDefaultPosition,
              wxSize(1280, 720)) {
  buildMenu();
  auto sb = CreateStatusBar(2);

  _plot = new PlotWidget(this);
  _plot->setDataSet(createSineWave(1, 10, 0, 200));
  _plot->setPlotScale(PlotScale(0, 1, 0, 3));
  _plot->setGrid(PlotGrid(12, 10, PlotGridStyle()));

  wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

  topsizer->Add(_plot, 1, wxEXPAND | wxALL, 0);

  SetSizer(topsizer);

  Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnMenuSelected, this);
  Bind(PLOT_MOUSE_HOVER_EVENT, &MainFrame::OnPlotMouseHover, this);
}

void MainFrame::OnMenuSelected(wxCommandEvent &event) noexcept {

  switch (event.GetId()) {
  case wxID_EXIT:
    Close(true);
    break;

  case (int)MenuItemId::ID_VIEW_GRID:
    _plot->enableGrid(_menuView->IsChecked((int)MenuItemId::ID_VIEW_GRID));
    _plot->Refresh();
    break;

  case (int)MenuItemId::ID_VIEW_CROSSHAIR:
    _plot->enableCrossHair(
        _menuView->IsChecked((int)MenuItemId::ID_VIEW_CROSSHAIR));
    _plot->Refresh();
    break;

  default:
    break;
  }
}

void MainFrame::OnPlotMouseHover(MouseHoverEvent &event) noexcept {
  const auto data = event.data;
  SetStatusText(fmt::format("({:f} , {:f})", data.x, data.y), 1);
}

PDataSet MainFrame::createSineWave(double amplitute, double frequency,
                                   double phase, int samplesCount) {

  const double pi = 3.1415926535;
  const double sampleInterval = 1.0 / (20 * frequency);

  auto dataset = DataSetBuilder::createDataSet(samplesCount, sampleInterval);

  for (int i = 0; i < samplesCount; i++) {
    dataset->insert(Eigen::Vector3d(
        i * sampleInterval,
        amplitute *
            sin(2.0 * pi * frequency * i * sampleInterval + (phase * pi / 180)),
        1.0));
  }
  return dataset;
}

PDataSet MainFrame::CreatePulseDcWave(double amplitute, double frequency,
                                      double phase, int samplesCount) {
  const double pi = 3.1415926535;
  const double sampleInterval = 1.0 / (20 * frequency);

  auto dataset = DataSetBuilder::createDataSet(samplesCount, sampleInterval);

  for (int i = 0; i < samplesCount; i++) {
    dataset->insert(Eigen::Vector3d(
        i * sampleInterval,
        abs(amplitute * sin(2.0 * pi * frequency * i * sampleInterval +
                            (phase * pi / 180))),
        1.0));
  }
  return dataset;
}

void MainFrame::buildMenu() noexcept {
  _menuBar = new wxMenuBar;
  _menuFile = new wxMenu;
  _menuView = new wxMenu;
  _menuStyle = new wxMenu;

  _menuFile->Append((int)MenuItemId::ID_MENU_SAVE, "Save");
  _menuFile->Append((int)MenuItemId::ID_MENU_LOAD, "Load");
  _menuFile->AppendSeparator();
  _menuFile->Append(wxID_EXIT, "Exit");

  _menuView->Append((int)MenuItemId::ID_VIEW_GRID, "Grid",
                    "Enables/disables the plot grid", true);
  _menuView->Append((int)MenuItemId::ID_VIEW_CROSSHAIR, "Crosshair",
                    "Enables/disables crosshair", true);

  _menuStyle->Append((int)MenuItemId::ID_STYLE_GRID, "Grid...",
                     "Edit grid style");
  _menuStyle->Append((int)MenuItemId::ID_STYLE_PLOT, "Plot...",
                     "Edit plot style");
  _menuStyle->Append((int)MenuItemId::ID_STYLE_WINDOW, "Window...",
                     "Edit window style");
  _menuStyle->Append((int)MenuItemId::ID_STYLE_LABELS, "Labels...",
                     "Edit labels style");

  _menuBar->Append(_menuFile, "File");
  _menuBar->Append(_menuView, "View");
  _menuBar->Append(_menuStyle, "Style");
  SetMenuBar(_menuBar);
}
