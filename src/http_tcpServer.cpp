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

      // Determine request method and route
      getMethodAndRoute();
      std::cout << "Request Route: " << request_route << "\n";
      std::cout << "Request Method: " << request_method << "\n";

      routeRequest();

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

  // Parse request method & route
  void TcpServer::getMethodAndRoute() {
    std::string request_method_str = "";
    request_method = UNKNOWN;
    request_route = "";
    int i = 0;

    // get request method string
    while(buffer[i] != ' ') {
      request_method_str.push_back(buffer[i]);
      i++;
    }

    // get request route
    i++;
    while(buffer[i] != ' ') {
      request_route.push_back(buffer[i]);
      i++;
    }

    // determine request method
    if(request_method_str == "GET") {
      request_method = GET;
    } else if(request_method_str == "POST") {
      request_method = POST;
    } else if(request_method_str == "PUT") {
      request_method = PUT;
    } else if(request_method_str == "DELETE") {
      request_method = DELETE;
    }
    
  }

  void TcpServer::routeRequest() {
    if(request_route == "/data" && request_method == GET) {
      respondData();
    } else if(request_route == "/auth" && request_method == POST) {
      respondAuth();
    } else {
      respond404();
    }
  }

  void TcpServer::respondData() {
    response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Connection: close\r\n"
      "\r\n"
      "You've reached data!\r\n";
  }

  void TcpServer::respond404() {
    response =
      "HTTP/1.1 404 Not Found\r\n"
      "Content-Type: text/plain\r\n"
      "Connection: close\r\n"
      "\r\n"
      "Not found, cheif!\r\n";
  }

  void TcpServer::respondAuth() {
    response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Connection: close\r\n"
      "\r\n"
      "You've reached auth!\r\n";
  }
}