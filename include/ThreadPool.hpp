#include "ThreadJoiner.hpp"
#include "ThreadSafeQueue.hpp"

#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

class ThreadPool {
  std::atomic<bool> done;
  ThreadSafeQueue<std::function<void()>> tasks_queue;
  std::vector<std::thread> threads;
  ThreadJoiner joiner;

  void task_worker() {
    while (!done) {
      std::function<void()> task;
      if (tasks_queue.try_pop(task)) {
        task();
      } else {
        std::this_thread::yield();
      }
    }
  }

public:
  ThreadPool() : done(false), joiner(threads) {
    unsigned threads_num = 2;
    try {
      for (unsigned i = 0; i < threads_num; i++) {
        threads.push_back(std::thread(&ThreadPool::task_worker, this));
      }
    } catch (...) {
      done = true;
      throw;
    }
  }

  ~ThreadPool() { done = true; }

  template <typename FunctionType> void function_input(FunctionType f) {
    tasks_queue.push(std::function<void()>(f));
  }
};