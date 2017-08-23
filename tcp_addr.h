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
/// @file tcp_addr.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef TCPADDR_H
#define TCPADDR_H
#include <string>
namespace jenio
{

class TcpAddr
{
public:
    TcpAddr();
    TcpAddr(std::string ip, std::size_t port);
    const std::string& ip() const;
    std::size_t port() const;
private:
    std::string ip_;
    std::size_t port_;
};

inline TcpAddr::TcpAddr() :
    ip_(),
    port_(0)
{

}

inline TcpAddr::TcpAddr(std::string ip, std::size_t port) :
    ip_(std::move(ip)),
    port_(port)
{
}

inline const std::string &TcpAddr::ip() const
{
    return ip_;
}

inline std::size_t TcpAddr::port() const
{
    return port_;
}

} //namespace
#endif // TCPADDR

