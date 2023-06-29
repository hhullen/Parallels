#include <functional>
#include <iostream>
#include <vector>

void foo(int a) { std::cout << a << " HERE WE GO\n"; }

int main() {
  int d = 15;
  std::vector<std::function<void()>> cnt;
  for (int i = 0; i < 2; ++i) {
    std::function<void()> s = std::bind(foo, i);
    cnt.push_back(s);
  }
  for (auto ff : cnt) {
    ff();
  }
}