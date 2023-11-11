#include "ThreadPool.h"

#include <utility>

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
    queueMutexJoinCond.notify_all();

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

std::future<void> ThreadPool::queueTask(const std::function<void(std::shared_ptr<ThreadTask>currentTask)> &func) {
    return this->queueTask(ThreadTask(func));
}

std::future<void> ThreadPool::queueTask(ThreadTask task) {
    std::future<void> out = task.getFuture();

    {
        std::unique_lock<std::mutex> lock(queueMutex);
        std::shared_ptr<ThreadTask> sharedPtr = std::make_shared<ThreadTask>(std::move(task));
        taskQueue.push(sharedPtr);
    }
    queueMutexTaskCond.notify_one();
    queueMutexJoinCond.notify_one();
    return out;
}

static bool futReady(std::future<void>& fut){
    return  fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

void ThreadPool::join(std::future<void>& fut) {

    while (!futReady(fut)){
        std::shared_ptr<ThreadTask> currentTask;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            while(taskQueue.empty()){

                queueMutexJoinCond.wait(lock, [this, &fut]{
                    return !taskQueue.empty() || stopped || futReady(fut);
                });

                if(stopped || futReady(fut)){
                    return;
                }
            }

            currentTask = taskQueue.front();
            taskQueue.pop();
        }

        (*currentTask)(currentTask);
        queueMutexNumCond.notify_all();
    }
}

void ThreadPool::threadLoop() {
    while (true){
        std::shared_ptr<ThreadTask> currentTask;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            queueMutexTaskCond.wait(lock, [this]{
                return !taskQueue.empty() || stopped;
            });
            --numWaiting;

            if(stopped){
                return;
            }
            currentTask = taskQueue.front();
            taskQueue.pop();
        }

        (*currentTask)(currentTask);
        ++numWaiting;
        queueMutexNumCond.notify_all();
        queueMutexJoinCond.notify_all();
    }
}



ThreadTask::ThreadTask(const std::function<void(std::shared_ptr<ThreadTask>)> &func,
                       const std::function<void(std::shared_ptr<ThreadTask>)> &onCompletion,
                       std::shared_ptr<ThreadTask> parent ):
        task(func),
        numActiveChildren(0),
        completionTask(onCompletion),
        parent(std::move(parent)){
    if(this->parent != nullptr){
        ++this->parent->numActiveChildren;
    }
}

ThreadTask::ThreadTask(ThreadTask&& threadTask) noexcept :
        task(std::move(threadTask.task)),
        completionTask(std::move(threadTask.completionTask)),
        parent(std::move(threadTask.parent)),
        numActiveChildren(threadTask.numActiveChildren.load()){
}

std::future<void> ThreadTask::getFuture() {
    return completionTask.get_future();
}

void ThreadTask::operator()(const std::shared_ptr<ThreadTask>& currentTask) {
    task(currentTask);
    if(numActiveChildren.load() == 0){
        onComplete(currentTask);
    }
}

void ThreadTask::onComplete(std::shared_ptr<ThreadTask> currentTask) {
    int numChildren = this->numActiveChildren.load();
    completionTask(std::move(currentTask));
    if(this->parent != nullptr){
        int parentNumChildren = --this->parent->numActiveChildren;
        if(parentNumChildren <= 0){
            this->parent->onComplete(parent);
        }
    }
}





