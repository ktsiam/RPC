#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>
#include "structs.idl"

using Person_T = std::tuple<int, string, string>;
using StructWithArrays_T = std::tuple<Person_T[10], int>;
using ThreePeople_T = std::tuple<Person_T, Person_T, Person_T>;
using rectangle_T = std::tuple<int, int>;


int __area_inter(rectangle_T& r) {
    return reinterpret_cast<int&&>(
        area(
            reinterpret_cast<rectangle&>(r)));
}

void __area() {
    __deserialize_and_apply<int, rectangle_T>(__area_inter);
}

Person_T __findPerson_inter(ThreePeople_T& tp) {
    return reinterpret_cast<Person_T&&>(
        findPerson(
            reinterpret_cast<ThreePeople&>(tp)));
}

void __findPerson() {
    __deserialize_and_apply<Person_T, ThreePeople_T>(__findPerson_inter);
}

void dispatchFunction() {
    char functionNameBuffer[50];
    getFunctionNameFromStream(functionNameBuffer,
                              sizeof(functionNameBuffer));
    if (!RPCSTUBSOCKET-> eof()) {
        // if (strcmp(functionNameBuffer,"area") == 0) {
        //     __area();
        //     return;
        // }
        if (strcmp(functionNameBuffer,"findPerson") == 0) {
            __findPerson();
            return;
        }
        assert(false && "function contract not found");
    }
}

// #include "rpcstubhelper.h"
// #include <tuple>
// #include <cassert>
// #include "structs.idl"

// using Person_T = std::tuple<int, std::string, std::string>;
// using ThreePeople_T = std::tuple<Person_T, Person_T, Person_T>;

// Person_T __findPerson_inter(ThreePeople_T &tp) {
//     return reinterpret_cast<Person_T&&>(
//         findPerson(reinterpret_cast<ThreePeople&>(tp)));
// }

// void __findPerson() {
//     __deserialize_and_apply<Person_T, ThreePeople_T>(__findPerson_inter);
// }

// void dispatchFunction() {
//     char functionNameBuffer[50];    
//     getFunctionNameFromStream(functionNameBuffer,
//                               sizeof(functionNameBuffer));
//     if (!RPCSTUBSOCKET-> eof()) {
//         if (strcmp(functionNameBuffer,"findPerson") == 0) {
//             __findPerson();
//             return;
//         }
//         // if (strcmp(functionNameBuffer, "sub") == 0) {
//         //     __sub();
//         //     return;
//         // }
//         assert(false && "function contract not found");
//     }
// }



// // #include "simplefunction.idl"
// // #include "rpcstubhelper.h"
// // #include <tuple>
// // #include <cassert>


// // void __add() {
// //     __deserialize_and_apply<void>(add);
// // }

// // int sub_intermediate(int &i, int &t) {
// //     return sub(reinterpret_cast<int&>(i), 
// //                reinterpret_cast<int&>(t));
// // }

// // void __sub() {
// //     __deserialize_and_apply<int, int, int>(sub_intermediate);
// // }

// // void dispatchFunction() {
// //     char functionNameBuffer[50];    
// //     getFunctionNameFromStream(functionNameBuffer,
// //                               sizeof(functionNameBuffer));
// //     if (!RPCSTUBSOCKET-> eof()) {
// //         if (strcmp(functionNameBuffer,"add") == 0) {
// //             __add();
// //             return;
// //         }
// //         if (strcmp(functionNameBuffer, "sub") == 0) {
// //             __sub();
// //             return;
// //         }
// //         assert(false && "function contract not found");
// //     }
// // }

