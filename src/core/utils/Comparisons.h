#ifndef _COMPARISONS_H_
#define _COMPARISONS_H_

namespace s21 {
    template<typename T>
    class Comparisons {
       public:
        static bool is_equal(T a, T b) {
            return a == b;
        }

        static bool is_not_equal(T a, T b) {
            return a != b;
        }

        static bool is_less(T a, T b) {
            return a < b;
        }

        static bool is_less_or_equal(T a, T b) {
            return a <= b;
        }

        static bool is_greater(T a, T b) {
            return a > b;
        }

        static bool is_greater_or_equal(T a, T b) {
            return a >= b;
        }   
    };
}

#endif  // _COMPARISONS_H_
