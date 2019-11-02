#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>

template <class> 
struct is_tuple: std::false_type {};
template <class ...T> 
struct is_tuple<std::tuple<T...>>: std::true_type {};

template <std::size_t SIZE, class Tp>
void for_each_in_array(Tp*);

template<class ..Tps>
void for_each_in_tuple(std::tuple<Tps...>&);

template <class Arg>
void type_without_strings(Arg &arg) {
    if constexpr (std::is_array_v<Arg>) {
        constexpr std::size_t size = sizeof(arg)/sizeof(arg[0]);
        for_each_in_array<size>(arg);
    }
    if constexpr (is_tuple<Arg>::value) {
        for_each_in_tuple(arg);
    }
    if constexpr (std::is_same_v<Arg, std::string>) {
        std::cerr << "IS STRING\n";
    }
       
}

template <class Arg, class ...Args>
void type_without_strings(std::tuple<Arg, Args...> &tup) {
    if constexpr (std::is_array_v<Arg>) {
        constexpr std::size_t size = sizeof(Arg)/sizeof(std::get<0>(tup)[0]);
        for_each_in_array<size>(&std::get<0>(tup)[0]);
    }
    if constexpr (is_tuple<Arg>::value) {
        type_without_strings(std::get<0>(tup));
    }
    if constexpr (std::is_same_v<Arg, std::string>) {
            std::cout << "REPLACING STRING\n";
        // std::size_t size = std::get<0>(tup).size();
        // char buffer[size];
        // RPCSTUBSOCKET->read(buffer, sizeof(buffer));
        // std::string buf{buffer[0], buffer[size]};
        // memcpy(&std::get<0>(tup), &buf, sizeof(buf));
    }
    // add recurive call
}

template <class Tp, std::size_t ...Is>
void for_each_in_array_helper(Tp* elements, 
                              std::index_sequence<Is...> const &) { 
    ((type_without_strings(std::tuple<Tp>(elements[Is]))), ...);
}

template <std::size_t SIZE, class Tp>
void for_each_in_array(Tp* elements) { 
    for_each_in_array_helper(elements, std::make_index_sequence<SIZE>{});
}

template<size_t i, class ...Tps>
void for_each_in_tuple(std::tuple<Tps...> &tup) {
    if (i >= sizeof...(Tps)) {
        return;
    }
    else {
        type_without_strings(std::get<i>(tup));
        for_each_in_tuple<i+1>(tup);
    } 
}

template<class ..Tps>
void for_each_in_tuple(std::tuple<Tps...> &tup) {
    for_each_in_tuple<0>(tup);
}


template <class Ret_T, class ...Args, class F>
void __generic(F&& fun) {
    std::tuple<Args...> args;
    RPCSTUBSOCKET->read((char*)&args, sizeof(args));

    Ret_T ret = std::apply(fun, args);    
    RPCSTUBSOCKET->write((char*)&ret, sizeof(ret));

    //std::tuple<std::string[4], int[16][24], std::string, std::tuple<std::string[5], bool>> a;
    std::tuple<std::string[2], int[5][2][3][2][1]> a;
    type_without_strings(a);
}

void getFunctionNameFromStream(char *buffer, unsigned int bufSize) {    
    for (unsigned i = 0; i < bufSize; i++) {
        ssize_t readlen = RPCSTUBSOCKET-> read(buffer, 1);
        if (readlen == 0) {
            throw C150Exception("client hang");
        }
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

