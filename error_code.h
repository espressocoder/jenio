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
/// @file error_code.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef ERRORCODE_H
#define ERRORCODE_H
#include <string>
#include <string.h>
#include <stdio.h>
namespace jenio
{

class ErrorCode
{
public:
    static const int kOK = -1;
    static const int kERROR = kOK - 1;
    static const int kEOF = kERROR - 1;
    ErrorCode();
    ErrorCode(int code, std::string msg);
    ErrorCode(int errno_code);
    bool ok() const;
    bool error() const;
    int code() const;
    const std::string& message() const;
    operator bool() const;
    bool operator!() const;
    void set_code(int code);
    void set_message(std::string msg);
private:
    int error_code_;
    std::string msg_;
};


} //namespace
#endif // ERRORCODE_H
