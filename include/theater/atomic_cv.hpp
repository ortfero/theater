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



#include <chrono>


#if defined(_WIN32)

#include <Windows.h>

#pragma comment(lib, "synchronization.lib")


#else

#error Unsupported OS

#endif



namespace theater {


#if defined(_WIN32)


  struct atomic_cv {

    atomic_cv() noexcept = default;
    atomic_cv(atomic_cv const&) = delete;
    atomic_cv& operator = (atomic_cv const&) = delete;

    void notify_one() {
      raised_ = true;
      WakeByAddressSingle(&raised_);
    }


    void notify_all() {
      raised_ = true;
      WakeByAddressAll(&raised_);
    }


    void reset() {
      raised_ = false;
    }


    void wait() {
      bool raised = false;
      while(!raised_)
        WaitOnAddress(&raised_, &raised, sizeof(bool), INFINITE);
    }


    bool wait(std::chrono::milliseconds timeout) {
      bool raised = false;
      while(!raised_)
        if(!WaitOnAddress(&raised_, &raised, sizeof(bool), DWORD(timeout.count())))
          return false;
      return true;
    }


  private:

    bool raised_{false};
  }; // atomic_cv

#else

#error Unsupported OS

#endif



} // theater
