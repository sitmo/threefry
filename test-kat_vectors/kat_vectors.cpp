/* kat_vecotors.cpp
 *
 * Copyright (c) 2014 M.A. (Thijs) van den Berg
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 */
 
/*
These test cases are taken from: https://github.com/girving/random123/blob/master/examples/kat_vectors

# For each generator, we test:  gen(0, 0), gen(fff, fff) and gen(ctr=digits_of_pi, key=more_digits_of_pi).
# Ignoring endianness, these are the first few hexdigits of pi:
# 243F6A88 85A308D3 13198A2E 03707344 A4093822 299F31D0 082EFA98 EC4E6C89 452821E6 38D01377 BE5466CF 34E90C6C C0AC29B7 C97C50DD 3F84D5B5 B5470917 9216D5D9 8979FB1BD
#
#nameNxW      R  CTR                                                                 KEY                                                                 EXPECTED
#threefry4x64 13 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 4071fabee1dc8e05 02ed3113695c9c62 397311b5b89f9d49 e21292c3258024bc
#threefry4x64 13 ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff 7eaed935479722b5 90994358c429f31c 496381083e07a75b 627ed0d746821121
#threefry4x64 13 243f6a8885a308d3 13198a2e03707344 a4093822299f31d0 082efa98ec4e6c89 452821e638d01377 be5466cf34e90c6c c0ac29b7c97c50dd 3f84d5b5b5470917 4361288ef9c1900c 8717291521782833 0d19db18c20cf47e a0b41d63ac8581e5
#threefry4x64 20 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 09218ebde6c85537 55941f5266d86105 4bd25e16282434dc ee29ec846bd2e40b
#threefry4x64 20 ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff 29c24097942bba1b 0371bbfb0f6f4e11 3c231ffa33f83a1c cd29113fde32d168
#threefry4x64 20 243f6a8885a308d3 13198a2e03707344 a4093822299f31d0 082efa98ec4e6c89 452821e638d01377 be5466cf34e90c6c be5466cf34e90c6c c0ac29b7c97c50dd a7e8fde591651bd9 baafd0c30138319b 84a5c1a729e685b9 901d406ccebc1ba4
*/

#define BOOST_TEST_MODULE threefry
#include <istream>
#include <ostream>

#include <boost/test/included/unit_test.hpp>
#include <boost/random/threefry.hpp>

BOOST_AUTO_TEST_CASE( threefry13_64_00 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 13> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;
    
    BOOST_CHECK_EQUAL( eng(), 0x4071fabee1dc8e05 );
    BOOST_CHECK_EQUAL( eng(), 0x02ed3113695c9c62 );
    BOOST_CHECK_EQUAL( eng(), 0x397311b5b89f9d49 );
    BOOST_CHECK_EQUAL( eng(), 0xe21292c3258024bc );
}

BOOST_AUTO_TEST_CASE( threefry13_64_ff )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 13> eng;

    std::ostringstream os;
    os << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' '
       << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0;
    std::istringstream is( os.str() );
    is >> eng;
        
    BOOST_CHECK_EQUAL( eng(), 0x7eaed935479722b5 );
    BOOST_CHECK_EQUAL( eng(), 0x90994358c429f31c );
    BOOST_CHECK_EQUAL( eng(), 0x496381083e07a75b );
    BOOST_CHECK_EQUAL( eng(), 0x627ed0d746821121 );
}

BOOST_AUTO_TEST_CASE( threefry13_64_24 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 13> eng;

    std::ostringstream os;
    os << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' ' << 0x3f84d5b5b5470917 << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is( os.str() );
    is >> eng;

    BOOST_CHECK_EQUAL( eng(), 0x4361288ef9c1900c );
    BOOST_CHECK_EQUAL( eng(), 0x8717291521782833 );
    BOOST_CHECK_EQUAL( eng(), 0x0d19db18c20cf47e );
    BOOST_CHECK_EQUAL( eng(), 0xa0b41d63ac8581e5 );
}



BOOST_AUTO_TEST_CASE( threefry20_64_00 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 20> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;
    
    BOOST_CHECK_EQUAL( eng(), 0x09218ebde6c85537 );
    BOOST_CHECK_EQUAL( eng(), 0x55941f5266d86105 );
    BOOST_CHECK_EQUAL( eng(), 0x4bd25e16282434dc );
    BOOST_CHECK_EQUAL( eng(), 0xee29ec846bd2e40b );
}

BOOST_AUTO_TEST_CASE( threefry20_64_ff )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 20> eng;
    
    std::ostringstream os;
    os << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' '
       << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0;
    std::istringstream is( os.str() );
    is >> eng;
        
    BOOST_CHECK_EQUAL( eng(), 0x29c24097942bba1b );
    BOOST_CHECK_EQUAL( eng(), 0x0371bbfb0f6f4e11 );
    BOOST_CHECK_EQUAL( eng(), 0x3c231ffa33f83a1c );
    BOOST_CHECK_EQUAL( eng(), 0xcd29113fde32d168 );
}

BOOST_AUTO_TEST_CASE( threefry20_64_24 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 20> eng;

    std::ostringstream os;
    os << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is( os.str() );
    is >> eng;

    BOOST_CHECK_EQUAL( eng(), 0xa7e8fde591651bd9 );
    BOOST_CHECK_EQUAL( eng(), 0xbaafd0c30138319b );
    BOOST_CHECK_EQUAL( eng(), 0x84a5c1a729e685b9 );
    BOOST_CHECK_EQUAL( eng(), 0x901d406ccebc1ba4 );
}
