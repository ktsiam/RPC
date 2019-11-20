
#include <cstdio>
#include <cctype>
#include "c150debug.h"

#include "lotsofstuff.idl"
#include <iostream>

using namespace C150NETWORK;  // for all the comp150 utilities 

#include <cmath>
#include <algorithm>

void func1() { std::cout << "func1 called\n"; }
void func3() { std::cout << "func2 called\n"; }
void func2() { std::cout << "func3 called\n"; }

int sqrt(int x, int y) {
    return x + y;
}

int takesTwoArrays(int x[24], int y[24]) {
    return x[0] + y[0];
}

int showsArraysofArrays(int x[24], int y[24][15], int z[24][15]) {
    return x[0] + y[0][0] + z[0][0];
}

string upcase(string s) {
    for (char &c : s) {
        c = std::toupper(c);
    }
    return s;
}

Person findPerson(ThreePeople tp) {
    return tp.p1;
}

float multiply(float x, float y) {
    return x*y;
}

int area(rectangle r) {
    return r.x * r.y;
}

void searchRectangles(rectangle rects[200]) { }
