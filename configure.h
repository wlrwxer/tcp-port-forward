//
//  configure.h
//  mars_online_server
//
//  Created by baba on 3/11/13.
//  Copyright (c) 2013 baba. All rights reserved.
//

#pragma once

#include <mutex>
#include <unordered_map>
#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>

#include "configure.pb.h"

#include "logger.hpp"

class global {
    global()
    : current_(new ::configure())
    , backup_(new ::configure())
    {
    

    }
    
    ::configure *current_;
    ::configure *backup_;
    
    
    std::string lua_file_;

public:

    
    static global& instance() {
        static global g;
        return g;
    }
    
    static std::shared_ptr<boost::asio::deadline_timer> make_timer(boost::asio::io_service& io_service, uint32_t millisec) {
        auto xtimer = std::make_shared<boost::asio::deadline_timer>(io_service);
        xtimer->expires_from_now(boost::posix_time::millisec(millisec));
        return xtimer;
    }
    
//    static boost::asio::io_service& io_service() {
//        static boost::asio::io_service io_service;
//        return io_service;
//    }

    int update_configure_from_lua(const char *luafile);
    const ::configure& configure() const { return *current_; }

};
