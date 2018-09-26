//
//  main.cpp
//  port-forward
//
//  Created by baba on 13-10-29.
//  Copyright (c) 2013年 baba. All rights reserved.
//

#include <iostream>
#include <thread>
#include <array>

#include <string.h>

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>

#include <openssl/ssl.h>

#include "message_block.hpp"
#include "helper.hpp"
#include "configure.h"

#include "evp-encrypt-using.hpp"


//#define PASSTHROUGH

#define BUFSIZE 256

#pragma pack(push, 1)

struct HEAD {
    uint16_t length;
    uint64_t uid;
};

#pragma pack(pop)

typedef std::shared_ptr<message_block> shared_ptr_message_block;

typedef std::shared_ptr<boost::asio::ip::tcp::socket> shared_ptr_socket;

template<typename T, typename C>
class acceptor : public std::enable_shared_from_this<acceptor<T, C>> {
    std::function<boost::asio::io_service&()> get_io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    C context_;
    
public:
    acceptor(std::function<boost::asio::io_service&()> get_io_service, const char *addr, const int port, C context)
    : get_io_service_(get_io_service)
    , acceptor_(get_io_service_(), boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(addr), port))
    , context_(context) {}
    virtual ~acceptor() {}
    
public:
    void startup_async_accept() {
        auto &io_service = get_io_service_();
        auto peer(std::make_shared<boost::asio::ip::tcp::socket>(io_service));
        acceptor_.async_accept(*peer,
                               std::bind(&acceptor::async_accept_handler,
                                         std::enable_shared_from_this<acceptor<T, C>>::shared_from_this(),
                                         peer,
                                         std::ref(io_service),
                                         std::placeholders::_1));
    }
    
    void async_accept_handler(shared_ptr_socket peer,
                              boost::asio::io_service &io_servcie,
                              const boost::system::error_code &ec) {
        startup_async_accept();
        if (ec) {
            print_error("acceptor::async_accept_handler failure %d\n", ec.value());
        } else {
            static boost::asio::ip::tcp::no_delay option(true);
            peer->set_option(option);
            auto obj(std::make_shared<T>(io_servcie, context_));
            obj->startup(peer);
        }
    }
};


typedef std::list<shared_ptr_message_block> buffers_list;

typedef std::function<shared_ptr_message_block(shared_ptr_message_block)> preprocesser_f;

typedef std::tuple<shared_ptr_socket, buffers_list, preprocesser_f, std::string> forwared_peer_context;

template<typename C>
class forward_peer : public std::enable_shared_from_this<forward_peer<C>> {
    
    typedef forward_peer<C> THIS_T;
    
    
    
    message_block pool_;
    uint64_t uid_;
    
private:
    const bool has_uid() const  { return (uid_ != 0); }
    
public:
    forward_peer(boost::asio::io_service& io_service, C ep)
    : io_service_(io_service)
    , forward_endport_(ep)
    , pool_(1024 * 1024 * 2)
    , uid_(0) {}
    
