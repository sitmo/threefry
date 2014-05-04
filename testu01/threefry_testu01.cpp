/*
* Run the BigCrush test from the TestU01 framework.
*
* To run this test you will first need to make the testU01 framework.
* homepage:      http://www.iro.umontreal.ca/~simardr/testu01/tu01.html
* documentation: http://www.iro.umontreal.ca/~lecuyer/myftp/papers/testu01.pdf
*/


extern "C"
{
    #include "unif01.h"
    #include "bbattery.h"
}


#include <boost/random/threefry4x64.hpp>

boost::random::threefry4x64_13 engine;
uint32_t engine_function() { return engine(); }


int main (void)
{
    char engine_name[] = "threefry4x64_13";

    unif01_Gen *gen;
    gen = unif01_CreateExternGenBits(engine_name, engine_function);
    bbattery_BigCrush (gen);
    unif01_DeleteExternGenBits (gen);
    return 0;
}
