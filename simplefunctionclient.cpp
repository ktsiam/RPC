

#include "rpcproxyhelper.h"
#include "lotsofstuff.idl"

#include "c150debug.h"
#include "c150grading.h"
#include <fstream>
#include <iostream>

using namespace std;          // for C++ std library
using namespace C150NETWORK;  // for all the comp150 utilities 

// forward declarations
void setUpDebugLogging(const char *logname, int argc, char *argv[]);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//                    Command line arguments
//
// The following are used as subscripts to argv, the command line arguments
// If we want to change the command line syntax, doing this
// symbolically makes it a bit easier.
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

const int serverArg = 1;     // server name is 1st arg


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//                           main program
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
int 
main(int argc, char *argv[]) {

     //
     //  Set up debug message logging
     //
     setUpDebugLogging("simplefunctionclientdebug.txt",argc, argv);

     //
     // Make sure command line looks right
     //
     if (argc != 2) {
       fprintf(stderr,"Correct syntxt is: %s <servername> \n", argv[0]);
       exit(1);
     }

     //
     //  DO THIS FIRST OR YOUR ASSIGNMENT WON'T BE GRADED!
     //
     
     GRADEME(argc, argv);

     //
     //     Call the functions and see if they return
     //
     try {

       //
       // Set up the socket so the proxies can find it
       //
       rpcproxyinitialize(argv[serverArg]);

       // 
       // Call (possibly remote) func1
       //       
       // add(); int x = 3;
       // add(); int y = 4;
       // int z = sub(16, 8);
       // std::cerr << "DONE!!!!!!!\n";
       // std::cerr << "got " << x << ' ' << y << ' ' << z << std::endl;
       
       Person bob{"bob", "builder", 47};
       Person mark = findPerson({bob, bob, bob});
       std::cout << "name : " << mark.firstname << ' ' << mark.lastname << ' ' << mark.age << std::endl;

       func1();
       func2();
       func3();
       
       std::cout << "1 + 2 = " << sqrt(1,2) << std::endl;
       int arr[24] = {0};
       int arr2[24] = {0};
       // std::array<int, 24> arr = {0};
       // std::array<int, 24> arr2 = {0};

       arr[0] = 17;
       arr2[0] = 36;
       
       std::cout << "17 + 36 = " << takesTwoArrays(arr, arr2) << std::endl;

       int arr3[24][15] = {0};
       arr3[0][0] = 3;

       //std::cout << "17 + 3 + 3 = " << showsArraysofArrays(arr, arr3, arr3) << std::endl;
       
       std::string s = "Mr Krabs";

       std::cout << "mr krabs: " << upcase(s) << std::endl;

       std::cout << "3.1 * 4.2 = " << multiply(3.1, 4.2) << std::endl;

       rectangle r = {3, 4};
       
       std::cout << "3 * 4 = " << area(r) << std::endl;

       exit(0);
     }

     //
     //  Handle networking errors -- for now, just print message and give up!
     //
     catch (C150Exception& e) {
       // Write to debug log
       c150debug->printf(C150ALWAYSLOG,"Caught C150Exception: %s\n",
			 e.formattedExplanation().c_str());
       // In case we're logging to a file, write to the console too
       cerr << argv[0] << ": caught C150NetworkException: " << e.formattedExplanation() << endl;
     }

     return 0;
}



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//                     setUpDebugLogging
//
//        For COMP 150-IDS, a set of standards utilities
//        are provided for logging timestamped debug messages.
//        You can use them to write your own messages, but 
//        more importantly, the communication libraries provided
//        to you will write into the same logs.
//
//        As shown below, you can use the enableLogging
//        method to choose which classes of messages will show up:
//        You may want to turn on a lot for some debugging, then
//        turn off some when it gets too noisy and your core code is
//        working. You can also make up and use your own flags
//        to create different classes of debug output within your
//        application code
//
//        NEEDSWORK: should be factored into shared code w/pingstreamserver
//        NEEDSWORK: document arguments
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
void setUpDebugLogging(const char *logname, int argc, char *argv[]) {

     //   
     //           Choose where debug output should go
     //
     // The default is that debug output goes to cerr.
     //
     // Uncomment the following three lines to direct
     // debug output to a file. Comment them
     // to default to the console.
     //
     // Note: the new DebugStream and ofstream MUST live after we return
     // from setUpDebugLogging, so we have to allocate
     // them dynamically.
     //
     //
     // Explanation: 
     // 
     //     The first line is ordinary C++ to open a file
     //     as an output stream.
     //
     //     The second line wraps that will all the services
     //     of a comp 150-IDS debug stream, and names that filestreamp.
     //
     //     The third line replaces the global variable c150debug
     //     and sets it to point to the new debugstream. Since c150debug
     //     is what all the c150 debug routines use to find the debug stream,
     //     you've now effectively overridden the default.
     //
     ofstream *outstreamp = new ofstream(logname);
     DebugStream *filestreamp = new DebugStream(outstreamp);
     DebugStream::setDefaultLogger(filestreamp);

     //
     //  Put the program name and a timestamp on each line of the debug log.
     //
     c150debug->setPrefix(argv[0]);
     c150debug->enableTimestamp(); 

     //
     // Ask to receive all classes of debug message
     //
     // See c150debug.h for other classes you can enable. To get more than
     // one class, you can or (|) the flags together and pass the combined
     // mask to c150debug -> enableLogging 
     //
     // By the way, the default is to disable all output except for
     // messages written with the C150ALWAYSLOG flag. Those are typically
     // used only for things like fatal errors. So, the default is
     // for the system to run quietly without producing debug output.
     //
     c150debug->enableLogging(C150ALLDEBUG | C150RPCDEBUG | C150APPLICATION | C150NETWORKTRAFFIC | 
			      C150NETWORKDELIVERY); 
}
