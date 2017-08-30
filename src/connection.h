#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <iostream>
#include <array>

#include <boost/asio.hpp>

namespace Server
{
  class ClientConn : public std::enable_shared_from_this<ClientConn>
  {
  public:
    ClientConn(boost::asio::io_service& ios);
    void start();
    boost::asio::ip::tcp::socket& socket();
    ~ClientConn();
    
  private:
    void handleRead(const boost::system::error_code& err,
		    size_t bytesTransferred);    
    void handleWrite(const boost::system::error_code& err,
		     size_t bytesTransferred);
    std::weak_ptr<ClientConn> getWeak();
    void doWork(const std::string& input);
    bool checkPrimality(long long int input);

    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::io_service::strand strand_;
    std::array<char, 1024> data_;
  };

} // Server

#endif // SERVER_CONNECTION_H
