#include "task_manager.h"

void foo(int a) { std::cout << a << " HERE WE GO\n"; }

int main() {
  s21::TaskManager tt;
  tt.AddTask(foo, 3);
  tt.AddTask(foo, 2);
  std::cout << " AA\n";
  return 0;
}