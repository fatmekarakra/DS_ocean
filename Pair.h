#include <iostream>
class Pair {
public:
    int first;
    int second;
    Pair(const int& f,const int& s) : first(f), second(s) {}
    Pair(const Pair& other) : first(other.first), second(other.second) {}
    Pair() : first(int()), second(int()) {}
    ~Pair()=default;
    bool operator==(const Pair& other) const {
        return (first == other.first && second == other.second);
    }
    bool operator>(const Pair& other) const {
        return (first > other.first || (first == other.first && second > other.second));
    }
    bool operator<(const Pair& other) const {
        return (first < other.first || (first == other.first && second < other.second));
    }
    bool operator>=(const Pair& other) const {
        return (first >= other.first || (first == other.first && second >= other.second));
    }
    bool operator<=(const Pair& other) const {
        return (first <= other.first || (first == other.first && second <= other.second));
    }
    friend std::ostream& operator<<(std::ostream& os, const Pair& pair);
};
std::ostream& operator<<(std::ostream& os, const Pair& pair);