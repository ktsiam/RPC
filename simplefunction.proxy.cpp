#include "rpcproxyhelper.h"
#include <tuple>
#include "lotsofstuff.idl"

using MT_T = std::tuple<>;
using Person_T = std::tuple<int, string, string>;
using StructWithArrays_T = std::tuple<Person_T[10], int>;
using ThreePeople_T = std::tuple<Person_T, Person_T, Person_T>;
using rectangle_T = std::tuple<int, int>;
using s_T = std::tuple<int[4][10][100], int[4][10], int[4]>;


int area(rectangle r) {
    return cast_to<int>(
        serialize_and_apply<int, rectangle_T>(
            "area",
            cast_to<rectangle_T>(r)));
}

Person findPerson(ThreePeople tp) {
    return cast_to<Person>(
        serialize_and_apply<Person_T, ThreePeople_T>(
            "findPerson",
            cast_to<ThreePeople_T>(tp)));
}

void func1() {
    return (
        serialize_and_apply<void>(
            "func1"));
}

void func2() {
    return (
        serialize_and_apply<void>(
            "func2"));
}

void func3() {
    return (
        serialize_and_apply<void>(
            "func3"));
}

float multiply(float x, float y) {
    return cast_to<float>(
        serialize_and_apply<float, float, float>(
            "multiply",
            cast_to<float>(x),
            cast_to<float>(y)));
}

void searchRectangles(rectangle rects[200]) {
    return (
        serialize_and_apply<void, std::array<rectangle_T, 200>>(
            "searchRectangles",
            cast_to<std::array<rectangle_T, 200>>(rects)));
}

int showsArraysofArrays(int x[24], int y[24][15], int z[24][15]) {
    return cast_to<int>(
        serialize_and_apply<int, std::array<int, 24>, std::array<int[15], 24>, std::array<int[15], 24>>(
            "showsArraysofArrays",
            cast_to<std::array<int, 24>>(x),
            cast_to<std::array<int[15], 24>>(y),
            cast_to<std::array<int[15], 24>>(z)));
}

int sqrt(int x, int y) {
    return cast_to<int>(
        serialize_and_apply<int, int, int>(
            "sqrt",
            cast_to<int>(x),
            cast_to<int>(y)));
}

int takesTwoArrays(int x[24], int y[24]) {
    return cast_to<int>(
        serialize_and_apply<int, std::array<int, 24>, std::array<int, 24>>(
            "takesTwoArrays",
            cast_to<std::array<int, 24>>(x),
            cast_to<std::array<int, 24>>(y)));
}

string upcase(string s) {
    return cast_to<string>(
        serialize_and_apply<string, string>(
            "upcase",
            cast_to<string>(s)));
}
