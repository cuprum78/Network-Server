#include <iostream>
#include <thread>

#include "listener.h"

int main()
{
  unsigned int cores = std::thread::hardware_concurrency();

  std::cout << "number of cores: " << cores << std::endl;
  
  if(cores == 0)
    cores = 5;
  
  std::cout<< "start" <<std::endl;
  std::shared_ptr<Server::Listener> lis(new Server::Listener(cores + 2, 9080));
  lis->run();
  return 0;
}
