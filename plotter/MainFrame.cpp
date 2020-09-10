#include "MainFrame.h"

#pragma comment(lib, "wplotlib")

MainFrame::MainFrame()
    : wxFrame(nullptr, -1, "Plot Example", wxDefaultPosition,
              wxSize(640, 480)) {

  _plot = new PlotWidget(this);
  auto dataset = createSineWave(1, 10, 0, 50);
  _plot->setDataSet(dataset);
}

PDataSet MainFrame::createSineWave(double amplitute, double frequency,
                                   double phase, int samplesCount) {

  const double pi = 3.1415926535;
  const double sampleInterval = 1.0 / (20 * frequency);

  auto dataset = DataSetBuilder::createDataSet(samplesCount, sampleInterval);

  for (int i = 0; i < samplesCount; i++) {
    dataset->insert(amplitute * sin(2.0 * pi * frequency * i * sampleInterval +
                                    (phase * pi / 180)));
  }
  return dataset;
}
