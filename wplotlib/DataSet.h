#pragma once
#include "Lock.h"
#include <cmath>
#include <memory>
#include <vector>

class DataSet {
public:
  DataSet(int nSamples, double sampleInterval);
  ~DataSet() = default;

  double at(int index) noexcept;
  void insert(double sample) noexcept;
  PSRWLOCK getLock() noexcept;

  auto begin() noexcept;
  auto end() noexcept;
  double timeWindow() noexcept;
  double maxValue() noexcept;
  double minValue() noexcept;
  int getNumberOfSamples() noexcept;
  double getSampleInterval() noexcept;
  double getTimeWindow() noexcept;

private:
  int _nSamples;
  int _nextSample;
  double _sampleInterval;
  double _timeWindow;
  std::vector<double> _samples;
  SRWLOCK _lock;
};

using PDataSet = std::shared_ptr<DataSet>;

struct DataSetBuilder {
  static PDataSet createDataSet(int nSamples, double sampleInterval);
};