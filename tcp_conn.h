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
/// @file tcp_conn.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef TCPCONN_H
#define TCPCONN_H
#include <memory>
#include <vector>
#include "tcp_addr.h"
#include "event.h"

namespace jenio
{
class EventBase;
class TcpConn : public std::enable_shared_from_this<TcpConn>
{
public:
    typedef std::shared_ptr<TcpConn> Ptr;
    typedef std::function<void(TcpConn::Ptr, std::size_t, ErrorCode)> Callback;

public:
    static TcpConn* create(EventBase* base, int fd);
    static TcpConn* create(EventBase* base);

public:
    TcpConn(EventBase* base, int fd);
    TcpConn(EventBase* base);
    void set_address(TcpAddr addr);
    void set_address(std::string ip, std::size_t port);
    int fd() const;
    char* buffer();
    void read(Callback cb);
    void send(const std::string& msg, Callback);
    void send(const char* msg, std::size_t size, Callback);

private:
    TcpConn& operator =(const TcpConn& other)  = delete;
    TcpConn(const TcpConn& other) = delete;

private:
    EventBase* base_;
    int fd_;
    std::vector<char> buffer_;
    TcpAddr peer_;
    ReadEvent::Ptr read_event_;
    WriteEvent::Ptr write_event_;
};

}//namespace
#endif // TCPCONN_H
