/* test_mt19937.cpp
 *
 * Copyright Steven Watanabe 2011
 * Copyright Thijs van den Berg 2014
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 *
 */

#include <boost/random/threefry4x64.hpp>
#include <cmath>

#define BOOST_RANDOM_URNG boost::random::threefry4x64_13

#define BOOST_RANDOM_SEED_WORDS 2 // the default engine has 1x a 64bit key: 2x 32 bits

#define BOOST_RANDOM_VALIDATION_VALUE 0x8d7f035bU            // The 10000th consecutive invocation of a default-constructed object shall produce
#define BOOST_RANDOM_SEED_SEQ_VALIDATION_VALUE 0x397317e1U
#define BOOST_RANDOM_ITERATOR_VALIDATION_VALUE 0x5aed945cU

#define BOOST_RANDOM_GENERATE_VALUES { 0xe1dc8e05U, 0x4071fabeU, 0x695c9c62U, 0x2ed3113U }
        
#include "test_generator.ipp"

