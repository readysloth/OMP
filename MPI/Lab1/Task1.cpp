
#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstddef>

#include "ring.hpp"

int main(int argc, char *argv[]){
    std::string message("Message to root");

    ring(message);

    return 0;
}

