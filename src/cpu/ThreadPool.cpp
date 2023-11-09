#include "ThreadPool.h"

ThreadPool::~ThreadPool() {
    this->stop();
}

void ThreadPool::start(unsigned numThreads) {
    if(numThreads == 0){
        return;
    }
    stopped = false;
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
    queueMutexCondVar.notify_all();

    for(std::thread& thread : threads){
        thread.join();
    }
    threads.clear();
}

std::future<void> ThreadPool::queueTask(const std::function<void()> &task) {
    std::packaged_task<void()> packaged(task);
    std::future<void> out = packaged.get_future();
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(std::move(packaged));
    }
    queueMutexCondVar.notify_one();
    return out;
}

void ThreadPool::threadLoop() {
    while (true){
        std::packaged_task<void()> currentTask;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueMutexCondVar.wait(lock, [this]{
                return !taskQueue.empty() || stopped;
            });

            if(stopped){
                return;
            }
            currentTask = std::move(taskQueue.front());
            taskQueue.pop();
        }
        currentTask();
    }
}
