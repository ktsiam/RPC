#include "rpcstubhelper.h"

using namespace C150NETWORK;  // for all the comp150 utilities 

C150StreamSocket *RPCSTUBSOCKET;

void rpcstubinitialize() {

    // create new socket
    c150debug->printf(C150RPCDEBUG,"rpcstubinitialize: Creating C150StreamSocket");
    RPCSTUBSOCKET = new C150StreamSocket();

    // Tell the OS to start allowing connections
    // The application will pick those up one at a time by doing accept calls
    RPCSTUBSOCKET -> listen();  
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
