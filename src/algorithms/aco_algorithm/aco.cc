#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx; // Объект мьютекса

void printMessage(const std::string &message) {
  std::lock_guard<std::mutex> lock(mtx); // Блокировка мьютекса
  std::cout << message << std::endl;     // Критическая секция
  // Мьютекс будет автоматически разблокирован при выходе из области видимости
}

int main() {
  std::thread t1(printMessage, "Hello");
  std::thread t2(printMessage, "World");

  t1.join();
  t2.join();

  return 0;
}