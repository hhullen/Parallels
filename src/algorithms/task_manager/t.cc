#include "task_manager.h"

class AA {
 public:
  void foo(int a) {
    std::cout << a << " " << std::this_thread::get_id() << " START\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(a));
    std::cout << a << " " << std::this_thread::get_id() << " DONE\n";
  }
};

int main() {
  AA aa;
  s21::TaskManager tt(3);
  tt.AddTask(&AA::foo, aa, 3000);
  tt.AddTask(&AA::foo, aa, 1000);
  tt.AddTask(&AA::foo, aa, 2000);
  tt.AwaitAll();
  // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  tt.AddTask(&AA::foo, aa, 3000);
  tt.AddTask(&AA::foo, aa, 1000);
  tt.AddTask(&AA::foo, aa, 2000);
  tt.AwaitAll();
  std::cout << " AA\n";
  return 0;
}