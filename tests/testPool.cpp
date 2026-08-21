#include "../ThreadPool.h"
#include "../catch/catch.hpp"
#include <atomic>
#include <chrono>
#include <thread>

//Parking the only worker on a blocking job forces everything else into the
//queue, which makes the shedding behaviour deterministic to test.

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
  //one worker, room for two waiting behind it
  ThreadPool pool(1, 2);

  std::atomic<bool> release(false);
  std::atomic<int> started(0);
  std::atomic<int> ran(0);

  //occupies the worker until released
  bool first = pool.submit([&]{
    started++;
    while(!release.load()){
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ran++;
  });
  REQUIRE(first);

  for(int i = 0; i < 1000 && started.load() == 0; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  REQUIRE(started.load() == 1);

  //the queue holds two...
  REQUIRE(pool.submit([&]{ ran++; }));
  REQUIRE(pool.submit([&]{ ran++; }));

  //...so the third is turned away
  REQUIRE_FALSE(pool.submit([&]{ ran++; }));

  release.store(true);
  for(int i = 0; i < 2000 && ran.load() < 3; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  REQUIRE(ran.load() == 3);
}
