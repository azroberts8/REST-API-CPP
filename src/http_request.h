#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdlib.h>
#include <stdint.h>
#include <map>

namespace http {
  enum request_method_t {
    UNKNOWN,
    GET,
    POST,
    PUT,
    DELETE
  };

  enum request_status_t {
    OK,
    INVALID_REQUEST_METHOD,
    INVALID_PATH,
    INVALID_BUFFER
  };

  class HTTPRequest {
    public:
      HTTPRequest(char* request_buffer, uint32_t request_buffer_size);
      ~HTTPRequest();
      request_status_t getStatus();
      request_method_t getMethod();
      std::string getPath();
      std::string getHeader(std::string header);
      std::string getFormParameter(std::string parameter);

    private:
      request_status_t request_status;
      request_method_t request_method;
      std::string request_path;
      std::map<std::string, std::string> request_headers;
      std::map<std::string, std::string> form_parameters;
  };
}

#endif