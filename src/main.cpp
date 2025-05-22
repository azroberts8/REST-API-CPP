#include <iostream>
#include <http_tcpServer.h>

int main() {
  std::cout << "Starting server...\n";
  http::TcpServer server = http::TcpServer();
  return 0;
}