#ifndef HTTP_TCPSERVER
#define HTTP_TCPSERVER

#include <stdlib.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>

namespace http {
  enum request_method_t {
    UNKNOWN,
    GET,
    POST,
    PUT,
    DELETE
  };

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
      request_method_t request_method;
      std::string request_route;

      void getMethodAndRoute();
  };
}

#endif