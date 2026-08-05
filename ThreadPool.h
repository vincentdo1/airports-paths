#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

/*
    A small fixed-size thread pool with a bounded backlog. The server hands it one
    job per accepted connection and a fixed number of worker threads pull those jobs
    off a shared queue and run them. Two limits keep resource use in check: the
    worker count caps how many requests run at once, and the queue has a maximum
    depth so a burst of traffic can't pile work (and memory, and latency) up without
    bound. When the queue is full submit() returns false, which lets the server shed
    load with an explicit overload response instead of tipping over. The graph is
    read-only once it is loaded, so the workers can all read it at the same time
    without any locking of their own.
*/
class ThreadPool {
  public:
    ThreadPool(unsigned int workers, unsigned int maxQueue);
    ~ThreadPool();

    //Try to hand a job to the pool. Returns false if the backlog is already at its
    //cap, so the caller can reject the request instead of queuing unbounded work.
    bool submit(std::function<void()> job);

  private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()> > jobs;
    std::mutex jobsMutex;
    std::condition_variable jobsCV;
    unsigned int maxQueue;
    bool stopping;

    //The loop each worker runs: wait for a job, run it, repeat until shutdown.
    void workerLoop();
};