    ~forward_peer() {}
    
    
private:
    
    
    shared_ptr_message_block on_incoming_data_handler_decrypt(shared_ptr_message_block data) {
        
        std::cout << "on_incoming_data_handler_decrypt" << std::endl;
        
        // not allowed a packet which size exceed 2MB
        if (pool_.length() + data->length() > pool_.capacity()) {
            std::cout << "on_incoming_data_handler_decrypt packet tooooooo length" << std::endl;
            return nullptr;
        }

        pool_ << *data;
        
        if (pool_.length() < sizeof(HEAD)) {
            std::cout << "on_incoming_data_handler_decrypt not enough length" << std::endl;
            return nullptr;
        }
        
        auto const header = pool_.reference<HEAD>();
        
        if (!has_uid()) {
            
            uid_ = header.uid;
            
        }
        
        if (pool_.length() < header.length) {
            std::cout << "on_incoming_data_handler_decrypt not enough data" << std::endl;
            return nullptr;
        }
        
        // do data decrypt here
        
        secure_string plain_text;
        secure_string cipher_text(pool_.rd_ptr() + sizeof(HEAD), header.length - sizeof(HEAD));
        
        evp_encrypt_aes_256_cfb engine;
        
        engine.decrypt(cipher_text, plain_text);
        
        auto r = message_block::from_size(plain_text.length());
       
        r->copy(plain_text.data(), plain_text.length());
        
        pool_.rd_ptr(header.length);
        
        pool_.crunch();
        
        return r;
    }
    
    
    shared_ptr_message_block on_outgoing_data_handler_encrypt(shared_ptr_message_block data) {

        std::cout << "on_outgoing_data_handler_encrypt" << std::endl;

        secure_string cipher_text;
        secure_string plain_text(data->rd_ptr(), data->length());
        
        evp_encrypt_aes_256_cfb engine;
        
        engine.encrypt(plain_text, cipher_text);

        auto result = message_block::from_size(sizeof(HEAD) + cipher_text.length());

        assert(has_uid());
        
        HEAD header = { uint16_t(sizeof(HEAD) + cipher_text.length()), uid_};
        
        *result << header;
        
        result->copy(cipher_text.data(), cipher_text.length());
        
        return result;
    }
    
private:
    void async_receive_startup(std::shared_ptr<forwared_peer_context> receiver,
                               std::shared_ptr<forwared_peer_context> sender) {
        auto buffer(message_block::from_size(1024*32));
        std::get<0>(*receiver)->async_receive(boost::asio::buffer(buffer->wr_ptr(), buffer->capacity()),
                                              std::bind<int>(&forward_peer::async_receive_handler,
                                                             std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                                             receiver,
                                                             sender,
                                                             buffer,
                                                             std::placeholders::_1,
                                                             std::placeholders::_2));
    };
    
public:
    void async_connect_handler(std::shared_ptr<forwared_peer_context> receiver,
                               std::shared_ptr<forwared_peer_context> sender,
                               std::weak_ptr<boost::asio::deadline_timer> timer,
                               const boost::system::error_code& ec) {
        if (ec) {
            print_error("forward_peer::async_connect_handler failure %d, retry next element.\n", ec.value());
            startup(std::get<0>(*receiver));
        } else {
            auto xtimer = timer.lock();
            if (xtimer) {
                xtimer->cancel();
            }
            
            async_receive_startup(receiver, sender);
            async_receive_startup(sender, receiver);
        }
    }
    void async_connect_handler_2(std::shared_ptr<forwared_peer_context> receiver,
                                 std::shared_ptr<forwared_peer_context> sender,
                                 std::weak_ptr<boost::asio::deadline_timer> timer,
                                 const boost::system::error_code& ec,
                                 boost::asio::ip::tcp::resolver::iterator iterator,
                                 shared_ptr_socket forward_peer,
                                 shared_ptr_socket incoming_peer) {
        if (ec) {
            print_error("forward_peer::async_connect_handler failure %d, retry next element.\n", ec.value());
            
#if !defined(PASSTHROUGH)
            
            preprocesser_f encrypt = std::bind(&forward_peer::on_outgoing_data_handler_encrypt,
                                               std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                               std::placeholders::_1);
            preprocesser_f decrypt = std::bind(&forward_peer::on_incoming_data_handler_decrypt,
                                               std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                               std::placeholders::_1);
            
#else
            
            preprocesser_f encrypt = std::bind([](shared_ptr_message_block data) -> shared_ptr_message_block {
                return data;
            }, std::placeholders::_1);
            
            preprocesser_f decrypt = std::bind([](shared_ptr_message_block data) -> shared_ptr_message_block {
                return data;
            }, std::placeholders::_1);
            
#endif
            
            
            if (iterator != boost::asio::ip::tcp::resolver::iterator()) {
                auto timer(std::make_shared<boost::asio::deadline_timer>(io_service_));
                timer->expires_from_now(boost::posix_time::seconds(1));
                std::weak_ptr<boost::asio::deadline_timer> wptr_timer = timer;
                
                auto epp = *iterator;
                forward_peer->async_connect(epp,
                                            std::bind(&forward_peer::async_connect_handler_2,
                                                      std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                                      std::make_shared<forwared_peer_context>(std::make_tuple(incoming_peer, buffers_list(), decrypt, std::string(">>>>>"))),
                                                      std::make_shared<forwared_peer_context>(std::make_tuple(forward_peer, buffers_list(), encrypt, std::string("<<<<<"))),
                                                      wptr_timer,
                                                      std::placeholders::_1,
                                                      ++iterator,
                                                      forward_peer,
                                                      incoming_peer));
                
            } else {
                
                startup(std::get<0>(*receiver));
            }
        } else {
            auto xtimer = timer.lock();
            if (xtimer) {
                xtimer->cancel();
            }
            
            async_receive_startup(receiver, sender);
            async_receive_startup(sender, receiver);
        }
    }
public:
    int startup(shared_ptr_socket &incoming_peer) {
        auto forward_peer(std::make_shared<boost::asio::ip::tcp::socket>(io_service_));
        
        auto timer(std::make_shared<boost::asio::deadline_timer>(io_service_));
        timer->expires_from_now(boost::posix_time::seconds(3));
        std::weak_ptr<boost::asio::deadline_timer> wptr_timer = timer;
        
        boost::asio::ip::tcp::endpoint epp;
        
        
        
#if !defined(PASSTHROUGH)
        
        preprocesser_f encrypt = std::bind(&forward_peer::on_outgoing_data_handler_encrypt,
                                           std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                           std::placeholders::_1);
        preprocesser_f decrypt = std::bind(&forward_peer::on_incoming_data_handler_decrypt,
                                           std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                           std::placeholders::_1);
        
#else
        
        preprocesser_f encrypt = std::bind([](shared_ptr_message_block data) -> shared_ptr_message_block {
            return data;
        }, std::placeholders::_1);
        
        preprocesser_f decrypt = std::bind([](shared_ptr_message_block data) -> shared_ptr_message_block {
            return data;
        }, std::placeholders::_1);
        
#endif

        
        std::pair<std::string, uint32_t>& endp = forward_endport_();
        
        
        boost::regex expression(R"---(^\d+.\d+.\d+.\d+$)---");
        boost::cmatch what;
        
        if (boost::regex_match(endp.first.c_str(), what, expression)) {
            epp = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(endp.first), endp.second);
            
            forward_peer->async_connect(epp,
                                        std::bind(&forward_peer::async_connect_handler,
                                                  std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                                  std::make_shared<forwared_peer_context>(std::make_tuple(incoming_peer, buffers_list(), decrypt, std::string(">>>>>"))),
                                                                                          std::make_shared<forwared_peer_context>(std::make_tuple(forward_peer, buffers_list(), encrypt, std::string("<<<<<"))),
                                                  wptr_timer,
                                                  std::placeholders::_1));
            
        } else {
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::resolver::query query(endp.first, boost::str(boost::format("%d") % endp.second));
            //            boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
            
            
            boost::system::error_code ect;
            boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query, ect);
            
