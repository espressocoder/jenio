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
/// @file acceptor.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <functional>
#include "event.h"
#include "eventbase.h"
#include "tcp_conn.h"

namespace jenio
{
class Acceptor
{
public:
   typedef std::function<void(TcpConn::Ptr)> Callback;
public:
    Acceptor(EventBase* base, std::size_t port);
    void accept(Callback callback);
    ErrorCode listen();
private:
    EventBase* base_;
    AcceptEvent::Ptr event_;
    std::size_t port_;
};
}//namespace
#endif // ACCEPTOR_H
