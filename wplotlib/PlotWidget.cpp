#include "PlotWidget.h"

PlotWidget::PlotWidget(wxWindow *parent, wxStatusBar *statusBar)
    : wxPanel(parent), _pDataSet(nullptr), _minval(0.0), _maxVal(0.0),
      _nSamples(0), _sampleInterval(0.0), _timeWindow(0.0),
      _border(60, 15, 15, 30), _grid(10, 10, wxColour(200, 200, 200),
                                     wxPenStyle::wxPENSTYLE_SHORT_DASH) {

  _mainStatusBar = statusBar;
  m_parent = parent;
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

void PlotWidget::enableGrid(bool value) noexcept { _renderGrid = value; }

void PlotWidget::enableCrossHair(bool value) noexcept {
  _renderCrossHair = value;
}

void PlotWidget::OnSize(wxSizeEvent &event) {
  caculatePixelCoordinates(GetSize(), _border);
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
  bdc.DrawRectangle(_border.left, _border.up, _window_size.x - _border.width,
                    _window_size.y - _border.height);

  if (_pDataSet == nullptr)
    return;

  if (_renderGrid)
    drawGrid(bdc, _grid, _window_size, _border);

  drawData(bdc);

  if (_renderCrossHair)
    drawCrosshair(bdc);
}

void PlotWidget::OnMouseMovedEvent(wxMouseEvent &event) {
  _mouse_pos = event.GetPosition();
  if (_mainStatusBar != nullptr)
    _mainStatusBar->SetStatusText(
        fmt::format("({},{})", _mouse_pos.x, _mouse_pos.y), 1);
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

void PlotWidget::drawGrid(wxBufferedDC &bdc, const PlotGrid &grid,
                          wxSize &windowSize,
                          const PlotBorder &border) noexcept {
  bdc.SetPen(wxPen(_grid.lineColor, 1, grid.penStyle));
  const int yLenght = windowSize.y - border.height + border.up;
  const int xLenght = windowSize.x - border.width + border.left;
  const int xSpacing = xLenght / grid.cols;
  const int ySpacing = yLenght / grid.rows;

  wxPoint origin(border.left, border.up);

  // draw cols;
  for (int i = 0; i < grid.cols; i++) {
    const int cx = origin.x + xSpacing * i;
    bdc.DrawLine(cx, origin.y, cx, yLenght);
  }

  // draw rows
  for (int i = 0; i < grid.rows; i++) {
    const int cy = origin.y + ySpacing * i;
    bdc.DrawLine(origin.x, cy, xLenght, cy);
  }
}
