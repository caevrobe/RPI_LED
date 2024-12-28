#include "websockets.h"

void logError(const std::string& handler, boost::system::error_code ec) {
   std::cerr << handler << " failed: " << ec.message() << " (" << ec.value() << ")" << std::endl; 
}

void WebSocketClient::registerReadHandler(std::function<void(const std::string& msg)> read_handler) {
   _read_handler = read_handler;
}

void WebSocketClient::connect_and_handshake() {
   std::promise<void> handshake_promise;
   auto handshake_future = handshake_promise.get_future();

   // Resolve the host
   std::cout << "Resolving host..." << std::endl;
   resolver_.async_resolve(host_, port_,
      [this, &handshake_promise](boost::system::error_code ec, tcp::resolver::results_type results) {
         if (ec) {
            logError("Resolve", ec);
            if (shouldReconnect)
               connect_and_handshake();
            handshake_promise.set_exception(std::make_exception_ptr(std::runtime_error(ec.message())));
            return;
         }

         std::cout << "Connecting to host..." << std::endl;

         // Connect to the host
         asio::async_connect(ws_.next_layer(), results,
            [this, &handshake_promise](boost::system::error_code ec, tcp::endpoint) {
               if (ec) {
                     logError("Connect", ec);
                     if (shouldReconnect)
                        connect_and_handshake();
                     handshake_promise.set_exception(std::make_exception_ptr(std::runtime_error(ec.message())));
                     return;
               }

               std::cout << "Connection established, performing handshake..." << std::endl;
               // Perform WebSocket handshake
               ws_.async_handshake(host_, "/",
                  [&handshake_promise, this](boost::system::error_code ec) {
                     if (ec) {
                        logError("Handshake", ec);
                        if (shouldReconnect)
                           connect_and_handshake();
                        handshake_promise.set_exception(std::make_exception_ptr(std::runtime_error(ec.message())));
                     } else {
                        std::cout << "Handshake successful!" << std::endl;
                        handshake_promise.set_value();

                        async_write();
                     }
               });
         });
   });

   // Run the I/O context until the handshake completes
   //ioc_.run();
   while (ioc_.run_one()) {
      if (handshake_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
         break;
      }
   }

   //handshake_future.get(); 
   handshake_future.get(); // Block until the handshake is complete or an exception is thrown
}

void WebSocketClient::run_async() {
   try {
      std::cout << "Starting IO context." << std::endl;
      ioc_.run();
      std::cout << "IO context stopped." << std::endl;
   } catch (const std::exception &e) {
      std::cerr << "I/O Thread Exception: " << e.what() << "\n";

      if (shouldReconnect) {
         ioc_.restart();
         run_async();
      }
   }
}

void WebSocketClient::async_write() {
   std::string message = "listener";
   ws_.async_write(asio::buffer(message),
      [this](boost::system::error_code ec, std::size_t bytes_transferred) {
         if (ec) {
            logError("Send", ec);
            if (shouldReconnect)
               connect_and_handshake();  
         } else {
            std::cout << "Sent " << bytes_transferred << " bytes\n";
            // Start reading after sending message
            async_read();
         }
   });
}

void WebSocketClient::async_read() {
   ws_.async_read(buffer_,
      [this](boost::system::error_code ec, std::size_t bytes_transferred) {
         if (!ec) {
            //std::cout << "Received: " << boost::beast::make_printable(buffer_.data()) << "\n";
            
            _read_handler(boost::beast::buffers_to_string(buffer_.data()));
            buffer_.consume(bytes_transferred); // Clear the buffer

            // Continue reading
            async_read();
         } else {
            //if (ec.value() == boost::system::errc::address_family_not_supported)
            logError("Read", ec);
            if (shouldReconnect)
               connect_and_handshake();
         }
   });
}

void WebSocketClient::close() {
   shouldReconnect = false;

   ws_.async_close(websocket::close_code::normal, [this](boost::system::error_code ec) {
      if (ec) {
         logError("Close", ec);
      } else {
         std::cout << "Connection closed\n";
      }
   });
}