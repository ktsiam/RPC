# Makefile for COMP 117 RPC Samples
#
#    Copyright 2012 - Noah Mendelsohn
#
#
# Useful targets:
#
#
#    simplefunctionclient - sample RPC client
#    simplefunctionserver - sample RPC server
#
#    clean       - clean out all compiled object and executable files
#    all         - (default target) make sure everything's compiled
#

# Do all C++ compies with g++
CPP = g++ -std=c++17

# change following line if your rpgenerate is not in current directory
RPCGEN = ./rpcgenerate

# Where the COMP 150 shared utilities live, including c150ids.a and userports.csv
# Note that environment variable COMP117 must be set for this to work!

C150LIB = $(COMP117)/files/c150Utils/
C150AR = $(C150LIB)c150ids.a

# RPC framework is shared archive
C150IDSRPC = $(COMP117)/files/RPC.framework/
C150IDSRPCAR = $(C150IDSRPC)c150idsrpc.a

CPPFLAGS = -g -Wall -Werror -I$(C150IDSRPC) -I$(C150LIB)
# CPPFLAGS = -g -Wall -Werror  -I$(C150LIB)


LDFLAGS = 
INCLUDES = $(C150LIB)c150streamsocket.h $(C150LIB)c150network.h $(C150LIB)c150exceptions.h $(C150LIB)c150debug.h $(C150LIB)c150utility.h $(C150LIB)c150grading.h $(C150IDSRPC)IDLToken.h $(C150IDSRPC)tokenizeddeclarations.h  $(C150IDSRPC)tokenizeddeclaration.h $(C150IDSRPC)declarations.h $(C150IDSRPC)declaration.h $(C150IDSRPC)functiondeclaration.h $(C150IDSRPC)typedeclaration.h $(C150IDSRPC)arg_or_member_declaration.h rpcproxyhelper.h rpcstubhelper.h simplefunction.idl arithmetic.idl floatarithmetic.idl 

all: simplefunctionclient simplefunctionserver idl_to_json

########################################################################
#
#          Sample RPC client and server applications
#
#     THESE RULES ARE TO BE USED UNTIL RPCGENERATE IS AVAILABLE
#     AFTER THAT, COMMENT THEM
#
#     Demonstrating remote calls to functions as declared in simplefunctions.idl
#
#     The proxies and stubs used here are hand generated, but eventually
#     your rpcgenerate program will (should) generate them automatically
#     from any idl
#
########################################################################

simplefunctionclient: simplefunctionclient.o rpcproxyhelper.o simplefunction.proxy.o  $(C150AR) $(C150IDSRPCAR)  
	$(CPP) -o simplefunctionclient simplefunctionclient.o rpcproxyhelper.o simplefunction.proxy.o  $(C150AR) $(C150IDSRPCAR) 

# The following is NOT a mistake. The main program for any of the rpc servers
# is rpcserver.o.  This way, we can make a different one for each set 
# of functions, by linking the right specific stugs (in this case
# simplefunction.stub.o)
simplefunctionserver: simplefunction.stub.o rpcserver.o rpcstubhelper.o simplefunction.o  $(C150AR) $(C150IDSRPCAR)  
	$(CPP) -o simplefunctionserver rpcserver.o simplefunction.stub.o simplefunction.o rpcstubhelper.o $(C150AR) $(C150IDSRPCAR) 


########################################################################
#
#          General rules for building any client and server
#
#     Given any xxx.idl, these rules will build xxxclient and xxxserver
#
#     THESE RULES ARE SUPPLIED COMMENTED BECAUSE THEY WILL BREAK
#     IF USED BEFORE rpcgenerate IS AVAILABLE.
#
#     WHEN YOUR RPCGENERATE IS WORKING, DO THE FOLLOWING
#
#       1) Uncomment the rules below
#
#       2) Add to each of the dependency lists and the g++ invocations
#          any .o files that you need to link into clients and servers
#          respectively.
#
#
########################################################################

# Compile / link any client executable: 
%client: %.o %.proxy.o rpcserver.o rpcproxyhelper.o %client.o %.proxy.o
	$(CPP) -o $@ $@.o rpcproxyhelper.o $*.proxy.o  $(C150AR) $(C150IDSRPCAR) 

# Compile / link any server executable:
%server: %.o %.stub.o rpcserver.o rpcstubhelper.o %.stub.o
	$(CPP) -o $@ rpcserver.o $*.stub.o $*.o rpcstubhelper.o $(C150AR) $(C150IDSRPCAR) 



########################################################################
#
#          Generate C++ source from IDL files
#
#     Once you have written an rpcgenerate program, you can uncomment
#     the following two lines to have .cpp generated automatically from idl.
#     (but be sure not to have other .cpp files with the same base name
#     as the idl, e.g. the ones with your functions!)
#
#     The proxies and stubs used here are hand generated, but eventually
#     your rpcgenerate program will (should) generate them automatically
#     from any idl
#
#     WARNING! this may cause your rpcgenerate program to attempt to
#     rebuild supplied samples like simplefunction.proxy.cpp and 
#     simplefunction.stub.cpp. You may want to save backups for comparison.
#
########################################################################

%.proxy.cpp %.stub.cpp:%.idl $(RPCGEN)
	$(RPCGEN) $<

########################################################################
#
#                   idl_to_json
#
#     This program servers two purposes:
#
#     1) If you are writing your rpc generator in Python or Ruby,
#        this utility will convert .idl files to JSON for you.
#     2) Especially if you are writing rpcgenerate in C++, the
#        source code of idl_to_json.cpp is the best way to learn use
#        the provided .idl parser.
#
########################################################################

idl_to_json: idl_to_json.o $(C150AR) $(C150IDSRPCAR)
	$(CPP) -o idl_to_json idl_to_json.o $(C150AR) $(C150IDSRPCAR) 

########################################################################
#
#          Generate .json files from .idl files
#
#     This rule allows you to say:
#
#              make demo.json
#
#     if you already have demo.idl
#
########################################################################

%.json:%.idl idl_to_json
	     idl_to_json $< > $@

########################################################################
#
#                   Housekeeping
#
########################################################################


# make .o from .cpp

%.o:%.cpp 
	$(CPP) -c  $(CPPFLAGS) $< 



# clean up everything we build dynamically (probably missing .cpps from .idl)
clean:
	 rm -f idl_to_json simplefunctionclient simplefunctionserver *.o *.json *.pyc GRADELOG*


