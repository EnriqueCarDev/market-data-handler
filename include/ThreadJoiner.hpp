#pragma once

#include <thread>
#include <vector>

class ThreadJoiner {
  std::vector<std::thread> &_threads;

public:
  explicit ThreadJoiner(std::vector<std::thread> &threads)
      : _threads{threads} {}

  ~ThreadJoiner() {
    for (unsigned long i = 0; i < _threads.size(); i++) {
      if (_threads[i].joinable())
        _threads[i].join();
    }
  }
};