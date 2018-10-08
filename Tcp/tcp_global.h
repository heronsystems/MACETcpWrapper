#ifndef TCP_GLOBAL_H
#define TCP_GLOBAL_H

#ifdef _MSC_VER
#  if defined(TCP_LIBRARY)
#    define TCPSHARED_EXPORT  __declspec(dllexport)
#  else
#    define TCPSHARED_EXPORT  __declspec(dllimport)
#  endif
#else
#  define TCPSHARED_EXPORT
#endif

#include <stdint.h>

#endif // TCP_GLOBAL_H
