#include "rpcproxyhelper.h"
#include <tuple>
#include <iostream>

template <class Ret_T, class ...Args>
Ret_T generic(const char *fname, Args ...vargs) {
    std::cerr << "STARTING GENERIC\n";
    RPCPROXYSOCKET->write(fname, strlen(fname)+1); 
    std::cerr << "WRITING FILENAME: " << fname << std::endl;
    std::tuple<Args...> args{vargs...};    
    RPCPROXYSOCKET->write((char*)&args, sizeof(args));
    
    std::cerr << "READING RESULT\n";
    char readBuffer[sizeof(Ret_T)];
    RPCPROXYSOCKET->read(readBuffer, sizeof(Ret_T));
    Ret_T ret = *reinterpret_cast<Ret_T*>(readBuffer);
    std::cerr << "RETURNING : " << ret << std::endl;
    return ret;
}

#include "simplefunction.idl"

int add(int x, int y) {
    return generic<int, int, int>("add", x, y);
}

int sub(int x, int y) {
    return generic<int, int, int>("sub", x, y);
}
