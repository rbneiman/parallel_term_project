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
    void start(unsigned numThreads);
    void stop();

    std::future<void> queueTask(const std::function<void()>& task);

private:
    void threadLoop();

    bool stopped = true;
    std::queue<std::packaged_task<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable queueMutexCondVar;
    std::vector<std::thread> threads;
};

#endif //PARALLEL_TERM_PROJECT_THREADPOOL_H