            if (ect) {
                forward_peer::shutdown(forward_peer);
                
                print_trace("forward::startup %s\n", ect.message().c_str());
                return 0;
            } else {
                epp = *iterator;
                forward_peer->async_connect(epp,
                                            std::bind(&forward_peer::async_connect_handler_2,
                                                      std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                                      std::make_shared<forwared_peer_context>(std::make_tuple(incoming_peer, buffers_list(), decrypt, std::string(">>>>>"))),
                                                      std::make_shared<forwared_peer_context>(std::make_tuple(forward_peer, buffers_list(), encrypt, std::string("<<<<<"))),
                                                      wptr_timer,
                                                      std::placeholders::_1,
                                                      ++iterator,
                                                      forward_peer,
                                                      incoming_peer));
            }

        }
        
        
        
        
        struct __helper {
            static void on_timer(std::shared_ptr<boost::asio::deadline_timer> timer,
                                 shared_ptr_socket peer,
                                 const boost::system::error_code& ec) {
                if (ec) {
                    print_trace("forward::startup::__helper::on_timer %d\n", ec.value());
                } else {
                    forward_peer::shutdown(peer);
                }
            }
        };
        
        timer->async_wait(std::bind(__helper::on_timer,
                                    timer,
                                    forward_peer,
                                    std::placeholders::_1));
        
        return 0;
    }
    
    int async_send_handler(std::shared_ptr<forwared_peer_context> receiver,
                           std::shared_ptr<forwared_peer_context> sender,
                           boost::system::error_code ec,
                           size_t bytes_transferred) {
        if (ec) {
            print_error("forward_peer::async_send_handler failure %d %s\n", ec.value(), std::get<3>(*sender).c_str());
            shutdown(std::get<0>(*receiver));
        } else {
            auto &buffer = std::get<1>(*sender).front();
            buffer->rd_ptr(bytes_transferred);
            
            if (buffer->empty()) {
                std::get<1>(*sender).pop_front();
            }
            
            if (!std::get<1>(*sender).empty()) {
                auto &buf = std::get<1>(*sender).front();
                std::get<0>(*sender)->async_send(boost::asio::buffer(buf->rd_ptr(), buf->length()),
                                                 std::bind(&forward_peer::async_send_handler,
                                                           std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                                           receiver,
                                                           sender,
                                                           std::placeholders::_1,
                                                           std::placeholders::_2));
            }
        }
        return 0;
    }
    
    static void shutdown(shared_ptr_socket peer) {
        boost::system::error_code ec(boost::asio::error::timed_out);
        peer->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    }
    
    int async_receive_handler(std::shared_ptr<forwared_peer_context> receiver,
                              std::shared_ptr<forwared_peer_context> sender,
                              shared_ptr_message_block buffer,
                              boost::system::error_code ec,
                              size_t bytes_transferred) {
        if (ec || bytes_transferred == 0) {
            print_error("forward_peer::async_receive_handler failure %d %s\n", ec.value(), std::get<3>(*receiver).c_str());
            shutdown(std::get<0>(*sender));
        } else {
            buffer->wr_ptr(bytes_transferred);
//#if 0
//            {
//                char fn[1024];
//                memset(fn, 0, sizeof(fn));
//                sprintf(fn, "%x.bin", &*receiver);
//
//                FILE *fp = fopen(fn, "ab");
//                assert(fp);
//                auto rc = fwrite(buffer->data().begin(), 1, buffer->size(), fp);
//                assert(rc == buffer->size());
//                fflush(fp);
//                fclose(fp);
//            }
//#endif
            
            std::cout << "receiving data from " << std::get<3>(*receiver) << " " << buffer->length() << " bytes" << std::endl;
            
            async_receive_startup(receiver, sender);
            
            auto buffer1 = std::get<2>(*receiver)(buffer);
            
            if (buffer1) {
                
                auto &buffers = std::get<1>(*sender);
                if (buffers.empty()) {
                    buffers.push_back(buffer1);
                    auto &buffer = buffers.front();
                    std::get<0>(*sender)->async_send(boost::asio::buffer(buffer->rd_ptr(), buffer->length()),
                                                     std::bind(&forward_peer::async_send_handler,
                                                               std::enable_shared_from_this<THIS_T>::shared_from_this(),
                                                               receiver,
                                                               sender,
                                                               std::placeholders::_1,
                                                               std::placeholders::_2));
                } else {
                    buffers.push_back(buffer1);
                }
            }
        }
        return 0;
    }
    
