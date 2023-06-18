#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace s21 {
using Thread = std::thread;
using Mutex = std::mutex;
using Function = std::function<void()>;

class Worker {
  using DoneFunction = std::function<void(Worker *)>;

 public:
  Worker() : job_(nullptr), job_done_(nullptr) {
    this_thread_.lock();
    worker_ = Thread(&Worker::Main, this);
    worker_.detach();
  }

  ~Worker() { job_ = nullptr; }

  void SetJobDoneFunction(const DoneFunction &function) {
    job_done_ = function;
  }

  void RunFunction(const Function &&function) {
    job_ = function;
    this_thread_.unlock();
  }

 private:
  Mutex this_thread_;
  Thread worker_;
  Function job_;
  DoneFunction job_done_;

  void Main() {
    while (true) {
      this_thread_.lock();
      Work();
    }
  }

  void Work() {
    if (job_) {
      job_();
      if (job_done_) {
        job_done_(this);
      }
    }
  }
};

class WorkersManager {
  using Workers = std::vector<Worker>;
  using FreeWorkers = std::vector<Worker *>;
  using DoneFunction = std::function<void(Worker *)>;

 public:
  WorkersManager() = delete;
  WorkersManager(size_t amount)
      : free_workers_(FreeWorkers(amount)),
        workers_(Workers(amount)),
        all_done_(nullptr) {
    for (int i = 0; i < workers_.size(); ++i) {
      DoneFunction job_done = [this](Worker *worker) { Release(worker); };
      workers_[i].SetJobDoneFunction(job_done);
      free_workers_[i] = &(workers_[i]);
    }
  }

  void AddJob(const Function &function) {
    if (!(free_workers_.empty())) {
      free_workers_.back()->RunFunction(std::move(function));
      free_workers_.pop_back();
      std::cout << " Worker run\n";
    } else {
      jobs_queue_.push(function);
      std::cout << " Add to queue\n";
    }
  }

  bool IsAllFree() { return workers_.size() == free_workers_.size(); }

  void SetFunctionWhenAllJobsDone(const Function &function) {
    all_done_ = function;
  }

 private:
  FreeWorkers free_workers_;
  Workers workers_;
  Function all_done_;

  std::queue<Function> jobs_queue_;

  void Release(Worker *worker) {
    free_workers_.push_back(worker);

    if (!(jobs_queue_.empty())) {
      Function job = jobs_queue_.back();
      jobs_queue_.pop();
      free_workers_.back()->RunFunction(std::move(job));
      free_workers_.pop_back();
      std::cout << " Run from queue\n";
    } else if (IsAllFree() && all_done_) {
      all_done_();
      std::cout << "JobsDone\n";
    }
  }
};

}  // namespace s21
