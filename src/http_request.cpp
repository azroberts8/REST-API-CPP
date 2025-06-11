#include "http_request.h"
// #include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <map>

namespace http {
  // Constructor
  HTTPRequest::HTTPRequest(char* request_buffer, uint32_t request_buffer_size) {
    request_status = OK;
    int buffer_location = 0;

    // Parse Request Method
    std::string request_method_str = "";

    for(char i = 0; i < 7; i++) {
      if(request_buffer[buffer_location] == ' ') break;
      request_method_str.push_back(request_buffer[buffer_location]);
      buffer_location++;
    }

    if(request_method_str == "GET") {
      request_method = GET;
    } else if(request_method_str == "POST") {
      request_method = POST;
    } else if(request_method_str == "PUT") {
      request_method = PUT;
    } else if(request_method_str == "DELETE") {
      request_method = DELETE;
    } else {
      request_method = UNKNOWN;
      request_status = INVALID_REQUEST_METHOD;
      return;
    }

    // Parse Request Route
    buffer_location++;
    request_path = "";
    for(char i = 0; i < 256; i++) {
      if(request_buffer[buffer_location] == ' ') break;
      // Treat excessively long paths as invalid
      if(i == 255) {
        request_status = INVALID_PATH;
        return;
      }
      request_path.push_back(request_buffer[buffer_location]);
      buffer_location++;
    }

    // Finish first line of request
    for(char i = 0; i < 64; i++) {
      if(request_buffer[buffer_location] == '\n') break;
      
      // Treat excessively long tail as invalid
      if(i == 63) {
        request_status = INVALID_BUFFER;
        return;
      }
      buffer_location++;
    }

    // Parse Request Headers
    buffer_location++;
    char last_character = '\n';
    std::string key = "";
    std::string value = "";
    while(
      (buffer_location < request_buffer_size)
      && !(last_character == '\n' && request_buffer[buffer_location] == '\n')
    ) {
      // Header key
      while(
        (buffer_location < request_buffer_size)
        && (request_buffer[buffer_location] != ':') 
      ) {
        key.push_back(request_buffer[buffer_location]);
        buffer_location++;
      }

      // Header value
      buffer_location++;
      buffer_location++;
      while(
        (buffer_location < request_buffer_size)
        && (request_buffer[buffer_location] != '\n')
      ) {
        value.push_back(request_buffer[buffer_location]);
        buffer_location++;
      }
      last_character = request_buffer[buffer_location];
      buffer_location++;

      request_headers.insert(std::make_pair(key, value));
      key = "";
      value = "";
    }

    // Parse Request Parameters
  }

  // Destructor
  HTTPRequest::~HTTPRequest() {

  }

  // Status Accessor
  request_status_t HTTPRequest::getStatus() {
    return request_status;
  }

  // Request Method Accessor
  request_method_t HTTPRequest::getMethod() {
    return request_method;
  }

  // Request Path Accessor
  std::string HTTPRequest::getPath() {
    return request_path;
  }

  // Request Header Accessor
  std::string HTTPRequest::getHeader(std::string header) {
    return request_headers.at(header);
  }

  // Request Parameter Accessor
  std::string HTTPRequest::getFormParameter(std::string parameter) {
    return form_parameters.at(parameter);
  }
}