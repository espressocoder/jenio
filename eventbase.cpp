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
/// @file eventbase.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#include "eventbase.h"
#include <unistd.h>
namespace jenio
{
EventBase::EventBase() :
    ready_events_(),
    events_()
{

}

void EventBase::run()
{
    thread_pool_.set_func([this] (Thread::Ptr t)
                {
                    Event::Ptr event = events_.pop();
                    thread_pool_.set_thread_status(t, Thread::eBUSY);
                    event->do_async_task();
                    ready_events_.push(event);
                    thread_pool_.set_thread_status(t, Thread::eIDLE);
                });
    int kTHREAD_NUM = 4;
    thread_pool_.start(kTHREAD_NUM);
    while (true) 
    {
        if (!ready_events_.empty()) 
        {
            Event::Ptr event = ready_events_.pop();
            event->do_callback();
        }
    }
}

void EventBase::add_ready_event(Event::Ptr event) 
{
    ready_events_.push(event);
}

void EventBase::add_event(Event::Ptr event)
{
    events_.push(event);
}

}//namespace
