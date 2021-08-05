#include "log.hpp"
#include <iostream>

namespace framework{
    void log() {
        std::cout << "log" << std::endl;
    }
    unsigned multiply(unsigned a, unsigned b){
        return a * b;
    }
}
