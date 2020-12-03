#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int h = seconds / (60 * 60);
    seconds -= h * (60 * 60);
    int m = seconds / (60);
    seconds -= m * (60);
    int s = seconds;
    return std::to_string(h)+":"+std::to_string(m)+":"+std::to_string(s);
}
