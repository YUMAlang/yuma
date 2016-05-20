#ifndef _EXLIB_H_
#define _EXLIB_H_

#include <iostream>

class Exception: public std::exception
{
public:
  explicit Exception(const char* message): msg(message) {}

  explicit Exception(const std::string& message): msg(message) {}

  virtual ~Exception() throw () {}

  virtual const char* what() const throw ()
  {
    return msg.c_str();
  }

protected:
  // Error message
  std::string msg;
};

#endif
