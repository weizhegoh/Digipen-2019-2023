#pragma once
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <vector>
namespace Xivi
{
  class ThreadPool
  {
    using Job = std::function<void()>;
    std::vector<std::thread> workers;
    std::queue<Job> jobs;
    std::mutex m;
    std::condition_variable cv;
    std::atomic_bool stop;

    void CreateWorkers(size_t count)
    {
      workers.reserve(count);

      for (size_t i = 0; i < count; ++i)
      {
        workers.emplace_back(std::thread([this]() {
          while (true)
          {
            Job job;
            {
              std::unique_lock<std::mutex> lock(m);
              cv.wait(lock, [this] { return !jobs.empty() || stop.load(std::memory_order_relaxed); });

              if (stop.load(std::memory_order_relaxed))
                break;

              job = std::move(jobs.front());
              jobs.pop();
            }
            job();
          }
        }));
      }
    }

  public:
    ThreadPool(size_t count = std::thread::hardware_concurrency())
    {
      stop.store(false, std::memory_order_relaxed);
      CreateWorkers(count);
    }

    ~ThreadPool()
    {
      stop.store(true, std::memory_order_relaxed);
      cv.notify_all();

      for (auto &worker : workers)
        worker.join();
    }

    template <typename Func, typename... Args>
    auto Enqueue(Func &&f, Args &&...args)
    {
      using return_type = std::invoke_result_t<Func, Args...>;
      auto job = std::make_shared<std::packaged_task<return_type()>>([&f, &args...]() { return f(std::forward<Args>(args)...); });

      {
        std::scoped_lock<std::mutex> lock(m);
        jobs.emplace([job]() { (*job)(); });
      }

      cv.notify_one();

      return job->get_future();
    }

    size_t Count() const
    {
      return workers.size();
    }

    void Wait()
    {
      std::unique_lock<std::mutex> lock(m);

      cv.wait(lock, [this]() { return jobs.empty(); });
    }
  };
}