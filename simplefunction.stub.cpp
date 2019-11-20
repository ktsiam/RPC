#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>
#include "lotsofstuff.idl"

using MT_T = std::tuple<>;
using Person_T = std::tuple<int, string, string>;
using StructWithArrays_T = std::tuple<Person_T[10], int>;
using ThreePeople_T = std::tuple<Person_T, Person_T, Person_T>;
using rectangle_T = std::tuple<int, int>;
using s_T = std::tuple<int[4][10][100], int[4][10], int[4]>;


int __area_inter(rectangle_T& r) {
    return cast_to<int>(
        area(
            cast_to<rectangle>(r)));
}

void __area() {
    __deserialize_and_apply<int, rectangle_T>(__area_inter);
}

Person_T __findPerson_inter(ThreePeople_T& tp) {
    return cast_to<Person_T>(
        findPerson(
            cast_to<ThreePeople>(tp)));
}

void __findPerson() {
    __deserialize_and_apply<Person_T, ThreePeople_T>(__findPerson_inter);
}

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

float __multiply_inter(float& x, float& y) {
    return cast_to<float>(
        multiply(
            cast_to<float>(x)
            , cast_to<float>(y)));
}

void __multiply() {
    __deserialize_and_apply<float, float, float>(__multiply_inter);
}

void __searchRectangles_inter(std::array<rectangle_T, 200>& rects) {
    return (
        searchRectangles(
            cast_to<rectangle[200]>(rects)));
}

void __searchRectangles() {
    __deserialize_and_apply<void, std::array<rectangle_T, 200>>(__searchRectangles_inter);
}

int __showsArraysofArrays_inter(std::array<int, 24>& x, std::array<int[15], 24>& y, std::array<int[15], 24>& z) {
    return cast_to<int>(
        showsArraysofArrays(
            cast_to<int[24]>(x)
            , cast_to<int[24][15]>(y)
            , cast_to<int[24][15]>(z)));
}

void __showsArraysofArrays() {
    __deserialize_and_apply<int, std::array<int, 24>, std::array<int[15], 24>, std::array<int[15], 24>>(__showsArraysofArrays_inter);
}

int __sqrt_inter(int& x, int& y) {
    return cast_to<int>(
        sqrt(
            cast_to<int>(x)
            , cast_to<int>(y)));
}

void __sqrt() {
    __deserialize_and_apply<int, int, int>(__sqrt_inter);
}

int __takesTwoArrays_inter(std::array<int, 24>& x, std::array<int, 24>& y) {
    return cast_to<int>(
        takesTwoArrays(
            cast_to<int[24]>(x)
            , cast_to<int[24]>(y)));
}

void __takesTwoArrays() {
    __deserialize_and_apply<int, std::array<int, 24>, std::array<int, 24>>(__takesTwoArrays_inter);
}

string __upcase_inter(string& s) {
    return cast_to<string>(
        upcase(
            cast_to<string>(s)));
}

void __upcase() {
    __deserialize_and_apply<string, string>(__upcase_inter);
}

void dispatchFunction() {
    char functionNameBuffer[50];
    getFunctionNameFromStream(functionNameBuffer,
                              sizeof(functionNameBuffer));
    if (!RPCSTUBSOCKET-> eof()) {
        if (strcmp(functionNameBuffer,"area") == 0) {
            __area();
            return;
        }
        if (strcmp(functionNameBuffer,"findPerson") == 0) {
            __findPerson();
            return;
        }
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
        if (strcmp(functionNameBuffer,"multiply") == 0) {
            __multiply();
            return;
        }
        if (strcmp(functionNameBuffer,"searchRectangles") == 0) {
            __searchRectangles();
            return;
        }
        if (strcmp(functionNameBuffer,"showsArraysofArrays") == 0) {
            __showsArraysofArrays();
            return;
        }
        if (strcmp(functionNameBuffer,"sqrt") == 0) {
            __sqrt();
            return;
        }
        if (strcmp(functionNameBuffer,"takesTwoArrays") == 0) {
            __takesTwoArrays();
            return;
        }
        if (strcmp(functionNameBuffer,"upcase") == 0) {
            __upcase();
            return;
        }
        assert(false && "function contract not found");
    }
}

