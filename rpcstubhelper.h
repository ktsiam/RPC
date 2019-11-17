#ifndef __RPCSTUBHELPER_H_INCLUDED__  
#define __RPCSTUBHELPER_H_INCLUDED__  

#include "rpchelper.h"

extern C150StreamSocket *RPCSTUBSOCKET;
 
void rpcstubinitialize();
void dispatchFunction();
void getFunctionNameFromStream(char *buffer, unsigned int bufSize);

template <class Ret_T, class ...Args, class F>
void __deserialize_and_apply(F&& fun) {
    std::tuple<Args...> args = deserialize<Args...>(RPCSTUBSOCKET);
    if constexpr (std::is_void_v<Ret_T>) {
        std::apply(fun, args);
        std::tuple<Void> ret_tup;
        serialize(ret_tup, RPCSTUBSOCKET);
    } else {
        Ret_T ret = std::apply(fun, args);
        auto ret_tup = std::tuple<Ret_T>(ret);
        serialize(ret_tup, RPCSTUBSOCKET);
    }
}

#endif
