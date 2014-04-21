/* boost random/threefry.hpp header file
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

#ifndef BOOST_RANDOM_THREEFRY_HPP
#define BOOST_RANDOM_THREEFRY_HPP

#include <istream>
#include <algorithm>

#include <boost/cstdint.hpp>
#include <boost/config.hpp>
#include <boost/limits.hpp>

#include <boost/static_assert.hpp>

#include <boost/random/detail/config.hpp>
#include <boost/random/detail/const_mod.hpp>
#include <boost/random/detail/seed.hpp>
#include <boost/random/detail/seed_impl.hpp>

#include <boost/utility/swap.hpp>

namespace boost {
namespace random {

/**
* @brief The threefry random engine is a counter based random engine that uses a stripped-down Threefish cryptographic function that is optimised for speed.
*
* The template parameter @p UIntType must be an unsigned integral type. 
* The template parameter @p rounds should be 13 or 20
*
* @blockquote
* title:     Parallel random numbers: as easy as 1, 2, 3
* authors:   Salmon, John K. and Moraes, Mark A. and Dror, Ron O. and Shaw, David E.
* booktitle: Proceedings of 2011 International Conference for High Performance Computing, Networking, Storage and Analysis
* publicher: ACM
* year:      2011
* isbn:      978-1-4503-0771-0
* @endblockquote
*
* @xmlnote
* The boost variant has been implemented from scratch and does not
* derive from or use the Random123 library provided at http://www.thesalmons.org/john/random123/
* However, it  was verified that both produce identical output.
* Output was verified against the threefry4x64 unit test cases from https://github.com/girving/random123/blob/master/examples/kat_vectors
* @endxmlnote
*/
namespace detail {

    static const uint_least64_t threefry4x64_tweak = 0x1BD11BDAA9FC1A22;
    
    // primary template
    template< typename UIntType, std::size_t w>
    struct extract4x64_impl {
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n);
        inline static UIntType w_max();
    };
    
    // specialisation
    template< typename UIntType>
    struct extract4x64_impl<UIntType,64> {
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
        { return _output[n]; }
        inline static UIntType w_max()
        { return 0xFFFFFFFFFFFFFFFF; }
    };
    template< typename UIntType>
    struct extract4x64_impl<UIntType,32> {
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
        { return (_output[n>>1] >> ((n&1)<<5)) & 0xFFFFFFFF; }
        inline static UIntType w_max()
        { return 0xFFFFFFFF; }
    };
    template< typename UIntType>
    struct extract4x64_impl<UIntType,16> {
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
        { return (_output[n>>2] >> ((n&11)<<4)) & 0xFFFF; }
        inline static UIntType w_max()
        { return 0xFFFF; }
    };
    template< typename UIntType>
    struct extract4x64_impl<UIntType,8> {
        inline static UIntType nth(const boost::uint_least64_t (&_output)[4], std::size_t n)
        { return (_output[n>>3] >> ((n&111)<<3)) & 0xFF; }
        inline static UIntType w_max()
        { return 0xFF; }
    };
}

template <  typename UIntType,      // the return type
            std::size_t w,          // number of bits in the return type
            std::size_t r=20        // number of rounds
        >
class threefry4x64_engine
{
public:
    BOOST_STATIC_ASSERT( w==8 || w==16 || w==32 || w==64 ); // otherwise we would need to patch multiple fragments from our 4x64 together
 
    /** The type of the generated random value. */
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(std::size_t, word_size = w);
    BOOST_STATIC_CONSTANT(std::size_t, rounds = r);
    BOOST_STATIC_CONSTANT(bool, has_fixed_range = false);

    BOOST_STATIC_CONSTANT(std::size_t, samples_per_block = 256/w);

    
    /**
     * @brief Constructs the defafult %threefry4x64_engine.
     */
    threefry4x64_engine() 
    { seed(0); }
    
    /**
     * @brief Constructs a %threefry4x64_engine random number
     *        generator engine with seed @p value.  The default seed value
     *        is 0.
     *
     * @param value The initial seed value.
     */
    BOOST_RANDOM_DETAIL_ARITHMETIC_CONSTRUCTOR(threefry4x64_engine, UIntType, value)
    { seed(value); }
    
    /**
     * @brief Constructs a %threefry4x64_engine and seed it
     * with values taken from the iterator range [@p first, @p last)
     * and adjusts first iterator to point to the element after the last one
     * used.  If there are not enough elements, throws @c std::invalid_argument.
     *
     * @p first and @p last must be input iterators.
     */
    template<class It> threefry4x64_engine(It& first, It last)
    { seed(first,last); }
    
