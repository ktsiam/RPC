#include "rpcproxyhelper.h"
#include <tuple>

template <class Ret_T, class ...Args>
Ret_T generic(const char *fname, Args ...vargs) {
    RPCPROXYSOCKET->write(fname, strlen(fname)+1); 

    std::tuple<Args...> args{vargs...};    
    RPCPROXYSOCKET->write((char*)&args, sizeof(args));

    char readBuffer[sizeof(Ret_T)];
    RPCPROXYSOCKET->read(readBuffer, sizeof(Ret_T));
    Ret_T ret = *reinterpret_cast<Ret_T*>(readBuffer);
    
    return ret;
}

#include "simplefunction.idl"

int add(int x, int y) {
    return generic<int, int, int>("add", x, y);
}

int sub(int x, int y) {
    return generic<int, int, int>("sub", x, y);
}
