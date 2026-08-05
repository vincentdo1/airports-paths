#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int workers, unsigned int maxQueueSize) : maxQueue(maxQueueSize), stopping(false) {
    //A pool with no workers could never run anything, so keep at least one.
    if(workers == 0){
        workers = 1;
    }
    for(unsigned int i = 0; i < workers; i++){
        threads.push_back(std::thread(&ThreadPool::workerLoop, this));
    }
}

ThreadPool::~ThreadPool() {
    //Flip the stop flag, wake every worker, and wait for them all to finish so we
    //shut down cleanly instead of leaving threads running.
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
        //If the backlog is already at its cap, shed the job: better to reject it
        //quickly than to let the queue (and memory, and latency) grow without bound.
        if(maxQueue > 0 && jobs.size() >= maxQueue){
            return false;
        }
        jobs.push(job);
    }
    //Wake one waiting worker to handle the job we just queued.
    jobsCV.notify_one();
    return true;
}

void ThreadPool::workerLoop() {
    while(true){
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(jobsMutex);
            //Sleep until there is a job waiting or we are shutting down.
            jobsCV.wait(lock, [this]{ return stopping || !jobs.empty(); });
            //Only stop once the queue has drained, so no queued job is dropped.
            if(stopping && jobs.empty()){
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        //Run the job with the lock released so other workers can grab jobs too.
        job();
    }
}
