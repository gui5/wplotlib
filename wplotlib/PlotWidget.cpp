#include "PlotWidget.h"

wxDEFINE_EVENT(PLOT_MOUSE_HOVER_EVENT, MouseHoverEvent);

PlotWidget::PlotWidget(wxWindow *parent)
    : wxPanel(parent), _pDataSet(nullptr), _border(60, 15, 20, 30),
      _drawRegion(_border, parent->GetSize()), m_parent(parent),
      _scale(0, 1, -1, 1), _grid(8, 10, PlotGridStyle()),
      _sx(_pixelTransformMatrix(0, 0)), _sy(_pixelTransformMatrix(1, 1)),
      _offy(_pixelTransformMatrix(1, 2)), _offx(_pixelTransformMatrix(0, 2)) {

  _pixelTransformMatrix(2, 2) = 1.0;

  _windowSize = parent->GetSize();

  _renderLabels = true;

  // Bind events
  Bind(wxEVT_PAINT, &PlotWidget::OnPaint, this);
  Bind(wxEVT_SIZE, &PlotWidget::OnSize, this);
  Bind(wxEVT_MOTION, &PlotWidget::OnMouseMovedEvent, this);
  Refresh();
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

  for (int i = 0; i < dataset->size(); i++) {
    _pixelCoordinates.Append(new wxPoint());
  }

  scalePlot();
}

PDataSet PlotWidget::getDataSet() noexcept { return _pDataSet; }

void PlotWidget::enableGrid(bool value) noexcept {
  _renderGrid = value;
  if (_renderGrid) {
    _grid.update(_drawRegion);
    Refresh();
  }
}

void PlotWidget::setGrid(const PlotGrid &grid) noexcept {
  _grid = grid;
  _grid.update(_drawRegion);
  Refresh();
}

void PlotWidget::enableCrossHair(bool value) noexcept {
  _renderCrossHair = value;
}

void PlotWidget::setPlotScale(const PlotScale &scale) noexcept {
  _scale = scale;
  scalePlot();
}

void PlotWidget::OnSize(wxSizeEvent &event) {
  scalePlot();
  _labels.cacheLabels(_grid, _pixelTransformMatrix, _scale);
}

void PlotWidget::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);
  wxBufferedDC bdc(&dc); // bufferedDC to avoid flickering

  // set background color

  bdc.SetPen(_style.windowColor);
  bdc.SetBrush(wxBrush(_style.windowColor));
  bdc.DrawRectangle(0, 0, _windowSize.x, _windowSize.y);

  // draw axis rectangle
  bdc.SetPen(wxPen(_style.borderColor, 2));      // black
  bdc.SetBrush(wxBrush(_style.backgroundColor)); // yellow
  bdc.DrawRectangle(_drawRegion.x, _drawRegion.y, _drawRegion.width,
                    _drawRegion.height);

  if (_pDataSet == nullptr)
    return;

  if (_renderGrid)
    drawGrid(bdc);

  drawData(bdc);

  if (_renderCrossHair)
    drawCrosshair(bdc);

  if (_renderLabels)
    drawLables(bdc);
}

void PlotWidget::OnMouseMovedEvent(wxMouseEvent &event) {
  _mousePos = event.GetPosition();
  Eigen::Vector3d px = toData(_mousePos);
  wxPostEvent(m_parent, MouseHoverEvent(wxRealPoint(px(0), px(1))));

  if (_renderCrossHair) {
    this->Refresh(false);
  }
}

void PlotWidget::drawData(wxBufferedDC &bdc) noexcept {
  // draw data
  bdc.SetPen(wxPen(_style.lineColor, 1));
  bdc.DrawLines(&_pixelCoordinates);
}

void PlotWidget::drawCrosshair(wxBufferedDC &bdc) noexcept {
  // draw crosshair
  bdc.SetPen(wxPen(_style.crossHairColor, 1));
  bdc.CrossHair(_mousePos);
}

void PlotWidget::drawGrid(wxBufferedDC &bdc) noexcept {

  bdc.SetPen(_grid.style());
  auto &vLines = _grid.vLines();
  auto &hLines = _grid.hLines();
  for (int i = 0; i < _grid.maxcr(); i++) {
    if (i < hLines.size()) {
      const auto &h = hLines[i];
      bdc.DrawLine(h.p1, h.p2);
    }
    if (i < vLines.size()) {
      const auto &v = vLines[i];
      bdc.DrawLine(v.p1, v.p2);
    }
  }
}

void PlotWidget::drawLables(wxBufferedDC &bdc) noexcept {
  for (auto &xlabel : _labels.xLabel) {
    bdc.DrawText(xlabel.value, xlabel.position);
  }
  for (auto &ylabel : _labels.yLabel) {
    bdc.DrawText(ylabel.value, ylabel.position);
  }
}

void PlotWidget::scalePlot() noexcept {
  const auto sz = GetSize();
  if (sz != _windowSize) {
    _windowSize = sz;
    _drawRegion.Update(_border, _windowSize);

    computeTransformationMatrices();

    _zeroPos = toPixel(Eigen::Vector3d(0, 0, 1));

    Eigen::Vector3d px;

    for (int i = 0; i < _pDataSet->size(); i++) {
      px = toPixel(_pDataSet->at(i));
      _pixelCoordinates[i]->x = px(0);
      _pixelCoordinates[i]->y = px(1);
    }

    _grid.update(_drawRegion);

    Refresh();
  }
}

void PlotWidget::computeTransformationMatrices() noexcept {
  _sx = _drawRegion.width / _scale.xSpan();
  _sy = -1.0 * _drawRegion.height / _scale.ySpan();
  _offx = (double)_drawRegion.x;
  _offy = (double)_drawRegion.y + abs(_scale.yMax() * abs(_sy));
}

Eigen::Vector3d PlotWidget::toPixel(const Eigen::Vector3d &data) noexcept {
  return _pixelTransformMatrix * data;
}

Eigen::Vector3d PlotWidget::toData(const wxPoint &pixel) noexcept {
  return _pixelTransformMatrix.inverse() *
         Eigen::Vector3d((double)pixel.x, (double)pixel.y, 1.0);
}
