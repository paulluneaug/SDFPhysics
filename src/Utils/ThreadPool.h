#pragma once
#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>


class TaskQueue
{
private:
    std::queue<std::function<void()>> m_tasks;
    std::mutex                        m_mutex;
    std::atomic<uint32_t>             m_remainingTasks = 0;

public:
    template<typename TCallback>
    void AddTask(TCallback&& callback)
    {
        AddTaskNoIncrement(std::forward<TCallback>(callback));
        m_remainingTasks++;
    }

    template<typename TCallback>
    void AddTaskNoIncrement(TCallback&& callback)
    {
        std::lock_guard<std::mutex> lock_guard{ m_mutex };
        m_tasks.push(std::forward<TCallback>(callback));
    }

    void GetTask(std::function<void()>& target_callback)
    {
        {
            std::lock_guard<std::mutex> lock_guard{ m_mutex };
            if (m_tasks.empty()) {
                return;
            }
            target_callback = std::move(m_tasks.front());
            m_tasks.pop();
        }
    }

    static void Wait()
    {
        std::this_thread::yield();
    }

    void WaitForCompletion() const
    {
        while (m_remainingTasks > 0) 
        {
            Wait();
        }
    }

    void WorkDone()
    {
        m_remainingTasks--;
    }

    uint32_t GetRemainingTask() const 
    {
        return m_remainingTasks;
    }
};

class Worker
{
private:

    uint32_t m_id = 0;
    std::thread m_thread;
    std::function<void()> m_task = nullptr;
    bool m_running = true;
    TaskQueue* m_queue = nullptr;

public:
    Worker() = default;

    Worker(TaskQueue& queue, uint32_t id)
        : m_id{ id }
        , m_queue{ &queue }
    {
        m_thread = std::thread([this]() {
            Run();
            });
    }

    void Run()
    {
        while (m_running) {
            m_queue->GetTask(m_task);
            if (m_task == nullptr) {
                TaskQueue::Wait();
            }
            else {
                m_task();
                m_queue->WorkDone();
                m_task = nullptr;
            }
        }
    }

    void Stop()
    {
        m_running = false;
        m_thread.join();
    }
};

class ThreadPool
{
private :
    uint32_t m_threadCount = 0;
    TaskQueue m_queue;
    std::vector<Worker> m_workers;

public:
    explicit
        ThreadPool(uint32_t threadCount)
        : m_threadCount{ threadCount }
    {
        m_workers.reserve(threadCount);
        for (uint32_t i{ threadCount }; i > 0u; --i) 
        {
            m_workers.emplace_back(m_queue, static_cast<uint32_t>(m_workers.size()));
        }
    }

    virtual ~ThreadPool()
    {
        for (Worker& worker : m_workers) {
            worker.Stop();
        }
    }

    template<typename TCallback>
    void AddTask(TCallback&& callback)
    {
        m_queue.AddTask(std::forward<TCallback>(callback));
    }

    template<typename TCallback>
    void AddTaskNoIncrement(TCallback&& callback)
    {
        m_queue.AddTaskNoIncrement(std::forward<TCallback>(callback));
    }

    void WaitForCompletion() const
    {
        m_queue.WaitForCompletion();
    }

    bool IsDone() const
    {
        return m_queue.GetRemainingTask() == 0;
    }

    uint32_t ThreadCount() const {
        return m_threadCount;
    }

    template<typename TCallback>
    void Dispatch(uint32_t element_count, TCallback&& callback)
    {
        const uint32_t batch_size = element_count / m_threadCount;
        for (uint32_t i{ 0 }; i < m_threadCount; ++i) 
        {
            const uint32_t start = batch_size * i;
            const uint32_t end = start + batch_size;
            AddTask([start, end, &callback]() { callback(start, end); });
        }

        if (batch_size * m_threadCount < element_count) 
        {
            const uint32_t start = batch_size * m_threadCount;
            callback(start, element_count);
        }

        WaitForCompletion();
    }
};