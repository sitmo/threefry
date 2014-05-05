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
#threefry4x64 72 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 94eeea8b1f2ada84 adf103313eae6670 952419a1f4b16d53 d83f13e63c9f6b11
#threefry4x64 72 ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff 11518c034bc1ff4c 193f10b8bcdcc9f7 d024229cb58f20d8 563ed6e48e05183f
#threefry4x64 72 243f6a8885a308d3 13198a2e03707344 a4093822299f31d0 082efa98ec4e6c89 452821e638d01377 be5466cf34e90c6c be5466cf34e90c6c c0ac29b7c97c50dd acf412ccaa3b2270 c9e99bd53f2e9173 43dad469dc825948 fbb19d06c8a2b4dc

*/

#define BOOST_TEST_MODULE threefry
#include <istream>
#include <ostream>

#include <boost/test/included/unit_test.hpp>
#include <boost/random/threefry4x64.hpp>



BOOST_AUTO_TEST_CASE( threefry4x64_13_8 )
{
    boost::random::threefry4x64_engine<boost::uint32_t, 8, 13> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;

    BOOST_CHECK_EQUAL( eng(), 0x05 );
    BOOST_CHECK_EQUAL( eng(), 0x8e );
    BOOST_CHECK_EQUAL( eng(), 0xdc );
    BOOST_CHECK_EQUAL( eng(), 0xe1 );
    BOOST_CHECK_EQUAL( eng(), 0xbe );
    BOOST_CHECK_EQUAL( eng(), 0xfa );
    BOOST_CHECK_EQUAL( eng(), 0x71 );
    BOOST_CHECK_EQUAL( eng(), 0x40 );
    BOOST_CHECK_EQUAL( eng(), 0x62 );
    BOOST_CHECK_EQUAL( eng(), 0x9c );
    BOOST_CHECK_EQUAL( eng(), 0x5c );
    BOOST_CHECK_EQUAL( eng(), 0x69 );
    BOOST_CHECK_EQUAL( eng(), 0x13 );
    BOOST_CHECK_EQUAL( eng(), 0x31 );
    BOOST_CHECK_EQUAL( eng(), 0xed );
    BOOST_CHECK_EQUAL( eng(), 0x02 );
    BOOST_CHECK_EQUAL( eng(), 0x49 );
    BOOST_CHECK_EQUAL( eng(), 0x9d );
    BOOST_CHECK_EQUAL( eng(), 0x9f );
    BOOST_CHECK_EQUAL( eng(), 0xb8 );
    BOOST_CHECK_EQUAL( eng(), 0xb5 );
    BOOST_CHECK_EQUAL( eng(), 0x11 );
    BOOST_CHECK_EQUAL( eng(), 0x73 );
    BOOST_CHECK_EQUAL( eng(), 0x39 );
    BOOST_CHECK_EQUAL( eng(), 0xbc );
    BOOST_CHECK_EQUAL( eng(), 0x24 );
    BOOST_CHECK_EQUAL( eng(), 0x80 );
    BOOST_CHECK_EQUAL( eng(), 0x25 );
    BOOST_CHECK_EQUAL( eng(), 0xc3 );
    BOOST_CHECK_EQUAL( eng(), 0x92 );
    BOOST_CHECK_EQUAL( eng(), 0x12 );
    BOOST_CHECK_EQUAL( eng(), 0xe2 );
}

BOOST_AUTO_TEST_CASE( threefry4x64_13_16 )
{
    boost::random::threefry4x64_engine<boost::uint32_t, 16, 13> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;

    BOOST_CHECK_EQUAL( eng(), 0x8e05 );
    BOOST_CHECK_EQUAL( eng(), 0xe1dc );
    BOOST_CHECK_EQUAL( eng(), 0xfabe );
    BOOST_CHECK_EQUAL( eng(), 0x4071 );
    BOOST_CHECK_EQUAL( eng(), 0x9c62 );
    BOOST_CHECK_EQUAL( eng(), 0x695c );
    BOOST_CHECK_EQUAL( eng(), 0x3113 );
    BOOST_CHECK_EQUAL( eng(), 0x02ed );
    BOOST_CHECK_EQUAL( eng(), 0x9d49 );
    BOOST_CHECK_EQUAL( eng(), 0xb89f );
    BOOST_CHECK_EQUAL( eng(), 0x11b5 );
    BOOST_CHECK_EQUAL( eng(), 0x3973 );
    BOOST_CHECK_EQUAL( eng(), 0x24bc );
    BOOST_CHECK_EQUAL( eng(), 0x2580 );
    BOOST_CHECK_EQUAL( eng(), 0x92c3 );
    BOOST_CHECK_EQUAL( eng(), 0xe212 );
}

