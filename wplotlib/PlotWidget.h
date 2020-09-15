#pragma once
#include "DataSet.h"
#include <Eigen/Eigen>
#include <atomic>
#include <fmt/format.h>
#include <string>
#include <string_view>
#include <vector>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/wx.h>

struct PlotScale {

  PlotScale(double xMin, double xMax, double yMin, double yMax)
      : _xMin(xMin), _xMax(xMax), _yMin(yMin), _yMax(yMax),
        _ySpan(abs(yMax) + abs(yMin)), _xSpan(abs(xMax) + abs(xMin)) {}
  PlotScale() : _xMin(0), _xMax(0), _yMin(0), _yMax(0), _ySpan(0), _xSpan(0) {}

  ~PlotScale() = default;

  void setXmin(double value) {
    _xMin = value;
    computeXspan();
  };
  void setXmax(double value) {
    _xMax = value;
    computeXspan();
  };
  void SetYmin(double value) {
    _yMin = value;
    computeYspan();
  };
  void setYmax(double value) {
    _yMax = value;
    computeYspan();
  };

  double xMin() { return _xMin; }
  double xMax() { return _xMax; }
  double yMin() { return _yMin; }
  double yMax() { return _yMax; }

  double ySpan() { return _ySpan; }
  double xSpan() { return _xSpan; }

private:
  double _xMin;
  double _xMax;
  double _yMin;
  double _yMax;
  double _ySpan;
  double _xSpan;

  void computeXspan() { _xSpan = abs(_xMax) + abs(_xMin); }
  void computeYspan() { _ySpan = abs(_yMax) + abs(_yMin); }
};

struct LabelStyle {
  wxColour color;
  wxFont font;
  int size;
};

struct CrossHairStyle {};

struct WindoStyle {};

enum class DataPointType { circle, square, triangle };
enum class DataLineType { contunuous, dots, dash, longdash };

struct PlotStyle {
  wxColour backgroundColor;
  wxColour lineColor;
  wxColour dotColor;
  DataPointType dotType;
  DataLineType lineType;
  LabelStyle xLabelStyle;
  LabelStyle yLabelStyle;
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

struct DrawRegion {
  int x;
  int y;
  int width;
  int height;

  DrawRegion() : x(0), y(0), width(0), height(0) {}
  DrawRegion(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}

  DrawRegion(const PlotBorder &border, const wxSize &windowSize) {
    x = border.left;
    y = border.up;
    width = windowSize.x - (border.left + border.right);
    height = windowSize.y - (border.up + border.down);
  }

  void Update(const PlotBorder &border, const wxSize &windowSize) {
    x = border.left;
    y = border.up;
    width = windowSize.x - (border.left + border.right);
    height = windowSize.y - (border.up + border.down);
  }

  ~DrawRegion() = default;
};

enum class GridLineType { contunous, dot, dash, longdash };
struct PlotGrid {
  int rows;
  int cols;
  std::vector<std::pair<wxPoint, wxPoint>> hLines;
  std::vector<std::pair<wxPoint, wxPoint>> vLines;


  void update(const DrawRegion &drawRegion) {
    const int xSpacing = drawRegion.width / cols;
    const int ySpacing = drawRegion.height / rows;
    int cx = 0;
    int cy = 0;

    hLines.clear();
    vLines.clear();

    for (int i = 0; i < rows; i++) {
      cy = drawRegion.y + ySpacing * i;
      hLines.push_back({wxPoint(drawRegion.x, cy),
                        wxPoint(drawRegion.width + drawRegion.x, cy)});
    }

    for (int i = 0; i < cols; i++) {
      cx = drawRegion.x + xSpacing * i;
      vLines.push_back({wxPoint(cx, drawRegion.y),
                        wxPoint(cx, drawRegion.height + drawRegion.y)});
    }
  }

  PlotGrid(int rows, int cols, const DrawRegion &drawRegion)
      : rows(rows), cols(cols) {
    update(drawRegion);
  }

  ~PlotGrid() = default;
};

// Limited lenght plot, use when the dataset have a known lenght;
class PlotWidget : public wxPanel {
public:
  PlotWidget(wxWindow *parent, wxStatusBar *statusBar = nullptr);

  ~PlotWidget();

  wxWindow *m_parent;

  void setDataSet(PDataSet dataset) noexcept;
  PDataSet getDataSet() noexcept;
  void enableGrid(bool value = false) noexcept;
  void enableCrossHair(bool value = false) noexcept;
  void setPlotScale(const PlotScale &scale) noexcept;

private:
  wxPointList _pixelCoordinates;
  PDataSet _pDataSet;
  PlotBorder _border;
  PlotStyle _style;
  PlotGrid _grid;
  PlotScale _scale;
  wxSize _windowSize;
  DrawRegion _drawRegion;


  wxPoint _mousePos;
  wxStatusBar *_mainStatusBar;

  Eigen::Matrix2d _pixelTransformMatrix;
  Eigen::Vector2d _offsetVector;

  // control variables

  std::atomic_bool _renderGrid;
  std::atomic_bool _renderCrossHair;

  // Event handlers

  void OnSize(wxSizeEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnMouseMovedEvent(wxMouseEvent &event);

  // drawing functions

  void drawData(wxBufferedDC &bdc) noexcept;
  void drawCrosshair(wxBufferedDC &bdc) noexcept;
  void drawGrid(wxBufferedDC &bdc, const PlotGrid &grid,
                const DrawRegion &drawRegion) noexcept;

  void scalePlot() noexcept;
};
