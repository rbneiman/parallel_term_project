#ifndef PARALLEL_TERM_PROJECT_THREADPOOL_H
#define PARALLEL_TERM_PROJECT_THREADPOOL_H
#include <functional>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <future>


class ThreadPool{
public:
    ~ThreadPool();
    void start(int numThreads);
    void stop();
    void waitUntilDone();

    std::future<void> queueTask(const std::function<void()>& task);

private:
    void threadLoop();

    bool stopped = true;
    std::atomic_int numWaiting{0};
    std::queue<std::packaged_task<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueMutexTaskCond;
    std::condition_variable queueMutexNumCond;
    std::vector<std::thread> threads;
};

#endif //PARALLEL_TERM_PROJECT_THREADPOOL_H
