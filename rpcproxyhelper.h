#ifndef __RPCPROXYHELPER_H_INCLUDED__  
#define __RPCPROXYHELPER_H_INCLUDED__  

#include "rpchelper.h"

extern C150StreamSocket *RPCPROXYSOCKET;
 
void rpcproxyinitialize(char *servername);

template <class Ret_T, class ...Args>
Ret_T serialize_and_apply(const char *fname, Args ...vargs) {
    
    std::cerr << "ENTERED serialize_and_apply\n";
    RPCPROXYSOCKET->write(fname, strlen(fname)+1);
    std::tuple<Args...> args{vargs...};
    
    std::cerr << "CALLING serialize\n";
    serialize(args, RPCPROXYSOCKET);
    if constexpr (std::is_void_v<Ret_T>) {
        deserialize<Void>(RPCPROXYSOCKET);
    } else {
        std::cerr << "DESERIALIZING AND RETURNING\n";
        return std::get<0>(deserialize<Ret_T>(RPCPROXYSOCKET));        
    }
}


#endif
