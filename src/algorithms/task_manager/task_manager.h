#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using std::bind;
using std::condition_variable;
using std::function;
using std::lock_guard;
using std::move;
using std::mutex;
using std::queue;
using std::thread;
using std::try_to_lock;
using std::unique_lock;
using std::vector;

namespace s21 {

class TaskQueue {
 public:
  TaskQueue() {}

  template <typename Func>
  bool TryPush(Func &function) {
    unique_lock<mutex> locker(mutex_, try_to_lock);
    bool is_pushed = false;
    if (locker) {
      tasks_queue_.push(function);
      is_pushed = true;
    }
    locker.~unique_lock();
    notifier_.notify_one();
    return is_pushed;
  }

  template <typename Func>
  void ForcedPush(Func &function) {
    mutex_.lock();
    tasks_queue_.push(function);
    mutex_.unlock();
    // notifier_.notify_one();
  }

  bool TryPop(function<void()> &function_link) {
    unique_lock<mutex> locker(mutex_, try_to_lock);
    bool is_pushed = false;
    if (locker && !tasks_queue_.empty()) {
      function_link = move(tasks_queue_.front());
      tasks_queue_.pop();
      is_pushed = true;
    }
    return is_pushed;
  }

 private:
  mutex mutex_;
  queue<function<void()>> tasks_queue_;
  condition_variable notifier_;
};

class TaskManager {
 public:
  TaskManager(size_t threads = 2) {
    if (threads < 1) {
      threads = 1;
    }
    for (size_t i = 0; i != threads; ++i) {
      threads_.push_back(thread(&TaskManager::Run, this, i));
    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // notifier_.notify_one();

    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // notifier_.notify_one();
  }

  ~TaskManager() {
    running = false;
    notifier_.notify_all();
    for (thread &trd : threads_) {
      trd.join();
    }
  }

  template <typename Func, typename... Args>
  void AddTask(Func &function, Args &&...args) {
    bool is_pushed = false;
    size_t i = 0;

    std::function<void()> func = bind(function, args...);
    for (; !is_pushed && i != threads_amount_; ++i) {
      size_t index = (thread_index_ + i) % threads_amount_;
      is_pushed = tasks_queues_[index].TryPush(func);
    }

    if (!is_pushed) {
      tasks_queues_[thread_index_].ForcedPush(func);
    }
    ++thread_index_;
    notifier_.notify_one();
  }

  void AwaitAll() {}

 private:
  size_t threads_amount_{thread::hardware_concurrency() / 2};
  vector<thread> threads_;
  vector<TaskQueue> tasks_queues_{threads_amount_};
  size_t thread_index_{0};

  mutex mutex_;
  condition_variable notifier_;

  bool running{true};

  void Run(size_t index) {
    unique_lock<mutex> locker(mutex_);
    while (running) {
      notifier_.wait(locker);
      // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
      // break;
      // locker.unlock();
      bool is_poped = false;
      function<void()> f;
      for (size_t i = 0; !is_poped && i != threads_amount_; ++i) {
        // std::cout << index << " HERE\n";
        is_poped = tasks_queues_[i].TryPop(f);
      }
      if (is_poped) {
        f();
      }
    }
  }
};

};  // namespace s21
