#include "../ThreadPool.h"
#include "../catch/catch.hpp"
#include <atomic>
#include <chrono>
#include <thread>

TEST_CASE("ThreadPool runs a submitted job", "[weight=1]") {
  ThreadPool pool(2, 8);
  std::atomic<int> ran(0);
  REQUIRE(pool.submit([&]{ ran++; }));
  for(int i = 0; i < 1000 && ran.load() == 0; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  REQUIRE(ran.load() == 1);
}

TEST_CASE("ThreadPool sheds work once its queue is full", "[weight=1]") {
  ThreadPool pool(1, 2);

  std::atomic<bool> release(false);
  std::atomic<int> started(0);
  std::atomic<int> ran(0);

  // Hold the worker so later submissions remain queued.
  bool first = pool.submit([&]{
    started++;
    while(!release.load()){
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ran++;
  });
  for(int i = 0; i < 1000 && started.load() == 0; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  bool startedInTime = started.load() == 1;

  bool second = pool.submit([&]{ ran++; });
  bool third = pool.submit([&]{ ran++; });
  bool overflow = pool.submit([&]{ ran++; });

  release.store(true);
  for(int i = 0; i < 2000 && ran.load() < 3; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  REQUIRE(first);
  REQUIRE(startedInTime);
  REQUIRE(second);
  REQUIRE(third);
  REQUIRE_FALSE(overflow);
  REQUIRE(ran.load() == 3);
}
