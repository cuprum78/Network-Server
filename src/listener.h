//
#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H

#include <iostream>
#include <atomic>
#include <memory>

#include <boost/asio.hpp>

#include "connmanager.h"

namespace Server
{

class Listener : public std::enable_shared_from_this<Listener>
{
 public:
  Listener(size_t noofthreads, short port);
  void run();
  void stop();
  int getCurrentRequestCount() const;
  
 private:
  void acceptNext();
  static void handleNext(std::weak_ptr<Listener> weakThis,
			 const boost::system::error_code& err);
  void exiting();
  std::weak_ptr<Listener> getWeak();

  boost::asio::io_service ios_;
  std::atomic<bool> running_;
  size_t no_of_threads_;
  std::shared_ptr<ConnectionManager> acceptor_;
  boost::asio::signal_set signals_;
  std::shared_ptr<boost::asio::io_service::work> fillerWork_;
  std::atomic<int> currentReqCount_;
};
 
} // namespace

#endif // SERVER_LISTENER_H
