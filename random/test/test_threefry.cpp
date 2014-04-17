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

#include <boost/random/threefry.hpp>
#include <cmath>

#define BOOST_RANDOM_URNG boost::random::threefry20_32

#define BOOST_RANDOM_SEED_WORDS 8

#define BOOST_RANDOM_VALIDATION_VALUE 0xF224AAB8U            // The 10000th consecutive invocation of a default-constructed object shall produce
#define BOOST_RANDOM_SEED_SEQ_VALIDATION_VALUE 1412154127
#define BOOST_RANDOM_ITERATOR_VALIDATION_VALUE 2337410464

#define BOOST_RANDOM_GENERATE_VALUES { 0xE6C85537U, 0x09218EBDU, 0x66D86105U, 0x55941F52U }
        
#include "test_generator.ipp"

