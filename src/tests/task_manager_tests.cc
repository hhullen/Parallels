#include <gtest/gtest.h>
#include <task_manager/task_manager.h>

class TESTER {
 public:
  void method(int var) { std::cout << var; }
};

TEST(Task_manager_tests, constructor) {
  EXPECT_NO_THROW(s21::TaskManager object);
  EXPECT_NO_THROW(s21::TaskManager object(3));
}

TEST(Task_manager_tests, set_threads) {
  s21::TaskManager object(3);
  EXPECT_NO_THROW(object.SetThreads(8));
}

TEST(Task_manager_tests, add_task) {
  s21::TaskManager object(8);
  TESTER test_obj;
  EXPECT_NO_THROW(object.AddTask(&TESTER::method, test_obj, 20));
  EXPECT_NO_THROW(object.AddTask(&TESTER::method, test_obj, 21));
  object.AddTask(&TESTER::method, test_obj, 22);
  object.AddTask(&TESTER::method, test_obj, 245);
  object.AddTask(&TESTER::method, test_obj, 25);
  object.AddTask(&TESTER::method, test_obj, 100);
  EXPECT_NO_THROW(object.AwaitAll());
  object.AddTask(&TESTER::method, test_obj, 22);
  object.AddTask(&TESTER::method, test_obj, 245);
  object.AddTask(&TESTER::method, test_obj, 25);
  object.AddTask(&TESTER::method, test_obj, 100);
  EXPECT_NO_THROW(object.AwaitAll());
}
