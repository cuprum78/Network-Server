
#include <functional>
#include <thread>

#include <boost/bind.hpp>

#include "listener.h"

namespace Server
{

  //----------------------------
  Listener::Listener(size_t noofthread, short port)
    : ios_(),
      running_(false),
      no_of_threads_(noofthread),
      signals_(ios_),
      acceptor_(new Server::ConnectionManager(ios_, port)),
      fillerWork_(new boost::asio::io_service::work(ios_)),
      currentReqCount_(0)
  {
  }

  //----------------------------
  std::weak_ptr<Listener> Listener::getWeak()
  {
    return shared_from_this();
  }
  
  //----------------------------
  void Listener::acceptNext()
  {
    if (!acceptor_ || !running_)
      {	
	return;
      }

    acceptor_->asyncAccept(std::bind(&Listener::handleNext,
				     getWeak(),
				     std::placeholders::_1));
  }

  //----------------------------
  void Listener::run()
  {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(std::bind(&Listener::exiting, shared_from_this()));

    //start up the listener.
    acceptor_->start();
    running_ = true;
    acceptNext();
    
    std::vector<std::shared_ptr<std::thread>> threads;
    
    //this placement is needed for std::bind to resolve ambiguity between run(err) & run()
    size_t (boost::asio::io_service::*run)() = &boost::asio::io_service::run;
    
    for(size_t i = 0; i < no_of_threads_; ++i)
      {
	std::shared_ptr<std::thread> t(new std::thread(std::bind(run,
								 &ios_)));	
	if (t)
	  {
	    threads.push_back(t);
	  }
      }

    
    for(size_t i = 0; i < threads.size(); ++i)
      {
	threads[i]->join();
      }
  }
  
  //----------------------------
  void Listener::handleNext(std::weak_ptr<Listener> weakThis,
			    const boost::system::error_code& err)
  {
    std::shared_ptr<Listener> sharedThis = weakThis.lock();
    if (!sharedThis)
    {
	  return;
    }
    
    if (err)
    {
	//Log the error.
    }

    sharedThis->currentReqCount_++;
    sharedThis->acceptNext();
  }
  
  //----------------------------
  void Listener::stop()
  {
    running_ = false;
    return;
  }

  //----------------------------
  int Listener::getCurrentRequestCount() const
  {
    return currentReqCount_;
  }
  
  //----------------------------
  void Listener::exiting()
  {
    std::cout << "quitting..." <<std::endl;
    running_ = false;
    fillerWork_.reset();
    ios_.stop();
  }

  
} // namespace Server
