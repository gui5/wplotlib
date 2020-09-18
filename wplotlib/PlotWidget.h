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
#include "PlotEvent.h"

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

  double xMin()const { return _xMin; }
  double xMax()const { return _xMax; }
  double yMin()const { return _yMin; }
  double yMax()const { return _yMax; }

  double ySpan()const { return _ySpan; }
  double xSpan()const { return _xSpan; }

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
  wxColour windowColor;
  wxColour backgroundColor;
  wxColour lineColor;
  wxColour dotColor;
  wxColour borderColor;
  wxColour crossHairColor;
  DataPointType dotType;
  DataLineType lineType;
  LabelStyle xLabelStyle;
  LabelStyle yLabelStyle;

  PlotStyle()
      : windowColor(255, 255, 255), backgroundColor(255, 255, 220),
        lineColor(0, 0, 255), dotColor(0, 0, 255), borderColor(0, 0, 0),
        crossHairColor(0, 200, 0), dotType(DataPointType::square),
        lineType(DataLineType::contunuous) {}
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

enum class GridLineType { contunous, dot, dash, longdash };
struct PlotGrid {
  struct Line {
    const wxPoint p1;
    const wxPoint p2;
    Line(int x1, int y1, int x2, int y2) : p1(x1, y1), p2(x2, y2) {}
    ~Line() = default;
  };

  int rows;
  int cols;
  int maxcr;
  std::vector<Line> hLines;
  std::vector<Line> vLines;

  void update(const DrawRegion &drawRegion) {
    const int xSpacing = drawRegion.width / cols;
    const int ySpacing = drawRegion.height / rows;
    int cx = 0;
    int cy = 0;

    hLines.clear();
    vLines.clear();

    for (int i = 0; i < rows; i++) {
      cy = drawRegion.y + ySpacing * i;
      hLines.emplace_back(drawRegion.x, cy, drawRegion.width + drawRegion.x,
                          cy);
    }

    for (int i = 0; i < cols; i++) {
      cx = drawRegion.x + xSpacing * i;
      vLines.emplace_back(cx, drawRegion.y, cx,
                          drawRegion.height + drawRegion.y);
    }
  }

  PlotGrid(int rows, int cols, const DrawRegion &drawRegion)
      : rows(rows), cols(cols) {
    update(drawRegion);
    maxcr = rows > cols ? rows : cols;
  }

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

  // Event handlers

  void OnSize(wxSizeEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnMouseMovedEvent(wxMouseEvent &event);

  // drawing functions

  void drawData(wxBufferedDC &bdc) noexcept;
  void drawCrosshair(wxBufferedDC &bdc) noexcept;
  void drawGrid(wxBufferedDC &bdc) noexcept;

  //Transformation functions

  void scalePlot() noexcept;
  void computeTransformationMatrices()noexcept;
  Eigen::Vector3d toPixel(const Eigen::Vector3d &data) noexcept;
  Eigen::Vector3d toData(const wxPoint &pixel) noexcept;
};
