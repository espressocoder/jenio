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
/// @file tcp_conn.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#include "tcp_conn.h"
#include "eventbase.h"
namespace jenio
{

static const int kDEFAULT_TCP_CONN_BUFFER_SIZE = 1024 * 4;

TcpConn* TcpConn::create(EventBase* base, int fd)
{
    return new TcpConn(base, fd);
}

TcpConn* TcpConn::create(EventBase *base)
{
    return new TcpConn(base);
}

TcpConn::TcpConn(EventBase *base, int fd) :
    base_(base),
    fd_(fd),
    buffer_(kDEFAULT_TCP_CONN_BUFFER_SIZE),
    read_event_(ReadEvent::create(base, fd)),
    write_event_(WriteEvent::create(base, fd))
{
    read_event_->set_buffer(buffer_.data(), buffer_.size());
}

TcpConn::TcpConn(EventBase *base) :
    base_(base),
    fd_(0),
    buffer_(kDEFAULT_TCP_CONN_BUFFER_SIZE)
{

}

void TcpConn::set_address(TcpAddr addr)
{
    peer_ = std::move(addr);
}

void TcpConn::set_address(std::__cxx11::string ip, std::size_t port)
{
    peer_= TcpAddr(std::move(ip), port);
}

int TcpConn::fd() const
{
    return fd_;
}

char* TcpConn::buffer()
{
    return buffer_.data();
}

void TcpConn::read(TcpConn::Callback cb)
{
    if (!cb)
    {
        return;
    }
    TcpConn::Ptr self = shared_from_this();
    auto cbor = [self, cb] (ReadEvent::Ptr event)
    {
        cb(self, event->size(), event->error());
    };
    read_event_->async_read(cbor);
}

void TcpConn::send(const std::string& msg, Callback callback)
{
    send(msg.data(), msg.size(), std::move(callback));
}

void TcpConn::send(const char* msg, std::size_t size, Callback callback)
{
    if (!callback)
        return;
    TcpConn::Ptr self = shared_from_this();
    write_event_->set_msg(msg, size);
    write_event_->async_write([callback, self] (WriteEvent::Ptr event) 
                {
                    callback(self, event->transfer_bytes(), event->error());
                });
}

}//namespace

