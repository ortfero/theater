#pragma once


#include <thread>
#include <doctest/doctest.h>
#include <theater/atomic_cv.hpp>


TEST_CASE("atomic_cv::notify_one/1") {

  theater::atomic_cv cv;
  bool notified = false;

  auto waiter = std::thread([&]{
    notified = cv.wait(std::chrono::milliseconds{100});
    cv.reset();
  });

  while(!waiter.joinable());

  cv.notify_one();

  waiter.join();

  REQUIRE(notified);
}


TEST_CASE("atomic_cv::notify_one/2") {

  theater::atomic_cv cv;

  auto wait = [&](bool& notified) {
    notified = cv.wait(std::chrono::milliseconds{100});
    cv.reset();
  };

  bool notified1 = false;
  bool notified2 = false;

  auto waiter1 = std::thread([&]{ wait(notified1); });
  auto waiter2 = std::thread([&]{ wait(notified2); });

  while(!waiter1.joinable() || !waiter2.joinable());

  cv.notify_one();

  waiter1.join(); waiter2.join();

  bool const just_one_notified = (notified1 != notified2);

  REQUIRE(just_one_notified);
}


TEST_CASE("atomic_cv::notify_all") {

  theater::atomic_cv cv;

  auto wait = [&](bool& notified) {
    notified = cv.wait(std::chrono::milliseconds{100});
  };

  bool notified1 = false;
  bool notified2 = false;

  auto waiter1 = std::thread([&]{ wait(notified1); });
  auto waiter2 = std::thread([&]{ wait(notified2); });  

  while(!waiter1.joinable() || !waiter2.joinable());

  cv.notify_all();

  waiter1.join(); waiter2.join();

  bool const all_notified = notified1 && notified2;

  REQUIRE(all_notified);
}