    /**
     * @brief Constructs a %threefry4x64_engine random number
     *        generator engine seeded from the seed sequence @p seq.
     *
     * @param seq the seed sequence.
     */
    BOOST_RANDOM_DETAIL_SEED_SEQ_CONSTRUCTOR(threefry4x64_engine, SeedSeq, seq)
    { seed(seq); }

    /**
     * @brief Re-seed the %threefry4x64_engine to it's default seed.
     */
    void seed()
    { seed(0); }
    
    /**
     * @brief Re-seed the  %threefry4x64_engine random number
     *        generator engine with the seed @p value.
     *        The default seed value is 0.
     *
     * @param value the new seed.
     */
    BOOST_RANDOM_DETAIL_ARITHMETIC_SEED(threefry4x64_engine, UIntType, value)
    {
        UIntType v = value;
        for (unsigned int i=0; i<4; ++i) {
            _key[i] = v & 0xFFFFFFFFFFFFFFFF;
            
            // v >>= 64 causes undefined behaviour when the type of b <= 64 bits, but this works
            for (unsigned int b=0; b<8; ++b)
                v >>= 8;
        }
        reset_after_key_change();
    }

    /**
     * @brief Seeds the %threefry4x64_engine random number
     *        generator engine with the seed sequence @p seq.
     *
     * @param seq the seed sequence.
     */
    BOOST_RANDOM_DETAIL_SEED_SEQ_SEED(threefry4x64_engine, SeeqSeq, seq)
    {
        detail::seed_array_int<64>(seq, _key);
        reset_after_key_change();
    }

    /**
     * @brief Seed the %threefry4x64_engine with values taken
     * from the iterator range [@p first, @p last) and adjusts @p first to
     * point to the element after the last one used.  If there are
     * not enough elements, throws @c std::invalid_argument.
     *
     * @p first and @p last must be input iterators.
     */
    template<class It> void seed(It& first, It last)
    { 
        detail::fill_array_int<64>(first, last, _key);
        reset_after_key_change();
    }
    
    /**
     * @brief Gets the smallest possible value in the output range.
     */
    static result_type min BOOST_PREVENT_MACRO_SUBSTITUTION ()
    { return 0; }

    /**
     * @brief Gets the largest possible value in the output range.
     */
    static result_type max BOOST_PREVENT_MACRO_SUBSTITUTION ()
    { return detail::extract4x64_impl<UIntType,w>::w_max(); }


    /**
     * @brief Generate a random sample.
     */
    result_type operator()()
    {
        // can we return a value from the current block?
        if (_o_counter < samples_per_block)
            return detail::extract4x64_impl<UIntType,w>::nth(_output, _o_counter++);
            //return reinterpret_cast<result_type*>(_output)[_o_counter++];

        
        // generate a new block and return the first result_type 
        inc_counter();
        encrypt_counter();
        _o_counter = 1; // the next call
        return detail::extract4x64_impl<UIntType,w>::nth(_output, 0);
        //return *reinterpret_cast<result_type*>(_output);  // this call
    }

    /**
     * @brief Fills a range with random values.
     */
    template<class Iter>
    void generate(Iter first, Iter last)
    { detail::generate_from_int(*this, first, last); }
    
    /**
     * @brief Discard a number of elements from the random numbers sequence.
     *
     * @param z the number of elements to discard.
     */
    void discard(boost::uintmax_t z)
    {
        // check if we stay in the current block
        if (z < samples_per_block - _o_counter) {
            _o_counter += static_cast<unsigned short>(z);
            return;
        }

        // we will have to generate a new block...
        z -= (samples_per_block - _o_counter);  // discard the remainder of the current blok
        _o_counter = z % samples_per_block;     // set the pointer in the correct element in the new block
        z -= _o_counter;                        // update z
        z /= samples_per_block;                 // the number of bocks to skip is z/samples_per_block
        ++z;                                    // and one more because we crossed the buffer line
        inc_counter(z);
        encrypt_counter();
    }

#ifndef BOOST_RANDOM_NO_STREAM_OPERATORS
    /**
     * @brief Writes the textual representation of the state x(i) of x to
     *        @p _os.
     *
     * @param os  The output stream.
     * @param eng A %threefry4x64_engine random number generator.
     * @returns os.
     */
    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os, const threefry4x64_engine& eng)
    {
        for (unsigned short i=0; i<4; ++i)
            os << eng._key[i] << ' ';
            
        for (unsigned short i=0; i<4; ++i)
            os << eng._counter[i] << ' ';
            
        os << eng._o_counter;
       return os;
    }
   
    /**
     * @brief reads the textual representation of the state x(i) from
     *        @p is.
     *
     * @param is  The input stream.
     * @param eng A %threefry4x64_engine random number generator.
     * @returns is.
     */
    template<class CharT, class Traits>
    friend std::basic_istream<CharT, Traits>&
    operator >> (std::basic_istream<CharT, Traits>& is, threefry4x64_engine& eng)
    {
        for (unsigned short i=0; i<4; ++i) 
            is >> eng._key[i] >> std::ws;
            
        for (unsigned short i=0; i<4; ++i) 
            is >> eng._counter[i] >> std::ws;
            
        is >> eng._o_counter;
        
        eng._key[4] = detail::threefry4x64_tweak ^ eng._key[0] ^ eng._key[1] ^ eng._key[2] ^ eng._key[3];
        eng.encrypt_counter();
        return is;
    } 
