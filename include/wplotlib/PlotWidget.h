#pragma once
#include "DataSet.h"
#include "PlotEvent.h"
#include <eigen3/Eigen/Eigen>
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

  double xMin() const { return _xMin; }
  double xMax() const { return _xMax; }
  double yMin() const { return _yMin; }
  double yMax() const { return _yMax; }

  double ySpan() const { return _ySpan; }
  double xSpan() const { return _xSpan; }

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

struct CrossHairStyle {};

struct WindoStyle {};

struct PlotStyle {
  wxColour windowColor;
  wxColour backgroundColor;
  wxColour lineColor;
  wxColour dotColor;
  wxColour borderColor;
  wxColour crossHairColor;

  PlotStyle()
      : windowColor(255, 255, 255), backgroundColor(255, 255, 220),
        lineColor(0, 0, 255), dotColor(0, 0, 255), borderColor(0, 0, 0),
        crossHairColor(0, 200, 0) {}
  ~PlotStyle() = default;
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

struct PlotGridStyle {
public:
  PlotGridStyle(const wxColour &color, const wxPenStyle &penStyle,
                int lineThickness)
      : _color(color), _penStyle(penStyle), _lineThickness(lineThickness),
        _pen(color, lineThickness, penStyle) {}
  PlotGridStyle()
      : _color(200, 200, 200), _lineThickness(1),
        _penStyle(wxPenStyle::wxPENSTYLE_SHORT_DASH) {
    recomputePen();
  }

  ~PlotGridStyle() = default;

  void setColor(const wxColour &color) noexcept {
    _color = color;
    recomputePen();
  }

  void setLineThickness(int value) noexcept {
    _lineThickness = value;
    recomputePen();
  }

  void setPenStyle(const wxPenStyle &penStyle) noexcept {
    _penStyle = penStyle;
    recomputePen();
  }

  void setPen(const wxPen &pen) noexcept { _pen = pen; }

  operator wxPen &() { return _pen; }
  wxPen &pen() noexcept { return _pen; }
  wxColour &color() noexcept { return _color; }
  int lineThickness() noexcept { return _lineThickness; }
  wxPenStyle penStyle() noexcept { return _penStyle; }

private:
  wxColour _color;
  int _lineThickness;
  wxPenStyle _penStyle;
  wxPen _pen;

  void recomputePen() noexcept {
    _pen = wxPen(_color, _lineThickness, _penStyle);
  }
};
struct PlotGrid {
  struct Line {
    wxPoint p1;
    wxPoint p2;
    Line() {}
    Line(int x1, int y1, int x2, int y2) : p1(x1, y1), p2(x2, y2) {}
    ~Line() = default;
  };

  void setRowsNumber(int value) noexcept {
    _rows = value;
    _maxcr = _rows > _cols ? _rows : _cols;
  }

  void setColsNumber(int value) noexcept {
    _cols = value;
    _maxcr = _rows > _cols ? _rows : _cols;
  }

  void setStyle(const PlotGridStyle &value) noexcept { _style = value; }

  void update(const DrawRegion &drawRegion) {
    const int xSpacing = drawRegion.width / _cols;
    const int ySpacing = drawRegion.height / _rows;
    int cx = 0;
    int cy = 0;

    _hLines.clear();
    _vLines.clear();

    for (int i = 0; i <= _rows; i++) {
      cy = drawRegion.y + ySpacing * i;
      _hLines.emplace_back(drawRegion.x, cy, drawRegion.width + drawRegion.x,
                           cy);
    }

    for (int i = 0; i <= _cols; i++) {
      cx = drawRegion.x + xSpacing * i;
      _vLines.emplace_back(cx, drawRegion.y, cx,
                           drawRegion.height + drawRegion.y);
    }
  }

  PlotGrid(int rows, int cols, const PlotGridStyle &style)
      : _rows(rows), _cols(cols), _style(style) {
    _maxcr = rows > cols ? rows : cols;
  }

  ~PlotGrid() = default;

