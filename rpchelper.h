#ifndef __RPCHELPER_H_INCLUDED__  
#define __RPCHELPER_H_INCLUDED__  

#include <tuple>
#include "c150streamsocket.h"
#include "c150debug.h"

using namespace C150NETWORK;  // for all the comp150 utilities 

// reinterpret casts to output type
template <class Out, class In>
auto cast_to(In &&in) {
    using decay_In  = std::decay_t<In>;
    using decay_Out = std::decay_t<Out>;
    using Ref_Out = std::add_rvalue_reference_t<decay_Out>;
    
    // arrays -> array
    if constexpr (std::is_pointer_v<decay_In> &&
                  std::is_pointer_v<decay_Out>) {
        return reinterpret_cast<Ref_Out>(in);
    }
    // array -> non-array
    else if constexpr (std::is_pointer_v<decay_In>) {
        // dereferencing array to Out type
        return reinterpret_cast<Ref_Out>(*in);
    }
    // non-array -> array
    else if constexpr (std::is_pointer_v<decay_Out>) {
        return reinterpret_cast<Ref_Out>(&in);
    }
    // non-array -> non-array
    else {
        return reinterpret_cast<Ref_Out>(in);
    }
}

// Custom serializable 1-byte "void" return
struct Void {};


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * Serialization/Deserialization functions follow  *
 * * * * * * * * * (see line ~100) * * * * * * * * */


// determines whether type is specialization of tuple
template <class> 
struct is_tuple: std::false_type {};
template <class ...Ts> 
struct is_tuple<std::tuple<Ts...>>: std::true_type {};

// applies serialize_strings forall elements in tuple
template<class ...Tps>
void serialize_for_each_in_tuple(std::tuple<Tps...>&, C150StreamSocket *socket);

// applies deserialize_strings forall elements in tuple
template<class ...Tps>
void deserialize_for_each_in_tuple(std::tuple<Tps...>&, C150StreamSocket *socket);


// identifies expected strings in structure and deserializes them from socket
template <class Arg>
void deserialize_strings(Arg &arg, C150StreamSocket *socket) {
    if constexpr (std::is_array_v<Arg>) {
        for (std::size_t i = 0; i < sizeof(arg)/sizeof(arg[0]); ++i) {
            deserialize_strings(arg[i], socket);
        }
    }
    if constexpr (is_tuple<Arg>::value) {
        deserialize_for_each_in_tuple(arg, socket);
    }
    if constexpr (std::is_same_v<Arg, std::string>) {
        std::size_t size = arg.size()+1;
        char buffer[size];
        socket->read(buffer, sizeof(buffer));
        std::string buf{buffer};
        new (&arg) std::string(std::move(buf));
    }
}

// identifies expected strings in structure and serializes them to socket
template <class Arg>
void serialize_strings(Arg &arg, C150StreamSocket *socket) {
    if constexpr (std::is_array_v<Arg>) {
        for (std::size_t i = 0; i < sizeof(arg)/sizeof(arg[0]); ++i) {
            serialize_strings(arg[i], socket);
        }
    }
    if constexpr (is_tuple<Arg>::value) {
        serialize_for_each_in_tuple(arg, socket);
    }
    if constexpr (std::is_same_v<Arg, std::string>) {
        socket->write(arg.c_str(), arg.size()+1);
    }
}


// next two functions used by stub & proxy to serialize/deserialize
template <class ...Args>
std::tuple<Args...> deserialize(C150StreamSocket *socket) {
    std::tuple<Args...> args;
    socket->read((char*)&args, sizeof(args));
    deserialize_strings(args, socket);
    return args;
}

template <class ...Args>
void serialize(std::tuple<Args...> &args, C150StreamSocket *socket) {
    socket->write((char*)&args, sizeof(args));
    serialize_strings(args, socket);
}


// helper: Iterating through structure/tuple to identify strings

template<size_t I, class ...Tps>
void deserialize_for_each_in_tuple(std::tuple<Tps...> &tup,
                                   C150StreamSocket *socket) {
    if constexpr (I != sizeof...(Tps)) {
        deserialize_strings(std::get<I>(tup), socket);
        deserialize_for_each_in_tuple<I+1>(tup, socket);
    }
}

template<class ...Tps>
void deserialize_for_each_in_tuple(std::tuple<Tps...> &tup,
                                   C150StreamSocket *socket) {
    deserialize_for_each_in_tuple<0>(tup, socket);
}

template<size_t I, class ...Tps>
void serialize_for_each_in_tuple(std::tuple<Tps...> &tup,
                                 C150StreamSocket *socket) {
    if constexpr (I != sizeof...(Tps)) {
        serialize_strings(std::get<I>(tup), socket);
        serialize_for_each_in_tuple<I+1>(tup, socket);
    }
}

template<class ...Tps>
void serialize_for_each_in_tuple(std::tuple<Tps...> &tup,
                                 C150StreamSocket *socket) {
    serialize_for_each_in_tuple<0>(tup, socket);
}

#endif
