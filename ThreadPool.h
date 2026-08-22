#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
  public:
    ThreadPool(unsigned int workers, unsigned int maxQueue);
    ~ThreadPool();

    // Returns false when the queue is at capacity.
    bool submit(std::function<void()> job);

  private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()> > jobs;
    std::mutex jobsMutex;
    std::condition_variable jobsCV;
    unsigned int maxQueue;
    bool stopping;

    void workerLoop();
};
