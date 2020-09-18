#include "DataSet.h"
#include <algorithm>


DataSet::DataSet(int nSamples, double sampleInterval)
    : _nSamples(nSamples), _sampleInterval(sampleInterval), _nextSample(0) {
  InitializeSRWLock(&_lock);
  _samples.reserve(nSamples);
  _timeWindow = _nSamples * _sampleInterval;
  for (int i = 0; i < _nSamples; i++) {
    _samples.emplace_back(0.0,0.0,0.0);
  }
}

Eigen::Vector3d &DataSet::at(int index) noexcept {
  Lock lck(&_lock);

  if (index < _samples.size()) {
    return _samples.at(index);
  }

  static Eigen::Vector3d invalidSample((double)NAN, (double)NAN, (double)NAN);
  return invalidSample;
}

void DataSet::insert(const Eigen::Vector3d &sample) noexcept {
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


int DataSet::getNumberOfSamples() noexcept { return _nSamples; }

double DataSet::getSampleInterval() noexcept { return _sampleInterval; }

double DataSet::getTimeWindow() noexcept { return _timeWindow; }

int DataSet::size() noexcept { return (int)_samples.size(); }

PDataSet DataSetBuilder::createDataSet(int nSamples, double sampleInterval) {
  return std::make_shared<DataSet>(nSamples, sampleInterval);
}
