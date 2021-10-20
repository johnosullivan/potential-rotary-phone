#ifndef REGISTERS_H
#define REGISTERS_H

#include <map>

#include "common/common.h"

namespace core::compiler {

class Registers {
    public:
        Registers();
        ~Registers();
        static void setup_register(std::map<std::string, int>*, std::vector<std::string>*, int);
};

}

#endif //REGISTERS_H