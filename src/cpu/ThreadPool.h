#ifndef PARALLEL_TERM_PROJECT_THREADPOOL_H
#define PARALLEL_TERM_PROJECT_THREADPOOL_H
#include <functional>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <future>
#include <memory>

class ThreadTask;
using ThreadLamda_t = void(std::shared_ptr<ThreadTask> currentTask);

class ThreadPool{
public:
    ~ThreadPool();
    void start(int numThreads);
    void stop();
    void waitUntilDone();

    std::future<void> queueTask(const std::function<ThreadLamda_t>& func);
    std::future<void> queueTask(ThreadTask task);

    void join(std::future<void>& fut);
private:
    void threadLoop();

    bool stopped = true;
    std::atomic_int numWaiting{0};
    std::queue<std::shared_ptr<ThreadTask>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueMutexTaskCond;
    std::condition_variable queueMutexNumCond;
    std::condition_variable queueMutexJoinCond;
    std::vector<std::thread> threads;
};

class ThreadTask{
public:
    explicit ThreadTask(const std::function<ThreadLamda_t>& func,
               const std::function<ThreadLamda_t>& onCompletion = [](const std::shared_ptr<ThreadTask>&){},
               std::shared_ptr<ThreadTask> parent = std::shared_ptr<ThreadTask>(nullptr));
    ThreadTask(ThreadTask&& threadTask) noexcept ;

//    std::packaged_task<void(ThreadTask& currentTask)>& getTask();

    std::future<void> getFuture();
    void operator()(const std::shared_ptr<ThreadTask>& currentTask);

    void onComplete(std::shared_ptr<ThreadTask> currentTask);
private:
    std::packaged_task<ThreadLamda_t> task;
    std::packaged_task<ThreadLamda_t> completionTask;

    std::shared_ptr<ThreadTask> parent;
    std::atomic_int numActiveChildren;
};

#endif //PARALLEL_TERM_PROJECT_THREADPOOL_H
