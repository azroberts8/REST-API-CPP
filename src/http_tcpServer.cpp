#include <http_tcpServer.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>

namespace http {
  // Constructor
  TcpServer::TcpServer() {

    // Create socket file descriptor
    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_server_socket == 0) {
      perror("Socket failed!");
      exit(EXIT_FAILURE);
    }

    // Attach socket to the port
    int opt = 1;
    if(setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
      perror("Failed to attach to port");
      exit(EXIT_FAILURE);
    }

    // Bind socket to address
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(8080);
    m_address_len = sizeof(m_address);

    
    if(bind(m_server_socket, (struct sockaddr*)&m_address, sizeof(m_address)) < 0) {
      perror("Failed to bind address");
      exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if(listen(m_server_socket, 3) < 0) {
      perror("Failed to listen");
      exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on 8080...\n";

    while(true) {

      // Accept incoming connections;
      int m_client_socket;
      if((m_client_socket = accept(m_server_socket, (struct sockaddr*)&m_address, (socklen_t*)&m_address_len)) < 0) {
        perror("Failed to accept connection");
        continue;
      }

      read(m_client_socket, buffer, sizeof(buffer));
      std::cout << "Recieved request:\n" << buffer << "\n";

      // Send response
      const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello, World!\r\n";

      send(m_client_socket, response, strlen(response), 0);
      close(m_client_socket);
      std::cout << "Response sent!\n";
    }
    
  }

  // Destructor
  TcpServer::~TcpServer() {
    close(m_server_socket);
    exit(0);
  }
}