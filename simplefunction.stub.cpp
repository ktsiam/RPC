#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>


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
    std::cerr << "GETTING FUNCTION NAME\n";
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

