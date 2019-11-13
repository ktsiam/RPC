#ifndef __RPCSTUBHELPER_H_INCLUDED__  
#define __RPCSTUBHELPER_H_INCLUDED__  

#include "c150streamsocket.h"
#include "c150debug.h"
#include <tuple>
#include <cassert>

using namespace C150NETWORK;  // for all the comp150 utilities 

extern C150StreamSocket *RPCSTUBSOCKET;
 
void rpcstubinitialize();
void dispatchFunction();
void getFunctionNameFromStream(char *buffer, unsigned int bufSize);

template <class> 
struct is_tuple: std::false_type {};
template <class ...T> 
struct is_tuple<std::tuple<T...>>: std::true_type {};

template <std::size_t SIZE, class Tp>
void for_each_in_array(Tp*);

template<class ...Tps>
void for_each_in_tuple(std::tuple<Tps...>&);

template <class Arg>
void populate_strings_in(Arg &arg) {
    if constexpr (std::is_array_v<Arg>) {
        constexpr std::size_t size = sizeof(arg)/sizeof(arg[0]);
        for_each_in_array<size>(arg);
    }
    if constexpr (is_tuple<Arg>::value) {
        for_each_in_tuple(arg);
    }
    if constexpr (std::is_same_v<Arg, std::string>) {
        std::size_t size = arg.size();
        char buffer[size];
        //RPCSTUBSOCKET->read(buffer, sizeof(buffer));
        std::string buf{buffer[0], buffer[size]};
        new (&arg) std::string(std::move(buf));
        //memcpy((char*)&arg, (char*)&buf, sizeof(buf));
    }
}

template <class Tp, std::size_t ...Is>
void for_each_in_array_helper(Tp* elements, 
                              std::index_sequence<Is...> const &) { 
    (populate_strings_in(elements[Is]), ...);
}

template <std::size_t SIZE, class Tp>
void for_each_in_array(Tp* elements) { 
    for_each_in_array_helper(elements, std::make_index_sequence<SIZE>{});
}

template<size_t I, class ...Tps>
void for_each_in_tuple(std::tuple<Tps...> &tup) {
    if constexpr (I != sizeof...(Tps)) {
        populate_strings_in(std::get<I>(tup));
        for_each_in_tuple<I+1>(tup);
    }
}

template<class ...Tps>
void for_each_in_tuple(std::tuple<Tps...> &tup) {
    for_each_in_tuple<0>(tup);
}

template <class Ret_T, class ...Args, class F>
void __generic(F&& fun) {
    std::tuple<Args...> args;
    RPCSTUBSOCKET->read((char*)&args, sizeof(args));

    Ret_T ret = std::apply(fun, args);    
    RPCSTUBSOCKET->write((char*)&ret, sizeof(ret));
}

#endif
