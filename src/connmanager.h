#ifndef SERVER_CONNMANAGER_H
#define SERVER_CONNMANAGER_H

#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "connection.h"

namespace Server
{

  class ConnectionManager : public std::enable_shared_from_this<ConnectionManager>
{
 public:
  typedef std::function<void(const boost::system::error_code&)> AcceptCB;
  
  ConnectionManager(boost::asio::io_service& ios, short port);
  void asyncAccept(AcceptCB cb);
  void start();
  ~ConnectionManager();
  
 private:
  static void setupConnection(std::weak_ptr<ConnectionManager> weakThis,
		       const boost::system::error_code& err,
		       AcceptCB cb);
  
  std::weak_ptr<ConnectionManager> getWeak();
  
  boost::asio::io_service& ios_;
  boost::asio::ip::tcp::acceptor tcpAcceptor_;
  std::shared_ptr<Server::ClientConn> conn;
};
  


} // namespace

#endif // SERVER_CONNMANAGER_H
