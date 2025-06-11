#ifndef HTTP_TCPSERVER
#define HTTP_TCPSERVER

#include <stdlib.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include "http_request.h"

namespace http {
  class TcpServer {
    public:
      TcpServer();
      ~TcpServer();
      int startServer();

    private:
      int m_server_socket;
      struct sockaddr_in m_address;
      int m_address_len;
      char buffer[30000] = {0};
      HTTPRequest* request;
      char* response;

      void getMethodAndRoute();
      void routeRequest();
      void respondData();
      void respond404();
      void respondAuth();
  };
}

#endif