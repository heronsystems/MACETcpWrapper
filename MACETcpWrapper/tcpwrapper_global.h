#ifndef TCPWRAPPER_GLOBAL_H
#define TCPWRAPPER_GLOBAL_H

#ifdef _MSC_VER
#if defined(MACETCPWRAPPER_LIBRARY)
#  define MACETCPSHARED_EXPORT __declspec(dllexport)
#else
#  define MACETCPSHARED_EXPORT __declspec(dllimport)
#endif
#else
#  define MACETCPSHARED_EXPORT
#endif

#include <vector>
#include <functional>

template <typename ...T>
void Notify(std::vector<std::function<void(T...)>> lambdas, T... args)
{
    for(auto it = lambdas.cbegin() ; it != lambdas.cend() ; ++it) {
        (*it)(args...);
    }
}


template<const char* T>
void variadicExpand(std::function<void(const char* element)> lambda)
{
    lambda(T);
}

template<const char* Head, const char* Next, const char* ...Tail>
void variadicExpand(std::function<void(const char* element)> lambda)
{
    lambda(Head);
    variadicExpand<Next, Tail...>(lambda);
}

#endif // TCPWRAPPER_GLOBAL_H
