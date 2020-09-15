#include "PlotWidget.h"

PlotWidget::PlotWidget(wxWindow *parent, wxStatusBar *statusBar)
    : wxPanel(parent), _pDataSet(nullptr), _border(60, 15, 15, 30),
      _drawRegion(_border, parent->GetSize()), m_parent(parent),
      _scale(0, 1, -2, 2), _grid(8, 10, _drawRegion) {

  _windowSize.x = statusBar->GetSize().x;
  _windowSize.y = parent->GetSize().y - statusBar->GetSize().y * 3 - _border.up;

  _mainStatusBar = statusBar;

  // Bind events
  Bind(wxEVT_PAINT, &PlotWidget::OnPaint, this);
  Bind(wxEVT_SIZE, &PlotWidget::OnSize, this);
  Bind(wxEVT_MOTION, &PlotWidget::OnMouseMovedEvent, this);
}

PlotWidget::~PlotWidget() {
  if (_pDataSet != nullptr) {
    for (int i = 0; i < _pDataSet->size(); i++) {
      delete _pixelCoordinates[i];
    }
  }
}

void PlotWidget::setDataSet(PDataSet dataset) noexcept {
  _pDataSet = dataset;

  for (auto &pt : _pixelCoordinates) {
    delete pt;
  }

  Eigen::Vector2d px;

  for (int i = 0; i < _pDataSet->size(); i++) {
    px = _pixelTransformMatrix * _pDataSet->at(i) + _offsetVector;
    _pixelCoordinates.Append(new wxPoint(px(0), px(1)));
  }
}

PDataSet PlotWidget::getDataSet() noexcept { return _pDataSet; }

void PlotWidget::enableGrid(bool value) noexcept {
  _renderGrid = value;
  if (_renderGrid) {
    _grid.update(_drawRegion);
    Refresh();
  }
}

void PlotWidget::enableCrossHair(bool value) noexcept {
  _renderCrossHair = value;
}

void PlotWidget::setPlotScale(const PlotScale &scale) noexcept {
  _scale = scale;
  scalePlot();
}

void PlotWidget::OnSize(wxSizeEvent &event) { scalePlot(); }

void PlotWidget::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);
  wxBufferedDC bdc(&dc); // bufferedDC to avoid flickering

  // set background color
  bdc.SetPen(wxPen(wxColour(255, 255, 255)));
  bdc.SetBrush(wxBrush(wxColour(255, 255, 255)));
  bdc.DrawRectangle(0, 0, _windowSize.x, _windowSize.y);

  // draw axis rectangle
  bdc.SetPen(wxPen(wxColour(0, 0, 0), 2));        // black
  bdc.SetBrush(wxBrush(wxColour(255, 255, 220))); // yellow
  bdc.DrawRectangle(_drawRegion.x, _drawRegion.y, _drawRegion.width,
                    _drawRegion.height);

  if (_pDataSet == nullptr)
    return;

  if (_renderGrid)
    drawGrid(bdc, _grid, _drawRegion);

  drawData(bdc);

  if (_renderCrossHair)
    drawCrosshair(bdc);
}

void PlotWidget::OnMouseMovedEvent(wxMouseEvent &event) {
  _mousePos = event.GetPosition();
  if (_mainStatusBar != nullptr)
    _mainStatusBar->SetStatusText(
        fmt::format("({},{})", _mousePos.x, _mousePos.y), 1);
  this->Refresh(false);
}

void PlotWidget::drawData(wxBufferedDC &bdc) noexcept {
  // draw data
  bdc.SetPen(wxPen(wxColour(0, 0, 255), 1));
  bdc.DrawLines(&_pixelCoordinates);
}

void PlotWidget::drawCrosshair(wxBufferedDC &bdc) noexcept {
  // draw crosshair
  bdc.SetPen(wxPen(wxColour(0, 255, 0), 1));
  bdc.CrossHair(_mousePos);
}

void PlotWidget::drawGrid(wxBufferedDC &bdc, const PlotGrid &grid,
                          const DrawRegion &drawRegion) noexcept {

  bdc.SetPen(
      wxPen(wxColour(200, 200, 200), 1, wxPenStyle::wxPENSTYLE_SHORT_DASH));

  for (auto &row : grid.hLines) {
    bdc.DrawLine(row.first, row.second);
  }

  for (auto &col : grid.vLines) {
    bdc.DrawLine(col.first, col.second);
  }
}

void PlotWidget::scalePlot() noexcept {
  const auto sz = GetSize();
  if (sz != _windowSize) {

    _drawRegion.Update(_border, _windowSize);
    _pixelTransformMatrix(0, 0) = _drawRegion.width / _scale.xSpan();
    _pixelTransformMatrix(1, 1) = _drawRegion.height / _scale.ySpan();
    _offsetVector(0) = _drawRegion.x;
    _offsetVector(1) = (double)_drawRegion.y + (_drawRegion.height / 2.0);

    Eigen::Vector2d px;

    for (int i = 0; i < _pDataSet->size(); i++) {
      px = _pixelTransformMatrix * _pDataSet->at(i) + _offsetVector;
      _pixelCoordinates[i]->x = px(0);
      _pixelCoordinates[i]->y = px(1);
    }

    if (_renderGrid)
      _grid.update(_drawRegion);

    _windowSize = sz;
    Refresh();
  }
}
