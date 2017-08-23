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
/// @file event.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include <vector>
#include <functional>
#include <iostream>

#include "error_code.h"
#include "tcp_addr.h"
namespace jenio 
{

class EventBase;

class Event
{
public:
    typedef std::shared_ptr<Event> Ptr;
public:
    Event(EventBase* base, int fd);
    virtual ~Event();
    virtual void do_callback() = 0;
    virtual void do_async_task() = 0;
    int fd() const;
    const ErrorCode& error() const;
protected:
    EventBase* base_;
    int fd_;
    ErrorCode error_;
};


class ReadEvent : public Event , public std::enable_shared_from_this<ReadEvent>
{
public:
    typedef std::shared_ptr<ReadEvent> Ptr;
    typedef std::function<void(ReadEvent::Ptr)> Callback;
public:
    ReadEvent(EventBase* base, int fd);
    virtual ~ReadEvent();
    static ReadEvent* create(EventBase* base, int fd);

    void set_buffer(char* buffer, std::size_t size);
    char* buffer() { return start_; }
    const char* buffer() const { return start_; }
    std::size_t size() const { return size_; }
    void async_read(Callback callback);

    virtual void do_async_task() override;
    virtual void do_callback() override;

private:
    char* start_;
    char* end_;
    std::size_t size_;
    Callback callback_;
};


class AcceptEvent : public Event , public std::enable_shared_from_this<AcceptEvent>
{
public:
    typedef std::shared_ptr<AcceptEvent> Ptr;
    typedef std::function<void(AcceptEvent::Ptr)> Callback;
public:
    AcceptEvent(EventBase* base, int fd);
    virtual ~AcceptEvent();
    static AcceptEvent* create(EventBase* base, int fd);
    void async_accept(Callback callback);
    virtual void do_async_task() override;
    virtual void do_callback() override;
    const TcpAddr& address() const;
    int newconn_fd() const;
private:
    Callback callback_;
    int newconn_fd_;
    TcpAddr addr_;
};

class WriteEvent : public Event , public std::enable_shared_from_this<WriteEvent>
{
public:
    typedef std::shared_ptr<WriteEvent> Ptr;
    typedef std::function<void(WriteEvent::Ptr)> Callback;
public:
    WriteEvent(EventBase* base, int fd);
    virtual ~WriteEvent();
    static WriteEvent* create(EventBase* base, int fd);

    void set_msg(const char* buffer, std::size_t size);
    void set_msg(const std::string& msg);
    void async_write(Callback callback);
    std::size_t transfer_bytes() const;
    virtual void do_async_task() override;
    virtual void do_callback() override;

private:
    std::vector<char> buffer_;
    std::size_t transfer_bytes_;
    Callback callback_;
};

//class ReadEventBuilder : public EventBuilder
//{
//public:
//    ReadEventBuilder();
//    ReadEventBuilder& buffer(char* buffer, std::size_t size);
//    ReadEventBuilder& callback(ReadEvent::Callback callback);
//    ReadEvent* build();

//protected:
//    char* buffer_;
//    std::size_t size_;
//    ReadEvent::Callback callback_;
//};

}//namespace
#endif // EVENT_H
