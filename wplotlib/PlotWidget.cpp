#include "PlotWidget.h"

PlotWidget::PlotWidget(wxWindow *parent)
    : wxPanel(parent), _pDataSet(nullptr), _minval(0.0), _maxVal(0.0),
      _nSamples(0), _sampleInterval(0.0), _timeWindow(0.0) {

  m_parent = parent;
  _border.down = 30;
  _border.left = 60;
  _border.right = 15;
  _border.up = 15;

  // Bind events
  Bind(wxEVT_PAINT, &PlotWidget::OnPaint, this);
  Bind(wxEVT_SIZE, &PlotWidget::OnSize, this);
  Bind(wxEVT_MOTION, &PlotWidget::OnMouseMovedEvent, this);
}

PlotWidget::~PlotWidget() {
  if (_pDataSet != nullptr) {
    for (int i = 0; i < _nSamples; i++) {
      delete _pixel_coordinates[i];
    }
  }
}

void PlotWidget::setDataSet(PDataSet dataset) noexcept {
  _pDataSet = dataset;
  _minval = _pDataSet->minValue();
  _maxVal = _pDataSet->maxValue();
  _sampleInterval = _pDataSet->getSampleInterval();
  _nSamples = _pDataSet->getNumberOfSamples();
  _timeWindow = _pDataSet->getTimeWindow();

  for (auto &pt : _pixel_coordinates) {
    delete pt;
  }
  for (int i = 0; i < _nSamples; i++) {
    _pixel_coordinates.Append(new wxPoint(0, 0));
  }
}

PDataSet PlotWidget::getDataSet() noexcept { return _pDataSet; }

void PlotWidget::OnSize(wxSizeEvent &event) {
  caculatePixelCoordinates(GetSize(),_border);
  Refresh();
}

void PlotWidget::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);
  wxBufferedDC bdc(&dc); // bufferedDC to avoid flickering

  // set background color
  bdc.SetPen(wxPen(wxColour(255, 255, 255)));
  bdc.SetBrush(wxBrush(wxColour(255, 255, 255)));
  bdc.DrawRectangle(0, 0, _window_size.x, _window_size.y);

  // draw axis rectangle
  bdc.SetPen(wxPen(wxColour(0, 0, 0), 2)); // black
  bdc.SetBrush(wxBrush(wxColour(255, 255, 220)));
  bdc.DrawRectangle(wxRect(_border.left, _border.up,
                           _window_size.x - _border.right - _border.left,
                           _window_size.y - _border.down - _border.up));

  if (_pDataSet == nullptr)
    return;

  // drawGrid(bdc, 10, 10, size);
  drawData(bdc);
  drawCrosshair(bdc);
}

void PlotWidget::OnMouseMovedEvent(wxMouseEvent &event) {
  _mouse_pos = event.GetPosition();
  this->Refresh(false);
}

int PlotWidget::map_value(double val, double max, double min,
                          int window_size) noexcept {
  return (val / (max + (-1.0 * min)) * window_size);
}

void PlotWidget::caculatePixelCoordinates(
    const wxSize &windowSize,
    const PlotBorder &border) noexcept { // Cache data pixel
  if (windowSize != _window_size) {
    _window_size = windowSize;

    const int xpos = ((_maxVal / (_maxVal + (-1.0) * _minval)) * windowSize.y +
                      border.up - border.down);

    for (int i = 0; i < _nSamples; i++) {
      _pixel_coordinates[i]->x =
          map_value(i * _sampleInterval, _timeWindow, 0,
                    windowSize.x - border.left - border.right) +
          border.left;

      _pixel_coordinates[i]->y =
          map_value(_pDataSet->at(i), _maxVal, _minval,
                    windowSize.y - border.up - border.down * 2) +
          xpos + 5;
    }
  }
}

void PlotWidget::drawData(wxBufferedDC &bdc) noexcept {
  // draw data
  bdc.SetPen(wxPen(wxColour(0, 0, 255), 1));
  bdc.DrawLines(&_pixel_coordinates);
}

void PlotWidget::drawCrosshair(wxBufferedDC &bdc) noexcept {
  // draw crosshair
  bdc.SetPen(wxPen(wxColour(0, 255, 0), 1));
  bdc.CrossHair(_mouse_pos);
}

void PlotWidget::drawGrid(wxBufferedDC &bdc, int cols, int rows,
                          const wxSize &windowSize, const PlotBorder & border) noexcept {
  bdc.SetPen(wxPen(wxColour(200, 200, 200), 1, wxPENSTYLE_SHORT_DASH));
  int rowSpacing = (windowSize.y - border.down - border.up) / rows;
  int colSpacing = (windowSize.x - border.right - border.left) / cols;
  int limit_x = windowSize.x - border.right;
  int limit_y = windowSize.y - border.left;
  int offset_x = border.left;
  // draw rows
  for (int i = 0; i < rows; i++) {
    int offset_y = i * rowSpacing + border.up;
    bdc.DrawLine(offset_x, offset_y, limit_x, offset_y);
  }

  // draw cols
  int offset_y = border.up;
  for (int i = 0; i < rows; i++) {
    int offset_x = i * colSpacing + border.right;
    bdc.DrawLine(offset_x, offset_y, offset_x, limit_y);
  }
}
