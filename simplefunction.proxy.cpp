#include "rpcproxyhelper.h"
#include <tuple>

#include "simplefunction.idl"

int add() {
    return serialize_and_apply<int>("add");
}

int sub(int x, int y) {
    return serialize_and_apply<int, int, int>("sub", x, y);
}
