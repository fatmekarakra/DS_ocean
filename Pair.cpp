#include "Pair.h"
std::ostream& operator<<(std::ostream& os, const Pair& pair) {
    os << "(" << pair.first << ", " << pair.second <<  ")";
    return os;
}