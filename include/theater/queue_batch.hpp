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


#include "sequence.hpp"


namespace theater {
  
  
  template<typename Q>
  struct queue_batch {
    
    using size_type = typename Q::size_type;
    using value_type = typename Q::value_type;
    
    
    queue_batch() = delete;    
    queue_batch(const queue_batch&) = delete;
    queue_batch& operator = (const queue_batch&) = delete;
    queue_batch(Q& queue) noexcept: queue_(queue) { }
    
    size_type size() const noexcept { return queue_.size(); }
    sequence try_fetch() { return queue_.try_fetch(); }    
    void fetched() { queue_.fetched(); }
    value_type& operator [] (sequence n) { return queue_[n]; }

    
  private:
  
    Q& queue_;
    
  }; // queue_batch
  
  
} // theater
