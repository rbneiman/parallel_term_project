#include "ThreadPool.h"

ThreadPool::~ThreadPool() {
    this->stop();
}

void ThreadPool::start(int numThreads) {
    if(numThreads == 0){
        return;
    }

    stopped = false;
    numWaiting.store(numThreads);
    for(unsigned i=0; i<numThreads; ++i){
        threads.emplace_back(&ThreadPool::threadLoop, this);
    }
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if(stopped){
            return;
        }
        stopped = true;
    }
    queueMutexTaskCond.notify_all();

    for(std::thread& thread : threads){
        thread.join();
    }
    threads.clear();
}

void ThreadPool::waitUntilDone() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueMutexNumCond.wait(lock, [this] {
            return taskQueue.empty() && numWaiting == threads.size();
        });
    }
}

std::future<void> ThreadPool::queueTask(const std::function<void()> &task) {
    std::packaged_task<void()> packaged(task);
    std::future<void> out = packaged.get_future();
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(std::move(packaged));
    }
    queueMutexTaskCond.notify_one();
    return out;
}

void ThreadPool::threadLoop() {
    while (true){
        std::packaged_task<void()> currentTask;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueMutexTaskCond.wait(lock, [this]{
                return !taskQueue.empty() || stopped;
            });
            --numWaiting;

            if(stopped){
                return;
            }
            currentTask = std::move(taskQueue.front());
            taskQueue.pop();
        }

        currentTask();
        ++numWaiting;
        queueMutexNumCond.notify_one();
    }
}


