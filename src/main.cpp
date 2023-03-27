// #include "_MY_VECTOR_res_.hpp"
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include "my_fmt.hpp"

class C {
public:
    int k;
};

int operator<<(std::ostream& m, C c) {
    m << c.k;
    return 14;
}

int main() {
    std::string lol = "1234";
    fmt_print_base(lol);
    // fmt_print_base(1);
    C c;
    c.k = 99999;    
    std::cout << c;
    fmt_print_base(c);
    
    
    char* abc = "string";
    // fmt_print_base(abc);
    char test = 'a';
    fmt_print_base(test);
    
    std::cout << abc;
    // std::cout << 1;
    // std::cout
    // std::cout;
    /*int a = 0;
    unsigned b = 2;
    float c = 3.0;
    double d = 4.0;
    char e = 'e';
    char* f = "f";
    std::string g = "g";

    normalizeArg(a);
    normalizeArg(b);
    normalizeArg(a);
    normalizeArg(d);
    normalizeArg(e);
    normalizeArg(g);

    safe_printf("%d %s\n", a, g);
    // normalizeArg(g);
    
    DataStructures::Vector<bool> abc;
    // DataStructures::Vector<int> vec;
    abc.push_back(0);
    abc.push_back(1);
    abc.push_back(0);
    abc.push_back(1);
    abc.push_back(1);
    for (auto v = abc.begin(); v != abc.end(); v++) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    // std::sort(abc.begin(), abc.end());
    for (auto v = abc.begin(); v != abc.end(); v++) {
        std::cout << v << " ";
    }
    // std::cout << vec[0] << vec[1] << vec[2] << vec[3] << vec[4] << "\n";
    // DataStructures::Vector<int>::iterator it =  std::find(vec.begin(), vec.end(), 1);
    // auto vec_begin = vec.begin();
    // std::cout << vec_begin[4];
    // std::cout << *(vec_begin + 0) << *(vec_begin + 1) << *(vec_begin + 2) << *(vec_begin + 3) << *(vec_begin + 4);
    // std::cout << "found:" << (*it) << "\n";
    // std::sort(vec.rbegin(), vec.rend());
    // std::cout << "ff" << vec.end() - vec.begin() << "ff\n";
    // std::cout << vec[0] << vec[1] << vec[2] << vec[3] << vec[4] << "\n";
    // std::sort(vec.begin(), vec.end());
    // std::cout << vec[0] << vec[1] << vec[2] << vec[3] << vec[4] << "\n";



    // abc.push_back(1);
    // abc.push_back(0);
    // // Vector<bool> abc;
    // std::cout << abc.begin() << 
    // (((((abc.begin() + 121) - 120) + 15) + 53) - 68);
    // std::sort(abc.begin(), abc.end());*/
    return 0;
}