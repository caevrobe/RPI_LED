#ifndef _WEBSOCKETS_H_INCLUDE_
#define _WEBSOCKETS_H_INCLUDE_

#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <iostream>



namespace websocket = boost::beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;



class WebSocketClient {
   public:
      WebSocketClient(const std::string& host, const std::string& port)
        : resolver_(ioc_), ws_(ioc_), host_(host), port_(port) {}
      
      void registerReadHandler(std::function<void(const std::string& msg)> read_handler);
      void connect_and_handshake();
      void run_async();
      void async_write();
      void async_read();
      void close();

   private:
      asio::io_context ioc_;
      tcp::resolver resolver_;
      websocket::stream<tcp::socket> ws_;
      boost::beast::flat_buffer buffer_;
      std::string host_;
      std::string port_;
      std::function<void(const std::string& msg)> _read_handler;
      bool shouldReconnect = true;
};

#endif