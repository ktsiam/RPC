#ifndef __RPCPROXYHELPER_H_INCLUDED__  
#define __RPCPROXYHELPER_H_INCLUDED__  

#include "rpchelper.h"

extern C150StreamSocket *RPCPROXYSOCKET;
 

void rpcproxyinitialize(char *servername);

template <class Ret_T, class ...Args>
Ret_T serialize_and_apply(const char *fname, Args ...vargs) {
    
    RPCPROXYSOCKET->write(fname, strlen(fname)+1);
    std::tuple<Args...> args{vargs...};
    
    serialize(args, RPCPROXYSOCKET);
    return std::get<0>(deserialize<Ret_T>(RPCPROXYSOCKET));
}


#endif