#endif

    /**
     * @brief Compares two %threefry4x64_engine
     * objects of the same type for equality.
     *
     *
     * @param _lhs A threefry engine.
     * @param _rhs Another threefry engine.
     *
     * @returns true if the infinite sequences of generated values
     *          would be equal, false otherwise.
     */
    friend bool 
    operator==(const threefry4x64_engine& _lhs, const threefry4x64_engine& _rhs) 
    {
        if (_lhs._o_counter != _rhs._o_counter) return false;
        
        for (unsigned short i=0; i<4; ++i) {
            if (_lhs._counter[i] != _rhs._counter[i]) return false;
            if (_lhs._key[i]     != _rhs._key[i])     return false;
            if (_lhs._output[i]  != _rhs._output[i])  return false;
        }
        return true;
    }
    
    /**
     * @brief Compares two threefry engines for inequality.
     *
     *
     * @param _lhs A threefry engine.
     * @param _rhs Another threefry engine.
     *
     * @returns true if the infinite sequences of generated values
     *          would not be equal, false otherwise.
     */
    friend bool 
    operator!=(const threefry4x64_engine& _lhs, const threefry4x64_engine& _rhs) 
    { return !(_lhs == _rhs); }


private:
    inline void rotl64(boost::uint_least64_t& v, const boost::uint8_t bits) const
    { 
        v = (v << bits) | (v >> (sizeof(boost::uint_least64_t)*8-bits)); 
    }
    
    inline void mix64(boost::uint_least64_t& x0, boost::uint64_t& x1, const boost::uint8_t bits) const
    {
        x0 += x1;
        rotl64(x1, bits);
        x1 ^= x0;
    }

    inline void double_mix64(   boost::uint_least64_t& x0, boost::uint_least64_t& x1, const boost::uint8_t rx,
                                boost::uint_least64_t& z0, boost::uint_least64_t& z1, const boost::uint8_t rz) const
    {
        mix64(x0,x1,rx);
        mix64(z0,z1,rz);
    }

    template <std::size_t offset>
    inline void add_key64_t( boost::uint_least64_t (&output)[4], boost::uint_least64_t (&key)[5], const std::size_t c) const
    {
        output[0] += key[ offset   %5];
        output[1] += key[(offset+1)%5];
        output[2] += key[(offset+2)%5];
        output[3] += key[(offset+3)%5];
        output[3] += c;
    }

    template <std::size_t Rounds>
    inline void encrypt_counter_t(std::size_t& four_cycles)
    {
        if (Rounds>=1) double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16);
        if (Rounds>=2) double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57);
        if (Rounds>=3) double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40);
        if (Rounds>=4) {
            double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
            add_key64_t<1>(_output, _key, ++four_cycles);
        }
     
        if (Rounds>=5) double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33);
        if (Rounds>=6) double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12);
        if (Rounds>=7) double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22);
        if (Rounds>=8) {
            double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
            add_key64_t<2>(_output, _key, ++four_cycles);
        }

        if (Rounds>=9)  double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16);
        if (Rounds>=10) double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57);
        if (Rounds>=11) double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40);
        if (Rounds>=12) {
            double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
            add_key64_t<3>(_output, _key, ++four_cycles);
        }

        if (Rounds>=13) double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33);
        if (Rounds>=14) double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12);
        if (Rounds>=15) double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22);
        if (Rounds>=16) {
            double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
            add_key64_t<4>(_output, _key, ++four_cycles);
        }
        
        if (Rounds>=17) double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16);
        if (Rounds>=18) double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57);
        if (Rounds>=19) double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40);
        if (Rounds>=20) {
            double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
            add_key64_t<0>(_output, _key, ++four_cycles);
        }
     
        if (Rounds>=21) double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33);
        if (Rounds>=22) double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12);
        if (Rounds>=23) double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22);
        if (Rounds>=24) {
            double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
            add_key64_t<1>(_output, _key, ++four_cycles);
        }
        
        if (Rounds>=25) double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16);
        if (Rounds>=26) double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57);
        if (Rounds>=27) double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40);
        if (Rounds>=28) {
            double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
            add_key64_t<2>(_output, _key, ++four_cycles);
        }

        if (Rounds>=29) double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33);
        if (Rounds>=30) double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12);
        if (Rounds>=31) double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22);
        if (Rounds>=32) {
            double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
            add_key64_t<3>(_output, _key, ++four_cycles);
        }
        
        if (Rounds>=33) double_mix64( _output[0], _output[1], 14, _output[2], _output[3], 16);
        if (Rounds>=34) double_mix64( _output[0], _output[3], 52, _output[2], _output[1], 57);
        if (Rounds>=35) double_mix64( _output[0], _output[1], 23, _output[2], _output[3], 40);
        if (Rounds>=36) {
            double_mix64( _output[0], _output[3],  5, _output[2], _output[1], 37);
            add_key64_t<4>(_output, _key, ++four_cycles);
        }

     
        if (Rounds>=37) double_mix64( _output[0], _output[1], 25, _output[2], _output[3], 33);
        if (Rounds>=38) double_mix64( _output[0], _output[3], 46, _output[2], _output[1], 12);
        if (Rounds>=39) double_mix64( _output[0], _output[1], 58, _output[2], _output[3], 22);
        if (Rounds>=40) {
            double_mix64( _output[0], _output[3], 32, _output[2], _output[1], 32);
            add_key64_t<0>(_output, _key, ++four_cycles);
        }
    }
    
    void encrypt_counter()
    {

        for (std::size_t i=0; i<4; ++i) _output[i] = _counter[i];
        for (std::size_t i=0; i<4; ++i) _output[i] +=_key[i];

        std::size_t four_cycles = 0;

        for (std::size_t big_rounds=0; big_rounds < r/40; ++big_rounds)
            encrypt_counter_t<40>(four_cycles);
            
        encrypt_counter_t<r - 40*(r/40)>(four_cycles);
return;


        
        encrypt_counter_t<r - 40*(r/40)>(four_cycles);
    }
    
    // increment the counter with 1
    void inc_counter()
    {
        ++_counter[0]; if (_counter[0] != 0) return;
        ++_counter[1]; if (_counter[1] != 0) return;
        ++_counter[2]; if (_counter[2] != 0) return;
        ++_counter[3];
    }
    
    // increment the counter with z
    void inc_counter(boost::uintmax_t z)
    {
        if (z > 0xFFFFFFFFFFFFFFFF - _counter[0]) {   // check if we will overflow the first 64 bit int
            ++_counter[1];
            if (_counter[1] == 0) {
                ++_counter[2];
                if (_counter[2] == 0) {
                    ++_counter[3];
                }
            }
        }
        _counter[0] += z;
    }
    
    void reset_counter()
    {
        _counter[0] = 0;
        _counter[1] = 0;
        _counter[2] = 0;
        _counter[3] = 0;
        _o_counter  = 0;
    }

    // reset the counter to zero, and reset the keyx
    void reset_after_key_change()
    {
        _key[4] = detail::threefry4x64_tweak ^ _key[0] ^ _key[1] ^ _key[2] ^ _key[3];
        reset_counter();
        encrypt_counter();
    }
    
