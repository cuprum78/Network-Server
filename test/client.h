#ifndef CLIENT_SOCKCLIENT_H
#define CLIENT_SOCKCLIENT_H


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

class SockClient
{
public:
  SockClient(const std::string& port, const std::string& server)
    : port_(port),
      server_(server)
  {
  }

  bool sockConnect()
  {
    struct addrinfo hints;
    int ret;
  
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
  
    ret = getaddrinfo(server_.c_str(), port_.c_str(), &hints, &res_);
    if(ret != 0)
      {
	std::cout << "getaddrinfo error :" << ret << std::endl;
	return false;
      }

    sid_ = socket(res_->ai_family, res_->ai_socktype, res_->ai_protocol);
    if(sid_ < 0)
      {
	std::cout << "invalid socket :" << sid_ << std::endl;
	return false;
      }
  
    ret = connect(sid_, res_->ai_addr, res_->ai_addrlen);
    if(ret < 0)
      {
	std::cout << "connect error :" << ret << std::endl;
	return false;
      }        
    return true;
  }

  bool sendReq(std::string request)    
  {
    size_t bytes = 0;
    bytes = send(sid_, request.c_str(), request.size(), 0);
    if(bytes == -1)
    {
      std::cout << "send error" << std::endl;
      return false;
    }	
    std::cout << "bytes written : " << bytes << std::endl;
    return true;
  }

  bool recvResponse(std::string& response)
  {
    char data[1024];
    memset(data, 0, 1024);
    size_t bytes = recv(sid_, data, 1024,0);
    if(bytes == -1)
    {
      std::cout << "recv error " << std::endl;
      return false;
    }	
    data[bytes] = '\0';
    std::cout << "Received data: " << data << std::endl;
    return true;    
  }

  ~SockClient()
  {
    std::cout << "closing socket : " << sid_ << std::endl;
    freeaddrinfo(res_);
    close(sid_);
  }
  
private:
  std::string port_;
  std::string server_;
  struct addrinfo* res_;
  int sid_;
};

#endif
