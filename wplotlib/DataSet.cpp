#include "DataSet.h"
#include <algorithm>

constexpr double nval = (double)NAN;

DataSet::DataSet(int nSamples, double sampleInterval)
    : _nSamples(nSamples), _sampleInterval(sampleInterval), _nextSample(0) {
  InitializeSRWLock(&_lock);
  _samples.reserve(nSamples);
  _timeWindow = _nSamples * _sampleInterval;
  for (int i = 0; i < _nSamples; i++) {
    _samples.push_back(0.0);
  }
}

double DataSet::at(int index) noexcept {
  Lock lck(&_lock);

  if (index < _samples.size()) {
    return _samples.at(index);
  }
  return nval;
}

void DataSet::insert(double sample) noexcept {
  Lock lck(&_lock);
  if (_nextSample > _nSamples - 1) {
    _nextSample = 0;
  }
  _samples[_nextSample++] = sample;
}

PSRWLOCK DataSet::getLock() noexcept { return &_lock; }

auto DataSet::begin() noexcept { return _samples.begin(); }

auto DataSet::end() noexcept { return _samples.end(); }

double DataSet::timeWindow() noexcept { return _timeWindow; }

double DataSet::maxValue() noexcept {
  Lock lck(&_lock);
  double val = *std::max_element(_samples.begin(), _samples.end());
  return val;
}

double DataSet::minValue() noexcept {
  Lock lck(&_lock);
  double val = *std::min_element(_samples.begin(), _samples.end());
  return val;
}

int DataSet::getNumberOfSamples() noexcept { return _nSamples; }

double DataSet::getSampleInterval() noexcept { return _sampleInterval; }

double DataSet::getTimeWindow() noexcept { return _timeWindow; }

PDataSet DataSetBuilder::createDataSet(int nSamples, double sampleInterval) {
  return std::make_shared<DataSet>(nSamples, sampleInterval);
}
