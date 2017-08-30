#include <algorithm>
#include <thread>

#include <boost/bind.hpp>

#include "connection.h"


namespace Server
{

  //----------------------------
  ClientConn::ClientConn(boost::asio::io_service& ios)
    : ios_(ios),
      socket_(ios),
      strand_(ios)
  {
  }

  //----------------------------
  ClientConn::~ClientConn()
  {
    std::cout << " ClientConn~ for soc:" << socket_.native_handle() <<std::endl;
  }
  
//----------------------------
void ClientConn::start()
{
  std::cout << " connection started soc:"
	    << socket_.native_handle()
	    << ", tid:"
	    << std::this_thread::get_id()
	    << std::endl;

  socket_.async_read_some(boost::asio::buffer(data_),
			  strand_.wrap(std::bind(&ClientConn::handleRead,
						 shared_from_this(),
						 std::placeholders::_1 ,
						 std::placeholders::_2 )));

}

//----------------------------
std::weak_ptr<ClientConn> ClientConn::getWeak()
{
  return shared_from_this();
}
  
//----------------------------
void ClientConn::handleRead(const boost::system::error_code& err,
			    size_t bytesTransferred)
{
  if (err)
  {
    std::cout << "connection failed, err:" << err <<std::endl;
      //dont queue up any thing, will go out of scope
    return;
  }
  
  //could use boost::serialize or protobuf to serialize if data gets complex.
  //for now just read it straight.
  //also in real cases this call has to be posted again if data read is not complete, based on a predefined custom header.
      
  //post work.
  std::string input(data_.data());
  std::cout<< input << std::endl;
  
  //post this on the common pool. 
  strand_.post(std::bind(&ClientConn::doWork,
			 shared_from_this(),
			 input));
 
}


//----------------------------  
bool ClientConn::checkPrimality(long long int input)
{
    if (input <= 1)
    {
      return false;	  
    }
    for(long long int i=2; i < input; i++)
    {
      if (input%i == 0)
      {
	return false;
      }
    }
    return true;
}
  
//----------------------------
void ClientConn::doWork(const std::string& input)
{      
     //Get some work done.
     std::cout << "Doing work on thread:"
	       << std::this_thread::get_id()
	       << " with input:"
	       << input
	       << std::endl;

      long long int lli = atoll(input.c_str()); 

      bool result = checkPrimality(lli);
      
      std::string strresult = (result ? "true" : "false");
      std::fill(std::begin(data_), std::end(data_), 0);
      std::copy(strresult.begin(), strresult.end(), data_.data());
      
      //post the result back to the client.
      boost::asio::async_write(socket_,
			       boost::asio::buffer(data_),
			       strand_.wrap(std::bind(&ClientConn::handleWrite,
						      shared_from_this(),
						      std::placeholders::_1 /*error*/,
						      std::placeholders::_2 /*bytes trans*/)));
}
  
//----------------------------
  void ClientConn::handleWrite(const boost::system::error_code& err, size_t bytesTransferred)
{
  if (!err)
  {
    std::cout << "write done." << std::endl;
    boost::system::error_code nothing;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, nothing);
  }
  else
    {
      std::cout << "write err:" << err << std::endl;
    }
}

//----------------------------
boost::asio::ip::tcp::socket& ClientConn::socket()
 {
   return socket_;
 }

} // namespace
