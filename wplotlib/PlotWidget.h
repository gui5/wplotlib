#pragma once
#include "DataSet.h"
#include <fmt/format.h>
#include <string>
#include <string_view>
#include <vector>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/wx.h>

struct LabelStyle {
  wxColour color;
  wxFont font;
};

struct PlotStyle {
  wxColour windowColor;
  wxColour plotBackgroundColor;
  wxColour crossHairColor;
  wxColour dataColor;
  LabelStyle labelStyle;
};

struct PlotBorder {
  int left;
  int right;
  int up;
  int down;

  PlotBorder() : left(0), right(0), up(0), down(0) {}

  PlotBorder(int left, int right, int up, int down)
      : left(left), right(right), up(up), down(down) {}

  ~PlotBorder() = default;
};

struct PlotGrid {
  int rows;
  int cols;
  int rowsSpacing;
  int colsSpacing;
};

// Limited lenght plot, use when the dataset have a known lenght;
class PlotWidget : public wxPanel {
public:
  PlotWidget(wxWindow *parent);

  ~PlotWidget();

  wxWindow *m_parent;

  void setDataSet(PDataSet dataset) noexcept;
  PDataSet getDataSet() noexcept;

  // Event handlers
  void OnSize(wxSizeEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnMouseMovedEvent(wxMouseEvent &event);

private:
  wxPointList _pixel_coordinates;
  PDataSet _pDataSet;

  PlotBorder _border;
  PlotStyle _style;

  double _minval;
  double _maxVal;
  double _sampleInterval;
  int _nSamples;
  double _timeWindow;

  wxSize _window_size;
  wxPoint _mouse_pos;

  int map_value(double val, double max, double min, int window_size) noexcept;

  void caculatePixelCoordinates(const wxSize &windowSize,
                                const PlotBorder &border) noexcept;
  void drawData(wxBufferedDC &bdc, const wxColour &lineColor) noexcept;
  void drawData(wxBufferedDC &bdc) noexcept;
  void drawCrosshair(wxBufferedDC &bdc) noexcept;
  void drawCrosshair(wxBufferedDC &bdc, const wxColour &lineColor) noexcept;

  // TODO: Optimize this trash and fix it
  void drawGrid(wxBufferedDC &bdc, int cols, int rows, const wxSize &windowSize,
                const PlotBorder &border) noexcept;

  void drawPlotBackground(wxBufferedDC &bdc, const wxColour &windowColor,
                          const wxColour &plotBackgroundColor) noexcept;

  void drawLabels(wxBufferedDC &bdc) noexcept;
};
