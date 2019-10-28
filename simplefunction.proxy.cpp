// --------------------------------------------------------------
//
//                        simplefunction.proxy.cpp
//
//        Author: Noah Mendelsohn         
//   
//       This is a hand-crafted demonstration proxy.
//
//       For your project, your "rpcgen" program will generate
//       proxies like this one automatically from the idl
//       file. Note that this proxy also #includes the 
//       simplefunctions.idl file. Of course, your rpcgen
//       program will change that, as well as the number
//       of functions generated. More importantly, it will
//       generate code to handle function arguments and
//       return values.
//
//
//
//       Copyright: 2012 Noah Mendelsohn
//     
// --------------------------------------------------------------

// IMPORTANT! WE INCLUDE THE IDL FILE AS IT DEFINES THE INTERFACES
// TO THE FUNCTIONS WE'RE IMPLEMENTING. THIS MAKES SURE THE
// CODE HERE ACTUALLY MATCHES THE REMOTED INTERFACE

#include "simplefunction.idl"

#include "rpcproxyhelper.h"

#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <iostream>
#include <tuple>

using namespace C150NETWORK;  // for all the comp150 utilities 

template <class Ret_T, class ...Args>
Ret_T generic(const char *fname, Args ...vargs) {
    RPCPROXYSOCKET->write(fname, strlen(fname)+1); 

    std::tuple<Args...> args{vargs...};    
    RPCPROXYSOCKET->write((char*)&args, sizeof(args));

    char readBuffer[sizeof(Ret_T)];
    RPCPROXYSOCKET->read(readBuffer, sizeof(Ret_T));
    Ret_T ret = *reinterpret_cast<Ret_T*>(readBuffer);
    
    return ret;
}

int add(int x, int y) {
    return generic<int, int, int>("add", x, y);
}

int sub(int x, int y) {
	return generic<int, int, int>("sub", x, y);
}
