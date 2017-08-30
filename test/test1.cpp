#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE listenerTests

#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>

#include <chrono>
#include <iostream>
#include <thread>
#include <stdlib.h>

#include "client.h"
#include "listener.h"


BOOST_AUTO_TEST_SUITE(TestListener)

const short port = 9080;

static void clientThread()
{
  std::string strport = boost::lexical_cast<std::string>(port);
  SockClient sc(strport, "127.0.0.1");
  sc.sockConnect();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  sc.sendReq("56356345234234234234");
  std::string resp;
  sc.recvResponse(resp);  
}

static void serverThread()
{
  //Start server
  unsigned int cores = std::thread::hardware_concurrency();

  std::cout << "number of cores: " << cores << std::endl;
  
  if(cores == 0)
    cores = 5;
  
  std::cout<< "start" <<std::endl;
  std::shared_ptr<Server::Listener> lis(new Server::Listener(cores + 2, port));
  lis->run(); 
}


BOOST_AUTO_TEST_CASE(TestMultiConnections)
{
  std::thread server1(serverThread);

  std::this_thread::sleep_for(std::chrono::seconds(1));
  
  std::thread client1(clientThread);
  std::thread client2(clientThread);
  std::thread client3(clientThread);
  
  client1.join();
  client2.join();
  client3.join();
  server1.join();
}

//Add additional testcases here
//
//

BOOST_AUTO_TEST_SUITE_END()
