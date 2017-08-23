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
/// @file acceptor.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_addr.h"
#include "util.h"
#include "acceptor.h"

namespace jenio
{
Acceptor::Acceptor(EventBase* base, std::size_t port) :
    base_(base),
    event_(AcceptEvent::create(base,  ::socket(AF_INET, SOCK_STREAM, 0))),
    port_(port)
{
    ErrorCode err = listen();
    if (err)
        log_and_die(err.message());
}

void Acceptor::accept(Acceptor::Callback callback)
{
    if (!callback)
    {
        return;
    }
    event_->async_accept([this, callback] (AcceptEvent::Ptr event)
                {
                    TcpConn::Ptr conn(TcpConn::create(base_, event->newconn_fd()));
                    conn->set_address(event->address());
                    callback(conn);
                });
}

static const int kLISTENQ = 10;
ErrorCode Acceptor::listen()
{
    int ret;
    sockaddr_in serveraddr;
    ::memset(&serveraddr, 0, sizeof(sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY; //服务器IP地址--允许连接到所有本地地址上
    serveraddr.sin_port = ::htons(port_); //或者htons(SERV_PORT);
    int on = 1;
    ret = ::setsockopt(event_->fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ret = ::bind(event_->fd(), (sockaddr*)&serveraddr, sizeof(serveraddr));
    if (ret < 0)
    {
        ErrorCode err(errno);
        return err;
    }
    ret = ::listen(event_->fd(), kLISTENQ);
    if (ret < 0)
    {
        ErrorCode err(errno);
        return err;
    }
    return ErrorCode(ErrorCode::kOK, "ok");
}

} //namespace