BOOST_AUTO_TEST_CASE( threefry4x64_13_32 )
{
    boost::random::threefry4x64_engine<boost::uint32_t, 32, 13> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;

    BOOST_CHECK_EQUAL( eng(), 0xe1dc8e05 );
    BOOST_CHECK_EQUAL( eng(), 0x4071fabe );
    BOOST_CHECK_EQUAL( eng(), 0x695c9c62 );
    BOOST_CHECK_EQUAL( eng(), 0x02ed3113 );
    BOOST_CHECK_EQUAL( eng(), 0xb89f9d49 );
    BOOST_CHECK_EQUAL( eng(), 0x397311b5 );
    BOOST_CHECK_EQUAL( eng(), 0x258024bc );
    BOOST_CHECK_EQUAL( eng(), 0xe21292c3 );
}


BOOST_AUTO_TEST_CASE( threefry4x64_13_64_0x00 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 13> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;
    
    BOOST_CHECK_EQUAL( eng(), 0x4071fabee1dc8e05 );
    BOOST_CHECK_EQUAL( eng(), 0x02ed3113695c9c62 );
    BOOST_CHECK_EQUAL( eng(), 0x397311b5b89f9d49 );
    BOOST_CHECK_EQUAL( eng(), 0xe21292c3258024bc );
}


BOOST_AUTO_TEST_CASE(  threefry4x64_13_64_0xff )
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

BOOST_AUTO_TEST_CASE(  threefry4x64_13_64_0x45 )
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



BOOST_AUTO_TEST_CASE( threefry4x64_20_64_0x00 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 20> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;
    
    BOOST_CHECK_EQUAL( eng(), 0x09218ebde6c85537 );
    BOOST_CHECK_EQUAL( eng(), 0x55941f5266d86105 );
    BOOST_CHECK_EQUAL( eng(), 0x4bd25e16282434dc );
    BOOST_CHECK_EQUAL( eng(), 0xee29ec846bd2e40b );
}

BOOST_AUTO_TEST_CASE( threefry4x64_20_64_0xff )
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

BOOST_AUTO_TEST_CASE( threefry4x64_20_64_0x45 )
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



BOOST_AUTO_TEST_CASE( threefry4x64_72_64_0x00 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng;

    std::istringstream is("0 0 0 0 0 0 0 0 0");
    is >> eng;
    
    BOOST_CHECK_EQUAL( eng(), 0x94eeea8b1f2ada84 );
    BOOST_CHECK_EQUAL( eng(), 0xadf103313eae6670 );
    BOOST_CHECK_EQUAL( eng(), 0x952419a1f4b16d53 );
    BOOST_CHECK_EQUAL( eng(), 0xd83f13e63c9f6b11 );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_0xff )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng;
    
    std::ostringstream os;
    os << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' '
       << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0xffffffffffffffff << ' ' << 0;
    std::istringstream is( os.str() );
    is >> eng;
        
    BOOST_CHECK_EQUAL( eng(), 0x11518c034bc1ff4c );
    BOOST_CHECK_EQUAL( eng(), 0x193f10b8bcdcc9f7 );
    BOOST_CHECK_EQUAL( eng(), 0xd024229cb58f20d8 );
    BOOST_CHECK_EQUAL( eng(), 0x563ed6e48e05183f );
}


BOOST_AUTO_TEST_CASE( threefry4x64_72_64_0x45 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng;

    std::ostringstream os;
    os << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is( os.str() );
    is >> eng;

    BOOST_CHECK_EQUAL( eng(), 0xacf412ccaa3b2270 );
    BOOST_CHECK_EQUAL( eng(), 0xc9e99bd53f2e9173 );
    BOOST_CHECK_EQUAL( eng(), 0x43dad469dc825948 );
    BOOST_CHECK_EQUAL( eng(), 0xfbb19d06c8a2b4dc );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_key3 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0x0000000000000000 << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 3, 4> eng2;
    std::ostringstream os2;
    os2 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_key2 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 2, 4> eng2;
    std::ostringstream os2;
    os2 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_key1 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 4> eng2;
    std::ostringstream os2;
    os2 << 0x452821e638d01377 << ' ' 
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_key0 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 0, 4> eng2;
    std::ostringstream os2;
    os2 
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x082efa98ec4e6c89 << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}


BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter3 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0x0000000000000000 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 4, 3> eng2;
    std::ostringstream os2;
    os2 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0xa4093822299f31d0 << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}


BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter2 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 4, 2> eng2;
    std::ostringstream os2;
    os2 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x13198a2e03707344 << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}




BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter1 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 4, 1> eng2;
    std::ostringstream os2;
    os2 << 0x452821e638d01377 << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xbe5466cf34e90c6c << ' ' << 0xc0ac29b7c97c50dd << ' '
       << 0x243f6a8885a308d3  << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}



BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter1_key1 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' '
       << 0x243f6a8885a308d3 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0x0000000000000000 << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 1> eng2;
    std::ostringstream os2;
    os2 << 0x452821e638d01377 << ' '
       << 0x243f6a8885a308d3  << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}



BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter1_test1 )
{
    boost::uint64_t k,c,s;
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 1> eng;
    
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0x0000000000000000  << ' ' << 0;      // key, counter, sub-counter
    std::istringstream is1( os1.str() );
    is1 >> eng;
    
    eng();
    
    std::ostringstream os2;
    os2 << eng;
    std::istringstream is2( os2.str() );
    is2 >> k >> c >> s;
    BOOST_CHECK_EQUAL( c, 0 );
    BOOST_CHECK_EQUAL( s, 1 );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter1_test2 )
{
    boost::uint64_t k,c,s;
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 1> eng;
    
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0x0000000000000000  << ' ' << 4;      // key, counter, sub-counter
    std::istringstream is1( os1.str() );
    is1 >> eng;
    
    eng();
    
    std::ostringstream os2;
    os2 << eng;
    std::istringstream is2( os2.str() );
    is2 >> k >> c >> s;
    BOOST_CHECK_EQUAL( c, 1 );
    BOOST_CHECK_EQUAL( s, 1 );
}


BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter1_test3 )
{
    boost::uint64_t k,c,s;
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 1> eng;
    
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xFFFFFFFFFFFFFFFF  << ' ' << 4;      // key, counter, sub-counter
    std::istringstream is1( os1.str() );
    is1 >> eng;
    
    eng();
    
    std::ostringstream os2;
    os2 << eng;
    std::istringstream is2( os2.str() );
    is2 >> k >> c >> s;
    BOOST_CHECK_EQUAL( c, 0 );
    BOOST_CHECK_EQUAL( s, 1 );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter2_test1 )
{
    boost::uint64_t k,c0,c1,s;
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 2> eng;
    
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xFFFFFFFFFFFFFFFF << ' ' << 0x0  << ' ' << 4;
    std::istringstream is1( os1.str() );
    is1 >> eng;
    
    eng();
    
    std::ostringstream os2;
    os2 << eng;
    std::istringstream is2( os2.str() );
    is2 >> k >> c0 >> c1  >> s;
    BOOST_CHECK_EQUAL( c0, 0 );
    BOOST_CHECK_EQUAL( c1, 1 );
    BOOST_CHECK_EQUAL( s, 1 );
}

BOOST_AUTO_TEST_CASE( threefry4x64_72_64_counter2_test2 )
{
    boost::uint64_t k,c0,c1,s;
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 2> eng;
    
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0xFFFFFFFFFFFFFFFF << ' ' << 0xFFFFFFFFFFFFFFFF  << ' ' << 4;
    std::istringstream is1( os1.str() );
    is1 >> eng;
    
    eng();
    
    std::ostringstream os2;
    os2 << eng;
    std::istringstream is2( os2.str() );
    is2 >> k >> c0 >> c1  >> s;
    BOOST_CHECK_EQUAL( c0, 0 );
    BOOST_CHECK_EQUAL( c1, 0 );
    BOOST_CHECK_EQUAL( s, 1 );
}


BOOST_AUTO_TEST_CASE( threefry4x64_72_64_seed_test1 )
{
    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72,1 ,1> eng1;
    std::ostringstream os1;
    os1 << 0x452821e638d01377 << ' ' << 0  << ' ' << 0;
    std::istringstream is1( os1.str() );
    is1 >> eng1;

    boost::random::threefry4x64_engine<boost::uint64_t, 64, 72, 1, 1> eng2;
    std::ostringstream os2;
    os2 << 0x0 << ' ' << 0  << ' ' << 0;
    std::istringstream is2( os2.str() );
    is2 >> eng2;

    eng2.seed(0x452821e638d01377);
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
    BOOST_CHECK_EQUAL( eng1(), eng2() );
}
