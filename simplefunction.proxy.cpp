#include "rpcproxyhelper.h"
#include <tuple>
#include "structs.idl"

using Person_T = std::tuple<int, string, string>;
using StructWithArrays_T = std::tuple<Person_T[10], int>;
using ThreePeople_T = std::tuple<Person_T, Person_T, Person_T>;
using rectangle_T = std::tuple<int, int>;


int area(rectangle r) {
    return reinterpret_cast<int&&>(
        serialize_and_apply<int, rectangle_T>(
            "area",
            reinterpret_cast<rectangle_T&>(r)));
}

Person findPerson(ThreePeople tp) {
    return reinterpret_cast<Person&&>(
        serialize_and_apply<Person_T, ThreePeople_T>(
            "findPerson",
            reinterpret_cast<ThreePeople_T&>(tp)));
}



// #include "rpcproxyhelper.h"
// #include <tuple>
// #include "structs.idl"

// using Person_T = std::tuple<int, std::string, std::string>;
// using ThreePeople_T = std::tuple<Person_T, Person_T, Person_T>;

// Person findPerson(ThreePeople tp) {
//     return reinterpret_cast<Person&&>(
//         serialize_and_apply<Person_T, ThreePeople_T>(
//         "findPerson",
//         reinterpret_cast<ThreePeople_T&>(tp))
//     );

//     Person_T p = serialize_and_apply<Person_T, ThreePeople_T>(
//         "findPerson",
//         reinterpret_cast<ThreePeople_T&>(tp));

//     return reinterpret_cast<Person&>(p);
// }


// // #include "simplefunction.idl"
// // #include "rpcproxyhelper.h"
// // #include <tuple>


// // void add() {
// //     return serialize_and_apply<void>("add");
// // }

// // int sub(int x, int y) {
// //     return reinterpret_cast<int&&>(
// //         serialize_and_apply<int, int, int>("sub", x, y));
// // }
