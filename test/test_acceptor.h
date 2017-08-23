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
/// @file test_acceptor.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef TEST_ACCEPTOR_H
#define TEST_ACCEPTOR_H
#include "test.h"
#include "../acceptor.h"
#include <unistd.h>
namespace test
{

jenio::EventBase base;
std::size_t port = 23333;
jenio::Acceptor acceptor(&base, port);

static void write_callback(jenio::TcpConn::Ptr conn, std::size_t bytes, jenio::ErrorCode err)
{
    if (err.error())
    {
        DEBUG_INFO << "send:" << err.message();
        return;
    }
}

static void read_callback(jenio::TcpConn::Ptr conn, std::size_t bytes, jenio::ErrorCode err)
{
    if (err.error())
    {
        DEBUG_INFO << "recv:" << err.message();
        return;
    }
    std::string recv(conn->buffer(), bytes);
    DEBUG_INFO << recv;
    ::sleep(5);
    conn->send(recv, write_callback);
    conn->read(read_callback);
}

static void acceptor_callback(jenio::TcpConn::Ptr conn)
{
    conn->read(read_callback);
    acceptor.accept(acceptor_callback);
}

static void test_acceptor()
{
    acceptor.accept(acceptor_callback);
    base.run();
}


} //namespace

#endif // TEST_ACCEPTOR_H

