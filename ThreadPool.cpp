#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int workers, unsigned int maxQueueSize) : maxQueue(maxQueueSize), stopping(false) {
    if(workers == 0){
        workers = 1;
    }
    for(unsigned int i = 0; i < workers; i++){
        threads.push_back(std::thread(&ThreadPool::workerLoop, this));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(jobsMutex);
        stopping = true;
    }
    jobsCV.notify_all();
    for(unsigned int i = 0; i < threads.size(); i++){
        if(threads[i].joinable()){
            threads[i].join();
        }
    }
}

bool ThreadPool::submit(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(jobsMutex);
        // Reject work instead of allowing an unbounded backlog.
        if(maxQueue > 0 && jobs.size() >= maxQueue){
            return false;
        }
        jobs.push(job);
    }
    jobsCV.notify_one();
    return true;
}

void ThreadPool::workerLoop() {
    while(true){
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(jobsMutex);
            jobsCV.wait(lock, [this]{ return stopping || !jobs.empty(); });
            // Finish queued jobs before workers exit.
            if(stopping && jobs.empty()){
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}