private:
    boost::asio::io_service &io_service_;
    C forward_endport_;
};

int main(int argc, const char * argv[]) {
    
    if (argc < 2) {
        printf("USAGE:%s configure.lua\n", argv[0]);
        return 0;
    }
    
    if (global::instance().update_configure_from_lua(argv[1])) {
        fprintf(stderr, "::main can not load configure from lua.\n");
        return 1;
    }
    
    if (global::instance().configure().run_as_daemon()) {
        helper::init_daemon(1,"/tmp",
                            global::instance().configure().has_server_crash_run() ? global::instance().configure().server_crash_run().data() : nullptr);
    }
    
    
    
    //    loggee::init("loader", 0,
    //                 1024*1024*100,
    //                 1);
    
    print_info("::main startup ...\n");
    
    std::vector<boost::asio::io_service*> io_services;
    for (auto i = 0; i < global::instance().configure().nb_services(); ++i) {
        
        io_services.push_back(new boost::asio::io_service());
        new boost::asio::io_service::work(*io_services.back());
        
    }

    
    auto xf = [](std::vector<boost::asio::io_service*> &ios) ->boost::asio::io_service& {
        static std::size_t index = 0;
        return *ios[++index % ios.size()];
    };
    
    typedef std::tuple<std::pair<std::string, uint32_t>, std::shared_ptr<std::atomic<uint64_t>>> element;
    typedef std::vector<element> endpoints;
    
    auto lambda_ep_provider = [](std::shared_ptr<endpoints> eps,
                                 std::shared_ptr<std::atomic<uint64_t>> total_dispatched,
                                 ::addr_info a,
                                 std::shared_ptr<std::atomic<uint32_t>> counter) ->std::pair<std::string, uint32_t>& {
        //		static uint32_t pos = 0;
        print_info("dispatch for local %s:%d remote:%u  already dispatched: %u ", a.addr().data(), a.port(), eps->size(), counter->load());
        auto &ep = (*eps)[++*counter % eps->size()];
        
        ++(*total_dispatched);
        ++(*std::get<1>(ep));
        return std::ref(std::get<0>(ep));
    };
    
    for (auto i = 0; i < global::instance().configure().maps_size(); ++i) {
        auto provider = std::make_shared<endpoints>();
        print_info("\n\n\n");
        for (auto j = 0; j < global::instance().configure().maps(i).remotes_size(); ++j) {
            auto addr = global::instance().configure().maps(i).remotes(j).addr();
            auto port = global::instance().configure().maps(i).remotes(j).port();
            print_info("remote addr:%s %d", addr.data(), port);
            element e = std::make_tuple(//boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(addr), port),
                                        std::make_pair(addr, port),
                                        std::make_shared<std::atomic<uint64_t>>(0));
            provider->push_back(e);
            
        }
        
        typedef std::function<std::pair<std::string, uint32_t>&()> endpoint_provider;
        
        typedef acceptor<forward_peer<endpoint_provider>,endpoint_provider> ACCEPTOR;
        
        if (!provider->empty()) {
            
            auto total_dispatched = std::make_shared<std::atomic<uint64_t>>(0);
            
            for (auto j = 0; j < global::instance().configure().maps(i).locals_size(); ++j) {
                auto addr = global::instance().configure().maps(i).locals(j).addr();
                auto port = global::instance().configure().maps(i).locals(j).port();
                print_info("local addr:%s %d", addr.data(), port);
                
                typedef decltype(global::instance().configure().maps(i).locals(j)) xtype;
                auto acceptor = std::make_shared<ACCEPTOR>(std::bind(xf, std::ref(io_services)),
                                                           addr.c_str(),
                                                           port,
                                                           std::bind(lambda_ep_provider,
                                                                     provider,
                                                                     total_dispatched,
                                                                     global::instance().configure().maps(i).locals(j),
                                                                     std::make_shared<std::atomic<uint32_t>>(0)));
                acceptor->startup_async_accept();
            }
//            struct __helper {
//                static void on_timer(std::shared_ptr<boost::asio::deadline_timer> timer,
//                                     const ::addr_map &am,
//                                     std::shared_ptr<endpoints> peps,
//                                     std::shared_ptr<std::atomic<uint64_t>> total_dispatched,
//                                     const boost::system::error_code& ec) {
//                    if (ec) {
//                        print_error("::main::__helper::on_timer %d\n", ec.value());
//                    } else {
//                        std::string s;
//                        char buf[1024];
//                        memset(buf, 0, sizeof(buf));
//
//                        for (auto i = 0; i < am.locals_size(); ++i) {
//                            sprintf(buf, "%s:%u <- ", am.locals(i).addr().c_str(), am.locals(i).port());
//                            s += buf;
//                        }
//
//                        sprintf(buf, "total dispatched times: %lu ", total_dispatched->load());
//
//                        s += buf;
//
//                        for (auto &x: *peps) {
//                            memset(buf, 0, sizeof(buf));
//                            sprintf(buf, "-> %s:%u %lu ", std::get<0>(x).first.c_str(),
//                                    std::get<0>(x).second,
//                                    std::get<1>(x)->load());
//                            s += buf;
//                        }
//
//                        print_info("%s ", s.c_str());
//
//
//                        timer->expires_from_now(boost::posix_time::seconds(10));
//                        timer->async_wait(std::bind(__helper::on_timer,
//                                                    timer,
//                                                    std::ref(am),
//                                                    peps,
//                                                    total_dispatched,
//                                                    std::placeholders::_1));
//
//                    }
//
//
//                }
//            };
//
//
//            auto timer(std::make_shared<boost::asio::deadline_timer>(xf(io_services)));
//            timer->expires_from_now(boost::posix_time::seconds(1));
//            timer->async_wait(std::bind(__helper::on_timer,
//                                        timer,
//                                        std::ref(global::instance().configure().maps(i)),
//                                        provider,
//                                        total_dispatched,
//                                        std::placeholders::_1));
        }
    }
    
    
    print_info("startup asio threads pool ...\n");
    printf    ("startup asio threads pool ...\n");
    
    for (auto i = 0; i < io_services.size() - 1; ++i) {
        static auto f = static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run);
        boost::asio::io_service& io = *io_services[i];
        std::thread(std::bind(f, &io)).detach();
    }
    io_services.back()->run();
    return 0;
}

