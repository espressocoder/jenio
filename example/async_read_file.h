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
/// @file async_read_file.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-08-23

#ifndef ASYNC_READ_FILE_H
#define ASYNC_READ_FILE_H
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include "../eventbase.h"

using namespace jenio;

void callback(ReadEvent::Ptr event)
{
    if (!event->error().ok()) 
    {
        std::cout << "error!" << event->error().message() << std::endl;
        return;
    }
    if (event->size() == 0)
    {
        return;
    }
    std::string str(event->buffer(), event->size());
    std::cout << str << std::endl;
    event->async_read(callback);
}

int example_main_readfile()
{
    EventBase base;
    int fd = ::open("test.txt", O_RDONLY);
    if (fd<0)
      return -1;
    
    {
        ReadEvent::Ptr event(ReadEvent::create(&base, fd));
        char * buffer = new char[5];
        event->set_buffer(buffer, 5);
        event->async_read(callback);
    }
    base.run();
    return 0;
}
#endif // ASYNC_READ_FILE_H

