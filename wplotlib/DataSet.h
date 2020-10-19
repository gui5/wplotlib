#pragma once
#include "Lock.h"
#include <cmath>
#include <memory>
#include <vector>
#include <eigen3/Eigen/Eigen>


class DataSet {
public:
  DataSet(int nSamples, double sampleInterval);
  ~DataSet() = default;

  Eigen::Vector3d & at(int index) noexcept;
  void insert(const Eigen::Vector3d &sample) noexcept;
  PSRWLOCK getLock() noexcept;

  auto begin() noexcept;
  auto end() noexcept;
  double timeWindow() noexcept;
  int getNumberOfSamples() noexcept;
  double getSampleInterval() noexcept;
  double getTimeWindow() noexcept;
  int size() noexcept;

private:
  int _nSamples;
  int _nextSample;
  double _sampleInterval;
  double _timeWindow;
  std::vector<Eigen::Vector3d> _samples;
  SRWLOCK _lock;
};

using PDataSet = std::shared_ptr<DataSet>;

struct DataSetBuilder {
  static PDataSet createDataSet(int nSamples, double sampleInterval);
};