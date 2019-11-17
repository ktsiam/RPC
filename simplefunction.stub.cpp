#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>

#include "simplefunction.idl"

void __add() {
    __deserialize_and_apply<int>(add);
}

void __sub() {
    __deserialize_and_apply<int, int, int>(sub);
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

