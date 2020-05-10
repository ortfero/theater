#pragma once


#include <thread>
#include <string>
#include <doctest/doctest.h>
#include <theater/activity.hpp>


TEST_CASE("activity::activity") {

  theater::activity<int> target;
  REQUIRE(!target.active());
  REQUIRE(!target.claim());

}


TEST_CASE("activity::run/1") {

  theater::activity<int> target;
  bool const started_without_buffer = target.run([](auto&){ });
  REQUIRE(!started_without_buffer);
  target.reserve(16);
  bool const started_with_buffer = target.run([](auto&){ });
  REQUIRE(started_with_buffer);
  while(!target.active());
  bool const started_already_active = target.run([](auto&){ });
  REQUIRE(!started_already_active);
  target.stop();
  REQUIRE(!target.active());
}


TEST_CASE("activity::run/2") {

  theater::activity<int> target;
  target.reserve(3);
  bool const started = target.run([](auto& batch){
    theater::sequence n;
    while(!n) n = batch.try_fetch();
    REQUIRE(batch[n] == -3);
    batch.fetched();
  });
  REQUIRE(started);
  auto const n = target.claim();
  REQUIRE(!!n);
  target[n] = -3;
  target.publish(n);
}


TEST_CASE("activity::run/3") {

  theater::activity<std::string> target;

  target.reserve(1);
  target.run([](auto& batch) {
    auto const n = batch.try_fetch();
    if(!n)
      return;
    REQUIRE(batch[n] == "very very very long text argument");
    batch.fetched();
  });

  for(int i = 0; i != 100; ++i) {
    auto const n = target.claim();
    target[n] = "very very very long text argument";
    target.publish(n);
  }
}
