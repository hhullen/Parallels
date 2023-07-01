#ifndef SRC_ALGORITHMS_TASK_MANAGER_TASK_QUEUE_H_
#define SRC_ALGORITHMS_TASK_MANAGER_TASK_QUEUE_H_

#include <functional>
#include <mutex>
#include <queue>

using std::function;
using std::move;
using std::mutex;
using std::queue;
using std::try_to_lock;
using std::unique_lock;

namespace s21 {

class TaskQueue {
 public:
  TaskQueue() {}

  template <typename Func>
  bool TryPush(Func &&function) {
    unique_lock<mutex> locker(mutex_, try_to_lock);
    if (locker) {
      tasks_queue_.push(function);
      return true;
    }
    return false;
  }

  template <typename Func>
  void ForcedPush(Func &&function) {
    unique_lock<mutex> locker(mutex_);
    tasks_queue_.push(function);
  }

  bool TryPop(function<void()> &function_link) {
    unique_lock<mutex> locker(mutex_, try_to_lock);
    if (locker && !tasks_queue_.empty()) {
      function_link = move(tasks_queue_.front());
      tasks_queue_.pop();
      return true;
    }
    return false;
  }

 private:
  mutex mutex_;
  queue<function<void()>> tasks_queue_;
};

}  // namespace s21

#endif  // SRC_ALGORITHMS_TASK_MANAGER_TASK_QUEUE_H_