private:

    boost::uint_least64_t _counter[4];   // the 256 bit counter (message) that gets encrypted
    boost::uint_least64_t _output[4];    // the 256 bit cipher output 4 * 64 bit = 256 bit output
    boost::uint_least64_t _key[5];       // the 256 bit encryption key
    boost::uint_least16_t _o_counter;     // output chunk counter, e.g. for a 64 bit random engine
                            // the 256 bit output buffer gets split in 4x64bit chunks or 8x32bit chunks chunks.
    /// \endcond
};

/**
 * 32 bit version of the 13 rounds threefry engine
 */
typedef threefry4x64_engine<boost::uint32_t, 32, 13> threefry4x64_13;

/**
 * 64 bit version of the 13 rounds threefry engine
 */
typedef threefry4x64_engine<boost::uint64_t, 64, 13> threefry4x64_13_64;


/**
 * 32 bit version of the 20 rounds threefry engine
 */
typedef threefry4x64_engine<boost::uint32_t, 32, 20> threefry4x64_20;

/**
 * 64 bit version of the 20 rounds threefry engine
 */
typedef threefry4x64_engine<boost::uint64_t, 64, 20> threefry4x64_20_64;

} // namespace random


} // namespace boost

#include <boost/random/detail/enable_warnings.hpp>

#endif // BOOST_RANDOM_THREEFRY_HPP