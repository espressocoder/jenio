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
/// @file event.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "eventbase.h"
#include "tcp_addr.h"
#include "event.h"
namespace jenio 
{

Event::Event(EventBase *base, int fd) :
    base_(base),
    fd_(fd),
    error_()
{

}

Event::~Event()
{
    std::cout << "~event" << std::endl;
    ::close(fd_);
}

int Event::fd() const
{
    return fd_;
}

const ErrorCode &Event::error() const
{
    return error_;
}


//ReadEventBuilder::ReadEventBuilder() :
//    EventBuilder(),
//    buffer_(nullptr),
//    size_(0)
//{
//}

//ReadEventBuilder &ReadEventBuilder::buffer(char* buffer, std::size_t size)
//{
//    buffer_ = buffer;
//    size_ = size;
//}

//ReadEventBuilder &ReadEventBuilder::callback(ReadEvent::Callback callback)
//{
//    callback_ = callback;
//}

//ReadEvent* ReadEventBuilder::build()
//{
//    if (fd_ < 0)
//    {
//        return nullptr;
//    }
//    return new ReadEvent(fd_, std::move(callback_), buffer_, size_);
//}


ReadEvent::ReadEvent(EventBase* base, int fd) :
    Event(base, fd),
    start_(nullptr),
    end_(nullptr),
    size_(0)
{
}

ReadEvent::~ReadEvent()
{
    std::cout << "~readevent" << std::endl; 
    ::close(fd_);
}

ReadEvent* ReadEvent::create(EventBase* base, int fd)
{
    return new ReadEvent(base, fd);
}

void ReadEvent::set_buffer(char* buffer, std::size_t size)
{
    start_ = buffer;
    end_ = buffer + size;
    size_ = size;
}

void ReadEvent::async_read(ReadEvent::Callback callback)
{
    callback_ = std::move(callback);
    base_->add_event(shared_from_this());
}

void ReadEvent::do_async_task()
{
    int n = ::read(fd_, start_, end_ - start_);
    if (n < 0)
    {
        size_ = 0;
        error_.set_code(errno);
        error_.set_message(::strerror(error_.code()));
    }
    else if (n == 0) 
    {
        size_ = 0;
        error_.set_message("eof");
        error_.set_code(ErrorCode::kEOF);
    }
    else
    {
        size_ = static_cast<std::size_t>(n);
        error_.set_message("ok");
        error_.set_code(ErrorCode::kOK);
    }
}

void ReadEvent::do_callback()
{
    if (!callback_)
    {
        return;
    }
    callback_(shared_from_this());
}

AcceptEvent::AcceptEvent(EventBase* base, int fd) :
    Event(base, fd)
{
}

AcceptEvent::~AcceptEvent()
{

}

AcceptEvent* AcceptEvent::create(EventBase* base, int fd)
{
    return new AcceptEvent(base, fd);
}

void AcceptEvent::async_accept(AcceptEvent::Callback callback)
{
    callback_ = std::move(callback);
    base_->add_event(shared_from_this());
}

void AcceptEvent::do_async_task()
{
    sockaddr_in clientaddr;
    size_t clilen = sizeof(clientaddr);
    memset(&clientaddr, 0, sizeof(struct sockaddr_in));
    int connfd = ::accept(fd(), (sockaddr*)&clientaddr, &clilen);
    if (connfd < 0)
    {
        error_ = ErrorCode(errno);
        return;
    }
    error_.set_message("ok");
    error_.set_code(ErrorCode::kOK);
    newconn_fd_ = connfd;
    addr_ = TcpAddr(::inet_ntoa(clientaddr.sin_addr),
                     ::ntohs(clientaddr.sin_port));
}

void AcceptEvent::do_callback()
{
    if (!callback_)
    {
        return;
    }
    callback_(shared_from_this());
}

const TcpAddr &AcceptEvent::address() const
{
    return addr_;
}

int AcceptEvent::newconn_fd() const
{
    return newconn_fd_;
}

WriteEvent::WriteEvent(EventBase* base, int fd) :
    Event(base, fd),
    buffer_(),
    transfer_bytes_(0)
{

}

WriteEvent::~WriteEvent()
{
}

WriteEvent* WriteEvent::create(EventBase* base, int fd)
{
    return new WriteEvent(base, fd);
}

void WriteEvent::set_msg(const char* buffer, std::size_t size)
{
    buffer_.resize(size);
    std::copy(buffer, buffer + size, buffer_.begin());
}

void WriteEvent::set_msg(const std::string& msg)
{
    set_msg(msg.data(), msg.size());
}

void WriteEvent::async_write(WriteEvent::Callback callback)
{
    callback_ = std::move(callback);
    base_->add_event(shared_from_this());
}

std::size_t WriteEvent::transfer_bytes() const
{
    return transfer_bytes_;
}

void WriteEvent::do_async_task() 
{
    int n = ::write(fd_, buffer_.data(), buffer_.size());
    if (n <= 0)
    {
        transfer_bytes_ = 0;
        error_.set_code(errno);
        error_.set_message(::strerror(error_.code()));
    }
    else
    {
        transfer_bytes_ = static_cast<std::size_t>(n);
        error_.set_message("ok");
        error_.set_code(ErrorCode::kOK);
    }
}

void WriteEvent::do_callback()
{
    if (!callback_)
        return;
    callback_(shared_from_this());
}

}//namespace
