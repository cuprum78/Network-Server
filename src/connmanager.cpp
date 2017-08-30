#include <functional>
#include <thread>

#include <boost/bind.hpp>

#include "connmanager.h"


namespace Server
{

  //----------------------------
  ConnectionManager::ConnectionManager(boost::asio::io_service& ios, short port)
    : ios_(ios),
      tcpAcceptor_(ios)
  {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), port);
	tcpAcceptor_.open(ep.protocol());    
	tcpAcceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	tcpAcceptor_.bind(ep);
	tcpAcceptor_.listen();
  }

  //----------------------------
  void ConnectionManager::start()
  {
    return;
  }

  ConnectionManager::~ConnectionManager()
  {
    std::cout << "~ConnectionManager invoked tid: " << std::this_thread::get_id() << std::endl;
  }
  
  //----------------------------
  void ConnectionManager::asyncAccept(AcceptCB cb)
  {
    if (!cb)
      {
	//log error
	std::cout << "cb error" << std::endl;
	return;
      }

    std::cout << std::this_thread::get_id() << std::endl;
    conn.reset(new Server::ClientConn(ios_));
    tcpAcceptor_.async_accept(conn->socket(),
			      std::bind(&ConnectionManager::setupConnection,
					  getWeak(),		
					  std::placeholders::_1,
					  cb));

    std::cout << "started connection, move onto next request." << std::endl;
    
  }

  //----------------------------
  void ConnectionManager::setupConnection(std::weak_ptr<ConnectionManager> weakThis,
					  const boost::system::error_code& err,
					  AcceptCB cb)
  {
    std::shared_ptr<ConnectionManager> SharedThis = weakThis.lock();
    if (!SharedThis)
      {
	std::cout << "setupConnection failure" << std::endl;
	return;
      }
    
    if (!err)
      {
	std::cout << "accept connection." << std::endl;
	SharedThis->conn->start();	
      }

    cb(err);
  }

  //----------------------------
  std::weak_ptr<ConnectionManager> ConnectionManager::getWeak()
  {
    return shared_from_this();
  }
  

} // namespace
