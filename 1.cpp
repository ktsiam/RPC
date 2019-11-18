#include <tuple>
#include <cstring>
#include <iostream>
struct Person {
    std::string fname;
    std::string sname;
    int age;
    // int b;
    // int c;
    // long double a;
    // std::string hi;
};

using awdpokaspodk = Person[20][30];
using PersonT = std::tuple<int, std::string, std::string>;

struct ThreePeople {
    Person p1;
    Person p2;
    char p3[17] = "hello";
};

using ThreePeopleT = std::tuple<char[17], PersonT, PersonT>;

void print(ThreePeopleT tp) {
    std::cout << std::get<0>(tp) << '\n';
               << std::get<0>(std::get<1>(tp)) << '\n'
                      //           << &std::get<2>(tp) << '\n';
}


int main() {
    Person p = {"a", "b", 17};
    Person p2 = {"c", "d", 117};
    
    PersonT t = reinterpret_cast<PersonT&>(p);
    PersonT t2 = reinterpret_cast<PersonT&>(p2);


    ThreePeople tp;
    tp.p1 = p;
    tp.p2 = p2;

    print(reinterpret_cast<ThreePeopleT&>(tp));
    
    //std::get<1>(tp) = t2;
    // std::cout << std::get<0>(std::get<1>(tp)) << std::get<1>(std::get<1>(tp)) << std::get<2>(std::get<1>(tp)) << std::endl;
    // std::cout << std::get<0>(tp) << std::endl;
}

