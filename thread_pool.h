// Copyright (C) 
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU GeneratorExiteral Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., Free Road, Shanghai 000000, China.
// 
/// @file thread_pool.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef THREAD_POOL
#define THREAD_POOL
#include <thread>
#include <functional>
#include <atomic>
#include <vector>
#include <map>
#include <assert.h>
namespace jenio
{

class Thread : public std::enable_shared_from_this<Thread>
{
public:
    enum Status
    {
        eIDLE,
        eBUSY
    };

    typedef std::shared_ptr<Thread> Ptr;
    typedef std::function<void(Thread::Ptr)> Function;
    static Thread* create(Function func) { return new Thread(func); }

    Thread(Function func);
    void set_status(Status s);
    void set_quit(bool quit);
private:
    Status status_;
    std::atomic_bool quit_;
};

class ThreadPool
{
public:
    friend class Thread;
public:
    ThreadPool();
    void set_func(Thread::Function func);
    void stop();
    void start(std::size_t size);
    std::size_t size() const;
    void new_thread();
    void set_thread_status(Thread::Ptr, Thread::Status);
private:
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(const ThreadPool&) = delete;

private:
    std::atomic_uint idle_thread_num_;
    std::vector<Thread::Ptr> threads_;
    Thread::Function thread_loop_func_;
};

//const int kDEFUALT_THREAD_NUM = 0;

inline ThreadPool::ThreadPool() :
    idle_thread_num_(0),
    threads_()
{
}

inline void ThreadPool::set_func(Thread::Function func)
{
    thread_loop_func_ = func;
}

inline void ThreadPool::start(std::size_t size)
{
    threads_.reserve(size);
    for (std::size_t i = 0; i < size; ++i)
    {
       new_thread();
    }
}

inline std::size_t ThreadPool::size() const
{
    return threads_.size();
}

inline void ThreadPool::new_thread()
{
    Thread::Ptr t(Thread::create(thread_loop_func_));
    threads_.push_back(t);
}

inline void ThreadPool::set_thread_status(Thread::Ptr thread, Thread::Status status)
{
    thread->set_status(status);
    if (status == Thread::eIDLE)
    {
        ++idle_thread_num_;
    }
    else 
    {
        --idle_thread_num_;
    }
}

inline Thread::Thread(Function func)
{
    if (!func)
        return;
    std::thread t([func, this] ()
                {
                    Thread::Ptr self = shared_from_this();
                    while (!quit_)
                    {
                        func(self);
                    }
                });
    t.detach();
}

inline void Thread::set_status(Thread::Status s)
{
    status_ = s;
}

} //namespace
#endif // THREAD_POOL

