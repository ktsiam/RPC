
#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>
#include "simplefunction.idl"



void __func1_inter() {
	return (
	func1());
}

void __func1() {
	__deserialize_and_apply<void>(__func1_inter);
}

void __func2_inter() {
	return (
	func2());
}

void __func2() {
	__deserialize_and_apply<void>(__func2_inter);
}

void __func3_inter() {
	return (
	func3());
}

void __func3() {
	__deserialize_and_apply<void>(__func3_inter);
}

void dispatchFunction() {
    char functionNameBuffer[50];
    getFunctionNameFromStream(functionNameBuffer,
                              sizeof(functionNameBuffer));
    if (!RPCSTUBSOCKET-> eof()) {
	if (strcmp(functionNameBuffer,"func1") == 0) {
		__func1();
		return;
	}
	if (strcmp(functionNameBuffer,"func2") == 0) {
		__func2();
		return;
	}
	if (strcmp(functionNameBuffer,"func3") == 0) {
		__func3();
		return;
	}
	assert(false && "function contract not found");
	}
}
