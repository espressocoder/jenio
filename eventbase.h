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
/// @file eventbase.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef EVENTBASE_H
#define EVENTBASE_H
#include <set>
#include "blocking_queue.h"
#include "event.h"
#include "thread_pool.h"
namespace jenio 
{

class EventBase
{
public:
    EventBase();
    void run();
    void add_ready_event(Event::Ptr event);
    void add_event(Event::Ptr event);

private:
    EventBase(const EventBase&) = delete;
    EventBase& operator=(const EventBase&) = delete;

private:
    ThreadPool thread_pool_;
    BlockingQueue<Event::Ptr> ready_events_;
    BlockingQueue<Event::Ptr> events_;
};

} //namespace
#endif // EVENTBASE_H
