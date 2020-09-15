#include "MainFrame.h"
#include <fmt/format.h>

#pragma comment(lib, "wplotlib")

MainFrame::MainFrame()
    : wxFrame(nullptr, -1, "Plot Example", wxDefaultPosition,
              wxSize(1280, 720)) {

  buildMenu();

  _plot = new PlotWidget(this, CreateStatusBar(2));
  _plot->setDataSet(createSineWave(1, 10, 0, 50));

  Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnMenuSelected, this);
}

void MainFrame::OnMenuSelected(wxCommandEvent &event) noexcept {

  switch (event.GetId()) {
  case wxID_EXIT:
    Close(true);
    break;

  case (int)MenuItemId::ID_VIEW_GRID:
    _plot->enableGrid(_menuView->IsChecked((int)MenuItemId::ID_VIEW_GRID));
    break;

  case (int)MenuItemId::ID_VIEW_CROSSHAIR:
    _plot->enableCrossHair(
        _menuView->IsChecked((int)MenuItemId::ID_VIEW_CROSSHAIR));
    break;

  default:
    break;
  }
}

PDataSet MainFrame::createSineWave(double amplitute, double frequency,
                                   double phase, int samplesCount) {

  const double pi = 3.1415926535;
  const double sampleInterval = 1.0 / (20 * frequency);

  auto dataset = DataSetBuilder::createDataSet(samplesCount, sampleInterval);

  for (int i = 0; i < samplesCount; i++) {
    dataset->insert(Eigen::Vector2d(
        i * sampleInterval,
        amplitute * sin(2.0 * pi * frequency * i * sampleInterval +
                        (phase * pi / 180))));
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
