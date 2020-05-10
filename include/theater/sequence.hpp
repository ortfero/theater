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


#include <cstdint>


namespace theater {


  struct sequence {

    using value_type = int64_t;

    constexpr sequence() noexcept = default;
    sequence(sequence const&) noexcept = default;
    sequence& operator = (sequence const&) noexcept = default;
    explicit operator bool () const noexcept { return value_ != -1; }
    value_type value() const noexcept { return value_; }
    explicit constexpr sequence(value_type v): value_{v} { }

    bool operator == (sequence const& other) const noexcept {
      return value_ == other.value_;
    }

    bool operator != (sequence const& other) const noexcept {
      return value_ != other.value_;
    }

  private:

    value_type value_{-1};

  }; // sequence


} // theater
