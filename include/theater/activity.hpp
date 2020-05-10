/* This file is part of theater library
 * Copyright 2020 Andrei Ilin <ortfero@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#pragma once


#include <thread>
#include <memory>
#include "mpsc_queue.hpp"
#include "atomic_cv.hpp"
#include "queue_batch.hpp"


namespace theater {
  

  template<typename M, typename Q = mpsc_queue<M>>
  struct activity {
    
    using message_type = M;
    using queue_type = Q;
    using size_type = typename Q::size_type;
    using batch = queue_batch<queue_type>;

    activity() noexcept = default;
    activity(activity const&) noexcept = delete;
    activity& operator = (activity const&) noexcept = delete;
    ~activity() { stop(); }
    bool active() const noexcept { return worker_.joinable(); }    
    sequence claim() noexcept { return messages_.claim(); }
    message_type& operator [] (sequence n) noexcept { return messages_[n]; }
    void reserve(size_type n) noexcept { messages_.reserve(n); }
    size_type blocks_count() const noexcept { return messages_.blocks_count(); }

    template<typename Rep, typename Period>
    sequence claim_for(std::chrono::duration<Rep, Period> const& duration) noexcept {
      return messages_.claim_for(duration);
    }
    
 
    void publish(sequence n) noexcept {
      messages_.publish(n);
      new_message_.notify_one();
    }


    void stop() noexcept {
      if(!worker_.joinable() || stopping_)
        return;
      stopping_.store(true, std::memory_order_relaxed);
      new_message_.notify_one();
      worker_.join();
    }


    template<typename H>
    bool run(H&& handler) {
      
      if(worker_.joinable() || stopping_ || !messages_)
        return false;
      
      worker_ = std::thread{[handler, this]() {
        
        batch batch(messages_);

        if(messages_.size() != 0)
          handler(batch);
        
        while(!stopping_.load(std::memory_order_relaxed)) {
          new_message_.wait();
          new_message_.reset();
          handler(batch);
        }
        
        if(messages_.size() != 0)
          handler(batch);
        
        stopping_ = false;
        
      }};
      
      return true;
    }


  public:
    
    std::thread worker_;
    queue_type messages_;
    atomic_cv new_message_;
    std::atomic<bool> stopping_{false};

  }; // activity

  
} // theater
