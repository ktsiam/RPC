#include "simplefunction.idl"

#include "rpcstubhelper.h"

#include <cstdio>
#include <cstring>
#include <tuple>
#include "c150debug.h"
#include <functional>

using namespace C150NETWORK;  // for all the comp150 utilities 

template <class Ret_T, class ...Args, class F>
void __generic(F&& fun) {
    std::tuple<Args...> args;
	RPCSTUBSOCKET->read((char*)&args, sizeof(args));

    Ret_T ret = std::apply(fun, args); 
   
    RPCSTUBSOCKET->write((char*)&ret, sizeof(ret));
}

void __add() {
    __generic<int, int, int>(add);
}

void __sub() {
    __generic<int, int, int>(sub);
}


// void __add(/*const std::tuple<int, int> &args*/) {
//     using Args_type = typename std::tuple<int, int>;
//     using Ret_type  = int;
//     
//     Args_type args;
//     RPCSTUBSOCKET->read((char*)&args, sizeof(args));
//     
//     Ret_type ret = add(std::get<0>(args), std::get<1>(args));    
//     RPCSTUBSOCKET->write((char*)&ret, sizeof(ret));
// }

void __badFunction(char *functionName) {
  char doneBuffer[5] = "BAD";  // to write magic value DONE + null

  c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: received call for nonexistent function %s()",functionName);
  RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}



// ======================================================================
//
//                        COMMON SUPPORT FUNCTIONS
//
// ======================================================================


void getFunctionNamefromStream();

// forward declaration
void getFunctionNameFromStream(char *buffer, unsigned int bufSize);


void dispatchFunction() {


  char functionNameBuffer[50];

  //std::pair<std::string, 
  getFunctionNameFromStream(functionNameBuffer,sizeof(functionNameBuffer));

  //
  // We've read the function name, call the stub for the right one
  // The stub will invoke the function and send response.
  //

  if (!RPCSTUBSOCKET-> eof()) {
    if (strcmp(functionNameBuffer,"add") == 0)
      __add();
    else if (strcmp(functionNameBuffer, "sub") == 0)
      __sub();
    else
      __badFunction(functionNameBuffer);
  }
}

 
//
//                   getFunctionNamefromStream
//
//   Helper routine to read function name from the stream. 
//   Note that this code is the same for all stubs, so can be generated
//   as boilerplate.
//
//   Important: this routine must leave the sock open but at EOF
//   when eof is read from client.
//
void getFunctionNameFromStream(char *buffer, unsigned int bufSize) {
  unsigned int i;
  char *bufp;    // next char to read
  bool readnull;
  ssize_t readlen;             // amount of data read from socket
  
  //
  // Read a message from the stream
  // -1 in size below is to leave room for null
  //
  readnull = false;
  bufp = buffer;
  for (i=0; i< bufSize; i++) {
    readlen = RPCSTUBSOCKET-> read(bufp, 1);  // read a byte
    // check for eof or error
    if (readlen == 0) {
      break;
    }
    // check for null and bump buffer pointer
    if (*bufp++ == '\0') {
      readnull = true;
      break;
    }
  }
  
  //
  // With TCP streams, we should never get a 0 length read
  // except with timeouts (which we're not setting in pingstreamserver)
  // or EOF
  //
  if (readlen == 0) {
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub: read zero length message, checking EOF");
    if (RPCSTUBSOCKET-> eof()) {
      c150debug->printf(C150RPCDEBUG,"simplefunction.stub: EOF signaled on input");

    } else {
      throw C150Exception("simplefunction.stub: unexpected zero length read without eof");
    }
  }

  //
  // If we didn't get a null, input message was poorly formatted
  //
  else if(!readnull) 
    throw C150Exception("simplefunction.stub: method name not null terminated or too long");

  
  //
  // Note that eof may be set here for our caller to check
  //

}


