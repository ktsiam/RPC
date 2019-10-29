#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>

template <class Ret_T, class ...Args, class F>
void __generic(F&& fun) {
    std::tuple<Args...> args;
    RPCSTUBSOCKET->read((char*)&args, sizeof(args));

    Ret_T ret = std::apply(fun, args);    
    RPCSTUBSOCKET->write((char*)&ret, sizeof(ret));
}

void getFunctionNameFromStream(char *buffer, unsigned int bufSize) {    
    for (unsigned i = 0; i < bufSize; i++) {
        ssize_t readlen = RPCSTUBSOCKET-> read(buffer, 1);
        if (readlen == 0) assert(false && "something wrong");
        if (*buffer++ == '\0') return;
    }
}

/* GENERATED */

#include "simplefunction.idl"

void __add() {
    __generic<int, int, int>(add);
}

void __sub() {
    __generic<int, int, int>(sub);
}

void dispatchFunction() {
    char functionNameBuffer[50];    
    getFunctionNameFromStream(functionNameBuffer,
                              sizeof(functionNameBuffer));
    if (!RPCSTUBSOCKET-> eof()) {
        if (strcmp(functionNameBuffer,"add") == 0) {
            __add();
            return;
        }
        if (strcmp(functionNameBuffer, "sub") == 0) {
            __sub();
            return;
        }
        assert(false && "function contract not found");
    }
}



/* /GENERATED */

