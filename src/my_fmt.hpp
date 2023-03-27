#ifndef SAFE_PRINTF
#define SAFE_PRINTF

#include <exception>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <cxxabi.h>
#include <iostream>
#include <utility>
#include <type_traits>
#include <typeinfo>

#define PRINTF_EXCEPTION(msg) \
    { \
        fprintf(stderr, "\e[31mException: %s\033[0m\n", (msg)); \
        throw std::invalid_argument(msg); \
    }

// ------- ALLOWED TYPES FOR PRINTF -------
template <class T>
    requires std::is_integral<T>::value
T normalizeArg(T arg) { return arg; }

template <class T>
    requires std::is_floating_point<T>::value
T normalizeArg(T arg) { return arg; }

template <class T>
    requires std::is_pointer<T>::value
T normalizeArg(T arg) { return arg; }


const char* normalizeArg(std::string str) { return str.c_str(); }

// ----- END ALLOWED TYPES FOR PRINTF -----



// ----- Base check case for printf  (no %* in fmt and no passed args) -----
void check_printf(const char* fmt) {
    if (fmt == NULL) {
        PRINTF_EXCEPTION("fmt is NULL");
    }

    for (; *fmt; ++fmt) {
        //guard if %% in fmt
        if (*fmt != '%' || (*(fmt+1) != 0 && *++fmt == '%')) continue;
        PRINTF_EXCEPTION("fmt contains %, but no args left");
    }
}


// ---- Recursive check case for printf (fmt contains %* or passed args) ----

template <class T, class... T_it>
void check_printf(const char* fmt, const T&, const T_it&... other) {
    for (; *fmt; ++fmt) {
        // skip non-format symbols
        if (*fmt != '%' || (*(fmt+1) != 0 && *++fmt == '%')) continue;

        switch(*fmt) {
            default:
                printf("'%c'", *fmt);
                assert("unknown format specifier after %");
                PRINTF_EXCEPTION("unknown format specifier after %");

            case 'f':
            case 'g':
                assert(std::is_floating_point<T>::value && "floating point type expected");
                break;
            
            case 'd':
            case 'u':
            case 'z':
                assert((std::is_integral<T>::value && "integral type expected"));
                break;

            case 's':
                assert((std::is_same<T, char*>::value || std::is_same<T, const char*>::value) && "char* type expected");
                break;

            case 'c':
                assert((std::is_same<T, char>::value && "char type expected"));
                break;
        }

        return check_printf(fmt+1, other...);
    }
    
    printf("not enough args");
    PRINTF_EXCEPTION("not enough arguments for format string");
}



// ----- safe_printf function -----
template <typename... T_it>
int safe_printf(const char* fmt, const T_it&... args) {
    check_printf(fmt, normalizeArg(args)...);
    return printf(fmt, normalizeArg(args)...);
}












// void check_fmt_printf(const char* fmt) {
//     if (fmt == NULL) {
//         PRINTF_EXCEPTION("fmt is NULL");
//     }

//     for (; *fmt; ++fmt) {
//         //guard if %% in fmt
//         if (*fmt != '%' || (*(fmt+1) != 0 && *++fmt == '%')) continue;
//         PRINTF_EXCEPTION("fmt contains %, but no args left");
//     }
// }

// template <class T, class... T_it>
// void check_fmt_printf(const char* fmt, const T&, const T_it&... other) {
//     for (; *fmt; ++fmt) {
//         // skip non-format symbols
//         if (*fmt != '%' || (*(fmt+1) != 0 && *++fmt == '%')) continue;

//         switch(*fmt) {
//             default:
//                 printf("'%c'", *fmt);
//                 assert("unknown format specifier after %");
//                 PRINTF_EXCEPTION("unknown format specifier after %");

//             case 'f':
//             case 'g':
//                 assert(std::is_floating_point<T>::value && "floating point type expected");
//                 break;
            
//             case 'd':
//             case 'u':
//             case 'z':
//                 assert((std::is_integral<T>::value && "integral type expected"));
//                 break;

//             case 's':
//                 assert((std::is_same<T, char*>::value || std::is_same<T, const char*>::value) && "char* type expected");
//                 break;

//             case 'c':
//                 assert((std::is_same<T, char>::value && "char type expected"));
//                 break;
//             case '#':
                
//         }

//         return check_fmt_printf(fmt+1, other...);
//     }
    
//     printf("not enough args");
//     PRINTF_EXCEPTION("not enough arguments for format string");
// }




// cheeeeeck

template<typename T>
concept NotFundamentalPrintable = 
    not (std::is_fundamental_v<T>) && 
    not (std::is_same_v<char&, T>)
     &&
    requires(std::ostream& a, T&& b) {
        { a << b };
        // requires std::is_integral<T>;
        // { a << b } -> std::same_as<std::ostream&>;
    };


template<typename T>
    requires NotFundamentalPrintable<T>
int fmt_print_base(T&& current) {
    std::cout << "is_same:" << std::is_same_v<T, char*&> << ", ";
    std::cout << typeid(T).name() << ", ";
    std::cout << current;
    std::cout << "\n";
    return 0;
}


// template<typename T>
// concept IsFundamentalPrintable = 
//     std::is_fundamental_v<T> || 
//     std::is_same_v<char*&, T>


// template<typename T>
//     requires IsFundamentalPrintable<T>
// int fmt_print_base(T&& current) {
    
//     return 0;
// }


// template<typename T>
//     requires requires (std::ostream& a, T b) {
//         typename std::is_integral<T>::value;
//         // { a << b };// && 
//         typename std::is_same<decltype(a << b), std::ostream&>;
//     }
// int fmt_print_base(T current) {
//     std::cout << current;
//     return 0;
// }



int fmt_print(const char* fmt) {

}

template <typename T1, typename... T_it>
int fmt_print(const char* fmt, const T1& current, const T_it&... other) {
    
    return 0;
}



// template <class T>
//     requires {  requires std::same_as<T, double>; }
//     T normalizeArg(T arg) { return arg; }

// template <class T>
//     requires {  requires std::same_as<T, int>; }
//     T normalizeArg(T arg) { return arg; }









#endif