CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -I. -I/mo_server_runtime/libcxx/include -I/home/baba/development/mo/include-c++11 -g -O2  -DENABLE_TRACE -DENABLE_DEBUG -DENABLE_INFO -DENABLE_WARNING -DENABLE_ERROR -DENABLE_FETAL -DENABLE_LOG_TO_CONSOLE -DENABLE_LOG4CPLUS
LDFLAGS=-L/home/baba/development/boost-trunk/stage/lib -lboost_log -lboost_log_setup -lboost_thread -lboost_system -L/usr/local/lib -lprotobuf -lcurl -ltcmalloc_minimal -lprotobuf -lluabind -llog4cplus
LIBS=-llua -lc++ -lpthread
AUTOMAKE_OPTIONS= foreign 
bin_PROGRAMS= load_server
load_server_SOURCES= main.cpp configure.cpp configure.pb.cc
