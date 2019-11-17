#include "rpcproxyhelper.h"
#include <tuple>
#include "structs.idl"

using Person_T = std::tuple<std::string, std::string, int>;
using ThreePeople_T = std::tuple<Person_T, Person_T, Person_T>;

Person findPerson(ThreePeople tp) {
    std::cerr << "START findPerson\n";
    
    Person_T p = serialize_and_apply<Person_T, ThreePeople_T>(
        "findPerson",
        reinterpret_cast<ThreePeople_T&>(tp));
    std::cerr << "Getting person & returning\n";
    return reinterpret_cast<Person&>(p);
}


// #include "simplefunction.idl"
// #include "rpcproxyhelper.h"
// #include <tuple>


// void add() {
//     return serialize_and_apply<void>("add");
// }

// int sub(int x, int y) {
//     return reinterpret_cast<int&&>(
//         serialize_and_apply<int, int, int>("sub", x, y));
// }
