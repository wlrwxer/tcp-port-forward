//
//  configure.cpp
//  mars_online_server
//
//  Created by baba on 9/3/13.
//  Copyright (c) 2013 baba. All rights reserved.
//

#include <luabind/luabind.hpp>
#include "configure.h"

void logging_lua(const char *) {
    
}

int global::update_configure_from_lua(const char *luafile) {

    assert(luafile);

    lua_file_ = luafile;
    try {
        boost::shared_ptr<lua_State> L(luaL_newstate(), boost::bind(lua_close, _1));
        luaL_openlibs(&*L);
        luabind::open(&*L);
        luabind::module(&*L)
        [
         luabind::def("logging", logging_lua),

#include "tcp_port_forward/configure.cc"
         ];
        if (luaL_dofile(&*L, luafile)) {
            print_error("global::update_configure_from_lua lua_dofile failed.");
            return 1;
        }
        luabind::call_function<void>(&*L, "update_configure", backup_);
        std::swap(current_, backup_);

    } catch (luabind::error& e) {
        print_error("global::update_configure_from_lua %s", e.what());
    }
    return 0;
}



