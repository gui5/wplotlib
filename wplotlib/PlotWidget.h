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
  const int left;
  const int right;
  const int up;
  const int down;

  const int width;
  const int height;

  PlotBorder() : left(0), right(0), up(0), down(0), width(0), height(0) {}

  PlotBorder(int left, int right, int up, int down)
      : left(left), right(right), up(up), down(down), width(left + right),
        height(up + down) {}

  ~PlotBorder() = default;
};

struct PlotGrid {
  int rows;
  int cols;
  wxColour lineColor;
  wxPenStyle penStyle;

  PlotGrid(int rows, int cols, const wxColour &lineColor,
           const wxPenStyle &penStyle)
      : rows(rows), cols(cols), lineColor(lineColor), penStyle(penStyle) {}

  ~PlotGrid() = default;
};

// Limited lenght plot, use when the dataset have a known lenght;
class PlotWidget : public wxPanel {
public:
  PlotWidget(wxWindow *parent);

  ~PlotWidget();

  wxWindow *m_parent;

  void setDataSet(PDataSet dataset) noexcept;
  PDataSet getDataSet() noexcept;

 
private:
  wxPointList _pixel_coordinates;
  PDataSet _pDataSet;

  PlotBorder _border;
  PlotStyle _style;
  PlotGrid _grid;

  double _minval;
  double _maxVal;
  double _sampleInterval;
  int _nSamples;
  double _timeWindow;

  wxSize _window_size;
  wxPoint _mouse_pos;

   // Event handlers
  void OnSize(wxSizeEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnMouseMovedEvent(wxMouseEvent &event);

  int map_value(double val, double max, double min, int window_size) noexcept;
  void caculatePixelCoordinates(const wxSize &windowSize,
                                const PlotBorder &border) noexcept;
  void drawData(wxBufferedDC &bdc) noexcept;
  void drawCrosshair(wxBufferedDC &bdc) noexcept;
  void drawGrid(wxBufferedDC &bdc, const PlotGrid &grid, wxSize &windowSize,
                const PlotBorder &border) noexcept;

};