  int rows() noexcept { return _rows; }
  int cols() noexcept { return _cols; }
  int maxcr() noexcept { return _maxcr; }
  const std::vector<Line> &vLines() const noexcept { return _vLines; }
  const std::vector<Line> &hLines() const noexcept { return _hLines; }
  PlotGridStyle &style() noexcept { return _style; }

private:
  int _rows;
  int _cols;
  int _maxcr;
  std::vector<Line> _hLines;
  std::vector<Line> _vLines;
  PlotGridStyle _style;
};

struct LabelStyle {
  wxColour color;
  wxFont font;
  int size;
};

struct PlotLabel {
  struct Label {
    wxPoint position;
    std::string value;

    Label(const wxPoint &pos, std::string_view value)
        : position(pos), value(value) {}
    Label() {}

    ~Label() = default;
  };

  std::vector<Label> xLabel;
  std::vector<Label> yLabel;

  LabelStyle xLabelStyle;
  LabelStyle yLabelStyle;

  void cacheLabels(const PlotGrid &grid, Eigen::Matrix3d &pixelTransformMatrix,
                   const PlotScale &scale) noexcept {

    auto m = pixelTransformMatrix.inverse();

    xLabel.clear();
    yLabel.clear();
    Eigen::Vector3d val;
    auto &vLines = grid.vLines();

    for (auto &x : grid.vLines()) {
      val = m * Eigen::Vector3d(x.p2.x, x.p2.y, 1.0);
      auto &pos = x.p2;
      xLabel.emplace_back(wxPoint(pos.x - 15, pos.y),
                          fmt::format("{:.3f}", val(0)));
    }

    for (auto &y : grid.hLines()) {
      val = m * Eigen::Vector3d(y.p1.x, y.p1.y, 1.0);
      auto &pos = y.p1;
      yLabel.emplace_back(wxPoint(pos.x - 40, pos.y - 10),
                          fmt::format("{:+.3f}", val(1)));
    }
  }
};

// Limited lenght plot, use when the dataset have a known lenght;
class PlotWidget : public wxPanel {
public:
  PlotWidget(wxWindow *parent);

  ~PlotWidget();

  wxWindow *m_parent;

  void setDataSet(PDataSet dataset) noexcept;
  PDataSet getDataSet() noexcept;
  void enableGrid(bool value = false) noexcept;
  void setGrid(const PlotGrid &grid) noexcept;
  void enableCrossHair(bool value = false) noexcept;
  void setPlotScale(const PlotScale &scale) noexcept;

private:
  wxPointList _pixelCoordinates;
  PDataSet _pDataSet;
  PlotBorder _border;

  PlotLabel _labels;
  PlotStyle _style;
  PlotGrid _grid;
  PlotScale _scale;
  wxSize _windowSize;
  DrawRegion _drawRegion;

  wxPoint _mousePos;

  Eigen::Vector3d _zeroPos;

  /*
    transformation Matrices

    | sx 0  offx |
    | 0  sy offy |
    | 0  0   1   |

    sx = scale x
    sy = scale y
    offx = offset x
    offy = offset y

    P = A * D
    D = A^-1 *D

  */

  double &_sx;
  double &_sy;
  double &_offx;
  double &_offy;

  Eigen::Matrix3d _pixelTransformMatrix; // from data to pixel transfrom

  // control variables

  std::atomic_bool _renderGrid;
  std::atomic_bool _renderCrossHair;
  std::atomic_bool _renderLabels;

  // Event handlers

  void OnSize(wxSizeEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnMouseMovedEvent(wxMouseEvent &event);

  // drawing functions

  void drawData(wxBufferedDC &bdc) noexcept;
  void drawCrosshair(wxBufferedDC &bdc) noexcept;
  void drawGrid(wxBufferedDC &bdc) noexcept;
  void drawLables(wxBufferedDC &bdc) noexcept;

  // Transformation functions

  void scalePlot() noexcept;
  void computeTransformationMatrices() noexcept;
  Eigen::Vector3d toPixel(const Eigen::Vector3d &data) noexcept;
  Eigen::Vector3d toData(const wxPoint &pixel) noexcept;
};
