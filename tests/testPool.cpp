#include "../ThreadPool.h"
#include "../catch/catch.hpp"
#include <atomic>
#include <chrono>
#include <thread>

/*
    Tests for the bounded thread pool. The interesting behavior is what happens
    when the backlog fills up: submit() should refuse the job so the server can
    answer with an overload status instead of queuing work without limit. We drive
    that deterministically by parking the single worker on a job that won't finish
    until we release it, which forces everything else to sit in the queue.
*/

TEST_CASE("ThreadPool runs a submitted job", "[weight=1]") {
  ThreadPool pool(2, 8);
  std::atomic<int> ran(0);
  REQUIRE(pool.submit([&]{ ran++; }));
  //Wait a little for a worker to pick it up.
  for(int i = 0; i < 1000 && ran.load() == 0; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  REQUIRE(ran.load() == 1);
}

TEST_CASE("ThreadPool sheds work once its queue is full", "[weight=1]") {
  //One worker, room for two more jobs waiting behind it.
  ThreadPool pool(1, 2);

  std::atomic<bool> release(false);
  std::atomic<int> started(0);
  std::atomic<int> ran(0);

  //This job occupies the single worker until we let it go, so anything else we
  //submit has to wait in the queue.
  bool first = pool.submit([&]{
    started++;
    while(!release.load()){
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ran++;
  });
  REQUIRE(first);

  //Wait until the worker has actually picked that job up and is stuck in it.
  for(int i = 0; i < 1000 && started.load() == 0; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  REQUIRE(started.load() == 1);

  //The queue holds two, so these two are accepted...
  REQUIRE(pool.submit([&]{ ran++; }));
  REQUIRE(pool.submit([&]{ ran++; }));

  //...but now the queue is full, so the next one is turned away.
  REQUIRE_FALSE(pool.submit([&]{ ran++; }));

  //Let everything finish, then confirm only the three accepted jobs ran.
  release.store(true);
  for(int i = 0; i < 2000 && ran.load() < 3; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  REQUIRE(ran.load() == 3);
}
